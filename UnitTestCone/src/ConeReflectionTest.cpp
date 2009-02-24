#include <cmath>

#include "ConeReflectionTest.h"
#include "DifferentialGeometry.h"
#include "Point3d.h"
#include "Ray.h"
#include "tgc.h"
#include "tgf.h"
#include "ShapeCone.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ConeReflectionTest);


void ConeReflectionTest::setUp()
{
	m_pCone = new ShapeCone();
	m_pCone->m_baseradius.setValue( 10.0);
	m_pCone->m_topradius.setValue( 2.0);
	m_pCone->m_height.setValue( 8.0);
	m_pCone->m_phiMax.setValue( tgc::TwoPi );
}

void ConeReflectionTest::tearDown()
{

}

void ConeReflectionTest::testReflection()
{
	Point3D* target = new Point3D( 0.0, 20.0, 5.0 );
	Ray* incident = new Ray ( Point3D(0.0, 5.0, 100.0), Vector3D( 0.0, 0.0, -1.0 ));

	double thit = 0.0;
	DifferentialGeometry* dg = new DifferentialGeometry;
	m_pCone->Intersect( *incident, &thit, dg );

	Ray* reflected  = new Ray();
	reflected->origin = dg->point;
	double cosTheta = DotProduct( dg->normal, incident->direction );
	reflected->direction = incident->direction - 2.0 * dg->normal * cosTheta;

	bool test = false;

	if ((fabs(((target->x - reflected->origin.x)*( reflected->direction.y ))-((target->y - reflected->origin.y)*( reflected->direction.x )))<1e-10)&& (fabs(((target->y - reflected->origin.y)*( reflected->direction.z ))-((target->z - reflected->origin.z)*( reflected->direction.y )))<1e-10)){test = true;}

	CPPUNIT_ASSERT_MESSAGE("Ray doesn't pass through target",test);
}


void ConeReflectionTest::testGetPoint3D(){

	Point3D result1=m_pCone->Sample(0.0,0.0);
	Point3D answer1( 2, 0, 8 );

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Error at test1",answer1, result1);

	Point3D result2 = m_pCone->Sample( 0.8, 0.3 );
	Point3D answer2(1.3596747752497686660500910356044,
					-4.1846486716986757173123330668693,
					5.6 );

	bool equal2= false;
	if ( ( fabs( result2.x - answer2.x ) < 1e-10 ) && ( fabs( result2.y - answer2.y ) < 1e-10 ) && ( fabs( result2.z - answer2.z ) < 1e-10 ) ) equal2 = true;

	CPPUNIT_ASSERT_MESSAGE("Error at test2", equal2);

	Point3D result3=m_pCone->Sample( 0.4, 0.7 );
	Point3D answer3( 	-6.1485291572496004231774299705894,
						4.467167917422795781682165255257,
						2.4 );


	bool equal3= false;
	if ( ( fabs( result3.x - answer3.x ) < 1e-10 ) && ( fabs( result3.y - answer3.y ) < 1e-10 ) && ( fabs( result3.z - answer3.z ) < 1e-10 ) ) equal3 = true;

	CPPUNIT_ASSERT_MESSAGE("Error at test3", equal3);
}

