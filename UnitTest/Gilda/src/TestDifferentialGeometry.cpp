/*
 * TestDifferentialGeometry.cpp
 *
 *  Created on: Mar 10, 2009
 *      Author: ENGINEERING
 */

#include "DifferentialGeometry.h"
#include "Point3D.h"
#include "TestDifferentialGeometry.h"
#include "Vector3D.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestDifferentialGeometry);

void TestDifferentialGeometry::setUp()
{
	m_differentialGeometry = new DifferentialGeometry( Point3D( 0.0, 0.0, 0.0 ),
										Vector3D( 1.0, 0.0, 0.0 ),
										Vector3D( 0.0, 0.0, 1.0 ),
										Vector3D( 0.0, 0.0, 0.0 ),
										Vector3D( 0.0, 0.0, 0.0 ),
										1.0,
										1.0,
										NULL );

}
void TestDifferentialGeometry::tearDown()
{
	delete m_differentialGeometry;
}

void TestDifferentialGeometry::TestNormal()
{
	NormalVector expected( 0.0, -1.0, 0.0 );

	CPPUNIT_ASSERT_MESSAGE( "WORKING", expected == m_differentialGeometry->normal );

}
