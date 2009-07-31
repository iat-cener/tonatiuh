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

#include <Inventor/SbLinear.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <QString>
#include <iostream>
#include "TestCube.h"
#include "NormalVector.h"
#include "Point3D.h"
#include "TCube.h"
#include "Trace.h"
#include "NormalVector.h"
using namespace std;


CPPUNIT_TEST_SUITE_REGISTRATION( TestCube );

class TCube;

void TestCube::test_init_Class()
{

}

void TestCube::setUp()
{
	m_cube = new TCube();
	m_cube->m_width = 4.0;
	m_cube->m_height = 5.0;
	m_cube->m_depth = 3.0;
	m_cube->ref();
}

void TestCube::tearDown()
{

	m_cube->unref();
}

void TestCube::test_computeBBox()
{

	SoGetBoundingBoxAction* ba = new SoGetBoundingBoxAction( SbViewportRegion(0,1) );
	ba->apply(m_cube);
	m_bbox = ba->getBoundingBox();

	SbVec3f min, minexpected;
	SbVec3f max, maxexpected;

	min = m_bbox.getMin();
	max = m_bbox.getMax();

	minexpected.setValue(-2.0,-2.5,-1.5);
	maxexpected.setValue(2.0,2.5,1.5);

	CPPUNIT_ASSERT_EQUAL( min[0], minexpected[0] );
	CPPUNIT_ASSERT_EQUAL( min[1], minexpected[1] );
	CPPUNIT_ASSERT_EQUAL( min[2], minexpected[2] );
	CPPUNIT_ASSERT_EQUAL( max[0], maxexpected[0] );
	CPPUNIT_ASSERT_EQUAL( max[1], maxexpected[1] );
	CPPUNIT_ASSERT_EQUAL( max[2], maxexpected[2] );
}

void TestCube::test_get_Icon()
{
	QString icon = m_cube->getIcon();
	QString expected(":/icons/tcube.png");
	CPPUNIT_ASSERT_MESSAGE("Error getting Icon", icon==expected);
}

void TestCube::test_get_Normal()
{
	CPPUNIT_ASSERT_MESSAGE("Not Implemented", (1==0) );
}

void TestCube::test_intersect()
{
	CPPUNIT_ASSERT_MESSAGE( "Not Implemented", (1==0) );
}

void TestCube::test_intersectP()
{
	CPPUNIT_ASSERT_MESSAGE( "Not Implemented", (1==0) );
}

void TestCube::	test_Sample()
{
	CPPUNIT_ASSERT_MESSAGE( "Not Implemented", (1==0) );
}



