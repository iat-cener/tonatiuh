/*
 * RayTracerPhotonTest.cpp
 *
 *  Created on: 31/08/2011
 *      Author: sgarayalde
 */

#include "RayTracerPhoton.h"

#include <gtest/gtest.h>

TEST(RayTracerPhotonTests,DefaultConstructor){

	Point3D point(752.2,52.123,5854);
	int shape=345;
	double id=120;
	RayTracerPhoton rPhoton(point,shape,id,0);

	EXPECT_DOUBLE_EQ( rPhoton.pos.x,point.x );
	EXPECT_DOUBLE_EQ( rPhoton.pos.y,point.y );
	EXPECT_DOUBLE_EQ( rPhoton.pos.z,point.z );
	EXPECT_DOUBLE_EQ( double(rPhoton.shapeFront),double(shape));
	EXPECT_DOUBLE_EQ( rPhoton.id,120);

}

TEST(RayTracerPhotonTests,ConstructorUsingRayTracerPhoton){

	Point3D point(752.2,52.123,5854);
	int shape=345;
	double id=120;
	RayTracerPhoton rPhoton(point,shape,id,0);
    RayTracerPhoton result(rPhoton);
	EXPECT_DOUBLE_EQ( rPhoton.pos.x,result.pos.x );
	EXPECT_DOUBLE_EQ( rPhoton.pos.y,result.pos.y );
	EXPECT_DOUBLE_EQ( rPhoton.pos.z,result.pos.z );
	EXPECT_DOUBLE_EQ( double(rPhoton.shapeFront),double(result.shapeFront));
	EXPECT_DOUBLE_EQ( rPhoton.id,result.id);

}
