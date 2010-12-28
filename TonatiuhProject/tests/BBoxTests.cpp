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

	  Point3D pointB1( -102.9347*rand(), 304.23909*rand(), 3.568*rand() );
	  Point3D pointB2( -1000.167*rand(), 473.3738*rand(), 48.17*rand() );
	  BBox boundingBoxB( pointB1, pointB2 );

	  bool overlap = ( boundingBoxA.pMin.x <= boundingBoxB.pMax.x   ) &
			         ( boundingBoxA.pMax.x >= boundingBoxB.pMin.x   ) &
			         ( boundingBoxA.pMin.y <= boundingBoxB.pMax.y   ) &
			         ( boundingBoxA.pMax.y >= boundingBoxB.pMin.y   ) &
			         ( boundingBoxA.pMin.z <= boundingBoxB.pMax.z   ) &
			         ( boundingBoxA.pMax.z >= boundingBoxB.pMin.z   );

	  EXPECT_TRUE( boundingBoxA.Overlaps( boundingBoxB ) == overlap );
	  EXPECT_TRUE( boundingBoxB.Overlaps( boundingBoxA ) == overlap );
  }
}
