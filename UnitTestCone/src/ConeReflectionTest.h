#ifndef CONETEST_H_
#define CONETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class Point3D;
class Ray;
class ShapeCone;



class ConeReflectionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(ConeReflectionTest);
		CPPUNIT_TEST(testReflection);
		CPPUNIT_TEST(testGetPoint3D);

	CPPUNIT_TEST_SUITE_END();
public:

	void setUp();
	void tearDown();

protected:

	void testReflection();
	void testGetPoint3D();

private:

	ShapeCone* m_pCone;
};



#endif /*CONETEST_H_*/

