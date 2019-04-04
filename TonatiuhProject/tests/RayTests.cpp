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

#include <vector>

#include "Ray.h"

#include <gtest/gtest.h>

static void generateRays( std::vector<Ray>& rays );


TEST(RayTests, ConstructorDefault) {
  Ray ray;
  EXPECT_TRUE( ray.mint == gc::Epsilon );
  EXPECT_TRUE( ray.maxt == gc::Infinity );
}

TEST(RayTests, ConstructorOriginVectorDefaultts) {
  double number[6] = { 0.0, -1.2, 900.02, -120.23, 14.234, 3.82 };
  double combination[18][6]= { { 0,1,2,3,4,5 }, { 0,1,2,3,5,4 },
		                       { 0,1,2,4,3,5 }, { 0,1,2,4,5,3 },
                               { 0,1,2,5,3,4 }, { 0,1,2,5,4,3 },
                               { 0,1,3,4,4,2 }, { 0,1,3,5,4,2 },
                               { 0,1,4,3,4,2 }, { 0,1,4,5,3,2 },
                               { 0,1,5,3,4,2 }, { 0,1,5,4,3,2 },
                               { 0,1,3,4,2,5 }, { 0,1,4,5,2,4 },
                               { 0,1,4,3,2,5 }, { 0,1,4,5,2,3 },
                               { 0,1,5,3,2,4 }, { 0,1,5,4,2,3 } };
  int index[6];
  for( int i = 0; i<18; ++i )
  {
	  {
		  for( int j = 0; j<6; ++j) index[j]=combination[i][j];
		  Point3D origin( number[index[0]], number[index[1]], number[index[2]]);
		  Vector3D direction( number[index[3]], number[index[4]], number[index[5]]);
		  Ray ray( origin, direction );

		  EXPECT_TRUE(ray.origin.x == number[index[0]]);
		  EXPECT_TRUE(ray.origin.y == number[index[1]]);
		  EXPECT_TRUE(ray.origin.z == number[index[2]]);

		  EXPECT_TRUE(ray.direction().x == number[index[3]]);
		  EXPECT_TRUE(ray.direction().y == number[index[4]]);
		  EXPECT_TRUE(ray.direction().z == number[index[5]]);

		  EXPECT_TRUE(ray.mint == gc::Epsilon);
		  EXPECT_TRUE(ray.maxt == gc::Infinity);
	  }
  }
}

TEST(RayTests, ConstructorOriginVectorNoDefaultts) {
  double number[6] = { 0.0, -1.2, 900.02, -120.23, 14.234, 3.82 };
  double combination[18][6]= { { 0,1,2,3,4,5 }, { 0,1,2,3,5,4 },
		                       { 0,1,2,4,3,5 }, { 0,1,2,4,5,3 },
                               { 0,1,2,5,3,4 }, { 0,1,2,5,4,3 },
                               { 0,1,3,4,4,2 }, { 0,1,3,5,4,2 },
                               { 0,1,4,3,4,2 }, { 0,1,4,5,3,2 },
                               { 0,1,5,3,4,2 }, { 0,1,5,4,3,2 },
                               { 0,1,3,4,2,5 }, { 0,1,4,5,2,4 },
                               { 0,1,4,3,2,5 }, { 0,1,4,5,2,3 },
                               { 0,1,5,3,2,4 }, { 0,1,5,4,2,3 } };
  int index[6];
  for( int i = 0; i<18; ++i )
  {
	  {
	  	  for( int j = 0; j<6; ++j) index[j]=combination[i][j];
	  	  Point3D origin( -number[index[0]], -number[index[1]], -number[index[2]]);
	  	  Vector3D direction( -number[index[3]], -number[index[4]], -number[index[5]]);
	  	  double tmin = number[index[2]]*number[index[4]];
	  	  double tmax = number[index[0]]*number[index[5]];
	  	  Ray ray( origin, direction, tmin, tmax );

	  	  EXPECT_TRUE(ray.origin.x == -number[index[0]]);
	  	  EXPECT_TRUE(ray.origin.y == -number[index[1]]);
	  	  EXPECT_TRUE(ray.origin.z == -number[index[2]]);

	  	  EXPECT_TRUE(ray.direction().x == -number[index[3]]);
	  	  EXPECT_TRUE(ray.direction().y == -number[index[4]]);
	  	  EXPECT_TRUE(ray.direction().z == -number[index[5]]);

	  	  EXPECT_TRUE(ray.mint == tmin);
	  	  EXPECT_TRUE(ray.maxt == tmax);
	    }
  }
}

TEST(RayTests, FunctionOperator) {
	using std::vector;

	vector<Ray> rays;
	generateRays( rays );

	vector<Ray>::iterator it = rays.begin();
	while ( it != rays.end() )
	{
		Ray& ray = (*it);
		Point3D& origin = ray.origin;
		const Vector3D& direction = ray.direction();
		double t = (ray.mint + ray.maxt)/2.0;
		Point3D actualResult = ray(t);
		Point3D expectedResult = origin + direction * t;

		EXPECT_DOUBLE_EQ( expectedResult.x, actualResult.x );
		EXPECT_DOUBLE_EQ( expectedResult.y, actualResult.y );
		EXPECT_DOUBLE_EQ( expectedResult.z, actualResult.z );

		double xO = origin.x;
		double yO = origin.y;
		double zO = origin.z;

		double xD = direction.x;
		double yD = direction.y;
		double zD = direction.z;

		double xResult = xO + t*xD;
		double yResult = yO + t*yD;
		double zResult = zO + t*zD;

		EXPECT_DOUBLE_EQ( xResult, actualResult.x );
		EXPECT_DOUBLE_EQ( yResult, actualResult.y );
		EXPECT_DOUBLE_EQ( zResult, actualResult.z );
		++it;
	}
}

void generateRays( std::vector<Ray>& rays )
{
	double number[6] = { 0.021, -1.2, 900.02, -120.23, 14.234, 3.82 };
	double combination[18][6]= { { 0,1,2,3,4,5 }, { 2,1,0,3,5,4 },
							     { 1,0,2,4,3,5 }, { 2,1,0,4,5,3 },
							     { 1,2,0,5,3,4 }, { 0,1,2,5,4,3 },
							     { 0,1,3,4,4,2 }, { 3,1,0,5,4,2 },
							     { 1,0,4,3,4,2 }, { 4,1,0,5,3,2 },
							     { 1,0,5,3,4,2 }, { 0,1,5,4,3,2 },
							     { 1,0,3,4,2,5 }, { 0,1,4,5,2,4 },
							     { 0,1,4,3,2,5 }, { 4,1,0,5,2,3 },
							     { 0,1,5,3,2,4 }, { 0,1,5,4,2,3 } };
	int index[6];
	for( int i = 0; i<18; ++i )
	{
	    for( int j = 0; j<6; ++j) index[j]=combination[i][j];
	    Point3D origin( -number[index[0]], -number[index[1]], -number[index[2]]);
	    Vector3D direction( -number[index[3]], -number[index[4]], -number[index[5]]);
	    double tmin = number[index[2]]*number[index[4]];
	    double tmax = tmin + fabs( (0.7324*i)*146.26 );
	    Ray ray( origin, direction, tmin, tmax );
	    rays.push_back( ray  );
	}
}

