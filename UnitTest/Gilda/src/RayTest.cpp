#include "RayTest.h"
#include "Ray.h"
#include"Point3D.h"
#include "Vector3D.h"
#include "tgc.h"
#include "Trace.h"
#include <iostream>


CPPUNIT_TEST_SUITE_REGISTRATION( RayTest );

class Vector3D;
class Point3D;
class Ray;

void RayTest ::setUp()
{


	Point3D origin( 0.0,0.0,0.0 );
	Vector3D direction( 0.0, 0.0, 0.0);
    double mint( 0.0);
    double maxt(0.0);
    Ray  T2(origin,direction,mint,maxt);

}
void RayTest ::tearDown()
{

//delete T2;

}

void RayTest:: test_direction()
{
	   Ray  T2(origin,direction,mint,maxt);
	   CPPUNIT_ASSERT_EQUAL(direction.x,0.0);
       CPPUNIT_ASSERT_EQUAL(direction.y,0.0);
       CPPUNIT_ASSERT_EQUAL(direction.z,0.0);

}
void RayTest::test_maxt()
{

	  Ray  T2(origin,direction,mint,maxt);
	  CPPUNIT_ASSERT_EQUAL(maxt,0.0);


}
void RayTest::test_mint()
{
	  Ray  T2(origin,direction,mint,maxt);
	  CPPUNIT_ASSERT_EQUAL(mint,0.0);
}
void RayTest::test_origin()
{
	   Ray  T2(origin,direction,mint,maxt);
		CPPUNIT_ASSERT_EQUAL(origin.x,0.0);
	    CPPUNIT_ASSERT_EQUAL(origin.y,0.0);
	    CPPUNIT_ASSERT_EQUAL(origin.z,0.0);


}
void RayTest::test_operator()
{
	Trace trace( "Ray operator<<" ,false );
}

