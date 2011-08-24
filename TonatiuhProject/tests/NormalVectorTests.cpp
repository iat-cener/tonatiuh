#include "Vector3D.h"
#include "NormalVector.h"
#include <cmath>
#include <gtest/gtest.h>

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
  Vector3D vector( 6789.0,893,0.56273);
  NormalVector normalVector( vector );
  EXPECT_DOUBLE_EQ( vector.x, normalVector.x );
  EXPECT_DOUBLE_EQ( vector.y, normalVector.y );
  EXPECT_DOUBLE_EQ( vector.z, normalVector.z );
}

TEST(NormalVectorTests, OperatorPlusEqual)
{
  double x0 = 23.4;
  double y0 = 78.9;
  double z0 = 254.76;
  NormalVector nv1( x0, y0, z0 );
  NormalVector nv2( 89.12, 76.24, 1.243 );
  nv1 += nv2;
  EXPECT_DOUBLE_EQ( (x0+nv2.x), nv1.x );
  EXPECT_DOUBLE_EQ( (y0+nv2.y), nv1.y );
  EXPECT_DOUBLE_EQ( (z0+nv2.z), nv1.z );
}

TEST(NormalVectorTests, OperatorMinusEqual)
{
  double x0 = 43.3;
  double y0 = 172.9;
  double z0 = 828.6;
  NormalVector nv1( x0, y0, z0 );
  NormalVector nv2( 64.20, 676.46, 12.43 );
  nv1 -= nv2;
  EXPECT_DOUBLE_EQ( (x0-nv2.x), nv1.x );
  EXPECT_DOUBLE_EQ( (y0-nv2.y), nv1.y );
  EXPECT_DOUBLE_EQ( (z0-nv2.z), nv1.z );
}

TEST(NormalVectorTests, OperatorTimesEqualScalar)
{
  double x0 = 43.3;
  double y0 = 172.9;
  double z0 = 828.6;
  NormalVector normalVector( x0, y0, z0 );
  double scalar = 233.6785;
  normalVector *= scalar;
  EXPECT_DOUBLE_EQ( (x0 * scalar), normalVector.x );
  EXPECT_DOUBLE_EQ( (y0 * scalar), normalVector.y );
  EXPECT_DOUBLE_EQ( (z0 * scalar), normalVector.z );
}

TEST(NormalVectorTests, OperatorTimesScalar)
{
  NormalVector normalVector( 589.37, 28.229, 147.16 );
  double scalar = 145.89;
  NormalVector result;
  result = normalVector * scalar;
  EXPECT_DOUBLE_EQ( ( normalVector.x * scalar ), result.x );
  EXPECT_DOUBLE_EQ( ( normalVector.y * scalar ), result.y );
  EXPECT_DOUBLE_EQ( ( normalVector.z * scalar ), result.z );
}

TEST(NormalVectorTests, OperatorDivideEqualScalar)
{
  double x0 = 438.9;
  double y0 = 12.2;
  double z0 = 628.46;
  NormalVector normalVector( x0, y0, z0 );
  double scalar = 6785.234;
  normalVector /= scalar;
  EXPECT_DOUBLE_EQ( (x0 / scalar), normalVector.x );
  EXPECT_DOUBLE_EQ( (y0 / scalar), normalVector.y );
  EXPECT_DOUBLE_EQ( (z0 / scalar), normalVector.z );
}

TEST(NormalVectorTests, OperatorDivideScalar)
{
  NormalVector normalVector( 593.75, 82.292, 714.16 );
  double scalar = 145.89;
  NormalVector result;
  result = normalVector / scalar;
  EXPECT_DOUBLE_EQ( ( normalVector.x / scalar ), result.x );
  EXPECT_DOUBLE_EQ( ( normalVector.y / scalar ), result.y );
  EXPECT_DOUBLE_EQ( ( normalVector.z / scalar ), result.z );
}

TEST(NormalVectorTests, UnaryOperatorMinus)
{
  double x0 = 438.9;
  double y0 = 12.2;
  double z0 = 628.46;
  NormalVector normalVector( x0, y0, z0 );
  NormalVector result = -normalVector;
  EXPECT_DOUBLE_EQ( -x0, result.x );
  EXPECT_DOUBLE_EQ( -y0, result.y );
  EXPECT_DOUBLE_EQ( -z0, result.z );
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
  double coordinate[] = { 3456.78, -348.89, -28.223 };
  double value = 0.0;
  NormalVector normalVector( coordinate[0], coordinate[1], coordinate[2] );
  for( int i = 0; i<3; i++ )
  {
	  EXPECT_DOUBLE_EQ( coordinate[i], normalVector[i] );
	  value = normalVector[i];
	  EXPECT_DOUBLE_EQ( coordinate[i], value );
  }
}

TEST(NormalVectorTests, IndexingOperatorsLhs)
{
  double coordinate[] = { 3456.78, -348.89, -28.223 };
  NormalVector normalVector( 0, 0, 0 );
  for( int i = 0; i<3; i++ )
  {
	  normalVector[i] = coordinate[i];
	  EXPECT_DOUBLE_EQ(  coordinate[i], normalVector[i] );
  }
}

TEST(NormalVectorTests, lengthSquared )
{
	  double x0 = 438.9;
	  double y0 = 12.2;
	  double z0 = 628.46;
	  NormalVector normalVector( x0, y0, z0 );
	  double result = x0*x0 + y0*y0 + z0*z0;
	  EXPECT_DOUBLE_EQ( result, normalVector.lengthSquared() );
}

TEST(NormalVectorTests, length )
{
	  double x0 = 438.9;
	  double y0 = 12.2;
	  double z0 = 628.46;
	  NormalVector normalVector( x0, y0, z0 );
	  double result = sqrt( x0*x0 + y0*y0 + z0*z0 );
	  EXPECT_DOUBLE_EQ( result, normalVector.length() );
}

TEST(NormalVectorTests, NonMemberFunctionOperatorPlus)
{
  NormalVector lhs( 130.43, 728.29, 54.716 );
  NormalVector rhs( 89.12, 76.24, 1.243 );
  NormalVector result = lhs + rhs;
  EXPECT_DOUBLE_EQ( ( lhs.x + rhs.x ), result.x );
  EXPECT_DOUBLE_EQ( ( lhs.y + rhs.y ), result.y );
  EXPECT_DOUBLE_EQ( ( lhs.z + rhs.z ), result.z );
}

TEST(NormalVectorTests, NonMemberFunctionOperatorMinus)
{
  NormalVector lhs( 130.43, 728.29, 54.716 );
  NormalVector rhs( 89.12, 76.24, 1.243 );
  NormalVector result = lhs - rhs;
  EXPECT_DOUBLE_EQ( ( lhs.x - rhs.x ), result.x );
  EXPECT_DOUBLE_EQ( ( lhs.y - rhs.y ), result.y );
  EXPECT_DOUBLE_EQ( ( lhs.z - rhs.z ), result.z );
}

TEST(NormalVectorTests, NonMemberFunctionOperatorPlusScalar)
{
  NormalVector normalVector( 130.43, 728.29, 54.716 );
  double scalar = 345.0;
  NormalVector result = scalar * normalVector;
  result = scalar * normalVector;
  EXPECT_DOUBLE_EQ( ( scalar*normalVector.x ), result.x );
  EXPECT_DOUBLE_EQ( ( scalar*normalVector.y ), result.y );
  EXPECT_DOUBLE_EQ( ( scalar*normalVector.z ), result.z );
}

TEST(NormalVectorTests, FunctionAbsDotProduct)
{
	NormalVector normal1( 130.43, 728.29, 54.716 );
	NormalVector normal2( 89.12, 76.24, 1.243 );

	double result=fabs(normal1[0]*normal2[0]+normal1[1]*normal2[1]+normal1[2]*normal2[3]);

	EXPECT_DOUBLE_EQ( AbsDotProduct(normal1,normal2), result);

}
