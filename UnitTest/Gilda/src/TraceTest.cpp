/*
 * TraceTest.cpp
 *
 *  Created on: Mar 9, 2009
 *      Author: ENGINEERING
 */
#include "TraceTest.h"
#include "Trace.h"
#include<cstring>
#include "Point3D.h"
#include <iostream>
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( TraceTest );


void TraceTest ::tearDown()
{


Trace trace(  " This test the trace constructor ", false);

}
void TraceTest ::setUp()
 {


 }


 void TraceTest ::testm_functionName()
 {

	    string trace1("This Test the Trace Constructor",false);
		Trace trace( "This Test the Trace Constructor",false);
		CPPUNIT_ASSERT_EQUAL(trace1, m_functionName);

}
void TraceTest::testm_functionTrace()
{


	bool trace1= false;
	Trace trace( "This Test the Trace Constructor",false);
	CPPUNIT_ASSERT_EQUAL(trace1,m_functionTrace);


}
void TraceTest::testlevel()
{
 static int level;

    CPPUNIT_ASSERT_EQUAL(level, 0);

}

void TraceTest::testTrace_Enabled()
{


    bool TraceEnabled = true;
	CPPUNIT_ASSERT_EQUAL(TraceEnabled,true);


}



