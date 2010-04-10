/*
 * RayTests.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: mblanco
 */

#include "Ray.h"
#include <gtest/gtest.h>

TEST(RayTests, ConstructorDefault) {
  Ray ray;
  EXPECT_TRUE(ray.mint == tgc::Epsilon);
  EXPECT_TRUE(ray.maxt == tgc::Infinity);
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

		  EXPECT_TRUE(ray.direction.x == number[index[3]]);
		  EXPECT_TRUE(ray.direction.y == number[index[4]]);
		  EXPECT_TRUE(ray.direction.z == number[index[5]]);

		  EXPECT_TRUE(ray.mint == tgc::Epsilon);
		  EXPECT_TRUE(ray.maxt == tgc::Infinity);
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

	  	  EXPECT_TRUE(ray.direction.x == -number[index[3]]);
	  	  EXPECT_TRUE(ray.direction.y == -number[index[4]]);
	  	  EXPECT_TRUE(ray.direction.z == -number[index[5]]);

	  	  EXPECT_TRUE(ray.mint == tmin);
	  	  EXPECT_TRUE(ray.maxt == tmax);
	    }
  }
}
