/*
 * DifferentialGeometryTests.cpp
 *
 *  Created on: 31/08/2011
 *      Author: sgarayalde
 */

#include "DifferentialGeometry.h"

#include <gtest/gtest.h>

#include "TestsAuxiliaryFunctions.h"
#include <time.h>

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 60000;

TEST(DiferencialGeometryTests, DesfaultConstructor)
{
	DifferentialGeometry dif;
	EXPECT_DOUBLE_EQ( dif.u,0.0 );
	EXPECT_DOUBLE_EQ( dif.v,0.0 );

}

TEST(DiferencialGeometryTests, Constructor)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		Point3D point=taf::randomPoint(a,b);
		Vector3D dpdu(taf::randomPoint(a,b));
		Vector3D dpdv(taf::randomPoint(a,b));
		Vector3D dndu(taf::randomPoint(a,b));
		Vector3D dndv(taf::randomPoint(a,b));
		double u = taf::randomNumber(a,b);
		double v = taf::randomNumber(a,b);
		TShape* shape = 0;

		DifferentialGeometry dif(point,dpdu,dpdv,dndu,dndv,u,v,shape);

		EXPECT_DOUBLE_EQ( dif.u,u );
		EXPECT_DOUBLE_EQ( dif.v,v );
		EXPECT_DOUBLE_EQ( dif.point.x,point.x );
		EXPECT_DOUBLE_EQ( dif.point.y,point.y );
		EXPECT_DOUBLE_EQ( dif.point.z,point.z );
		EXPECT_DOUBLE_EQ( dif.dpdu.x,dpdu.x);
		EXPECT_DOUBLE_EQ( dif.dpdu.y,dpdu.y);
		EXPECT_DOUBLE_EQ( dif.dpdu.z,dpdu.z);
		EXPECT_DOUBLE_EQ( dif.dpdv.x,dpdv.x);
		EXPECT_DOUBLE_EQ( dif.dpdv.y,dpdv.y);
		EXPECT_DOUBLE_EQ( dif.dpdv.z,dpdv.z);
		EXPECT_DOUBLE_EQ( dif.dndu.x,dndu.x);
		EXPECT_DOUBLE_EQ( dif.dndu.y,dndu.y);
		EXPECT_DOUBLE_EQ( dif.dndu.z,dndu.z);
		EXPECT_DOUBLE_EQ( dif.dndv.x,dndv.x);
		EXPECT_DOUBLE_EQ( dif.dndv.y,dndv.y);
		EXPECT_DOUBLE_EQ( dif.dndv.z,dndv.z);
		EXPECT_DOUBLE_EQ( dif.normal.x, Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) ).x);
		EXPECT_DOUBLE_EQ( dif.normal.y, Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) ).y);
		EXPECT_DOUBLE_EQ( dif.normal.z, Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) ).z);
	}

}
