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


#include <time.h>

#include <gtest/gtest.h>

#include "tgc.h"
#include "TestsAuxiliaryFunctions.h"

#include "BBox.h"
#include "NormalVector.h"
#include "Ray.h"
#include "Transform.h"

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 1000;

TEST( TransformTests, ConstructorDefault)
{
	Transform	t;

	EXPECT_TRUE( t.GetMatrix() == 0);
	//EXPECT_TRUE( t.m_minv == 0);
}

TEST( TransformTests, ConstructorBidimensionalArray)
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
		Transform  t( m );

		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][0], m[0][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][1], m[0][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][2], m[0][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][3], m[0][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][0], m[1][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][1], m[1][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][2], m[1][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][3], m[1][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][0], m[2][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][1], m[2][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][2], m[2][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][3], m[2][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][0], m[3][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][1], m[3][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][2], m[3][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][3], m[3][3] );
	}

}

TEST( TransformTests, ConstructorFromPtrMatrix4x4)
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
		Matrix4x4 mat(m);
		Ptr<Matrix4x4> matrix(&mat);
		Transform  t( matrix );

		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][0], m[0][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][1], m[0][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][2], m[0][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][3], m[0][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][0], m[1][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][1], m[1][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][2], m[1][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][3], m[1][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][0], m[2][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][1], m[2][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][2], m[2][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][3], m[2][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][0], m[3][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][1], m[3][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][2], m[3][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][3], m[3][3] );
	}

}

TEST( TransformTests, ConstructorUsingTwoPtrMatrix4x4)
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
		Matrix4x4 mat(m);
		Ptr<Matrix4x4> matrix(&mat);
		Ptr<Matrix4x4> inv=mat.Inverse();
		Transform  t( matrix,inv );

		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][0], m[0][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][1], m[0][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][2], m[0][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][3], m[0][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][0], m[1][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][1], m[1][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][2], m[1][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][3], m[1][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][0], m[2][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][1], m[2][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][2], m[2][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][3], m[2][3] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][0], m[3][0] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][1], m[3][1] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][2], m[3][2] );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][3], m[3][3] );

		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[0][0], inv->m[0][0] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[0][1], inv->m[0][1] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[0][2], inv->m[0][2] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[0][3], inv->m[0][3] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[1][0], inv->m[1][0] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[1][1], inv->m[1][1] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[1][2], inv->m[1][2] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[1][3], inv->m[1][3] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[2][0], inv->m[2][0] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[2][1], inv->m[2][1] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[2][2], inv->m[2][2] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[2][3], inv->m[2][3] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[3][0], inv->m[3][0] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[3][1], inv->m[3][1] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[3][2], inv->m[3][2] );
		EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[3][3], inv->m[3][3] );
	}

}
TEST( TransformTests, ConstructorBidimensionalArrayFromPoints)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{

		double t00= taf::randomNumber( a, b );
		double t01= taf::randomNumber( a, b );
		double t02= taf::randomNumber( a, b );
		double t03= taf::randomNumber( a, b );
		double t10= taf::randomNumber( a, b );
		double t11= taf::randomNumber( a, b );
		double t12= taf::randomNumber( a, b );
		double t13= taf::randomNumber( a, b );
		double t20= taf::randomNumber( a, b );
		double t21= taf::randomNumber( a, b );
		double t22= taf::randomNumber( a, b );
		double t23= taf::randomNumber( a, b );
		double t30= taf::randomNumber( a, b );
		double t31= taf::randomNumber( a, b );
		double t32= taf::randomNumber( a, b );
		double t33= taf::randomNumber( a, b );

		Transform  t( t00,t01,t02,t03,
				      t10,t11,t12,t13,
				      t20,t21,t22,t23,
				      t30,t31,t32,t33 );

		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][0], t00 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][1], t01 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][2], t02 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[0][3], t03 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][0], t10 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][1], t11 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][2], t12 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[1][3], t13 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][0], t20 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][1], t21 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][2], t22 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[2][3], t23 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][0], t30 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][1], t31 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][2], t32 );
		EXPECT_DOUBLE_EQ( t.GetMatrix()->m[3][3], t33 );
	}

}

TEST( TransformTests, TransformPointUsingMatrix)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{

		double t00= taf::randomNumber( a, b );
		double t01= taf::randomNumber( a, b );
		double t02= taf::randomNumber( a, b );
		double t03= taf::randomNumber( a, b );
		double t10= taf::randomNumber( a, b );
		double t11= taf::randomNumber( a, b );
		double t12= taf::randomNumber( a, b );
		double t13= taf::randomNumber( a, b );
		double t20= taf::randomNumber( a, b );
		double t21= taf::randomNumber( a, b );
		double t22= taf::randomNumber( a, b );
		double t23= taf::randomNumber( a, b );
		double t30= taf::randomNumber( a, b );
		double t31= taf::randomNumber( a, b );
		double t32= taf::randomNumber( a, b );
		double t33= taf::randomNumber( a, b );

		double x0= taf::randomNumber( a, b );
		double y0= taf::randomNumber( a, b );
		double z0= taf::randomNumber( a, b );

		 Transform t( t00,t01,t02,t03,
				      t10,t11,t12,t13,
				      t20,t21,t22,t23,
				      t30,t31,t32,t33 );
		 Point3D point(x0,y0,z0);
		x0 = t00*point.x + t01*point.y + t02*point.z + t03;
		y0 = t10*point.x + t11*point.y + t12*point.z + t13;
		z0 = t20*point.x + t21*point.y + t22*point.z + t23;
		double wp = t30*point.x + t31*point.y + t32*point.z + t33;
		if( wp != 1.0 ){
			 x0/=wp;
		     y0/=wp;
		     z0/=wp;
		}

		EXPECT_DOUBLE_EQ(t.operator ()(point).x, x0 );
		EXPECT_DOUBLE_EQ(t.operator ()(point).y, y0 );
		EXPECT_DOUBLE_EQ(t.operator ()(point).z, z0 );

	}

}

TEST( TransformTests, TransformPointFromPointUsingMatrix)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{

		double t00= taf::randomNumber( a, b );
		double t01= taf::randomNumber( a, b );
		double t02= taf::randomNumber( a, b );
		double t03= taf::randomNumber( a, b );
		double t10= taf::randomNumber( a, b );
		double t11= taf::randomNumber( a, b );
		double t12= taf::randomNumber( a, b );
		double t13= taf::randomNumber( a, b );
		double t20= taf::randomNumber( a, b );
		double t21= taf::randomNumber( a, b );
		double t22= taf::randomNumber( a, b );
		double t23= taf::randomNumber( a, b );
		double t30= taf::randomNumber( a, b );
		double t31= taf::randomNumber( a, b );
		double t32= taf::randomNumber( a, b );
		double t33= taf::randomNumber( a, b );

		double x0= taf::randomNumber( a, b );
		double y0= taf::randomNumber( a, b );
		double z0= taf::randomNumber( a, b );

		 Transform t( t00,t01,t02,t03,
				      t10,t11,t12,t13,
				      t20,t21,t22,t23,
				      t30,t31,t32,t33 );
		 Point3D point(x0,y0,z0);
		 Point3D result(0,0,0);
		x0 = t00*point.x + t01*point.y + t02*point.z + t03;
		y0 = t10*point.x + t11*point.y + t12*point.z + t13;
		z0 = t20*point.x + t21*point.y + t22*point.z + t23;
		double wp = t30*point.x + t31*point.y + t32*point.z + t33;
		if( wp != 1.0 ){
			 x0/=wp;
		     y0/=wp;
		     z0/=wp;
		}
		t.operator ()(point,result);
		EXPECT_DOUBLE_EQ(result.x, x0 );
		EXPECT_DOUBLE_EQ(result.y, y0 );
		EXPECT_DOUBLE_EQ(result.z, z0 );

	}

}

TEST( TransformTests, TransformVectorUsingMatrix)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{

		double t00= taf::randomNumber( a, b );
		double t01= taf::randomNumber( a, b );
		double t02= taf::randomNumber( a, b );
		double t03= taf::randomNumber( a, b );
		double t10= taf::randomNumber( a, b );
		double t11= taf::randomNumber( a, b );
		double t12= taf::randomNumber( a, b );
		double t13= taf::randomNumber( a, b );
		double t20= taf::randomNumber( a, b );
		double t21= taf::randomNumber( a, b );
		double t22= taf::randomNumber( a, b );
		double t23= taf::randomNumber( a, b );
		double t30= taf::randomNumber( a, b );
		double t31= taf::randomNumber( a, b );
		double t32= taf::randomNumber( a, b );
		double t33= taf::randomNumber( a, b );

		double x0= taf::randomNumber( a, b );
		double y0= taf::randomNumber( a, b );
		double z0= taf::randomNumber( a, b );

		 Transform t( t00,t01,t02,t03,
				      t10,t11,t12,t13,
				      t20,t21,t22,t23,
				      t30,t31,t32,t33 );
		 Vector3D vector(x0,y0,z0);
		x0 = t00*vector.x + t01*vector.y + t02*vector.z;
		y0 = t10*vector.x + t11*vector.y + t12*vector.z;
		z0 = t20*vector.x + t21*vector.y + t22*vector.z;

		EXPECT_DOUBLE_EQ(t.operator ()(vector).x, x0 );
		EXPECT_DOUBLE_EQ(t.operator ()(vector).y, y0 );
		EXPECT_DOUBLE_EQ(t.operator ()(vector).z, z0 );

	}

}

TEST( TransformTests, TransformVectorFromVectorUsingMatrix)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{

		double t00= taf::randomNumber( a, b );
		double t01= taf::randomNumber( a, b );
		double t02= taf::randomNumber( a, b );
		double t03= taf::randomNumber( a, b );
		double t10= taf::randomNumber( a, b );
		double t11= taf::randomNumber( a, b );
		double t12= taf::randomNumber( a, b );
		double t13= taf::randomNumber( a, b );
		double t20= taf::randomNumber( a, b );
		double t21= taf::randomNumber( a, b );
		double t22= taf::randomNumber( a, b );
		double t23= taf::randomNumber( a, b );
		double t30= taf::randomNumber( a, b );
		double t31= taf::randomNumber( a, b );
		double t32= taf::randomNumber( a, b );
		double t33= taf::randomNumber( a, b );

		double x0= taf::randomNumber( a, b );
		double y0= taf::randomNumber( a, b );
		double z0= taf::randomNumber( a, b );

		 Transform t( t00,t01,t02,t03,
				      t10,t11,t12,t13,
				      t20,t21,t22,t23,
				      t30,t31,t32,t33 );
		 Vector3D vector(x0,y0,z0);
		 Vector3D result(0,0,0);
		x0 = t00*vector.x + t01*vector.y + t02*vector.z;
		y0 = t10*vector.x + t11*vector.y + t12*vector.z;
		z0 = t20*vector.x + t21*vector.y + t22*vector.z;

		t.operator ()(vector,result);
		EXPECT_DOUBLE_EQ(result.x, x0 );
		EXPECT_DOUBLE_EQ(result.y, y0 );
		EXPECT_DOUBLE_EQ(result.z, z0 );

	}

}

TEST( TransformTests, TransformNormalVectorUsingMatrix)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{

		double t00= taf::randomNumber( a, b );
		double t01= taf::randomNumber( a, b );
		double t02= taf::randomNumber( a, b );
		double t03= taf::randomNumber( a, b );
		double t10= taf::randomNumber( a, b );
		double t11= taf::randomNumber( a, b );
		double t12= taf::randomNumber( a, b );
		double t13= taf::randomNumber( a, b );
		double t20= taf::randomNumber( a, b );
		double t21= taf::randomNumber( a, b );
		double t22= taf::randomNumber( a, b );
		double t23= taf::randomNumber( a, b );
		double t30= taf::randomNumber( a, b );
		double t31= taf::randomNumber( a, b );
		double t32= taf::randomNumber( a, b );
		double t33= taf::randomNumber( a, b );

		double x0= taf::randomNumber( a, b );
		double y0= taf::randomNumber( a, b );
		double z0= taf::randomNumber( a, b );

		 Transform t( t00,t01,t02,t03,
				      t10,t11,t12,t13,
				      t20,t21,t22,t23,
				      t30,t31,t32,t33 );
		 NormalVector nvector(x0,y0,z0);
		 Matrix4x4 mat(t00,t01,t02,t03,
			      t10,t11,t12,t13,
			      t20,t21,t22,t23,
			      t30,t31,t32,t33 );
		 Ptr<Matrix4x4>in=mat.Inverse();
		x0 =in->m[0][0]*nvector.x + in->m[1][0]*nvector.y + in->m[2][0]*nvector.z;
		y0 =in->m[0][1]*nvector.x + in->m[1][1]*nvector.y + in->m[2][1]*nvector.z;
		z0 = in->m[0][2]*nvector.x + in->m[1][2]*nvector.y + in->m[2][2]*nvector.z;

		EXPECT_DOUBLE_EQ(t.operator ()(nvector).x, x0 );
		EXPECT_DOUBLE_EQ(t.operator ()(nvector).y, y0 );
		EXPECT_DOUBLE_EQ(t.operator ()(nvector).z, z0 );

	}

}

TEST( TransformTests, TransformNormalVectorFromNormalVectorUsingMatrix)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
	{

		double t00= taf::randomNumber( a, b );
		double t01= taf::randomNumber( a, b );
		double t02= taf::randomNumber( a, b );
		double t03= taf::randomNumber( a, b );
		double t10= taf::randomNumber( a, b );
		double t11= taf::randomNumber( a, b );
		double t12= taf::randomNumber( a, b );
		double t13= taf::randomNumber( a, b );
		double t20= taf::randomNumber( a, b );
		double t21= taf::randomNumber( a, b );
		double t22= taf::randomNumber( a, b );
		double t23= taf::randomNumber( a, b );
		double t30= taf::randomNumber( a, b );
		double t31= taf::randomNumber( a, b );
		double t32= taf::randomNumber( a, b );
		double t33= taf::randomNumber( a, b );

		double x0= taf::randomNumber( a, b );
		double y0= taf::randomNumber( a, b );
		double z0= taf::randomNumber( a, b );

		 Transform t( t00,t01,t02,t03,
				      t10,t11,t12,t13,
				      t20,t21,t22,t23,
				      t30,t31,t32,t33 );
		 NormalVector nvector(x0,y0,z0);
		 NormalVector result(0,0,0);
		 Matrix4x4 mat(t00,t01,t02,t03,
		 			   t10,t11,t12,t13,
		 			   t20,t21,t22,t23,
		 			   t30,t31,t32,t33 );
		Ptr<Matrix4x4>in=mat.Inverse();
		x0 =in->m[0][0]*nvector.x + in->m[1][0]*nvector.y + in->m[2][0]*nvector.z;
		y0 =in->m[0][1]*nvector.x + in->m[1][1]*nvector.y + in->m[2][1]*nvector.z;
		z0 = in->m[0][2]*nvector.x + in->m[1][2]*nvector.y + in->m[2][2]*nvector.z;


		t.operator ()(nvector,result);
		EXPECT_DOUBLE_EQ(result.x, x0 );
		EXPECT_DOUBLE_EQ(result.y, y0 );
		EXPECT_DOUBLE_EQ(result.z, z0 );

	}

}

TEST( TransformTests, FunctionGetInverse)
{
	/* initialize random seed: */
		srand ( time(NULL) );

		// Extension of the testing space
		double b = maximumCoordinate;
		double a = -b;

		for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
		{

			double t00= taf::randomNumber( a, b );
			double t01= taf::randomNumber( a, b );
			double t02= taf::randomNumber( a, b );
			double t03= taf::randomNumber( a, b );
			double t10= taf::randomNumber( a, b );
			double t11= taf::randomNumber( a, b );
			double t12= taf::randomNumber( a, b );
			double t13= taf::randomNumber( a, b );
			double t20= taf::randomNumber( a, b );
			double t21= taf::randomNumber( a, b );
			double t22= taf::randomNumber( a, b );
			double t23= taf::randomNumber( a, b );
			double t30= taf::randomNumber( a, b );
			double t31= taf::randomNumber( a, b );
			double t32= taf::randomNumber( a, b );
			double t33= taf::randomNumber( a, b );

			Transform t(  t00,t01,t02,t03,
					      t10,t11,t12,t13,
					      t20,t21,t22,t23,
					      t30,t31,t32,t33 );

			Matrix4x4 mat( t00,t01,t02,t03,
				           t10,t11,t12,t13,
				           t20,t21,t22,t23,
				           t30,t31,t32,t33);

			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[0][0], mat.Inverse()->m[0][0] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[0][1], mat.Inverse()->m[0][1] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[0][2], mat.Inverse()->m[0][2] );
		     EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[0][3], mat.Inverse()->m[0][3] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[1][0], mat.Inverse()->m[1][0] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[1][1], mat.Inverse()->m[1][1] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[1][2], mat.Inverse()->m[1][2] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[1][3], mat.Inverse()->m[1][3] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[2][0], mat.Inverse()->m[2][0] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[2][1], mat.Inverse()->m[2][1] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[2][2], mat.Inverse()->m[2][2] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[2][3], mat.Inverse()->m[2][3] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[3][0], mat.Inverse()->m[3][0] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[3][1], mat.Inverse()->m[3][1] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[3][2], mat.Inverse()->m[3][2] );
			 EXPECT_DOUBLE_EQ( t.GetInverse().GetMatrix()->m[3][3], mat.Inverse()->m[3][3] );
		}
}

TEST( TransformTests, FunctionGetTranspose)
{
	/* initialize random seed: */
		srand ( time(NULL) );

		// Extension of the testing space
		double b = maximumCoordinate;
		double a = -b;

		for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
		{

			double t00= taf::randomNumber( a, b );
			double t01= taf::randomNumber( a, b );
			double t02= taf::randomNumber( a, b );
			double t03= taf::randomNumber( a, b );
			double t10= taf::randomNumber( a, b );
			double t11= taf::randomNumber( a, b );
			double t12= taf::randomNumber( a, b );
			double t13= taf::randomNumber( a, b );
			double t20= taf::randomNumber( a, b );
			double t21= taf::randomNumber( a, b );
			double t22= taf::randomNumber( a, b );
			double t23= taf::randomNumber( a, b );
			double t30= taf::randomNumber( a, b );
			double t31= taf::randomNumber( a, b );
			double t32= taf::randomNumber( a, b );
			double t33= taf::randomNumber( a, b );

			Transform t(  t00,t01,t02,t03,
					      t10,t11,t12,t13,
					      t20,t21,t22,t23,
					      t30,t31,t32,t33 );

			Matrix4x4 mat( t00,t01,t02,t03,
				           t10,t11,t12,t13,
				           t20,t21,t22,t23,
				           t30,t31,t32,t33);

			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[0][0], mat.Transpose()->m[0][0] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[0][1], mat.Transpose()->m[0][1] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[0][2], mat.Transpose()->m[0][2] );
		     EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[0][3], mat.Transpose()->m[0][3] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[1][0], mat.Transpose()->m[1][0] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[1][1], mat.Transpose()->m[1][1] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[1][2], mat.Transpose()->m[1][2] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[1][3], mat.Transpose()->m[1][3] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[2][0], mat.Transpose()->m[2][0] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[2][1], mat.Transpose()->m[2][1] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[2][2], mat.Transpose()->m[2][2] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[2][3], mat.Transpose()->m[2][3] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[3][0], mat.Transpose()->m[3][0] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[3][1], mat.Transpose()->m[3][1] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[3][2], mat.Transpose()->m[3][2] );
			 EXPECT_DOUBLE_EQ( t.Transpose().GetMatrix()->m[3][3], mat.Transpose()->m[3][3] );
		}
}

TEST( TransformTests, FunctionMultVecMatrix)
{
	/* initialize random seed: */
			srand ( time(NULL) );

			// Extension of the testing space
			double b = maximumCoordinate;
			double a = -b;

			for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
			{
				/*Matrix*/
				double t00= taf::randomNumber( a, b );
				double t01= taf::randomNumber( a, b );
				double t02= taf::randomNumber( a, b );
				double t03= taf::randomNumber( a, b );
				double t10= taf::randomNumber( a, b );
				double t11= taf::randomNumber( a, b );
				double t12= taf::randomNumber( a, b );
				double t13= taf::randomNumber( a, b );
				double t20= taf::randomNumber( a, b );
				double t21= taf::randomNumber( a, b );
				double t22= taf::randomNumber( a, b );
				double t23= taf::randomNumber( a, b );
				double t30= taf::randomNumber( a, b );
				double t31= taf::randomNumber( a, b );
				double t32= taf::randomNumber( a, b );
				double t33= taf::randomNumber( a, b );

				Transform t(  t00,t01,t02,t03,
							  t10,t11,t12,t13,
							  t20,t21,t22,t23,
							  t30,t31,t32,t33 );

				/*vector*/
				double x0= taf::randomNumber( a, b );
				double y0= taf::randomNumber( a, b );
				double z0= taf::randomNumber( a, b );

				Vector3D vector(x0,y0,z0);
				double W=vector.x * t30 + vector.y * t31 + vector.z * t32 + t33;
				 if (W != 0.0f){
					 	 x0=( vector.x * t00 + vector.y * t01 + vector.z * t02 + t03 )/W;
					 	 y0=( vector.x * t10 + vector.y * t11 + vector.z * t12 + t13 )/W;
					 	 z0=( vector.x * t20 + vector.y * t21 + vector.z * t22 + t23 )/W;
				 }
				 else{
					 x0=y0=z0=0;
				 }
				EXPECT_DOUBLE_EQ( t.multVecMatrix(vector).x, x0 );
				EXPECT_DOUBLE_EQ( t.multVecMatrix(vector).y, y0 );
				EXPECT_DOUBLE_EQ( t.multVecMatrix(vector).z, z0 );
			}
}

TEST( TransformTests, FunctionMultDirMatrix)
{
	/* initialize random seed: */
			srand ( time(NULL) );

			// Extension of the testing space
			double b = maximumCoordinate;
			double a = -b;

			for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
			{
				/*Matrix*/
				double t00= taf::randomNumber( a, b );
				double t01= taf::randomNumber( a, b );
				double t02= taf::randomNumber( a, b );
				double t03= taf::randomNumber( a, b );
				double t10= taf::randomNumber( a, b );
				double t11= taf::randomNumber( a, b );
				double t12= taf::randomNumber( a, b );
				double t13= taf::randomNumber( a, b );
				double t20= taf::randomNumber( a, b );
				double t21= taf::randomNumber( a, b );
				double t22= taf::randomNumber( a, b );
				double t23= taf::randomNumber( a, b );
				double t30= taf::randomNumber( a, b );
				double t31= taf::randomNumber( a, b );
				double t32= taf::randomNumber( a, b );
				double t33= taf::randomNumber( a, b );

				Transform t(  t00,t01,t02,t03,
							  t10,t11,t12,t13,
							  t20,t21,t22,t23,
							  t30,t31,t32,t33 );

				/*vector*/
				double x0= taf::randomNumber( a, b );
				double y0= taf::randomNumber( a, b );
				double z0= taf::randomNumber( a, b );

				Vector3D vector(x0,y0,z0);

				x0=( vector.x * t00 + vector.y * t01 + vector.z * t02 );
				y0=( vector.x * t10 + vector.y * t11 + vector.z * t12 );
				z0=( vector.x * t20 + vector.y * t21 + vector.z * t22 );

				EXPECT_DOUBLE_EQ( t.multDirMatrix(vector).x, x0 );
				EXPECT_DOUBLE_EQ( t.multDirMatrix(vector).y, y0 );
				EXPECT_DOUBLE_EQ( t.multDirMatrix(vector).z, z0 );
			}
}

TEST( TransformTests, FunctionSwapsHandedness)
{
	/* initialize random seed: */
		srand ( time(NULL) );

		// Extension of the testing space
		double b = maximumCoordinate;
		double a = -b;

		for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
		{

			double t00= taf::randomNumber( a, b );
			double t01= taf::randomNumber( a, b );
			double t02= taf::randomNumber( a, b );
			double t03= taf::randomNumber( a, b );
			double t10= taf::randomNumber( a, b );
			double t11= taf::randomNumber( a, b );
			double t12= taf::randomNumber( a, b );
			double t13= taf::randomNumber( a, b );
			double t20= taf::randomNumber( a, b );
			double t21= taf::randomNumber( a, b );
			double t22= taf::randomNumber( a, b );
			double t23= taf::randomNumber( a, b );
			double t30= taf::randomNumber( a, b );
			double t31= taf::randomNumber( a, b );
			double t32= taf::randomNumber( a, b );
			double t33= taf::randomNumber( a, b );

			Transform t(  t00,t01,t02,t03,
					      t10,t11,t12,t13,
					      t20,t21,t22,t23,
					      t30,t31,t32,t33 );

			Matrix4x4 mat( t00,t01,t02,t03,
				           t10,t11,t12,t13,
				           t20,t21,t22,t23,
				           t30,t31,t32,t33);
             double det= ( ( t00 *( t11 * t22 - t12 * t21 ) ) -
                         ( t01 *( t10 * t22 - t12 * t20 ) ) +
                         ( t02 *( t10 * t21 - t11 * t20 ) ) );
			 EXPECT_TRUE(t.SwapsHandedness()==(det < 0) );

		}
}
