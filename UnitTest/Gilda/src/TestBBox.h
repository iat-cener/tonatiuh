/*
 * TestBBox.h
 *
 *  Created on: Mar 10, 2009
 *      Author: ENGINEERING
 */


#ifndef TESTBBOX_H_
#define TESTBBOX_H_

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class BBox;


class TestBBox : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TestBBox);
		CPPUNIT_TEST(TestOverlaps);
		CPPUNIT_TEST(TestInside);
		CPPUNIT_TEST(TestExpand);
		CPPUNIT_TEST(TestVolume);
        CPPUNIT_TEST(TestBoundingSphere);
		CPPUNIT_TEST(TestIntersectP);
	   CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

protected:
	void TestOverlaps();
	void TestInside();
	void TestExpand();
	void TestVolume();
    void TestBoundingSphere();
	void TestIntersectP();


private:
	BBox* m_bBox;
	//BBox* zBBox;
	//BBox* nBBox;

};

#endif /* TESTBBOX_H_ */


