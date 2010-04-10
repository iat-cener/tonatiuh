#include "Point3D.h"
#include "Vector3D.h"
#include <gtest/gtest.h>

// Tests the Set method.
TEST(Point3DTests, ConstructorDefault) {
  Point3D point;
  EXPECT_TRUE(point.x == 0.0);
  EXPECT_TRUE(point.y == 0.0);
  EXPECT_TRUE(point.z == 0.0); 
}

TEST(Point3DTests, ConstructorFromVector) {

  Vector3D vector( 6789.0,893,0.56273);
  Point3D point( vector );
  EXPECT_DOUBLE_EQ( vector.x, point.x );
  EXPECT_DOUBLE_EQ( vector.y, point.y );
  EXPECT_DOUBLE_EQ( vector.z, point.z );
}

TEST(Point3DTests, OperatorPlusEqVector) {
  double x0 = 23.4;
  double y0 = 78.9;
  double z0 = 254.76;
  Point3D point( x0, y0, z0 );
  Vector3D vector( 89.12, 76.24, 1.243 );
  point += vector;
  EXPECT_DOUBLE_EQ( (x0+vector.x), point.x );
  EXPECT_DOUBLE_EQ( (y0+vector.y), point.y );
  EXPECT_DOUBLE_EQ( (z0+vector.z), point.z );
}

TEST(Point3DTests, OperatorPlusVector) {
  Point3D point( 130.43, 728.29, 54.716 );
  Vector3D vector( 89.12, 76.24, 1.243 );
  Point3D result;
  result = point + vector;
  EXPECT_DOUBLE_EQ( ( point.x + vector.x ), result.x );
  EXPECT_DOUBLE_EQ( ( point.y + vector.y ), result.y );
  EXPECT_DOUBLE_EQ( ( point.z + vector.z ), result.z );
}

TEST(Point3DTests, OperatorMinusEqVector) {
  double x0 = 43.3;
  double y0 = 172.9;
  double z0 = 828.6;
  Point3D point( x0, y0, z0 );
  Vector3D vector( 64.20, 676.46, 12.43 );
  point -= vector;
  EXPECT_DOUBLE_EQ( (x0-vector.x), point.x );
  EXPECT_DOUBLE_EQ( (y0-vector.y), point.y );
  EXPECT_DOUBLE_EQ( (z0-vector.z), point.z );
}

TEST(Point3DTests, OperatorMinusVector) {
  Point3D point( 589.37, 28.229, 147.16 );
  Vector3D vector( 1.243, 19.12, 67.24 );
  Point3D result;
  result = point - vector;
  EXPECT_DOUBLE_EQ( ( point.x - vector.x ), result.x );
  EXPECT_DOUBLE_EQ( ( point.y - vector.y ), result.y );
  EXPECT_DOUBLE_EQ( ( point.z - vector.z ), result.z );
}

TEST(Point3DTests, OperatorTimesEqScalar) {
  double x0 = 43.3;
  double y0 = 172.9;
  double z0 = 828.6;
  Point3D point( x0, y0, z0 );
  double scalar = 233.6785;
  point *= scalar;
  EXPECT_DOUBLE_EQ( (x0 * scalar), point.x );
  EXPECT_DOUBLE_EQ( (y0 * scalar), point.y );
  EXPECT_DOUBLE_EQ( (z0 * scalar), point.z );
}

TEST(Point3DTests, OperatorTimesScalar) {
  Point3D point( 589.37, 28.229, 147.16 );
  double scalar = 145.89;
  Point3D result;
  result = point * scalar;
  EXPECT_DOUBLE_EQ( ( point.x * scalar ), result.x );
  EXPECT_DOUBLE_EQ( ( point.y * scalar ), result.y );
  EXPECT_DOUBLE_EQ( ( point.z * scalar ), result.z );
}

TEST(Point3DTests, OperatorDivideEqScalar) {
  double x0 = 438.9;
  double y0 = 12.2;
  double z0 = 628.46;
  Point3D point( x0, y0, z0 );
  double scalar = 6785.234;
  point /= scalar;
  EXPECT_DOUBLE_EQ( (x0 / scalar), point.x );
  EXPECT_DOUBLE_EQ( (y0 / scalar), point.y );
  EXPECT_DOUBLE_EQ( (z0 / scalar), point.z );
}

TEST(Point3DTests, OperatorDivideScalar) {
  Point3D point( 593.75, 82.292, 714.16 );
  double scalar = 145.89;
  Point3D result;
  result = point / scalar;
  EXPECT_DOUBLE_EQ( ( point.x / scalar ), result.x );
  EXPECT_DOUBLE_EQ( ( point.y / scalar ), result.y );
  EXPECT_DOUBLE_EQ( ( point.z / scalar ), result.z );
}

TEST(Point3DTests, OperatorMinusPoint) {
  Point3D pointA( 0.37, 228.229, -47.16 );
  Point3D pointB( 3.46, 26.4, 87.2 );
  Vector3D vector = pointA - pointB;
  EXPECT_DOUBLE_EQ( ( pointA.x - pointB.x ), vector.x );
  EXPECT_DOUBLE_EQ( ( pointA.y - pointB.y ), vector.y );
  EXPECT_DOUBLE_EQ( ( pointA.z - pointB.z ), vector.z );
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
  double coordinate[] = { 3456.78, -348.89, -28.223 };
  double value = 0.0;
  Point3D point( coordinate[0], coordinate[1], coordinate[2] );
  for( int i = 0; i<3; i++ )
  {
	  EXPECT_DOUBLE_EQ( coordinate[i], point[i] );
	  value = point[i];
	  EXPECT_DOUBLE_EQ( coordinate[i], value );
  }
}

TEST(Point3DTests, IndexingOperatorsLhs) {
  double coordinate[] = { 3456.78, -348.89, -28.223 };
  Point3D point( 0, 0, 0 );
  for( int i = 0; i<3; i++ )
  {
	  point[i] = coordinate[i];
	  EXPECT_DOUBLE_EQ(  coordinate[i], point[i] );
  }
}
