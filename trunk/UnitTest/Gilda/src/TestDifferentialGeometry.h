/*
 * TestDifferentialGeometry.h
 *
 *  Created on: Mar 10, 2009
 *      Author: ENGINEERING
 */

#ifndef TESTDIFFERENTIALGEOMETRY_H_


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class DifferentialGeometry;


class TestDifferentialGeometry : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TestDifferentialGeometry);
		CPPUNIT_TEST(TestNormal);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

protected:
	void TestNormal();

private:
	DifferentialGeometry* m_differentialGeometry;

};

#endif /* TESTDIFFERENTIALGEOMETRY_H_ */



