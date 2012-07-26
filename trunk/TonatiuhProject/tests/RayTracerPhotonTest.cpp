/*
 * RayTracerPhotonTest.cpp
 *
 *  Created on: 31/08/2011
 *      Author: sgarayalde
 */

#include <time.h>

#include <gtest/gtest.h>

#include "Photon.h"
#include "TestsAuxiliaryFunctions.h"

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 60000;

TEST(RayTracerPhotonTests,DefaultConstructor){

	srand ( time(NULL) );

		// Extension of the testing space
		double b = maximumCoordinate;
		double a = -b;

		for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){

				Point3D point = taf::randomPoint( a, b );
				int shape = int( taf::randomNumber(a,b) );
				double id=taf::randomNumber(a,b);
				Photon rPhoton( point, shape, id, 0 );

				EXPECT_DOUBLE_EQ( rPhoton.pos.x,point.x );
				EXPECT_DOUBLE_EQ( rPhoton.pos.y,point.y );
				EXPECT_DOUBLE_EQ( rPhoton.pos.z,point.z );
				EXPECT_DOUBLE_EQ( rPhoton.side, shape );
				EXPECT_DOUBLE_EQ( rPhoton.id, id );
		}

}

TEST(RayTracerPhotonTests,ConstructorUsingRayTracerPhoton){

	srand ( time(NULL) );

		// Extension of the testing space
		double b = maximumCoordinate;
		double a = -b;

		for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){

				Point3D point = taf::randomPoint( a, b );
				int shape = int( taf::randomNumber( a, b ) );
				double id = taf::randomNumber( a, b );
				Photon rPhoton( point, shape, id, 0 );
				Photon result( rPhoton );
				EXPECT_DOUBLE_EQ( rPhoton.pos.x ,result.pos.x );
				EXPECT_DOUBLE_EQ( rPhoton.pos.y, result.pos.y );
				EXPECT_DOUBLE_EQ( rPhoton.pos.z, result.pos.z );
				EXPECT_DOUBLE_EQ( rPhoton.side, result.side );
				EXPECT_DOUBLE_EQ( rPhoton.id, result.id );
		}

}
