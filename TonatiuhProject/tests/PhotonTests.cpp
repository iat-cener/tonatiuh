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

#include "Photon.h"

#include <gtest/gtest.h>

#include "TestsAuxiliaryFunctions.h"
#include <time.h>

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 60000;
TEST(PhotonTests, DefaultConstructor){
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){

		Point3D point=taf::randomPoint(a,b);
		int side=int(taf::randomNumber(a,b));
		Photon ph( point, side, 0, 0 );
		EXPECT_DOUBLE_EQ( ph.id,0 );
		EXPECT_DOUBLE_EQ( ph.pos.x,point.x );
		EXPECT_DOUBLE_EQ( ph.pos.y,point.y );
		EXPECT_DOUBLE_EQ( ph.pos.z,point.z );
		EXPECT_DOUBLE_EQ( ph.side,side );
		//EXPECT_TRUE(Photon* ==0);
	}

}

TEST(PhotonTests, ConstructorFromPhoton){
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		Point3D point=taf::randomPoint(a,b);
		double side=taf::randomNumber(a,b);
		Photon ph( point, side, 0, 0 );
		Photon result(ph);
		EXPECT_DOUBLE_EQ( ph.id,result.id );
		EXPECT_DOUBLE_EQ( ph.pos.x,result.pos.x );
		EXPECT_DOUBLE_EQ( ph.pos.y,result.pos.y );
		EXPECT_DOUBLE_EQ( ph.pos.z,result.pos.z );
		EXPECT_DOUBLE_EQ( double(-1),double(result.side) );
	}
}


