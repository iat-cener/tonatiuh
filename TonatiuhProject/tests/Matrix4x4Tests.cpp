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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.
tgc
Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <time.h>
#include <gtest/gtest.h>

#include "Matrix4x4.h"
#include "TestsAuxiliaryFunctions.h"
#include "tgc.h"

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 1000;

TEST( Matrix4x4Tests, ConstructorDefault)
{
	Matrix4x4	matrix;

	EXPECT_DOUBLE_EQ( matrix.m[0][0], 1.0 );
	EXPECT_DOUBLE_EQ( matrix.m[0][1], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[0][2], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[0][3], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[1][0], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[1][1], 1.0 );
	EXPECT_DOUBLE_EQ( matrix.m[1][2], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[1][3], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[2][0], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[2][1], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[2][2], 1.0 );
	EXPECT_DOUBLE_EQ( matrix.m[2][3], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[3][0], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[3][1], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[3][2], 0.0 );
	EXPECT_DOUBLE_EQ( matrix.m[3][3], 1.0 );
}

TEST( Matrix4x4Tests, ConstructorBidimensionalArray)
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
		Matrix4x4 matrix1( m );

		EXPECT_DOUBLE_EQ( matrix1.m[0][0], m[0][0] );
		EXPECT_DOUBLE_EQ( matrix1.m[0][1], m[0][1] );
		EXPECT_DOUBLE_EQ( matrix1.m[0][2], m[0][2] );
		EXPECT_DOUBLE_EQ( matrix1.m[0][3], m[0][3] );
		EXPECT_DOUBLE_EQ( matrix1.m[1][0], m[1][0] );
		EXPECT_DOUBLE_EQ( matrix1.m[1][1], m[1][1] );
		EXPECT_DOUBLE_EQ( matrix1.m[1][2], m[1][2] );
		EXPECT_DOUBLE_EQ( matrix1.m[1][3], m[1][3] );
		EXPECT_DOUBLE_EQ( matrix1.m[2][0], m[2][0] );
		EXPECT_DOUBLE_EQ( matrix1.m[2][1], m[2][1] );
		EXPECT_DOUBLE_EQ( matrix1.m[2][2], m[2][2] );
		EXPECT_DOUBLE_EQ( matrix1.m[2][3], m[2][3] );
		EXPECT_DOUBLE_EQ( matrix1.m[3][0], m[3][0] );
		EXPECT_DOUBLE_EQ( matrix1.m[3][1], m[3][1] );
		EXPECT_DOUBLE_EQ( matrix1.m[3][2], m[3][2] );
		EXPECT_DOUBLE_EQ( matrix1.m[3][3], m[3][3] );
	}

}

TEST( Matrix4x4Tests, ConstructorDoubleElements)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{
		double t00 = taf::randomNumber( a, b );
		double t01 = taf::randomNumber( a, b );
		double t02 = taf::randomNumber( a, b );
		double t03 = taf::randomNumber( a, b );
		double t10 = taf::randomNumber( a, b );
		double t11 = taf::randomNumber( a, b );
		double t12 = taf::randomNumber( a, b );
		double t13 = taf::randomNumber( a, b );
		double t20 = taf::randomNumber( a, b );
		double t21 = taf::randomNumber( a, b );
		double t22 = taf::randomNumber( a, b );
		double t23 = taf::randomNumber( a, b );
		double t30 = taf::randomNumber( a, b );
		double t31 = taf::randomNumber( a, b );
		double t32 = taf::randomNumber( a, b );
		double t33 = taf::randomNumber( a, b );


		Matrix4x4 matrix1( t00, t01, t02, t03,
		t10, t11, t12, t13,
		t20, t21, t22, t23,
		t30, t31, t32, t33 );

		EXPECT_DOUBLE_EQ( matrix1.m[0][0], t00 );
		EXPECT_DOUBLE_EQ( matrix1.m[0][1], t01 );
		EXPECT_DOUBLE_EQ( matrix1.m[0][2], t02 );
		EXPECT_DOUBLE_EQ( matrix1.m[0][3], t03 );
		EXPECT_DOUBLE_EQ( matrix1.m[1][0], t10 );
		EXPECT_DOUBLE_EQ( matrix1.m[1][1], t11 );
		EXPECT_DOUBLE_EQ( matrix1.m[1][2], t12 );
		EXPECT_DOUBLE_EQ( matrix1.m[1][3], t13 );
		EXPECT_DOUBLE_EQ( matrix1.m[2][0], t20 );
		EXPECT_DOUBLE_EQ( matrix1.m[2][1], t21 );
		EXPECT_DOUBLE_EQ( matrix1.m[2][2], t22 );
		EXPECT_DOUBLE_EQ( matrix1.m[2][3], t23 );
		EXPECT_DOUBLE_EQ( matrix1.m[3][0], t30 );
		EXPECT_DOUBLE_EQ( matrix1.m[3][1], t31 );
		EXPECT_DOUBLE_EQ( matrix1.m[3][2], t32 );
		EXPECT_DOUBLE_EQ( matrix1.m[3][3], t33 );
	}

}
TEST( Matrix4x4Tests, ConstructorCopy)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{
		Matrix4x4 matrix1;

		for( int i = 0; i < 4; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				matrix1.m[i][j] = taf::randomNumber( a, b );

			}
		}

		Matrix4x4 matrix2( matrix1 );

		EXPECT_DOUBLE_EQ( matrix1.m[0][0], matrix2.m[0][0]);
		EXPECT_DOUBLE_EQ( matrix1.m[0][1], matrix2.m[0][1] );
		EXPECT_DOUBLE_EQ( matrix1.m[0][2], matrix2.m[0][2] );
		EXPECT_DOUBLE_EQ( matrix1.m[0][3], matrix2.m[0][3] );
		EXPECT_DOUBLE_EQ( matrix1.m[1][0], matrix2.m[1][0] );
		EXPECT_DOUBLE_EQ( matrix1.m[1][1], matrix2.m[1][1] );
		EXPECT_DOUBLE_EQ( matrix1.m[1][2], matrix2.m[1][2] );
		EXPECT_DOUBLE_EQ( matrix1.m[1][3], matrix2.m[1][3] );
		EXPECT_DOUBLE_EQ( matrix1.m[2][0], matrix2.m[2][0] );
		EXPECT_DOUBLE_EQ( matrix1.m[2][1], matrix2.m[2][1] );
		EXPECT_DOUBLE_EQ( matrix1.m[2][2], matrix2.m[2][2] );
		EXPECT_DOUBLE_EQ( matrix1.m[2][3], matrix2.m[2][3] );
		EXPECT_DOUBLE_EQ( matrix1.m[3][0], matrix2.m[3][0] );
		EXPECT_DOUBLE_EQ( matrix1.m[3][1], matrix2.m[3][1] );
		EXPECT_DOUBLE_EQ( matrix1.m[3][2], matrix2.m[3][2] );
		EXPECT_DOUBLE_EQ( matrix1.m[3][3], matrix2.m[3][3] );
	}

}
TEST( Matrix4x4Tests, Transpose )
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{
		Matrix4x4 matrix1;
		Matrix4x4 matrix2;

		for( int i = 0; i < 4; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				double d = taf::randomNumber( a, b );

				matrix1.m[i][j] = d;
				matrix2.m[j][i] = d;

			}
		}

		Ptr<Matrix4x4> tMatrix1 = matrix1.Transpose();

	  EXPECT_TRUE( *tMatrix1 == matrix2 );
	}

}
TEST( Matrix4x4Tests, Inverse )
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{
		Matrix4x4 matrix1;

		for( int i = 0; i < 4; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				double d = taf::randomNumber( a, b );

				matrix1.m[i][j] = d;
			}
		}

		//Matrix determinant
		double det = matrix1.m[0][3] * matrix1.m[1][2] * matrix1.m[2][1] * matrix1.m[3][0]
		            - matrix1.m[0][2] * matrix1.m[1][3] * matrix1.m[2][1] * matrix1.m[3][0]
		            - matrix1.m[0][3] * matrix1.m[1][1] * matrix1.m[2][2] * matrix1.m[3][0]
		            + matrix1.m[0][1] * matrix1.m[1][3] * matrix1.m[2][2] * matrix1.m[3][0]
		            + matrix1.m[0][2] * matrix1.m[1][1] * matrix1.m[2][3] * matrix1.m[3][0]
		            - matrix1.m[0][1] * matrix1.m[1][2] * matrix1.m[2][3] * matrix1.m[3][0]
		            - matrix1.m[0][3] * matrix1.m[1][2] * matrix1.m[2][0] * matrix1.m[3][1]
		            + matrix1.m[0][2] * matrix1.m[1][3] * matrix1.m[2][0] * matrix1.m[3][1]
		            + matrix1.m[0][3] * matrix1.m[1][0] * matrix1.m[2][2] * matrix1.m[3][1]
		            - matrix1.m[0][0] * matrix1.m[1][3] * matrix1.m[2][2] * matrix1.m[3][1]
		            - matrix1.m[0][2] * matrix1.m[1][0] * matrix1.m[2][3] * matrix1.m[3][1]
		            + matrix1.m[0][0] * matrix1.m[1][2] * matrix1.m[2][3] * matrix1.m[3][1]
		            + matrix1.m[0][3] * matrix1.m[1][1] * matrix1.m[2][0] * matrix1.m[3][2]
		            - matrix1.m[0][1] * matrix1.m[1][3] * matrix1.m[2][0] * matrix1.m[3][2]
		            - matrix1.m[0][3] * matrix1.m[1][0] * matrix1.m[2][1] * matrix1.m[3][2]
		            + matrix1.m[0][0] * matrix1.m[1][3] * matrix1.m[2][1] * matrix1.m[3][2]
		            + matrix1.m[0][1] * matrix1.m[1][0] * matrix1.m[2][3] * matrix1.m[3][2]
		            - matrix1.m[0][0] * matrix1.m[1][1] * matrix1.m[2][3] * matrix1.m[3][2]
		            - matrix1.m[0][2] * matrix1.m[1][1] * matrix1.m[2][0] * matrix1.m[3][3]
		            + matrix1.m[0][1] * matrix1.m[1][2] * matrix1.m[2][0] * matrix1.m[3][3]
		            + matrix1.m[0][2] * matrix1.m[1][0] * matrix1.m[2][1] * matrix1.m[3][3]
		            - matrix1.m[0][0] * matrix1.m[1][2] * matrix1.m[2][1] * matrix1.m[3][3]
		            - matrix1.m[0][1] * matrix1.m[1][0] * matrix1.m[2][2] * matrix1.m[3][3]
		            + matrix1.m[0][0] * matrix1.m[1][1] * matrix1.m[2][2] * matrix1.m[3][3];

		if( std::fabs( det ) > tgc::Epsilon )
		{
			double cofactor[4][4];

			cofactor[0][0]= -matrix1.m[1][3] * matrix1.m[2][2] * matrix1.m[3][1] + matrix1.m[1][2] * matrix1.m[2][3] * matrix1.m[3][1]
						   + matrix1.m[1][3] * matrix1.m[2][1] * matrix1.m[3][2] - matrix1.m[1][1] * matrix1.m[2][3] * matrix1.m[3][2]
						   - matrix1.m[1][2] * matrix1.m[2][1] * matrix1.m[3][3] + matrix1.m[1][1] * matrix1.m[2][2] * matrix1.m[3][3];

			cofactor[0][1]= - ( -matrix1.m[1][3] * matrix1.m[2][2] * matrix1.m[3][0] + matrix1.m[1][2] * matrix1.m[2][3] * matrix1.m[3][0]
						   + matrix1.m[1][3] * matrix1.m[2][0] * matrix1.m[3][2] - matrix1.m[1][0] * matrix1.m[2][3] * matrix1.m[3][2]
						   - matrix1.m[1][2] * matrix1.m[2][0] * matrix1.m[3][3] + matrix1.m[1][0] * matrix1.m[2][2] * matrix1.m[3][3] );

			cofactor[0][2]= -matrix1.m[1][3] * matrix1.m[2][1] * matrix1.m[3][0] + matrix1.m[1][1] * matrix1.m[2][3] * matrix1.m[3][0]
						   + matrix1.m[1][3] * matrix1.m[2][0] * matrix1.m[3][1] - matrix1.m[1][0] * matrix1.m[2][3] * matrix1.m[3][1]
						   - matrix1.m[1][1] * matrix1.m[2][0] * matrix1.m[3][3] + matrix1.m[1][0] * matrix1.m[2][1] * matrix1.m[3][3];

			cofactor[0][3]=  - ( -matrix1.m[1][2] * matrix1.m[2][1] * matrix1.m[3][0] + matrix1.m[1][1] * matrix1.m[2][2] * matrix1.m[3][0]
						   + matrix1.m[1][2] * matrix1.m[2][0] * matrix1.m[3][1] - matrix1.m[1][0] * matrix1.m[2][2] * matrix1.m[3][1]
						   - matrix1.m[1][1] * matrix1.m[2][0] * matrix1.m[3][2] + matrix1.m[1][0] * matrix1.m[2][1] * matrix1.m[3][2] );

			cofactor[1][0]= - (  -matrix1.m[0][3] * matrix1.m[2][2] * matrix1.m[3][1] + matrix1.m[0][2] * matrix1.m[2][3] * matrix1.m[3][1]
						   + matrix1.m[0][3] * matrix1.m[2][1] * matrix1.m[3][2] - matrix1.m[0][1] * matrix1.m[2][3] * matrix1.m[3][2]
						   - matrix1.m[0][2] * matrix1.m[2][1] * matrix1.m[3][3] + matrix1.m[0][1] * matrix1.m[2][2] * matrix1.m[3][3] );

			cofactor[1][1]= -matrix1.m[0][3] * matrix1.m[2][2] * matrix1.m[3][0] + matrix1.m[0][2] * matrix1.m[2][3] * matrix1.m[3][0]
						   + matrix1.m[0][3] * matrix1.m[2][0] * matrix1.m[3][2] - matrix1.m[0][0] * matrix1.m[2][3] * matrix1.m[3][2]
						   - matrix1.m[0][2] * matrix1.m[2][0] * matrix1.m[3][3] + matrix1.m[0][0] * matrix1.m[2][2] * matrix1.m[3][3];

			cofactor[1][2]=  - ( -matrix1.m[0][3] * matrix1.m[2][1] * matrix1.m[3][0] + matrix1.m[0][1] * matrix1.m[2][3] * matrix1.m[3][0]
						   + matrix1.m[0][3] * matrix1.m[2][0] * matrix1.m[3][1] - matrix1.m[0][0] * matrix1.m[2][3] * matrix1.m[3][1]
						   - matrix1.m[0][1] * matrix1.m[2][0] * matrix1.m[3][3] + matrix1.m[0][0] * matrix1.m[2][1] * matrix1.m[3][3] );

			cofactor[1][3]= -matrix1.m[0][2] * matrix1.m[2][1] * matrix1.m[3][0] + matrix1.m[0][1] * matrix1.m[2][2] * matrix1.m[3][0]
						   + matrix1.m[0][2] * matrix1.m[2][0] * matrix1.m[3][1] - matrix1.m[0][0] * matrix1.m[2][2] * matrix1.m[3][1]
						   - matrix1.m[0][1] * matrix1.m[2][0] * matrix1.m[3][2] + matrix1.m[0][0] * matrix1.m[2][1] * matrix1.m[3][2];

			cofactor[2][0]= -matrix1.m[0][3] * matrix1.m[1][2] * matrix1.m[3][1] + matrix1.m[0][2] * matrix1.m[1][3] * matrix1.m[3][1]
						   + matrix1.m[0][3] * matrix1.m[1][1] * matrix1.m[3][2] - matrix1.m[0][1] * matrix1.m[1][3] * matrix1.m[3][2]
						   - matrix1.m[0][2] * matrix1.m[1][1] * matrix1.m[3][3] + matrix1.m[0][1] * matrix1.m[1][2] * matrix1.m[3][3];

			cofactor[2][1]=  - ( -matrix1.m[0][3] * matrix1.m[1][2] * matrix1.m[3][0] + matrix1.m[0][2] * matrix1.m[1][3] * matrix1.m[3][0]
						   + matrix1.m[0][3] * matrix1.m[1][0] * matrix1.m[3][2] - matrix1.m[0][0] * matrix1.m[1][3] * matrix1.m[3][2]
						   - matrix1.m[0][2] * matrix1.m[1][0] * matrix1.m[3][3] + matrix1.m[0][0] * matrix1.m[1][2] * matrix1.m[3][3] );

			cofactor[2][2]= -matrix1.m[0][3] * matrix1.m[1][1] * matrix1.m[3][0] + matrix1.m[0][1] * matrix1.m[1][3] * matrix1.m[3][0]
						   + matrix1.m[0][3] * matrix1.m[1][0] * matrix1.m[3][1] - matrix1.m[0][0] * matrix1.m[1][3] * matrix1.m[3][1]
						   - matrix1.m[0][1] * matrix1.m[1][0] * matrix1.m[3][3] + matrix1.m[0][0] * matrix1.m[1][1] * matrix1.m[3][3];

			cofactor[2][3]=  - ( -matrix1.m[0][2] * matrix1.m[1][1] * matrix1.m[3][0] + matrix1.m[0][1] * matrix1.m[1][2] * matrix1.m[3][0]
						   + matrix1.m[0][2] * matrix1.m[1][0] * matrix1.m[3][1] - matrix1.m[0][0] * matrix1.m[1][2] * matrix1.m[3][1]
						   - matrix1.m[0][1] * matrix1.m[1][0] * matrix1.m[3][2] + matrix1.m[0][0] * matrix1.m[1][1] * matrix1.m[3][2] );

			cofactor[3][0]= - (  -matrix1.m[0][3] * matrix1.m[1][2] * matrix1.m[2][1] + matrix1.m[0][2] * matrix1.m[1][3] * matrix1.m[2][1]
						   + matrix1.m[0][3] * matrix1.m[1][1] * matrix1.m[2][2] - matrix1.m[0][1] * matrix1.m[1][3] * matrix1.m[2][2]
						   - matrix1.m[0][2] * matrix1.m[1][1] * matrix1.m[2][3] + matrix1.m[0][1] * matrix1.m[1][2] * matrix1.m[2][3] );

			cofactor[3][1]= -matrix1.m[0][3] * matrix1.m[1][2] * matrix1.m[2][0] + matrix1.m[0][2] * matrix1.m[1][3] * matrix1.m[2][0]
						   + matrix1.m[0][3] * matrix1.m[1][0] * matrix1.m[2][2] - matrix1.m[0][0] * matrix1.m[1][3] * matrix1.m[2][2]
						   - matrix1.m[0][2] * matrix1.m[1][0] * matrix1.m[2][3] + matrix1.m[0][0] * matrix1.m[1][2] * matrix1.m[2][3];

			cofactor[3][2]= - (  -matrix1.m[0][3] * matrix1.m[1][1] * matrix1.m[2][0] + matrix1.m[0][1] * matrix1.m[1][3] * matrix1.m[2][0]
						   + matrix1.m[0][3] * matrix1.m[1][0] * matrix1.m[2][1] - matrix1.m[0][0] * matrix1.m[1][3] * matrix1.m[2][1]
						   - matrix1.m[0][1] * matrix1.m[1][0] * matrix1.m[2][3] + matrix1.m[0][0] * matrix1.m[1][1] * matrix1.m[2][3] );

			cofactor[3][3]= -matrix1.m[0][2] * matrix1.m[1][1] * matrix1.m[2][0] + matrix1.m[0][1] * matrix1.m[1][2] * matrix1.m[2][0]
						   + matrix1.m[0][2] * matrix1.m[1][0] * matrix1.m[2][1] - matrix1.m[0][0] * matrix1.m[1][2] * matrix1.m[2][1]
						   - matrix1.m[0][1] * matrix1.m[1][0] * matrix1.m[2][2] + matrix1.m[0][0] * matrix1.m[1][1] * matrix1.m[2][2];

			Matrix4x4 matrix2;

			for( int i = 0; i < 4; ++i )
			{
				for( int j = 0; j < 4; ++j )
				{
					matrix2.m[i][j] = ( 1 / det ) * cofactor[j][i];
				}
			}


			Ptr<Matrix4x4> iMatrix1 = matrix1.Inverse();

			EXPECT_TRUE( *iMatrix1 == matrix2 );
		}
	}

}

TEST( Matrix4x4Tests, operatorEqual )
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{
		Matrix4x4 matrix1;
		Matrix4x4 matrix2;

		for( int i = 0; i < 4; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				double d = taf::randomNumber( a, b );

				matrix1.m[i][j] = d;
				matrix2.m[i][j] = d;
			}
		}

	  EXPECT_TRUE( matrix1 == matrix2 );
	}

}

TEST( Matrix4x4Tests, Mul )
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{
		Ptr<Matrix4x4> matrix1 = new Matrix4x4;
		Ptr<Matrix4x4> matrix2 = new Matrix4x4;

		for( int i = 0; i < 4; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				matrix1->m[i][j] = taf::randomNumber( a, b );;
				matrix2->m[i][j] = taf::randomNumber( a, b );
			}
		}

		Matrix4x4 matrix3;
		for( int i = 0; i < 4; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				matrix3.m[i][j] = 0;
				for( int k = 0; k < 4; k++ )
					matrix3.m[i][j] += ( matrix1->m[i][k] * matrix2->m[k][j] );
			}
		}

		Ptr<Matrix4x4> mMatrix = Mul( matrix1, matrix2 );
		EXPECT_TRUE( *mMatrix == matrix3 );

		//delete matrix1;
		//delete matrix3;
	}

}
