/*
 * DifferentialGeometryTests.cpp
 *
 *  Created on: 31/08/2011
 *      Author: sgarayalde
 */

#include "DifferentialGeometry.h"

#include <gtest/gtest.h>

TEST(DiferencialGeometryTests, DesfaultConstructor)
{
	DifferentialGeometry dif;
	EXPECT_DOUBLE_EQ( dif.u,0.0 );
	EXPECT_DOUBLE_EQ( dif.v,0.0 );

}

TEST(DiferencialGeometryTests, Constructor)
{
	Point3D point(752.2,52.123,5854);
	Vector3D dpdu(123,456,789);
	Vector3D dpdv(14.03,165.96,762.1);
	Vector3D dndu(432.1,85694,3004.152);
	Vector3D dndv(5256,0.4103,6.693);
	double u=1346.18;
	double v=23.0095;
	TShape* shape;

	DifferentialGeometry dif(point,dpdu,dpdv,dndu,dndv,u,v,shape);

	EXPECT_DOUBLE_EQ( dif.u,u );
	EXPECT_DOUBLE_EQ( dif.v,v );
	EXPECT_DOUBLE_EQ( dif.point.x,point.x );
	EXPECT_DOUBLE_EQ( dif.point.y,point.y );
	EXPECT_DOUBLE_EQ( dif.point.z,point.z );
	EXPECT_DOUBLE_EQ( dif.dpdu.x,dpdu.x);
	EXPECT_DOUBLE_EQ( dif.dpdu.y,dpdu.y);
	EXPECT_DOUBLE_EQ( dif.dpdu.z,dpdu.z);
	EXPECT_DOUBLE_EQ( dif.dpdv.x,dpdv.x);
	EXPECT_DOUBLE_EQ( dif.dpdv.y,dpdv.y);
	EXPECT_DOUBLE_EQ( dif.dpdv.z,dpdv.z);
	EXPECT_DOUBLE_EQ( dif.dndu.x,dndu.x);
	EXPECT_DOUBLE_EQ( dif.dndu.y,dndu.y);
	EXPECT_DOUBLE_EQ( dif.dndu.z,dndu.z);
	EXPECT_DOUBLE_EQ( dif.dndv.x,dndv.x);
	EXPECT_DOUBLE_EQ( dif.dndv.y,dndv.y);
	EXPECT_DOUBLE_EQ( dif.dndv.z,dndv.z);
    EXPECT_DOUBLE_EQ( dif.normal.x, Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) ).x);
    EXPECT_DOUBLE_EQ( dif.normal.y, Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) ).y);
    EXPECT_DOUBLE_EQ( dif.normal.z, Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) ).z);

}
