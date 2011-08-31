/*
* PhotonTests.cpp
,*
 *  Created on: 30/08/2011
 *      Author: sgarayalde
 */

#include "Photon.h"

#include <gtest/gtest.h>


TEST(PhotonTests, DefaultConstructor){

	Point3D point(124.2,685.3,762);
	Photon ph(point,25,0,0,0,0);
	EXPECT_DOUBLE_EQ( ph.id,0 );
	EXPECT_DOUBLE_EQ( ph.pos.x,124.2 );
	EXPECT_DOUBLE_EQ( ph.pos.y,685.3 );
	EXPECT_DOUBLE_EQ( ph.pos.z,762 );
	EXPECT_DOUBLE_EQ( double(ph.side),25 );
	//EXPECT_TRUE(Photon* ==0);

}

TEST(PhotonTests, ConstructorFromPhoton){

	Point3D point(124.2,685.3,762);
	Photon ph(point,25,0,0,0,0);
	Photon result(ph);
	EXPECT_DOUBLE_EQ( ph.id,result.id );
	EXPECT_DOUBLE_EQ( ph.pos.x,result.pos.x );
	EXPECT_DOUBLE_EQ( ph.pos.y,result.pos.y );
	EXPECT_DOUBLE_EQ( ph.pos.z,result.pos.z );
	EXPECT_DOUBLE_EQ( double(-1),double(result.side) );
}


