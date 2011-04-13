/*
 * TransformTests.cpp
 *
 *  Created on: 13/04/2011
 *      Author: amutuberria
 */

#include <gtest/gtest.h>

#include "Transform.h"
#include "TestsAuxiliaryFunctions.h"

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 1000;

TEST( TransformTests, ConstructorDefault)
{
	Transform	t;

	EXPECT_TRUE( t.GetMatrix() == 0);
	//EXPECT_TRUE( t.m_minv == 0);
}
