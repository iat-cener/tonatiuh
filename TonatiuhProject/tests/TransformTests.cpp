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

TEST( TransformTests, ConstructorBidimensionalArray)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{

		double m[4][4];

		for( int i = 0; i < 4; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				m[i][j] = taf::randomNumber( a, b );
			}
		}
		Transform  t( m );

		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][0], m[0][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][1], m[0][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][2], m[0][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][3], m[0][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][0], m[1][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][1], m[1][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][2], m[1][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][3], m[1][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][0], m[2][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][1], m[2][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][2], m[2][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][3], m[2][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][0], m[3][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][1], m[3][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][2], m[3][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][3], m[3][3] );
	}

}
