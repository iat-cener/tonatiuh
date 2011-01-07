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
#include <algorithm>
#include <time.h>

#include "tgc.h"
#include "TestsAuxiliaryFunctions.h"
#include "BBox.h"
#include "Ray.h"

// Extension of the testing space
const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 500000;

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

  // Extension of the testing space
  double b = maximumCoordinate;
  double a = -b;

  Point3D point;

  for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
  {
	  point = taf::randomPoint( a, b );
	  BBox boundingBox( point );
	  EXPECT_TRUE( boundingBox.pMin == point );
	  EXPECT_TRUE( boundingBox.pMax == point );
  }
}

TEST( BBoxTests, ConstructorTwoPoint3D )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  // Extension of the testing space
  double b = maximumCoordinate;
  double a = -b;

  Point3D point1, point2;

  for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
  {
	  point1 = taf::randomPoint( a, b );
	  point2 = taf::randomPoint( a, b );
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
  // initialize random seed
  srand ( time(NULL) );

  // Extension of the testing space
  double b = maximumCoordinate;
  double a = -b;

  BBox boundingBoxA, boundingBoxB;

  for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
  {
 	  boundingBoxA = taf::randomBox( a, b );
 	  boundingBoxB = taf::randomBox( a, b );

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
  // initialize random seed
  srand ( time(NULL) );

  // Extension of the testing space
  double b = maximumCoordinate;
  double a = -b;

  BBox boundingBox;
  Point3D point;

  for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
  {
 	  boundingBox = taf::randomBox( a, b );
      point = taf::randomPoint( a, b );

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

  // Extension of the testing space
  double b = maximumCoordinate;
  double a = -b;

  BBox boundingBoxA, boundingBoxB;
  double delta;
  double relativeError[6];

  for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
  {
 	  boundingBoxA = taf::randomBox( a, b );
 	  boundingBoxB = boundingBoxA;
	  delta = taf::randomNumber( tgc::Epsilon, b );
	  boundingBoxB.Expand( delta );

	  relativeError[0] = std::abs( boundingBoxA.pMin.x - boundingBoxB.pMin.x - delta )/delta;
	  relativeError[1] = std::abs( boundingBoxA.pMin.y - boundingBoxB.pMin.y - delta )/delta;
	  relativeError[2] = std::abs( boundingBoxA.pMin.z - boundingBoxB.pMin.z - delta )/delta;
	  relativeError[3] = std::abs( boundingBoxB.pMax.x - boundingBoxA.pMax.x - delta )/delta;
	  relativeError[4] = std::abs( boundingBoxB.pMax.y - boundingBoxA.pMax.y - delta )/delta;
	  relativeError[5] = std::abs( boundingBoxB.pMax.z - boundingBoxA.pMax.z - delta )/delta;

	  EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError[0], 0.00000001);
	  EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError[1], 0.00000001);
	  EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError[2], 0.00000001);
	  EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError[3], 0.00000001);
	  EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError[4], 0.00000001);
	  EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError[5], 0.00000001);
  }
}

TEST( BBoxTests, Volume )
{
  /* initialize random seed: */
  srand ( time(NULL) );

  // Extension of the testing space
  double b = maximumCoordinate;
  double a = -b;

  BBox boundingBox;

  for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
  {
 	  boundingBox = taf::randomBox( a, b );

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

  // Extension of the testing space
  double b = maximumCoordinate;
  double a = -b;

  BBox boundingBox;
  double dx, dy, dz;
  int maximumExtent;

  for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
  {
 	  boundingBox = taf::randomBox( a, b );
	  dx = boundingBox.pMax.x - boundingBox.pMin.x;
	  dy = boundingBox.pMax.y - boundingBox.pMin.y;
	  dz = boundingBox.pMax.z - boundingBox.pMin.z;
	  maximumExtent = ( dx >= dy ) ? ( ( dx >= dz ) ? 0 : 2 ) : ( ( dy >= dz ) ? 1 : 2 );

	  EXPECT_EQ( maximumExtent, boundingBox.MaximumExtent() );
  }
}

TEST( BBoxTests, BoundingSphere )
{
  // initialize random seed:
  srand ( time(NULL) );

  // Extension of the testing space
  double b = maximumCoordinate;
  double a = -b;

  BBox boundingBox;
  Point3D center;
  double radius;

  for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
  {
 	  boundingBox = taf::randomBox( a, b );
	  boundingBox.BoundingSphere( center, radius );

	  double xCenter = 0.5 * ( boundingBox.pMax.x + boundingBox.pMin.x );
	  double yCenter = 0.5 * ( boundingBox.pMax.y + boundingBox.pMin.y );
	  double zCenter = 0.5 * ( boundingBox.pMax.z + boundingBox.pMin.z );
	  double xDiameter = boundingBox.pMax.x - boundingBox.pMin.x;
	  double yDiameter = boundingBox.pMax.y - boundingBox.pMin.y;
	  double zDiameter = boundingBox.pMax.z - boundingBox.pMin.z;
	  double expectedRadius = 0.5 * std::sqrt( xDiameter*xDiameter + yDiameter*yDiameter + zDiameter*zDiameter );
	  double radiusRelativeError = std::abs( radius - expectedRadius )/expectedRadius;

	  EXPECT_PRED_FORMAT2(::testing::DoubleLE, radiusRelativeError, 0.00000001);
	  EXPECT_DOUBLE_EQ( xCenter, center.x );
	  EXPECT_DOUBLE_EQ( yCenter, center.y );
	  EXPECT_DOUBLE_EQ( zCenter, center.z );
  }
}

TEST( BBoxTests, IntersectP )
{
   // initialize random seed:
   srand ( time(NULL) );

   // Extension of the testing space
   double b = maximumCoordinate;
   double a = -b;

   BBox boundingBox;
   Ray ray;

   for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
   {
  	  boundingBox = taf::randomBox( a, b );
  	  ray = taf::randomRay( a, b );

      double tNear = ray.mint;
      double tFar = ray.maxt;
      bool intersection = boundingBox.IntersectP( ray, &tNear, &tFar );

      bool expectedIntersection = false;

	  Point3D intersectionPoint;
	  std::vector<double> tValidIntersection;
	  double t = ( boundingBox.pMin.x - ray.origin.x ) / ray.direction.x;
	  if( t >= ray.mint && t <= ray.maxt)
	  {
	     intersectionPoint = ray( t );
	     if ( ( intersectionPoint.y > boundingBox.pMin.y ) && ( intersectionPoint.y < boundingBox.pMax.y ) &&
		      ( intersectionPoint.z > boundingBox.pMin.z ) && ( intersectionPoint.z < boundingBox.pMax.z ) ) tValidIntersection.push_back(t);
	  }
	  t = ( boundingBox.pMax.x - ray.origin.x ) / ray.direction.x;
	  if( t >= ray.mint && t <= ray.maxt)
	  {
	     intersectionPoint = ray( t );
	     if ( ( intersectionPoint.y > boundingBox.pMin.y ) && ( intersectionPoint.y < boundingBox.pMax.y ) &&
		      ( intersectionPoint.z > boundingBox.pMin.z ) && ( intersectionPoint.z < boundingBox.pMax.z ) ) tValidIntersection.push_back(t);
	  }
	  t = ( boundingBox.pMin.y - ray.origin.y ) / ray.direction.y;
	  if( t >= ray.mint && t <= ray.maxt)
	  {
	     intersectionPoint = ray( t );
	     if ( ( intersectionPoint.x > boundingBox.pMin.x ) && ( intersectionPoint.x < boundingBox.pMax.x ) &&
		      ( intersectionPoint.z > boundingBox.pMin.z ) && ( intersectionPoint.z < boundingBox.pMax.z ) ) tValidIntersection.push_back(t);
	  }
	  t = ( boundingBox.pMax.y - ray.origin.y ) / ray.direction.y;
	  if( t >= ray.mint && t <= ray.maxt)
	  {
	     intersectionPoint = ray( t );
	     if ( ( intersectionPoint.x > boundingBox.pMin.x ) && ( intersectionPoint.x < boundingBox.pMax.x ) &&
		      ( intersectionPoint.z > boundingBox.pMin.z ) && ( intersectionPoint.z < boundingBox.pMax.z ) ) tValidIntersection.push_back(t);
	  }
	  t = ( boundingBox.pMin.z - ray.origin.z ) / ray.direction.z;
	  if( t >= ray.mint && t <= ray.maxt)
	  {
	     intersectionPoint = ray( t );
	     if ( ( intersectionPoint.x > boundingBox.pMin.x ) && ( intersectionPoint.x < boundingBox.pMax.x ) &&
		      ( intersectionPoint.y > boundingBox.pMin.y ) && ( intersectionPoint.y < boundingBox.pMax.y ) ) tValidIntersection.push_back(t);
	  }
	  t = ( boundingBox.pMax.z - ray.origin.z ) / ray.direction.z;
	  if( t >= ray.mint && t <= ray.maxt)
	  {
	     intersectionPoint = ray( t );
	     if ( ( intersectionPoint.x > boundingBox.pMin.x ) && ( intersectionPoint.x < boundingBox.pMax.x ) &&
		      ( intersectionPoint.y > boundingBox.pMin.y ) && ( intersectionPoint.y < boundingBox.pMax.y ) ) tValidIntersection.push_back(t);
	  }

	  double expectedtNear = tgc::Infinity;
	  double expectedtFar = tgc::Infinity;
      if( tValidIntersection.size() )
      {
    	  expectedIntersection = true;

    	  std::sort( tValidIntersection.begin(), tValidIntersection.end() );

    	  if( tValidIntersection.size() > 1 )
    	  {
        	  expectedtNear  = tValidIntersection[0];
    		  expectedtFar = tValidIntersection[1];
    	  }
    	  else
    	  {
        	  expectedtNear  = ray.mint;
    		  expectedtFar = tValidIntersection[0];
    	  }
      }

	  EXPECT_EQ( expectedIntersection, intersection );

	  if( intersection )
	  {
	     EXPECT_DOUBLE_EQ( expectedtNear, tNear );
	     EXPECT_DOUBLE_EQ( expectedtFar, tFar );
	  }

   }
}

TEST( BBoxTests, UnionBBoxPoint3D )
{
   // initialize random seed:
   srand ( time(NULL) );

   // Extension of the testing space
   double b = maximumCoordinate;
   double a = -b;

   Point3D point;
   BBox box, unionBox;

   for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
   {
      point = taf::randomPoint( a, b);
	  box = taf::randomBox( a, b );
	  unionBox = Union( box, point );

      double xmin = std::min( box.pMin.x, point.x );
      double ymin = std::min( box.pMin.y, point.y );
      double zmin = std::min( box.pMin.z, point.z );

	  double xmax = std::max( box.pMax.x, point.x );
	  double ymax = std::max( box.pMax.y, point.y );
	  double zmax = std::max( box.pMax.z, point.z );

      EXPECT_DOUBLE_EQ( xmin, unionBox.pMin.x );
	  EXPECT_DOUBLE_EQ( ymin, unionBox.pMin.y );
	  EXPECT_DOUBLE_EQ( zmin, unionBox.pMin.z );

	  EXPECT_DOUBLE_EQ( xmax, unionBox.pMax.x );
	  EXPECT_DOUBLE_EQ( ymax, unionBox.pMax.y );
	  EXPECT_DOUBLE_EQ( zmax, unionBox.pMax.z );
   }
}

TEST( BBoxTests, UnionBBoxBBox )
{
   // initialize random seed:
   srand ( time(NULL) );

   // Extension of the testing space
   double b = maximumCoordinate;
   double a = -b;

   BBox box1, box2, unionBox;

   for( unsigned long int i = 0; i < maximumNumberOfTests; i++ )
   {
	  box1 = taf::randomBox( a, b );
	  box2 = taf::randomBox( a, b );
	  unionBox = Union( box1, box2 );

      double xmin = std::min( box1.pMin.x, box2.pMin.x );
      double ymin = std::min( box1.pMin.y, box2.pMin.y );
      double zmin = std::min( box1.pMin.z, box2.pMin.z );

	  double xmax = std::max( box1.pMax.x, box2.pMax.x );
	  double ymax = std::max( box1.pMax.y, box2.pMax.y );
	  double zmax = std::max( box1.pMax.z, box2.pMax.z );

      EXPECT_DOUBLE_EQ( xmin, unionBox.pMin.x );
	  EXPECT_DOUBLE_EQ( ymin, unionBox.pMin.y );
	  EXPECT_DOUBLE_EQ( zmin, unionBox.pMin.z );

	  EXPECT_DOUBLE_EQ( xmax, unionBox.pMax.x );
	  EXPECT_DOUBLE_EQ( ymax, unionBox.pMax.y );
	  EXPECT_DOUBLE_EQ( zmax, unionBox.pMax.z );
   }
}
