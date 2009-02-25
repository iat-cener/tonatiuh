/*
 * TestPoint3D.h
 *
 *  Created on: Feb 9, 2009
 *      Author: ENGINEERING
 */

#ifndef TESTPOINT3D_H_
#define TESTPOINT3D_H

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class Point3D;


class TestPoint3D : public CPPUNIT_NS::TestFixture
{

		CPPUNIT_TEST_SUITE(TestPoint3D);
		CPPUNIT_TEST(TestEquality);
		CPPUNIT_TEST(TestAddPoints);
		CPPUNIT_TEST(TestAddVector);
		CPPUNIT_TEST(TestProduct);
		CPPUNIT_TEST(TestDistance);
		CPPUNIT_TEST(TestDistanceSquared);
		CPPUNIT_TEST_SUITE_END();


public:

	void setUp();
	void tearDown();

protected:
	void TestEquality();
	void TestAddPoints();
	void TestAddVector();
	void TestProduct();
	void TestDistance();
	void TestDistanceSquared();

private:

	Point3D* l1;
	Point3D* l2;
	Point3D* l3;
	Point3D* l4;
};
#endif /* TESTPOINT3D_H_ */
