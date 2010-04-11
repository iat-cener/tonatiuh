/*
 * RayTests.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: mblanco
 */

#include <vector>
#include "Ray.h"
#include <gtest/gtest.h>

static void generateRays( std::vector<Ray>& rays );


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

TEST(RayTests, FunctionOperator) {
	using std::vector;

	vector<Ray> rays;
	generateRays( rays );

	vector<Ray>::iterator it = rays.begin();
	while ( it != rays.end() )
	{
		Ray& ray = (*it);
		Point3D& origin = ray.origin;
		Vector3D& direction = ray.direction;
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
