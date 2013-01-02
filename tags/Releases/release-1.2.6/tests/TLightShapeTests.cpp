/*
 * TLightShapeTests.cpp
 *
 *  Created on: 01/09/2011
 *      Author: sgarayalde
 */

#include "TLightShape.h"
#include "Point3D.h"

#include <gtest/gtest.h>
#include "TestsAuxiliaryFunctions.h"
#include <time.h>

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 60000;

TEST(TLightShapeTests , DefaultConstructor){

	TLightShape* shape= new TLightShape;

	EXPECT_DOUBLE_EQ(double(shape->xMin.getValue()),-0.5);
	EXPECT_DOUBLE_EQ(double(shape->xMax.getValue()),0.5);
	EXPECT_DOUBLE_EQ(double(shape->zMin.getValue()),-0.5);
	EXPECT_DOUBLE_EQ(double(shape->zMax.getValue()),0.5);
	EXPECT_DOUBLE_EQ(double(shape->delta.getValue()),100.0);
}
