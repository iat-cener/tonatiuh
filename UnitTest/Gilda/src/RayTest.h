
#ifndef RAYTEST_H_
#define RAYTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include "Point3D.h"
#include "Vector3D.h"

struct Point3D;
struct Vector3D;
class Ray;

class RayTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(RayTest);
    CPPUNIT_TEST(test_direction);
    CPPUNIT_TEST(test_maxt);
    CPPUNIT_TEST(test_mint);
    CPPUNIT_TEST(test_origin);
    CPPUNIT_TEST(test_operator);
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

protected:

	void test_direction();
	void test_maxt();
	void test_mint();
	void test_origin();
	void test_operator();



private:


    Point3D origin;
    Vector3D direction;
    mutable double mint;
    mutable double maxt;

};

#endif

