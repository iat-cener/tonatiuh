/*
 * TLightKitTests.cpp
 *
 *  Created on: 13/04/2011
 *      Author: amutuberria
 */

#include <time.h>

#include <Inventor/nodes/SoTransform.h>

#include <gtest/gtest.h>

#include "gc.h"

#include "TLightKit.h"
#include "TestsAuxiliaryFunctions.h"

const unsigned long int maximumNumberOfTests = 1000;

TEST( TLightKitTests, ConstructorDefault)
{
	TLightKit* light = new TLightKit;

	EXPECT_TRUE( light->azimuth.getValue() == 0.0);
	EXPECT_TRUE( light->zenith.getValue() == 0.0);
	//EXPECT_TRUE( t.m_minv == 0);
}

TEST( TLightKitTests, ChangePosition)
{
	srand ( time(NULL) );
	double minA = 0;
	double maxA = gc::TwoPi;

	double minZ = -gc::Pi;
	double maxZ = gc::Pi;

	TLightKit* light = new TLightKit;
	SoTransform* lightTransform = static_cast< SoTransform* >( light->getPart( "transform", true ) );

	SbVec3f scaleFactor( 1, 1, 1 );

	for( unsigned int i = 0; i < maximumNumberOfTests; i++ )
	{
		double azimuth = taf::randomNumber( minA, maxA );
		double zenith = taf::randomNumber( minZ, maxZ );

		light->ChangePosition( azimuth, zenith );

		EXPECT_PRED_FORMAT2(::testing::DoubleLE, ( light->azimuth.getValue() - azimuth ) / azimuth, 1e-6);
		EXPECT_PRED_FORMAT2(::testing::DoubleLE, ( light->zenith.getValue() - zenith ) / zenith, 1e-6);
		EXPECT_TRUE( lightTransform->scaleFactor.getValue() == scaleFactor );
	}

}
