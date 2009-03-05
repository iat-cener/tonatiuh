
#include <cmath>
#include <cfloat>

#include "Matrix4x4.h"
#include "TestMatrix4x4.h"

const double Epsilon = DBL_EPSILON;

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(TestMatrix4x4);

void TestMatrix4x4::setUp()
{
	m_identity = new Matrix4x4(	1.0,0.0,0.0,0.0,
								0.0,1.0,0.0,0.0,
								0.0,0.0,1.0,0.0,
								0.0,0.0,0.0,1.0 );

	m_mat = new Matrix4x4(	1.1, 1.2, 1.3, 1.4,
							2.1, 2.2, 2.3, 2.4,
							3.1, 3.2, 3.3, 3.4,
							4.1, 4.2, 4.3, 4.4 );
}

void TestMatrix4x4::tearDown()
{
	delete m_identity;
	delete m_mat;
}

void TestMatrix4x4::TestOperator()
{
	Matrix4x4* newmat = new Matrix4x4();
	CPPUNIT_ASSERT_EQUAL((*newmat), (*m_identity));
}

void TestMatrix4x4::TestTranspose()
{
	Matrix4x4* p1= new Matrix4x4( 1.0,1.0,1.0,1.0,
											2.0,2.0,2.0,2.0,
											3.0,3.0,3.0,3.0,
											4.0,4.0,4.0,4.0 );
	Matrix4x4* p4 = new Matrix4x4(	1.0,2.0,3.0,4.0,
							1.0, 2.0, 3.0, 4.0,
							1.0, 2.0, 3.0, 4.0,
							1.0, 2.0, 3.0, 4.0 );

	Matrix4x4* p2 = new Matrix4x4( (*p4->Transpose() ) );

	CPPUNIT_ASSERT_EQUAL((*p1),(*p2));

}

void TestMatrix4x4:: TestInverse()
{

	Matrix4x4 *p1 = new Matrix4x4(0.0,1.0,0.0,0.0,
			-1.0,0.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0 );

	Matrix4x4 *p3 = new Matrix4x4(	0.0,-1.0,0.0,0.0,
										1.0,0.0,0.0,0.0,
										0.0,0.0,1.0,0.0,
										0.0,0.0,0.0,1.0 );

	Matrix4x4 *p2 = new Matrix4x4((*p1->Inverse()));
	CPPUNIT_ASSERT_EQUAL( (*p2), (*p3));

}
