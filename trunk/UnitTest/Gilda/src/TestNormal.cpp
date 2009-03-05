#include <cmath>
#include "NormalVector.h"
#include "TestNormal.h"
#include "Vector3D.h"


 CPPUNIT_TEST_SUITE_REGISTRATION( TestNormalVector);

 void TestNormalVector::setUp()
 {
 p1=new NormalVector( 1.0, 1.0, 1.0);
 p2=new NormalVector( 2.0, 2.0, 2.0);
 p3=new NormalVector( 3.0, 3.0, 3.0);
 }

 void TestNormalVector::tearDown()
 {
 delete p1;
 delete p2;
 delete p3;
 }

 void TestNormalVector::testEquality()
 {
 NormalVector test( 1.0, 1.0, 1.0);
 CPPUNIT_ASSERT( test == (*p1) );
 CPPUNIT_ASSERT( !(test == (*p2) ) );

 }

 void TestNormalVector::testAddVector()
 {
 NormalVector result;
 result = (*p1)+(*p2);
 NormalVector answer(3.0,3.0,3.0);
 CPPUNIT_ASSERT_EQUAL(answer, result);
 }

 void TestNormalVector::testDotProduct()
 {
 //double result = DotProduct((*p1), (*p2));
 //double answer = 6.0;
 //CPPUNIT_ASSERT_EQUAL(result,answer);

 NormalVector vect(3.1,-1.0,2.4);
 NormalVector norm(4.3,1.9,2.7);
 double answer = 17.91;
 double result = DotProduct(norm,vect);

 CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON);


 }


 void TestNormalVector::testAbsDotProduct()
 {

 double answer = 6.0;
 double result = AbsDotProduct(*p1,*p2);

 CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON);

 }


 void TestNormalVector::testLength()
 {
 NormalVector t(1.0,1.0,1.0);
 double answer = sqrt(3.0);
 double result = t.Length();

 CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON );
 }

 void TestNormalVector::testLengthSquared()
 {
 NormalVector t(1.0,1.0,1.0);
 double answer = 3.0;
 double result = t.LengthSquared();

 CPPUNIT_ASSERT_EQUAL(result, answer);
 }
 void TestNormalVector::testNormalize()
 {

 NormalVector t1 (2.0,2.0,0.0);
 NormalVector answer (0.7071067811865475,0.7071067811865475,0.0);
 NormalVector result = Normalize (t1);

 CPPUNIT_ASSERT_EQUAL(answer, result);

 }

