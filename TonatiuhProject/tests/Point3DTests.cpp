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

#include "Point3D.h"
#include "Vector3D.h"
#include <gtest/gtest.h>

#include <cmath>

#include "TestsAuxiliaryFunctions.h"
#include <time.h>

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 60000;

// Tests the Set method.
TEST(Point3DTests, ConstructorDefault) {
  Point3D point;
  EXPECT_TRUE(point.x == 0.0);
  EXPECT_TRUE(point.y == 0.0);
  EXPECT_TRUE(point.z == 0.0);
}

TEST(Point3DTests, ConstructorFromVector) {

	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Vector3D vector( taf::randomNumber(a,b),taf::randomNumber(a,b),taf::randomNumber(a,b));
		  Point3D point( vector );
		  EXPECT_DOUBLE_EQ( vector.x, point.x );
		  EXPECT_DOUBLE_EQ( vector.y, point.y );
		  EXPECT_DOUBLE_EQ( vector.z, point.z );
	}
}

TEST(Point3DTests, OperatorPlusEqVector) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double x0;
	double y0;
	double z0;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  x0=taf::randomNumber(a,b);
		  y0=taf::randomNumber(a,b);
		  z0=taf::randomNumber(a,b);
		  Point3D point( x0, y0, z0 );
		  Vector3D vector( 89.12, 76.24, 1.243 );
		  point += vector;
		  EXPECT_DOUBLE_EQ( (x0+vector.x), point.x );
		  EXPECT_DOUBLE_EQ( (y0+vector.y), point.y );
		  EXPECT_DOUBLE_EQ( (z0+vector.z), point.z );
	}
}

TEST(Point3DTests, OperatorPlusVector) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double x0;
	double y0;
	double z0;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  x0=taf::randomNumber(a,b);
		  y0=taf::randomNumber(a,b);
		  z0=taf::randomNumber(a,b);
		  Point3D point=taf::randomPoint(a,b);
		  Vector3D vector( x0, y0, z0 );
		  Point3D result;
		  result = point + vector;
		  EXPECT_DOUBLE_EQ( ( point.x + vector.x ), result.x );
		  EXPECT_DOUBLE_EQ( ( point.y + vector.y ), result.y );
		  EXPECT_DOUBLE_EQ( ( point.z + vector.z ), result.z );
	}
}

TEST(Point3DTests, OperatorMinusEqVector) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double x0;
	double y0;
	double z0;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  x0=taf::randomNumber(a,b);
		  y0=taf::randomNumber(a,b);
		  z0=taf::randomNumber(a,b);
		  Point3D point(x0, y0, z0);
		  Vector3D vector(taf::randomPoint(a,b));
		  point -= vector;
		  EXPECT_DOUBLE_EQ( (x0-vector.x), point.x );
		  EXPECT_DOUBLE_EQ( (y0-vector.y), point.y );
		  EXPECT_DOUBLE_EQ( (z0-vector.z), point.z );
	}
}

TEST(Point3DTests, OperatorMinusVector) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Point3D point=taf::randomPoint(a,b);
		  Vector3D vector( taf::randomPoint(a,b) );
		  Point3D result;
		  result = point - vector;
		  EXPECT_DOUBLE_EQ( ( point.x - vector.x ), result.x );
		  EXPECT_DOUBLE_EQ( ( point.y - vector.y ), result.y );
		  EXPECT_DOUBLE_EQ( ( point.z - vector.z ), result.z );
	}
}

TEST(Point3DTests, OperatorTimesEqScalar) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double x0;
	double y0;
	double z0;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  x0=taf::randomNumber(a,b);
		  y0=taf::randomNumber(a,b);
		  z0=taf::randomNumber(a,b);
		  Point3D point( x0, y0, z0 );
		  double scalar = taf::randomNumber(a,b);
		  point *= scalar;
		  EXPECT_DOUBLE_EQ( (x0 * scalar), point.x );
		  EXPECT_DOUBLE_EQ( (y0 * scalar), point.y );
		  EXPECT_DOUBLE_EQ( (z0 * scalar), point.z );
	}
}

TEST(Point3DTests, OperatorTimesScalar) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Point3D point( taf::randomPoint(a,b) );
		  double scalar = taf::randomNumber(a,b);
		  Point3D result;
		  result = point * scalar;
		  EXPECT_DOUBLE_EQ( ( point.x * scalar ), result.x );
		  EXPECT_DOUBLE_EQ( ( point.y * scalar ), result.y );
		  EXPECT_DOUBLE_EQ( ( point.z * scalar ), result.z );
	}
}

TEST(Point3DTests, OperatorDivideEqScalar) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double x0;
	double y0;
	double z0;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  x0=taf::randomNumber(a,b);
		  y0=taf::randomNumber(a,b);
		  z0=taf::randomNumber(a,b);
		  Point3D point( x0, y0, z0 );
		  double scalar = taf::randomNumber(a,b);
		  point /= scalar;
		  EXPECT_DOUBLE_EQ( (x0 / scalar), point.x );
		  EXPECT_DOUBLE_EQ( (y0 / scalar), point.y );
		  EXPECT_DOUBLE_EQ( (z0 / scalar), point.z );
	}
}

TEST(Point3DTests, OperatorDivideScalar) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Point3D point( taf::randomPoint(a,b) );
		  double scalar = taf::randomNumber(a,b);
		  Point3D result;
		  result = point / scalar;
		  EXPECT_DOUBLE_EQ( ( point.x / scalar ), result.x );
		  EXPECT_DOUBLE_EQ( ( point.y / scalar ), result.y );
		  EXPECT_DOUBLE_EQ( ( point.z / scalar ), result.z );
	}
}

TEST(Point3DTests, OperatorMinusPoint) {

	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Point3D pointA( taf::randomPoint(a,b) );
		  Point3D pointB( taf::randomPoint(a,b) );
		  Vector3D vector = pointA - pointB;
		  EXPECT_DOUBLE_EQ( ( pointA.x - pointB.x ), vector.x );
		  EXPECT_DOUBLE_EQ( ( pointA.y - pointB.y ), vector.y );
		  EXPECT_DOUBLE_EQ( ( pointA.z - pointB.z ), vector.z );
	}
}

TEST(Point3DTests, EqualityOperators) {
  Point3D pointA( 0.37, 228.229, -47.16 );
  Point3D pointB( 0.37, 228.229, -47.16 );
  Point3D pointC( 67.24, -23.56, 0.008 );
  EXPECT_TRUE( pointA == pointB );
  EXPECT_TRUE( pointA != pointC );
  EXPECT_TRUE( pointB != pointC );

  //Reflective property
  EXPECT_TRUE( pointA == pointA );
  EXPECT_TRUE( pointB == pointB );
  EXPECT_TRUE( pointC == pointC );
  EXPECT_FALSE( pointA != pointA );
  EXPECT_FALSE( pointB != pointB );
  EXPECT_FALSE( pointC != pointC );

  //Symmetric property
  EXPECT_TRUE( ( ( pointA == pointB ) && ( pointB == pointA ) ) ||
		       ( ( pointA != pointB ) && ( pointB != pointA ) ) );
  EXPECT_TRUE( ( ( pointA == pointC ) && ( pointC == pointA ) ) ||
		       ( ( pointA != pointC ) && ( pointC != pointA ) ) );
  EXPECT_TRUE( ( ( pointB == pointC ) && ( pointC == pointB ) ) ||
		       ( ( pointB != pointC ) && ( pointC != pointB ) ) );
}

TEST(Point3DTests, IndexingOperatorsRhs) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double x0;
	double y0;
	double z0;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  x0=taf::randomNumber(a,b);
		  y0=taf::randomNumber(a,b);
		  z0=taf::randomNumber(a,b);
		  double coordinate[] = { x0, y0, z0 };
		  double value = 0.0;
		  Point3D point( coordinate[0], coordinate[1], coordinate[2] );
		  for( int i = 0; i<3; i++ )
		  {
			  EXPECT_DOUBLE_EQ( coordinate[i], point[i] );
			  value = point[i];
			  EXPECT_DOUBLE_EQ( coordinate[i], value );
		  }
	}
}

TEST(Point3DTests, IndexingOperatorsLhs) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double x0;
	double y0;
	double z0;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  x0=taf::randomNumber(a,b);
		  y0=taf::randomNumber(a,b);
		  z0=taf::randomNumber(a,b);
		  double coordinate[] = { x0, y0, z0 };
		  Point3D point( 0, 0, 0 );
		  for( int i = 0; i<3; i++ )
		  {
			  point[i] = coordinate[i];
			  EXPECT_DOUBLE_EQ(  coordinate[i], point[i] );
		  }
	}
}

TEST(Point3DTests, DistanceFromPointToPoint) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Point3D pointA ( taf::randomPoint(a,b) );
		  Point3D pointB ( taf::randomPoint(a,b) );
		  double x0=(pointA.x)-(pointB.x);
		  double y0=(pointA.y)-(pointB.y);
		  double z0=(pointA.z)-(pointB.z);
		  double result=sqrt(x0*x0+y0*y0+z0*z0);
		  EXPECT_DOUBLE_EQ(result,Distance(pointA,pointB) );
	}

}

TEST(Point3DTests, DistanceSquaredFromPointToPoint) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){

		  Point3D pointA ( taf::randomPoint(a,b) );
		  Point3D pointB ( taf::randomPoint(a,b) );
		  double x0=(pointA.x)-(pointB.x);
		  double y0=(pointA.y)-(pointB.y);
		  double z0=(pointA.z)-(pointB.z);
		  double result=x0*x0+y0*y0+z0*z0;
		  EXPECT_DOUBLE_EQ(result,DistanceSquared(pointA,pointB) );
	}

}
