/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
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


