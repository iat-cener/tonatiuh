/*
 * TestBBox.cpp
 *
 *  Created on: Mar 10, 2009
 *      Author: ENGINEERING
 */

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
	//zBBox = new BBox( Point3D( -0.0, -0.0, -0.0 ), Point3D( 0.0, 0.0, 0.0 ) );
	//nBBox = new BBox();
}

void TestBBox::tearDown()
{
	delete m_bBox;
	//delete zBBox;
	//delete nBBox;
}

void TestBBox::TestOverlaps()
{
	m_bBox = new BBox( Point3D( -3.0, -3.0, -3.0 ), Point3D( 3.0, 3.0, 3.0 ) );
	BBox newBBox( Point3D( 0.0, 0.0, 0.0), Point3D( 1.0, 4.0, 1.0 ) );
	CPPUNIT_ASSERT_MESSAGE( "Not overlaps", m_bBox->Overlaps( newBBox ) );


}

void TestBBox::TestInside()
{

	Point3D newPoint(  1.0, 1.0, 1.0 );
	CPPUNIT_ASSERT_MESSAGE( "Is inside",  m_bBox->Inside( newPoint ) );




}

void TestBBox::TestExpand()
{
    Point3D newMin( -2.0, -2.0, -2.0 );
	Point3D newMax( 2.0, 2.0, 2.0 );

	m_bBox->Expand( 4.0 );

	CPPUNIT_ASSERT_MESSAGE( "Not Working", !( m_bBox->pMin == newMin && m_bBox->pMax == newMax ) );


	m_bBox->Expand( -4.0 );

}

void TestBBox::TestVolume()
{
	double T3 =m_bBox->Volume();
//T3=	m_bBox.Volume();
	CPPUNIT_ASSERT_EQUAL( T3 ,216.0 );



}



void TestBBox::TestBoundingSphere()
{


	/*


void BBox::BoundingSphere( Point3D* center, float* radius ) const
{
	Trace trace( "BBox::BoundingSphere", false );

	*center = 0.5 * ( pMin + pMax );
    *radius = Distance( *center, pMax );

}

*/

	//m_bBox = new BBox( Point3D( -4.0, -4.0, -4.0 ), Point3D( 4.0, 4.0, 4.0 ) );
	Point3D Center1;
	float Radious1;
	m_bBox->BoundingSphere( &Center1, &Radious1 );

	Point3D Center2( 0.0, 0.0, 0.0 );
	//float Radious2 = 2.000;

	CPPUNIT_ASSERT_MESSAGE( "working",  (Center2 == Center1 ));
  // CPPUNIT_ASSERT_MESSAGE("working",(Radious1 =! Radious2));
}

void TestBBox::TestIntersectP()
{




	Ray T2( Point3D( 0.0, 1.0, 0.0 ), Vector3D(0, -1.0, 0 ) );

	CPPUNIT_ASSERT_MESSAGE( "working", m_bBox->IntersectP( T2 ) );

}


