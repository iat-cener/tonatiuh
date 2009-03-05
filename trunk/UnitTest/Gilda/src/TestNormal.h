/*
 * TestNormal.h
 *
 *  Created on: Mar 2, 2009
 *      Author: ENGINEERING
 */

#ifndef TESTNORMAL_H_
#define TESTNORMAL_H_
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class NormalVector;


class TestNormalVector : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TestNormalVector);
	CPPUNIT_TEST(testEquality);
	CPPUNIT_TEST(testAddVector);
	CPPUNIT_TEST(testDotProduct);
	CPPUNIT_TEST(testAbsDotProduct);
	CPPUNIT_TEST(testLengthSquared);
	CPPUNIT_TEST(testLength);

	CPPUNIT_TEST(testNormalize);
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

protected:

	void testEquality();
	void testAddVector();
	void testDotProduct();
	void testAbsDotProduct();
	void testLengthSquared();
	void testLength();
	void testNormalize();

private:

	NormalVector* p1;
	NormalVector* p2;
	NormalVector* p3;

};


#endif /* TESTNORMAL_H_ */
