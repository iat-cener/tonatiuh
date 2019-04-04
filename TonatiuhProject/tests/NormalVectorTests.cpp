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

#include "Vector3D.h"
#include "NormalVector.h"
#include <cmath>
#include <gtest/gtest.h>

#include "TestsAuxiliaryFunctions.h"
#include <time.h>

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 60000;
// Tests the Set method.
TEST(NormalVectorTests, ConstructorDefault)
{
  NormalVector normalVector;
  EXPECT_TRUE(normalVector.x == 0.0);
  EXPECT_TRUE(normalVector.y == 0.0);
  EXPECT_TRUE(normalVector.z == 0.0);
}

TEST(NormalVectorTests, ConstructorFromVector)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Vector3D vector(taf::randomNumber(a,b),taf::randomNumber(a,b),taf::randomNumber(a,b));
		  NormalVector normalVector( vector );
		  EXPECT_DOUBLE_EQ( vector.x, normalVector.x );
		  EXPECT_DOUBLE_EQ( vector.y, normalVector.y );
		  EXPECT_DOUBLE_EQ( vector.z, normalVector.z );
	}
}

TEST(NormalVectorTests, OperatorPlusEqual)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double x1 = taf::randomNumber(a,b);
		  double y1 = taf::randomNumber(a,b);
		  double z1 = taf::randomNumber(a,b);
		  NormalVector nv1( x0, y0, z0 );
		  NormalVector nv2( x1, y1, z1 );
		  nv1 += nv2;
		  EXPECT_DOUBLE_EQ( (x0+nv2.x), nv1.x );
		  EXPECT_DOUBLE_EQ( (y0+nv2.y), nv1.y );
		  EXPECT_DOUBLE_EQ( (z0+nv2.z), nv1.z );
	}
}

TEST(NormalVectorTests, OperatorMinusEqual)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double x1 = taf::randomNumber(a,b);
		  double y1 = taf::randomNumber(a,b);
		  double z1 = taf::randomNumber(a,b);
		  NormalVector nv1( x0, y0, z0 );
		  NormalVector nv2( x1, y1, z1 );
		  nv1 -= nv2;
		  EXPECT_DOUBLE_EQ( (x0-nv2.x), nv1.x );
		  EXPECT_DOUBLE_EQ( (y0-nv2.y), nv1.y );
		  EXPECT_DOUBLE_EQ( (z0-nv2.z), nv1.z );
	}
}

TEST(NormalVectorTests, OperatorTimesEqualScalar)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double scalar=taf::randomNumber(a,b);
		  NormalVector normalVector( x0, y0, z0 );

		  normalVector *= scalar;
		  EXPECT_DOUBLE_EQ( (x0 * scalar), normalVector.x );
		  EXPECT_DOUBLE_EQ( (y0 * scalar), normalVector.y );
		  EXPECT_DOUBLE_EQ( (z0 * scalar), normalVector.z );
	}
}

TEST(NormalVectorTests, OperatorTimesScalar)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double scalar=taf::randomNumber(a,b);
		  NormalVector normalVector( x0, y0, z0 );
		  NormalVector result = normalVector * scalar;
		  EXPECT_DOUBLE_EQ( ( normalVector.x * scalar ), result.x );
		  EXPECT_DOUBLE_EQ( ( normalVector.y * scalar ), result.y );
		  EXPECT_DOUBLE_EQ( ( normalVector.z * scalar ), result.z );
	}
}

TEST(NormalVectorTests, OperatorDivideEqualScalar)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double scalar=taf::randomNumber(a,b);
		  NormalVector normalVector( x0, y0, z0 );
		  normalVector /= scalar;
		  EXPECT_DOUBLE_EQ( (x0 / scalar), normalVector.x );
		  EXPECT_DOUBLE_EQ( (y0 / scalar), normalVector.y );
		  EXPECT_DOUBLE_EQ( (z0 / scalar), normalVector.z );
	}
}

TEST(NormalVectorTests, OperatorDivideScalar)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double scalar=taf::randomNumber(a,b);
		  NormalVector normalVector( x0, y0, z0 );
		  NormalVector result;
		  result = normalVector / scalar;
		  EXPECT_DOUBLE_EQ( ( normalVector.x / scalar ), result.x );
		  EXPECT_DOUBLE_EQ( ( normalVector.y / scalar ), result.y );
		  EXPECT_DOUBLE_EQ( ( normalVector.z / scalar ), result.z );
	}
}

TEST(NormalVectorTests, UnaryOperatorMinus)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  NormalVector normalVector( x0, y0, z0 );
		  NormalVector result = -normalVector;
		  EXPECT_DOUBLE_EQ( -x0, result.x );
		  EXPECT_DOUBLE_EQ( -y0, result.y );
		  EXPECT_DOUBLE_EQ( -z0, result.z );
	}
}

TEST(NormalVectorTests, EqualityOperators)
{
  NormalVector nvA( 0.37, 228.229, -47.16 );
  NormalVector nvB( 0.37, 228.229, -47.16 );
  NormalVector nvC( 67.24, -23.56, 0.008 );
  EXPECT_TRUE( nvA == nvB );
  EXPECT_TRUE( nvA != nvC );
  EXPECT_TRUE( nvB != nvC );

  //Reflective property
  EXPECT_TRUE( nvA == nvA );
  EXPECT_TRUE( nvB == nvB );
  EXPECT_TRUE( nvC == nvC );
  EXPECT_FALSE( nvA != nvA );
  EXPECT_FALSE( nvB != nvB );
  EXPECT_FALSE( nvC != nvC );

  //Symmetric property
  EXPECT_TRUE( ( ( nvA == nvB ) && ( nvB == nvA ) ) ||
		       ( ( nvA != nvB ) && ( nvB != nvA ) ) );
  EXPECT_TRUE( ( ( nvA == nvC ) && ( nvC == nvA ) ) ||
		       ( ( nvA != nvC ) && ( nvC != nvA ) ) );
  EXPECT_TRUE( ( ( nvB == nvC ) && ( nvC == nvB ) ) ||
		       ( ( nvB != nvC ) && ( nvC != nvB ) ) );
}

TEST(NormalVectorTests, IndexingOperatorsRhs)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);

		  double coordinate[] = { x0, y0, z0 };
		  double value = 0.0;
		  NormalVector normalVector( coordinate[0], coordinate[1], coordinate[2] );
		  for( int i = 0; i<3; i++ )
		  {
			  EXPECT_DOUBLE_EQ( coordinate[i], normalVector[i] );
			  value = normalVector[i];
			  EXPECT_DOUBLE_EQ( coordinate[i], value );
		  }
	}
}

TEST(NormalVectorTests, IndexingOperatorsLhs)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);

		  double coordinate[] = { x0, y0, z0 };
		  NormalVector normalVector( 0, 0, 0 );
		  for( int i = 0; i<3; i++ )
		  {
			  normalVector[i] = coordinate[i];
			  EXPECT_DOUBLE_EQ(  coordinate[i], normalVector[i] );
		  }
	}
}

TEST(NormalVectorTests, lengthSquared )
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  NormalVector normalVector( x0, y0, z0 );

		  double result = x0*x0 + y0*y0 + z0*z0;
		  EXPECT_DOUBLE_EQ( result, normalVector.lengthSquared() );
	}
}

TEST(NormalVectorTests, length )
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  NormalVector normalVector( x0, y0, z0 );
		  double result = sqrt( x0*x0 + y0*y0 + z0*z0 );
		  EXPECT_DOUBLE_EQ( result, normalVector.length() );
	}
}

TEST(NormalVectorTests, NonMemberFunctionOperatorPlus)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double x1 = taf::randomNumber(a,b);
		  double y1 = taf::randomNumber(a,b);
		  double z1 = taf::randomNumber(a,b);


		  NormalVector lhs( x0, y0, z0 );
		  NormalVector rhs( x1, y1, z1 );
		  NormalVector result = lhs + rhs;
		  EXPECT_DOUBLE_EQ( ( lhs.x + rhs.x ), result.x );
		  EXPECT_DOUBLE_EQ( ( lhs.y + rhs.y ), result.y );
		  EXPECT_DOUBLE_EQ( ( lhs.z + rhs.z ), result.z );
	}
}

TEST(NormalVectorTests, NonMemberFunctionOperatorMinus)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double x1 = taf::randomNumber(a,b);
		  double y1 = taf::randomNumber(a,b);
		  double z1 = taf::randomNumber(a,b);

		  NormalVector lhs( x0, y0, z0 );
		  NormalVector rhs( x1, y1, z1 );
		  NormalVector result = lhs - rhs;
		  EXPECT_DOUBLE_EQ( ( lhs.x - rhs.x ), result.x );
		  EXPECT_DOUBLE_EQ( ( lhs.y - rhs.y ), result.y );
		  EXPECT_DOUBLE_EQ( ( lhs.z - rhs.z ), result.z );
	}
}

TEST(NormalVectorTests, NonMemberFunctionOperatorPlusScalar)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double scalar=taf::randomNumber(a,b);
		  NormalVector normalVector( x0, y0, z0 );
		  NormalVector result = scalar * normalVector;
		  result = scalar * normalVector;
		  EXPECT_DOUBLE_EQ( ( scalar*normalVector.x ), result.x );
		  EXPECT_DOUBLE_EQ( ( scalar*normalVector.y ), result.y );
		  EXPECT_DOUBLE_EQ( ( scalar*normalVector.z ), result.z );
	}
}

TEST(NormalVectorTests, FunctionAbsDotProduct)
{
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  double x0 = taf::randomNumber(a,b);
		  double y0 = taf::randomNumber(a,b);
		  double z0 = taf::randomNumber(a,b);
		  double x1 = taf::randomNumber(a,b);
		  double y1 = taf::randomNumber(a,b);
		  double z1 = taf::randomNumber(a,b);

		  NormalVector normal1( x0, y0, z0 );
		  NormalVector normal2( x1, y1, z1 );

		  double result=fabs(normal1[0]*normal2[0]+normal1[1]*normal2[1]+normal1[2]*normal2[3]);
		  EXPECT_DOUBLE_EQ( AbsDotProduct(normal1,normal2), result);
	}

}
