/*
 * TCubeTests.cpp
 *
 *  Created on: 31/08/2011
 *      Author: sgarayalde
 */

#include "TCube.h"
#include "BBox.h"

#include <gtest/gtest.h>

TEST(TCubeTests, DefaultConstructor){


	TCube* cube = new TCube;

	EXPECT_DOUBLE_EQ(double(cube->m_depth.getValue()),2.0);
	EXPECT_DOUBLE_EQ(double(cube->m_height.getValue()),2.0);
	EXPECT_DOUBLE_EQ(double(cube->m_width.getValue()),2.0);
}

TEST(TCubeTests, TCubeGetArea){


	TCube* cube = new TCube;

	EXPECT_DOUBLE_EQ(cube->GetArea(),24);
}

TEST(TCubeTests, TCubeGetVolume){

    double x=10;
    double y=12;
    double z=8;

	TCube* cube = new TCube;
    cube->m_depth.setValue(x);
    cube->m_height.setValue(y);
    cube->m_width.setValue(z);

	EXPECT_DOUBLE_EQ(cube->GetVolume(),x*y*z);
}

TEST(TCubeTests, TCubeGetBBox){

    double x=10;
    double y=12;
    double z=8;

	TCube* cube = new TCube;
	cube->m_width.setValue(x);
    cube->m_height.setValue(y);
    cube->m_depth.setValue(z);

    //BBox box;

	EXPECT_DOUBLE_EQ(cube->GetBBox().pMin.x,-x/2.0);
	EXPECT_DOUBLE_EQ(cube->GetBBox().pMin.y,-y/2.0);
	EXPECT_DOUBLE_EQ(cube->GetBBox().pMin.z,-z/2.0);
	EXPECT_DOUBLE_EQ(cube->GetBBox().pMax.x,x/2.0);
	EXPECT_DOUBLE_EQ(cube->GetBBox().pMax.y,y/2.0);
	EXPECT_DOUBLE_EQ(cube->GetBBox().pMax.z,z/2.0);
}
