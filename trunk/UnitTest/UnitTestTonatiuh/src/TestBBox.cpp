/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <cmath>

#include "BBox.h"
#include "Point3D.h"
#include "Ray.h"
#include "TestBBox.h"
#include "tgc.h"
#include "Vector3D.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBBox);

void TestBBox::setUp()
{
	m_bBox = new BBox( Point3D( -3.0, -3.0, -3.0 ), Point3D( 3.0, 3.0, 3.0 ) );
	m_zeroBBox = new BBox( Point3D( -0.0, -0.0, -0.0 ), Point3D( 0.0, 0.0, 0.0 ) );
	m_nullBBox = new BBox();
}

void TestBBox::tearDown()
{
	delete m_bBox;
	delete m_zeroBBox;
	delete m_nullBBox;
}

void TestBBox::TestOverlaps()
{
	BBox newBBox( Point3D( -1.0, -2.0, -1.0), Point3D( 1.0, 4.0, 1.0 ) );

	CPPUNIT_ASSERT_MESSAGE( "Not overlaps", m_bBox->Overlaps( newBBox ) );

	BBox equalBox( Point3D( -3.0, -3.0, -3.0 ), Point3D( 3.0, 3.0, 3.0 ) );
	CPPUNIT_ASSERT_MESSAGE( "Not overlaps equal bounding box", m_bBox->Overlaps( equalBox ) );

}

void TestBBox::TestInside()
{
	//Is inside
	Point3D insidePoint( 1.0, 1.0, 1.0 );
	CPPUNIT_ASSERT_MESSAGE( "Is not inside", m_bBox->Inside( insidePoint ) );

	//Is not inside
	Point3D notInsidePointPoint3D( 1.0, 1.0, 4.0 );

	CPPUNIT_ASSERT_MESSAGE( "Is not inside", !m_bBox->Inside( notInsidePointPoint3D ) );

	//Equal BBox
	Point3D equalPoint( -3.0, -3.0, -3.0 );
	CPPUNIT_ASSERT_MESSAGE( "Equal bounding box is not inside", m_bBox->Inside( equalPoint ) );
}

void TestBBox::TestExpand()
{
	Point3D newMin( -4.0, -4.0, -4.0 );
	Point3D newMax( 4.0, 4.0, 4.0 );

	m_bBox->Expand( 1.0 );


	CPPUNIT_ASSERT_MESSAGE( "Error expanding bounding box is not inside", ( m_bBox->pMin == newMin && m_bBox->pMax == newMax ) );

	m_bBox->Expand( -1.0 );
}

void TestBBox::TestVolume()
{

	CPPUNIT_ASSERT_MESSAGE( "Error computing bounding box volume", m_bBox->Volume() == 216.0 );

	//0 bounding box volume
	CPPUNIT_ASSERT_MESSAGE( "Error computing null bounding box volume", m_zeroBBox->Volume() == 0.0 );

	//Null bounding box volume
	CPPUNIT_ASSERT_MESSAGE( "Error computing null bounding box volume",  m_nullBBox->Volume() == -tgc::Infinity );
}

void TestBBox::TestMaximumExtent()
{

}

void TestBBox::TestBoundingSphere()
{
	Point3D computedCenter;
	float computedRadious;
	m_bBox->BoundingSphere( &computedCenter, &computedRadious );

	Point3D expectedCenter( 0.0, 0.0, 0.0 );
	float expectedRadious = 5.1961524227066318805823390245176;

	CPPUNIT_ASSERT_MESSAGE( "Error computing bounding box BoundingSphere",  ( expectedCenter == computedCenter && expectedRadious == computedRadious ) );

	//0 BBox
	Point3D zeroCenter;
	float zeroRadious;
	m_zeroBBox->BoundingSphere( &zeroCenter, &zeroRadious );
	CPPUNIT_ASSERT_MESSAGE( "Error computing zero BoundingBox BoundingSphere",  ( expectedCenter == zeroCenter && zeroRadious == 0.0  ) );

	/*//Null BBox
	Point3D nullCenter;
	float nullRadious;
	m_nullBBox->BoundingSphere( &zeroCenter, &zeroRadious );

	Point3D expectedNullCenter( 0.0, 0.0, 0.0 );
	std::cout<<nullCenter<<" "<<nullRadious<<std::endl;
	//CPPUNIT_ASSERT_MESSAGE( "Error computing zero BoundingBox BoundingSphere",  ( expectedNullCenter == nullCenter && nullRadious == tgc::Infinity  ) );
	CPPUNIT_ASSERT_MESSAGE( "Error computing zero BoundingBox BoundingSphere",  expectedNullCenter == nullCenter && nullRadious == tgc::Infinity );*/

}

void TestBBox::TestIntersectP()
{

	Ray incidentRay( Point3D( 0.0, 5.0, 0.0 ), Vector3D(0.0, -1.0, 0.0 ) );

	CPPUNIT_ASSERT_MESSAGE( "Error ray intersection", m_bBox->IntersectP( incidentRay ) );

}

void TestBBox::TestUnionWithPoint()
{
	Point3D insidePoint( 0.0, 0.0, 0.0 );
	BBox unionBBox = Union( *m_bBox, insidePoint );

	CPPUNIT_ASSERT_MESSAGE( "Error union with inside point", ( m_bBox->pMin == unionBBox.pMin && m_bBox->pMax == unionBBox.pMax ) );

	Point3D outPoint( 4.0, 2.0, -5.5 );
	BBox outUnionBBox = Union( *m_bBox, outPoint );
	CPPUNIT_ASSERT_MESSAGE( "Error union with out point", ( outUnionBBox.pMin == Point3D( -3.0, -3.0, -5.5 ) && outUnionBBox.pMax == Point3D( 4.0, 3.0, 3.0) ) );

	//0 BBox
	BBox zeroUnionBBox = Union( *m_zeroBBox, outPoint );
	CPPUNIT_ASSERT_MESSAGE( "Error union zeroBBox with out point", ( zeroUnionBBox.pMin == Point3D( 0.0, 0.0, -5.5 ) && zeroUnionBBox.pMax == Point3D( 4.0, 2.0, 0.0) ) );

	//Null BBox
	BBox nullUnionBBox = Union( *m_nullBBox, outPoint );
	CPPUNIT_ASSERT_MESSAGE( "Error union zeroBBox with out point", ( nullUnionBBox.pMin == outPoint && nullUnionBBox.pMax == outPoint ) );

}
void TestBBox::TestUnionWithBBox()
{

	BBox insideBBox( Point3D( -1.0, -1.0, -1.0 ), Point3D( 1.0, 1.0, 1.0 ) );
	BBox unionBBox = Union( *m_bBox, insideBBox );

	CPPUNIT_ASSERT_MESSAGE( "Error union with inside point", ( unionBBox.pMin == m_bBox->pMin && unionBBox.pMax  == m_bBox->pMax ) );

	BBox otherBBox( Point3D( -1.0, -1.0, -1.0 ), Point3D( 4.0, 4.0, 4.5 ) );
	BBox newBBox = Union( *m_bBox, otherBBox );
	CPPUNIT_ASSERT_MESSAGE( "Error union with out point", ( newBBox.pMin == m_bBox->pMin && newBBox.pMax == otherBBox.pMax ) );

	//0 BBox
	BBox zeroUnionBBox = Union( *m_zeroBBox, otherBBox );
	CPPUNIT_ASSERT_MESSAGE( "Error union zeroBBox with out point", ( zeroUnionBBox.pMin == otherBBox.pMin && zeroUnionBBox.pMax == otherBBox.pMax ) );

	//Null BBox
	BBox nullUnionBBox = Union( *m_nullBBox, otherBBox );
	CPPUNIT_ASSERT_MESSAGE( "Error union zeroBBox with out point", ( nullUnionBBox.pMin == otherBBox.pMin && nullUnionBBox.pMax == otherBBox.pMax ) );

}

