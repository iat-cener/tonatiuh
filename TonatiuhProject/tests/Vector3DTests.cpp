/*
 * Vector3DTests.cpp
 *
 *  Created on: 12/08/2011
 *      Author: sgarayalde
 */

#include <gtest/gtest.h>
#include <cmath>
#include "NormalVector.h"
#include "Point3D.h"
#include "Vector3D.h"

TEST(Vector3DTests, ConstructorDefault){
	Vector3D vector;
	EXPECT_TRUE(vector.x == 0.0);
	EXPECT_TRUE(vector.y == 0.0);
	EXPECT_TRUE(vector.z == 0.0);
}

TEST(Vector3DTests, ConstructorFromNormalVector){
	NormalVector normalvector(6789.0,893,0.56273);
	Vector3D vector(normalvector);
	EXPECT_DOUBLE_EQ(vector.x , normalvector.x);
	EXPECT_DOUBLE_EQ(vector.y , normalvector.y);
	EXPECT_DOUBLE_EQ(vector.z , normalvector.z);
}
TEST(Vector3DTests, ConstructorFromPoint){
	Point3D point(6789.0,893,0.56273);
	Vector3D vector(point);
	EXPECT_DOUBLE_EQ(vector.x , point.x);
	EXPECT_DOUBLE_EQ(vector.y , point.y);
	EXPECT_DOUBLE_EQ(vector.z , point.z);
}

//TEST(Vector3DTests, ConstructorFromSbVec3f){

/*	float x=6789.0;
	float y=893;
	float z=0.56273;


	Vector3D vector3D(SbVec3f(float(x),float(y),float(z)));


	EXPECT_DOUBLE_EQ(SbVec3f.vector3D.x,double(x));
	EXPECT_FLOAT_EQ(vector3D.y, y);
	EXPECT_FLOAT_EQ(vector3D.z, z);
}*/

/*TEST(Vector3DTests,Vector3DToSbVec3f){
	Vector3D vector(6789.0,893,0.56273);
	EXPECT_FLOAT_EQ(vector.ToSbVec3f().vec[0] , float(vector.x));
	EXPECT_FLOAT_EQ(vector3D.y , float(vector.y));
	EXPECT_FLOAT_EQ(vector3D.z , float(vector.z));
}*/
TEST(Vector3DTests,OperatorPlusEqVector){
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

TEST(Vector3DTests, OperatorMinusEqVector) {
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
TEST(Vector3DTests, OperatorTimesEqScalar) {
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
TEST(Vector3DTests, OperatorTimesScalar) {
	Point3D point( 589.37, 28.229, 147.16 );
	double scalar = 145.89;
	Point3D result;
	result = point * scalar;
	EXPECT_DOUBLE_EQ( ( point.x * scalar ), result.x );
	EXPECT_DOUBLE_EQ( ( point.y * scalar ), result.y );
	EXPECT_DOUBLE_EQ( ( point.z * scalar ), result.z );
}
TEST(Vector3DTests, OperatorDivideEqScalar) {
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
TEST(Vector3DTests, OperatorDivideScalar) {
	Point3D point( 593.75, 82.292, 714.16 );
	double scalar = 145.89;
	Point3D result;
	result = point / scalar;
	EXPECT_DOUBLE_EQ( ( point.x / scalar ), result.x );
	EXPECT_DOUBLE_EQ( ( point.y / scalar ), result.y );
	EXPECT_DOUBLE_EQ( ( point.z / scalar ), result.z );
}
TEST(Vector3DTests, OperatorNegativeVector) {
	Vector3D vector( 1.243, 19.12, 67.24 );
	Vector3D vectorInv=-vector;
	EXPECT_DOUBLE_EQ(-vector.x, vectorInv.x );
	EXPECT_DOUBLE_EQ(-vector.y, vectorInv.y );
  	EXPECT_DOUBLE_EQ(-vector.z, vectorInv.z );
}
TEST(Vector3DTests,CreateVectorZero){
	double x0=1.243;
	double y0=19.12;
	double z0=67.24;
	Vector3D vector(x0,y0,z0);
	EXPECT_DOUBLE_EQ(vector.x, x0);
	EXPECT_DOUBLE_EQ(vector.y, y0);
	EXPECT_DOUBLE_EQ(vector.z, z0);
	vector.zero();
	EXPECT_DOUBLE_EQ(vector.x, 0.0);
	EXPECT_DOUBLE_EQ(vector.y, 0.0);
	EXPECT_DOUBLE_EQ(vector.z, 0.0);
}
TEST(Vector3DTests,OperatorLengthSquared){
	double x0=1.243;
	double y0=19.12;
	double z0=67.24;
	Vector3D v(x0,y0,z0);
	double result=x0*x0+y0*y0+z0*z0;
	EXPECT_DOUBLE_EQ(result, v.lengthSquared());
}
TEST(Vector3DTests,OperatorLength){
	double x0=1.243;
	double y0=19.12;
	double z0=67.24;
	Vector3D v(x0,y0,z0);
	double result=sqrt(x0*x0+y0*y0+z0*z0);
	EXPECT_DOUBLE_EQ(result, v.length());
}
TEST(Vector3DTests, NonMemberFunctionOperatorPlus)
{
  Vector3D lhs( 130.43, 728.29, 54.716 );
  Vector3D rhs( 89.12, 76.24, 1.243 );
  Vector3D result = lhs + rhs;
  EXPECT_DOUBLE_EQ( ( lhs.x + rhs.x ), result.x );
  EXPECT_DOUBLE_EQ( ( lhs.y + rhs.y ), result.y );
  EXPECT_DOUBLE_EQ( ( lhs.z + rhs.z ), result.z );
}

TEST(Vector3DTests, NonMemberFunctionOperatorMinus)
{
  Vector3D lhs( 130.43, 728.29, 54.716 );
  Vector3D rhs( 89.12, 76.24, 1.243 );
  Vector3D result = lhs - rhs;
  EXPECT_DOUBLE_EQ( ( lhs.x - rhs.x ), result.x );
  EXPECT_DOUBLE_EQ( ( lhs.y - rhs.y ), result.y );
  EXPECT_DOUBLE_EQ( ( lhs.z - rhs.z ), result.z );
}

TEST(Vector3DTests, NonMemberFunctionOperatorPlusScalar)
{
  Vector3D normalVector( 130.43, 728.29, 54.716 );
  double scalar = 345.0;
  Vector3D result = scalar * normalVector;
  result = scalar * normalVector;
  EXPECT_DOUBLE_EQ( ( scalar*normalVector.x ), result.x );
  EXPECT_DOUBLE_EQ( ( scalar*normalVector.y ), result.y );
  EXPECT_DOUBLE_EQ( ( scalar*normalVector.z ), result.z );
}
TEST(Vector3DTests, OperatorDotProductVectorVector)
{
  Vector3D v0( 130.43, 728.29, 54.716 );
  Vector3D v1( 89.12, 76.24, 1.243 );
  double result =v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
  EXPECT_DOUBLE_EQ(DotProduct(v0,v1), result);
}
TEST(Vector3DTests, OperatorDotProductVectorNormalVector)
{
  Vector3D v0( 130.43, 728.29, 54.716 );
  NormalVector n0( 89.12, 76.24, 1.243 );
  double result =v0.x*n0.x + v0.y*n0.y + v0.z*n0.z;
  EXPECT_DOUBLE_EQ(DotProduct(v0,n0), result );
}
TEST(Vector3DTests, OperatorDotProductNormalVectorVector)
{
  NormalVector n0( 130.43, 728.29, 54.716 );
  Vector3D v0( 89.12, 76.24, 1.243 );
  double result =n0.x*v0.x + n0.y*v0.y + n0.z*v0.z;
  EXPECT_DOUBLE_EQ(DotProduct(n0,v0), result );
}
TEST(Vector3DTests, OperatorCrossProductVectorVector)
{
  Vector3D vA( 130.43, 728.29, 54.716 );
  Vector3D vB( 89.12, 76.24, 1.243 );
  double x0= ( vA.y * vB.z ) - ( vA.z * vB.y );
  double y0= ( vA.z*vB.x ) - ( vA.x*vB.z );
  double z0= ( vA.x*vB.y ) - ( vA.y*vB.x );
  Vector3D result(x0,y0 ,z0 );
  EXPECT_DOUBLE_EQ(CrossProduct(vA,vB).x, result.x );
  EXPECT_DOUBLE_EQ(CrossProduct(vA,vB).y, result.y );
  EXPECT_DOUBLE_EQ(CrossProduct(vA,vB).z, result.z );
}
TEST(Vector3DTests, OperatorCrossProductVectorNormalVector)
{
  Vector3D vA( 130.43, 728.29, 54.716 );
  NormalVector nB( 89.12, 76.24, 1.243 );
  double x0= ( vA.y * nB.z ) - ( vA.z * nB.y );
  double y0= ( vA.z*nB.x ) - ( vA.x*nB.z );
  double z0= ( vA.x*nB.y ) - ( vA.y*nB.x );
  Vector3D result(x0,y0,z0 );
  EXPECT_DOUBLE_EQ(CrossProduct(vA,nB).x, result.x );
  EXPECT_DOUBLE_EQ(CrossProduct(vA,nB).y, result.y );
  EXPECT_DOUBLE_EQ(CrossProduct(vA,nB).z, result.z );
}
TEST(Vector3DTests, OperatorCrossProductNormalVectorVector)
{
  Vector3D nA( 130.43, 728.29, 54.716 );
  Vector3D vB( 89.12, 76.24, 1.243 );
  double x0= ( nA.y * vB.z ) - ( nA.z * vB.y );
  double y0= ( nA.z*vB.x ) - ( nA.x*vB.z );
  double z0= ( nA.x*vB.y ) - ( nA.y*vB.x );
  Vector3D result(x0,y0 ,z0 );
  EXPECT_DOUBLE_EQ(CrossProduct(nA,vB).x, result.x );
  EXPECT_DOUBLE_EQ(CrossProduct(nA,vB).y, result.y );
  EXPECT_DOUBLE_EQ(CrossProduct(nA,vB).z, result.z );
}
