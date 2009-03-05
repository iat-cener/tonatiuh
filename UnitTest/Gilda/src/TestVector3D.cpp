#include <cmath>

 #include "NormalVector.h"
 #include "TestVector3D.h"
 #include "Vector3D.h"


 CPPUNIT_TEST_SUITE_REGISTRATION( TestVector3D );

 void TestVector3D::setUp()
 {
 p1=new Vector3D( 1.0, 1.0, 1.0);
 p2=new Vector3D( 2.0, 2.0, 2.0);
 p3=new Vector3D( 3.0, 3.0, 3.0);
 }

 void TestVector3D::tearDown()
 {
 delete p1;
 delete p2;
 delete p3;
 }

 void TestVector3D::testEquality()
 {
 Vector3D test( 1.0, 1.0, 1.0);
 CPPUNIT_ASSERT( test == (*p1) );
 CPPUNIT_ASSERT( !(test == (*p2) ) );

 }

 void TestVector3D::testInequality()
 {
 Vector3D test(1.0,1.0,1.0);
 CPPUNIT_ASSERT( !(test != (*p1) ) );
 CPPUNIT_ASSERT( test != (*p2) );
 }

 void TestVector3D::testAddVector()
 {
 Vector3D result;
 result = (*p1)+(*p2);
 Vector3D answer(3.0,3.0,3.0);
 CPPUNIT_ASSERT_EQUAL(answer, result);
 }

 void TestVector3D::testDotProduct()
 {
 double result = DotProduct((*p1), (*p2));
 double answer = 6.0;
 CPPUNIT_ASSERT_EQUAL(result,answer);
 }

 void TestVector3D::testCrossProduct()
 {

 Vector3D answer( 0,0,0 );
 Vector3D result = CrossProduct(*p1,*p2);
 CPPUNIT_ASSERT( result == answer );
 }

 void TestVector3D::testAbsDotProduct()
 {

 double answer = 6.0;
 double result = AbsDotProduct(*p1,*p2);

 CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON);
 }

 void TestVector3D::testDotProductWithNormal()
 {
	 Vector3D vect(3.1,-1.0,2.4);
	 NormalVector norm(4.3,1.9,2.7);
	 double answer = 17.91;
	 double result = DotProduct(norm,vect);

	 CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON);
 }

 void TestVector3D::testLength()
 {
 Vector3D t(1.0,1.0,1.0);
 double answer = sqrt(3.0);
 double result = t.Length();

 CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON );
 }

 void TestVector3D::testLengthSquared()
 {
 Vector3D t(1.0,1.0,1.0);
 double answer = 3.0;
 double result = t.LengthSquared();

 CPPUNIT_ASSERT_EQUAL(result, answer);
 }

 void TestVector3D::testCrossProductWithNormal()
 {
 NormalVector norm(1.0,1.0,-1.0 );

 //Cross poduct n x p1
 //Vector3D answer1( 5.0, 19.8, -12.5);
 //Vector3D result1 = CrossProduct( norm, *p1 );

 //Cross poduct p1 x n
 Vector3D answer2( -2.0,2.0,0);
 Vector3D result2 = CrossProduct( *p1, norm );

 CPPUNIT_ASSERT_EQUAL(result2, answer2);

 }

 void TestVector3D::testNormalize()
 {
  Vector3D t1 (2.0,2.0,0.0);
  Vector3D answer (0.7071067811865475,0.7071067811865475,0.0);
 Vector3D result = Normalize (t1);

 CPPUNIT_ASSERT_EQUAL(answer, result);
 }

 void TestVector3D::testSameHemisphere()
 {
 Vector3D p4(0.0,0.0,0.0);
 bool test = true;
 if ( CrossProduct(( *p1),(*p2)) == 0)
CPPUNIT_ASSERT_MESSAGE("The vector is in the same hemisphere",test);

 else test = false;
 }
 void TestVector3D::testZero()

 {
	Vector3D p5(0.0,0.0,0.0);
	bool test = true;
	if(p5.x = 0,p5.y = 0,p5.z = 0)
	CPPUNIT_ASSERT_MESSAGE("The vector is zero",test);
 }
