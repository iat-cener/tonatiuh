/***************************************************************************
 Copyright (C) 2008 by the Tonatiuh Software Development Team.

 This file is part of Tonatiuh.

 Tonatiuh program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.


 Acknowledgments:

 The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
 then Chair of the Department of Engineering of the University of Texas at
 Brownsville. From May 2004 to July 2008, it was supported by the Department
 of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
 the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
 During 2007, NREL also contributed to the validation of Tonatiuh under the
 framework of the Memorandum of Understanding signed with the Spanish
 National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
 Since June 2006, the development of Tonatiuh is being led by the CENER, under the
 direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

 Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

 Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola, Gilda Jimenez,
 Juana Amieva, Azael Mancillas, Cesar Cantu.
 ***************************************************************************/

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
