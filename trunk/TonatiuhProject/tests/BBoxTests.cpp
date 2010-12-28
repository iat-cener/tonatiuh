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

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <gtest/gtest.h>
#include <stdlib.h>
#include <time.h>

#include "tgc.h"




#include "BBox.h"

TEST( BBoxTests, ConstructorDefault )
{
  Point3D plusInfinity( tgc::Infinity, tgc::Infinity, tgc::Infinity );
  Point3D minusInfinity( -tgc::Infinity, -tgc::Infinity, -tgc::Infinity );

  BBox boundingBox;

  EXPECT_TRUE( boundingBox.pMin == plusInfinity );
  EXPECT_TRUE( boundingBox.pMax == minusInfinity );
}

TEST( BBoxTests, ConstructorOnePoint3D )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  int maxPoints = 50;
  for( int i = 0; i < maxPoints; i++ )
  {
	  Point3D point( 0.27*rand(), 1234.23*rand(), -23.568*rand() );
	  BBox boundingBox( point );
	  EXPECT_TRUE( boundingBox.pMin == point );
	  EXPECT_TRUE( boundingBox.pMax == point );
  }
}

TEST( BBoxTests, ConstructorTwoPoint3D )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  int maxPoints = 50;
  for( int i = 0; i < maxPoints; i++ )
  {
	  Point3D point1( 2.197*rand(), -34.23909*rand(), 3.568*rand() );
	  Point3D point2( -982.917*rand(), 423.2378*rand(), 68.21*rand() );

	  BBox boundingBox( point1, point2 );
	  EXPECT_TRUE( boundingBox.pMin.x <= point1.x );
	  EXPECT_TRUE( boundingBox.pMin.x <= point2.x );
	  EXPECT_TRUE( boundingBox.pMin.y <= point1.y );
	  EXPECT_TRUE( boundingBox.pMin.y <= point2.y );
	  EXPECT_TRUE( boundingBox.pMin.z <= point1.z );
	  EXPECT_TRUE( boundingBox.pMin.z <= point2.z );

	  EXPECT_TRUE( boundingBox.pMax.x >= point1.x );
	  EXPECT_TRUE( boundingBox.pMax.x >= point2.x );
	  EXPECT_TRUE( boundingBox.pMax.y >= point1.y );
	  EXPECT_TRUE( boundingBox.pMax.y >= point2.y );
	  EXPECT_TRUE( boundingBox.pMax.z >= point1.z );
	  EXPECT_TRUE( boundingBox.pMax.z >= point2.z );
  }
}

TEST( BBoxTests, Overlaps )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  int maxPoints = 200;
  for( int i = 0; i < maxPoints; i++ )
  {
	  Point3D pointA1( -107.9790*rand(), 299.39490*rand(), 8.628*rand() );
	  Point3D pointA2( -982.917*rand(), 423.2378*rand(), 68.21*rand() );
	  BBox boundingBoxA( pointA1, pointA2 );

	  Point3D pointB1(  -102.934*rand(), 304.23909*rand(),  3.568*rand() );
	  Point3D pointB2(  1000.167*rand(),  473.3738*rand(), 48.172*rand() );
	  BBox boundingBoxB( pointB1, pointB2 );

	  bool overlap = ( boundingBoxA.pMin.x <= boundingBoxB.pMax.x   ) &&
			         ( boundingBoxA.pMax.x >= boundingBoxB.pMin.x   ) &&
			         ( boundingBoxA.pMin.y <= boundingBoxB.pMax.y   ) &&
			         ( boundingBoxA.pMax.y >= boundingBoxB.pMin.y   ) &&
			         ( boundingBoxA.pMin.z <= boundingBoxB.pMax.z   ) &&
			         ( boundingBoxA.pMax.z >= boundingBoxB.pMin.z   );

	  EXPECT_TRUE( boundingBoxA.Overlaps( boundingBoxB ) == overlap );
	  EXPECT_TRUE( boundingBoxB.Overlaps( boundingBoxA ) == overlap );
  }
}

TEST( BBoxTests, Inside )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  int maxPoints = 200;
  for( int i = 0; i < maxPoints; i++ )
  {
	  Point3D point1( -107.9790*rand(), 299.39490*rand(), 8.628*rand() );
	  Point3D point2(  1000.167*rand(), 473.3738*rand(), 48.17*rand() );
	  BBox boundingBox( point1, point2 );

	  Point3D point( 503.47*rand(), 214.95*rand(), 23.568*rand() );

	  bool inside = ( point.x >= boundingBox.pMin.x   ) &&
			        ( point.x <= boundingBox.pMax.x   ) &&
			        ( point.y >= boundingBox.pMin.y   ) &&
			        ( point.y <= boundingBox.pMax.y   ) &&
			        ( point.z >= boundingBox.pMin.z   ) &&
			        ( point.z <= boundingBox.pMax.z   );

	  EXPECT_TRUE( boundingBox.Inside( point ) == inside );
  }
}

TEST( BBoxTests, Expand )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  int maxPoints = 200;
  for( int i = 0; i < maxPoints; i++ )
  {
	  Point3D point1( -107.9790*rand(), 299.39490*rand(), 8.628*rand() );
	  Point3D point2(  100.167*rand(), 473.3738*rand(), 48.17*rand() );
	  BBox boundingBoxA( point1, point2 );
	  BBox boundingBoxB( point1, point2 );

	  double delta = 0.369 * rand();
	  boundingBoxB.Expand( delta );

	  EXPECT_DOUBLE_EQ( boundingBoxB.pMin.x + delta, boundingBoxA.pMin.x );
	  EXPECT_DOUBLE_EQ( boundingBoxB.pMin.y + delta, boundingBoxA.pMin.y );
	  EXPECT_DOUBLE_EQ( boundingBoxB.pMin.z + delta, boundingBoxA.pMin.z );
	  EXPECT_DOUBLE_EQ( boundingBoxA.pMax.x + delta, boundingBoxB.pMax.x );
	  EXPECT_DOUBLE_EQ( boundingBoxA.pMax.y + delta, boundingBoxB.pMax.y );
	  EXPECT_DOUBLE_EQ( boundingBoxA.pMax.z + delta, boundingBoxB.pMax.z );


  }
}

TEST( BBoxTests, Volume )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  int maxPoints = 200;
  for( int i = 0; i < maxPoints; i++ )
  {
	  Point3D point1( -107.9790*rand(), 299.39490*rand(), 8.628*rand() );
	  Point3D point2(  100.167*rand(), 473.3738*rand(), 48.17*rand() );
	  BBox boundingBox( point1, point2 );

	  double xLength = boundingBox.pMax.x - boundingBox.pMin.x;
	  double yLength = boundingBox.pMax.y - boundingBox.pMin.y;
	  double zLength = boundingBox.pMax.z - boundingBox.pMin.z;
	  double volume = xLength * yLength * zLength;

	  EXPECT_DOUBLE_EQ( volume, boundingBox.Volume() );
  }
}

TEST( BBoxTests, MaximumExtent )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  int maxPoints = 200;
  for( int i = 0; i < maxPoints; i++ )
  {
	  Point3D point1( -107.9790*rand(), 299.39490*rand(), 8.628*rand() );
	  Point3D point2(  100.167*rand(), 473.3738*rand(), 48.17*rand() );
	  BBox boundingBox( point1, point2 );

	  double dx = boundingBox.pMax.x - boundingBox.pMin.x;
	  double dy = boundingBox.pMax.y - boundingBox.pMin.y;
	  double dz = boundingBox.pMax.z - boundingBox.pMin.z;

	  int maximumExtent = ( dx >= dy ) ? ( ( dx >= dz ) ? 0 : 2 ) : ( ( dy >= dz ) ? 1 : 2 );

	  EXPECT_DOUBLE_EQ( maximumExtent, boundingBox.MaximumExtent() );
  }
}

TEST( BBoxTests, BoundingSphere )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  int maxPoints = 200;
  for( int i = 0; i < maxPoints; i++ )
  {
	  Point3D point1( -1.079790*( rand() % 10000 ), 0.29939490*( rand() % 10000 ), 0.8628*( rand() % 10000 ) );
	  Point3D point2(  1.00167*( rand() % 10000 ), 0.4733738*( rand() % 10000 ), 0.4817*( rand() % 10000 ) );
	  BBox boundingBox( point1, point2 );

	  Point3D center;
	  double radius;
	  boundingBox.BoundingSphere( center, radius );

	  double xCenter = 0.5 * ( boundingBox.pMax.x + boundingBox.pMin.x );
	  double yCenter = 0.5 * ( boundingBox.pMax.y + boundingBox.pMin.y );
	  double zCenter = 0.5 * ( boundingBox.pMax.z + boundingBox.pMin.z );
	  double xRadius = 0.5 * ( boundingBox.pMax.x - boundingBox.pMin.x );
	  double yRadius = 0.5 * ( boundingBox.pMax.y - boundingBox.pMin.y );
	  double zRadius = 0.5 * ( boundingBox.pMax.z - boundingBox.pMin.z );

	  Point3D expectedCenter( xCenter, yCenter, zCenter );
	  double expectedRadius = Distance( expectedCenter, boundingBox.pMax );
	  double expectedRadius2 = std::sqrt( xRadius*xRadius + yRadius*yRadius + zRadius+zRadius );

	  EXPECT_DOUBLE_EQ( expectedRadius, radius );
	  EXPECT_DOUBLE_EQ( xCenter, center.x );
	  EXPECT_DOUBLE_EQ( yCenter, center.y );
	  EXPECT_DOUBLE_EQ( zCenter, center.z );
  }
}
