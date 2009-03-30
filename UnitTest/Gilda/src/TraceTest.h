/*
 * TraceTest.h
 *
 *  Created on: Mar 9, 2009
 *      Author: ENGINEERING
 */

#ifndef TRACETEST_H_
#define TRACETEST_H_

#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class Trace;


class TraceTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TraceTest);
	CPPUNIT_TEST(testm_functionName);
	CPPUNIT_TEST(testm_functionTrace);
	CPPUNIT_TEST(testlevel);
	CPPUNIT_TEST(testTrace_Enabled);
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

protected:

	void testm_functionName();
    void testm_functionTrace();
    void testlevel();
    void testTrace_Enabled();

private:

	 static int level;
	 static bool TraceEnabled;
	 std::string m_functionName;
	 bool m_functionTrace;
	// Point3D* p1;
};











#endif /* TRACETEST_H_ */
