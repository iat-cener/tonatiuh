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
#include <Inventor/SbVec3f.h>


#include "TestsAuxiliaryFunctions.h"
#include <time.h>

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 60000;

TEST(Vector3DTests, ConstructorDefault){
	Vector3D vector;
	EXPECT_TRUE(vector.x == 0.0);
	EXPECT_TRUE(vector.y == 0.0);
	EXPECT_TRUE(vector.z == 0.0);
}

TEST(Vector3DTests, ConstructorFromNormalVector){

	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	double x0;
	double y0;
	double z0;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		x0=taf::randomNumber( a, b );
		y0=taf::randomNumber( a, b );
		z0=taf::randomNumber( a, b );
		NormalVector normalvector(x0,y0,z0);
		Vector3D vector(normalvector);
		EXPECT_DOUBLE_EQ(vector.x , normalvector.x);
		EXPECT_DOUBLE_EQ(vector.y , normalvector.y);
		EXPECT_DOUBLE_EQ(vector.z , normalvector.z);
	}
}
TEST(Vector3DTests, ConstructorFromPoint){
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	Point3D point;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
	point=taf::randomNumber( a, b );
	Vector3D vector(point);
	EXPECT_DOUBLE_EQ(vector.x , point.x);
	EXPECT_DOUBLE_EQ(vector.y , point.y);
	EXPECT_DOUBLE_EQ(vector.z , point.z);
	}
}

TEST(Vector3DTests, ConstructorFromSbVec3f){
	srand ( time(NULL) );

		// Extension of the testing space
		double b = maximumCoordinate;
		double a = -b;
	    double x0;
	    double y0;
	    double z0;

		for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){

			x0=float(taf::randomNumber( a, b ));
			y0=float(taf::randomNumber( a, b ));
			z0=float(taf::randomNumber( a, b ));

			SbVec3f scaleFactor(x0,y0,z0);
			Vector3D vector3D(scaleFactor);


			EXPECT_FLOAT_EQ(vector3D.x, x0);
			EXPECT_FLOAT_EQ(vector3D.y, y0);
			EXPECT_FLOAT_EQ(vector3D.z, z0);
		}
}

TEST(Vector3DTests,Vector3DToSbVec3f){

	srand ( time(NULL) );

		// Extension of the testing space
		double b = maximumCoordinate;
		double a = -b;
		double x0;
	    double y0;
	    double z0;

		for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){

			x0=taf::randomNumber( a, b );
			y0=taf::randomNumber( a, b );
			z0=taf::randomNumber( a, b );

			Vector3D vector(x0,y0,z0);

			EXPECT_FLOAT_EQ(vector.ToSbVec3f()[0] , float(x0));
			EXPECT_FLOAT_EQ(vector.ToSbVec3f()[1] , float(y0));
			EXPECT_FLOAT_EQ(vector.ToSbVec3f()[2] , float(z0));
		}
}

TEST(Vector3DTests,OperatorPlusEqVector){
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double x0;
	double y0;
	double z0;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		x0=taf::randomNumber( a, b );
		y0=taf::randomNumber( a, b );
		z0=taf::randomNumber( a, b );
		Vector3D vector(taf::randomNumber(a, b),taf::randomNumber(a, b),taf::randomNumber(a, b));
		Vector3D result( x0, y0, z0 );
		result += vector;
		EXPECT_DOUBLE_EQ( (x0+vector.x), result.x );
		EXPECT_DOUBLE_EQ( (y0+vector.y), result.y );
		EXPECT_DOUBLE_EQ( (z0+vector.z), result.z );
	}
}

TEST(Vector3DTests, OperatorMinusEqVector) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double x0;
	double y0;
	double z0;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		x0=taf::randomNumber( a, b );
		y0=taf::randomNumber( a, b );
		z0=taf::randomNumber( a, b );
		Vector3D vector(taf::randomNumber(a, b),taf::randomNumber(a, b),taf::randomNumber(a, b));
		Vector3D result( x0, y0, z0 );
		result -= vector;
		EXPECT_DOUBLE_EQ( (x0-vector.x), result.x );
		EXPECT_DOUBLE_EQ( (y0-vector.y), result.y );
		EXPECT_DOUBLE_EQ( (z0-vector.z), result.z );
	}
}

TEST(Vector3DTests, OperatorTimesEqScalar) {

	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	double x0;
	double y0;
	double z0;
	Point3D point;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		x0=taf::randomNumber( a, b );
		y0=taf::randomNumber( a, b );
		z0=taf::randomNumber( a, b );
		point.x=x0;
		point.y=y0;
		point.z=z0;
		double scalar = taf::randomNumber(a,b);
		point *= scalar;
		EXPECT_DOUBLE_EQ( (x0 * scalar), point.x );
		EXPECT_DOUBLE_EQ( (y0 * scalar), point.y );
		EXPECT_DOUBLE_EQ( (z0 * scalar), point.z );
	}
}
TEST(Vector3DTests, OperatorTimesScalar) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	Point3D point;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		point=taf::randomPoint(a,b);
		double scalar = taf::randomNumber(a,b);
		Point3D result;
		result = point * scalar;
		EXPECT_DOUBLE_EQ( ( point.x * scalar ), result.x );
		EXPECT_DOUBLE_EQ( ( point.y * scalar ), result.y );
		EXPECT_DOUBLE_EQ( ( point.z * scalar ), result.z );
	}
}
TEST(Vector3DTests, OperatorDivideEqScalar) {
	srand ( time(NULL) );

	double x0;
	double y0;
	double z0;
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	Point3D point;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){

		x0=taf::randomNumber( a, b );
		y0=taf::randomNumber( a, b );
		z0=taf::randomNumber( a, b );
		Vector3D vector( x0, y0, z0 );
		double scalar = 6785.234;
		vector /= scalar;
		EXPECT_DOUBLE_EQ( (x0 / scalar), vector.x );
		EXPECT_DOUBLE_EQ( (y0 / scalar), vector.y );
		EXPECT_DOUBLE_EQ( (z0 / scalar), vector.z );
	}
}
TEST(Vector3DTests, OperatorDivideScalar) {
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
	Vector3D vector( taf::randomPoint(a,b) );
	double scalar = 145.89;
	Vector3D result;
	result = vector / scalar;
	EXPECT_DOUBLE_EQ( ( vector.x / scalar ), result.x );
	EXPECT_DOUBLE_EQ( ( vector.y / scalar ), result.y );
	EXPECT_DOUBLE_EQ( ( vector.z / scalar ), result.z );
	}
}
TEST(Vector3DTests, OperatorNegativeVector) {

	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
	Vector3D vector( taf::randomPoint(a,b));
	Vector3D vectorInv=-vector;
	EXPECT_DOUBLE_EQ(-vector.x, vectorInv.x );
	EXPECT_DOUBLE_EQ(-vector.y, vectorInv.y );
  	EXPECT_DOUBLE_EQ(-vector.z, vectorInv.z );
	}
}
TEST(Vector3DTests,CreateVectorZero){
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		Point3D point=taf::randomPoint(a,b);
		Vector3D vector(point);

		EXPECT_DOUBLE_EQ(vector.x, point.x);
		EXPECT_DOUBLE_EQ(vector.y, point.y);
		EXPECT_DOUBLE_EQ(vector.z, point.z);
		vector.zero();
		EXPECT_DOUBLE_EQ(vector.x, 0.0);
		EXPECT_DOUBLE_EQ(vector.y, 0.0);
		EXPECT_DOUBLE_EQ(vector.z, 0.0);
	}
}
TEST(Vector3DTests,OperatorLengthSquared){

	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
	double x0=taf::randomNumber(a,b);
	double y0=taf::randomNumber(a,b);
	double z0=taf::randomNumber(a,b);
	Vector3D v(x0,y0,z0);
	double result=x0*x0+y0*y0+z0*z0;
	EXPECT_DOUBLE_EQ(result, v.lengthSquared());
	}
}
TEST(Vector3DTests,OperatorLength){

	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		double x0=taf::randomNumber(a,b);
		double y0=taf::randomNumber(a,b);
		double z0=taf::randomNumber(a,b);
		Vector3D v(x0,y0,z0);
		double result=sqrt(x0*x0+y0*y0+z0*z0);
		EXPECT_DOUBLE_EQ(result, v.length());
	}
}
TEST(Vector3DTests, NonMemberFunctionOperatorPlus)
{
	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
	  Vector3D lhs( taf::randomPoint(a,b));
	  Vector3D rhs( taf::randomPoint(a,b) );
	  Vector3D result = lhs + rhs;
	  EXPECT_DOUBLE_EQ( ( lhs.x + rhs.x ), result.x );
	  EXPECT_DOUBLE_EQ( ( lhs.y + rhs.y ), result.y );
	  EXPECT_DOUBLE_EQ( ( lhs.z + rhs.z ), result.z );
	}
}

TEST(Vector3DTests, NonMemberFunctionOperatorMinus)
{
	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Vector3D lhs( taf::randomPoint(a,b));
		  Vector3D rhs( taf::randomPoint(a,b) );
		  Vector3D result = lhs - rhs;
		  EXPECT_DOUBLE_EQ( ( lhs.x - rhs.x ), result.x );
		  EXPECT_DOUBLE_EQ( ( lhs.y - rhs.y ), result.y );
		  EXPECT_DOUBLE_EQ( ( lhs.z - rhs.z ), result.z );
	}
}

TEST(Vector3DTests, NonMemberFunctionOperatorPlusScalar)
{
	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
	  Vector3D normalVector( taf::randomPoint(a,b) );
	  double scalar = taf::randomNumber(a,b);
	  Vector3D result = scalar * normalVector;
	  result = scalar * normalVector;
	  EXPECT_DOUBLE_EQ( ( scalar*normalVector.x ), result.x );
	  EXPECT_DOUBLE_EQ( ( scalar*normalVector.y ), result.y );
	  EXPECT_DOUBLE_EQ( ( scalar*normalVector.z ), result.z );
	}
}
TEST(Vector3DTests, OperatorDotProductVectorVector)
{
	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Vector3D v0( taf::randomPoint(a,b) );
		  Vector3D v1( taf::randomPoint(a,b) );
		  double result =v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
		  EXPECT_DOUBLE_EQ(DotProduct(v0,v1), result);
	}
}
TEST(Vector3DTests, OperatorDotProductVectorNormalVector)
{
	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Vector3D v0( taf::randomPoint(a,b) );
		  Vector3D vect(taf::randomPoint(a,b));
		  NormalVector n0( vect );
		  double result =v0.x*n0.x + v0.y*n0.y + v0.z*n0.z;
		  EXPECT_DOUBLE_EQ(DotProduct(v0,n0), result );
	}
}
TEST(Vector3DTests, OperatorDotProductNormalVectorVector)
{
	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		  Vector3D vect(taf::randomPoint(a,b));
		  NormalVector n0( vect );
		  Vector3D v0( taf::randomPoint(a,b) );
		  double result =n0.x*v0.x + n0.y*v0.y + n0.z*v0.z;
		  EXPECT_DOUBLE_EQ(DotProduct(n0,v0), result );
	}
}
TEST(Vector3DTests, OperatorCrossProductVectorVector){
	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
	  Vector3D vA( taf::randomPoint(a,b) );
	  Vector3D vB( taf::randomPoint(a,b) );
	  double x0= ( vA.y * vB.z ) - ( vA.z * vB.y );
	  double y0= ( vA.z*vB.x ) - ( vA.x*vB.z );
	  double z0= ( vA.x*vB.y ) - ( vA.y*vB.x );
	  EXPECT_DOUBLE_EQ(CrossProduct(vA,vB).x, x0 );
	  EXPECT_DOUBLE_EQ(CrossProduct(vA,vB).y, y0 );
	  EXPECT_DOUBLE_EQ(CrossProduct(vA,vB).z, z0 );
	}
}
TEST(Vector3DTests, OperatorCrossProductVectorNormalVector)
{
	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		Vector3D vA( taf::randomPoint(a,b) );
		NormalVector nB( Vector3D(taf::randomPoint(a,b)) );
		double x0= ( vA.y * nB.z ) - ( vA.z * nB.y );
		double y0= ( vA.z*nB.x ) - ( vA.x*nB.z );
		double z0= ( vA.x*nB.y ) - ( vA.y*nB.x );
		EXPECT_DOUBLE_EQ(CrossProduct(vA,nB).x, x0 );
	    EXPECT_DOUBLE_EQ(CrossProduct(vA,nB).y, y0 );
		EXPECT_DOUBLE_EQ(CrossProduct(vA,nB).z, z0 );
	}
}
TEST(Vector3DTests, OperatorCrossProductNormalVectorVector)
{
	srand ( time(NULL) );
	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		NormalVector nA( Vector3D(taf::randomPoint(a,b)) );
		Vector3D vB( taf::randomPoint(a,b) );
		double x0= ( nA.y * vB.z ) - ( nA.z * vB.y );
		double y0= ( nA.z*vB.x ) - ( nA.x*vB.z );
		double z0= ( nA.x*vB.y ) - ( nA.y*vB.x );

		EXPECT_DOUBLE_EQ(CrossProduct(nA,vB).x, x0 );
		EXPECT_DOUBLE_EQ(CrossProduct(nA,vB).y, y0 );
		EXPECT_DOUBLE_EQ(CrossProduct(nA,vB).z, z0 );
	}
}
