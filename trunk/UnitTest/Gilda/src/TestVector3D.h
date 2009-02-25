/*
 * TestPoint3D.h
 *
 *  Created on: Feb 9, 2009
 *      Author: ENGINEERING
 */

#ifndef TESTVECTOR3D_H_
#define TESTVECTOR3D_H_
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class Vector3D;


class TestVector3D : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TestVector3D);
	CPPUNIT_TEST(testEquality);
	CPPUNIT_TEST(testInequality);
	CPPUNIT_TEST(testAddVector);
	CPPUNIT_TEST(testDotProduct);
	CPPUNIT_TEST(testDotProductWithNormal);
	CPPUNIT_TEST(testAbsDotProduct);
	CPPUNIT_TEST(testLengthSquared);
	CPPUNIT_TEST(testLength);
	CPPUNIT_TEST(testCrossProduct);
	CPPUNIT_TEST(testCrossProductWithNormal);
	CPPUNIT_TEST(testNormalize);
	CPPUNIT_TEST(testSameHemisphere);
	CPPUNIT_TEST(testZero);
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

protected:

	void testEquality();
	void testInequality();
	void testAddVector();
	void testDotProduct();
	void testDotProductWithNormal();
	void testAbsDotProduct();
	void testCrossProduct();
	void testCrossProductWithNormal();
	void testLengthSquared();
	void testLength();
	void testNormalize();
	void testSameHemisphere();
	void testZero();
private:

	Vector3D* p1;
	Vector3D* p2;
	Vector3D* p3;

};

#endif /*TESTVECTOR3D_H_*/








