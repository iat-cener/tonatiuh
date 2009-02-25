/*
 * TestPoint3D.cpp
 *
 *  Created on: Feb 9, 2009
 *      Author: ENGINEERING
 */
#include "Point3D.h"

 #include "TestPoint3D.h"
 #include "Vector3D.h"

 CPPUNIT_TEST_SUITE_REGISTRATION(TestPoint3D);


void TestPoint3D::setUp()
{
	l1 = new Point3D( 1.0, 2.0, 3.0 );
	l2 = new Point3D( 2.0, 5.0, 4.0 );
	l3 = new Point3D( 2-1, 2.0, 1.0+2.0 );

}

void TestPoint3D::tearDown()
{
	delete 	l1;
	delete	l2;
	delete	l3;

}
void TestPoint3D::TestEquality()
 {
	l4=new Point3D(1,2,3);
	CPPUNIT_ASSERT( *l1 == *l3);

 CPPUNIT_ASSERT_MESSAGE("Two points aren't equal",( l1->x == l3->x ) && ( l1->y == l3->y ) &&( l1->z == l3->z ) );

 }
 void TestPoint3D::TestAddPoints()
 {
 Point3D result;
 result = (*l1)+(*l3);
 Point3D answer(2.0,4.0,6.0);

 CPPUNIT_ASSERT_EQUAL(answer, result);
 }

 void TestPoint3D::TestAddVector()
 {
 Vector3D v1(1.0,1.0,1.0);
 Point3D result2;

 result2 = (*l2)+v1;

 Point3D answer2(3.0,6.0,5.0);


 CPPUNIT_ASSERT_EQUAL(answer2, result2);
 }

 void TestPoint3D::TestProduct()
 {
 double scalar = 2;
 Point3D result1;

 result1 = (*l1)*scalar;


 Point3D answer1(2.0,4.0,6.0);


 CPPUNIT_ASSERT_EQUAL(answer1, result1);


 result1 = scalar*(*l1);


 CPPUNIT_ASSERT_EQUAL(answer1, result1);


 }



void TestPoint3D::TestDistance()
{
	double answer=3.31662479;


	CPPUNIT_ASSERT_DOUBLES_EQUAL(Distance((*l1),(*l2)),answer,0.000000001);

}
void TestPoint3D::TestDistanceSquared()
{
	double answer=11.0;

CPPUNIT_ASSERT_DOUBLES_EQUAL(DistanceSquared((*l1),(*l2)),answer,0.000000001);

}

