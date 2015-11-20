/*
 * Triangle.cpp
 *
 *  Created on: 03/11/2015
 *      Author: amutuberria
 */

#include "DifferentialGeometry.h"
#include "gc.h"
#include "gf.h"
#include "Ray.h"
#include "Triangle.h"

/*! *****************************
 * class Triangle
 * **************************** */
Triangle::Triangle( Point3D v1, Point3D v2, Point3D v3, NormalVector normal )
:m_bbox( ),
 m_centoid(),
 m_normal ( normal ),
 m_v1( v1 ),
 m_v2 ( v2 ),
 m_v3( v3 ),
 m_vB( Vector3D() ),
 m_vC( Vector3D() ),
 m_vW1( Vector3D() )
{

	double xMin = gc::Infinity;
	if( m_v1.x < xMin )	xMin = m_v1.x;
	if( m_v2.x < xMin )	xMin = m_v2.x;
	if( m_v3.x < xMin )	xMin = m_v3.x;
	double yMin = gc::Infinity;
	if( m_v1.y < yMin )	yMin = m_v1.y;
	if( m_v2.y < yMin )	yMin = m_v2.y;
	if( m_v3.y < yMin )	yMin = m_v3.y;
	double zMin = gc::Infinity;
	if( m_v1.z < zMin )	zMin = m_v1.z;
	if( m_v2.z < zMin )	zMin = m_v2.z;
	if( m_v3.z < zMin )	zMin = m_v3.z;
	double xMax = - gc::Infinity;
	if( m_v1.x > xMax )	xMax = m_v1.x;
	if( m_v2.x > xMax )	xMax = m_v2.x;
	if( m_v3.x > xMax )	xMax = m_v3.x;
	double yMax = - gc::Infinity;
	if( m_v1.y > yMax )	yMax = m_v1.y;
	if( m_v2.y > yMax )	yMax = m_v2.y;
	if( m_v3.y > yMax )	yMax = m_v3.y;
	double zMax = - gc::Infinity;
	if( m_v1.z > zMax )	zMax = m_v1.z;
	if( m_v2.z > zMax )	zMax = m_v2.z;
	if( m_v3.z > zMax )	zMax = m_v3.z;

	m_bbox.pMin = Point3D( xMin, yMin, zMin );
	m_bbox.pMax  = Point3D( xMax, yMax, zMax );

	m_centoid = Point3D( m_bbox.pMin.x + 0.5 * ( m_bbox.pMax.x - m_bbox.pMin.x  ),
			m_bbox.pMin.y + 0.5 * ( m_bbox.pMax.y - m_bbox.pMin.y  ),
			m_bbox.pMin.z + 0.5 * ( m_bbox.pMax.z - m_bbox.pMin.z  ) );


	 m_vB = Vector3D( m_v1 - m_v3 );
	 m_vC = Vector3D( m_v2 - m_v3 );
	 m_vW1 = CrossProduct( m_vB, m_vC );

}


/*!
 * Triangle intersection
 */
bool Triangle::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{

	//Jimenez algorithm
	double t0;
	double t1;
	if( !m_bbox.IntersectP(objectRay, &t0, &t1 ) )	return ( false );


	//Evaluate Tolerance
	t0 -= 0.1;
	t1 += 0.1;
	Point3D Q1 = objectRay( t0 );
	Point3D Q2 = objectRay( t1 );


	Vector3D vA = Vector3D( Q1 - m_v3 );
	double w = DotProduct( vA, m_vW1 );

	Vector3D vD = Vector3D( Q2 - m_v3 );
	double s = DotProduct( vD, m_vW1 );

	double tol = 0.000001;
	if( w > tol )
	{
		if( s > tol ) return ( false );
		Vector3D vW2 = CrossProduct( vA, vD );
		double t = DotProduct( vW2, m_vC );
		if( t < -tol ) return ( false );
		double u = DotProduct( - vW2, m_vB );
		if( u < -tol ) 	return ( false );
		if( w < ( s + t + u ) ) 	return ( false );
	}
	else if ( w < -tol )
	{

		if(  s < -tol ) 	return ( false );
		Vector3D vW2 = CrossProduct( vA, vD );
		double t = DotProduct( vW2, m_vC );
		if( t > tol ) return ( false );
		double u = DotProduct( - vW2, m_vB );
		if( u > tol ) 	return ( false );
		if( w > ( s + t + u ) ) 	return ( false );
	}
	else // w == 0, swap( Q1, Q2 )
	{

		Vector3D vW2 = CrossProduct( vD, vA );
		double t = DotProduct( vW2, m_vC );
		if( s > tol )
		{
			if( t < -tol ) return ( false );
			double u = DotProduct( - vW2, m_vB);
			if( u < -tol ) 	return ( false );
			if( -s < ( t + u ) ) 	return ( false );
		}
		else if( s < - tol )
		{
			if( t > tol ) return ( false );
			double u = DotProduct( - vW2, m_vB );
			if( u > tol ) 	return ( false );
			if( -s  > ( t + u ) ) 	return ( false );

		}
		else
			return ( false );
	}


	double t_param = ( DotProduct( Normalize( m_vW1 ), vA )  /  DotProduct( Normalize( m_vW1 ),  Vector3D( Q1 - Q2 ) ) );
	double thit = t0 + t_param * Distance( Q1, Q2 );


	if( thit > *tHit ) return false;
	if( (thit - objectRay.mint) < tol ) return false;


	Point3D hitPoint = objectRay( thit );


	Vector3D dpdu = Normalize( m_vB );
	Vector3D dpdv = Normalize( m_vC );

	// Compute ShapeCone \dndu and \dndv
	Vector3D d2Pduu( 0.0, 0.0, 0.0 );
	Vector3D d2Pduv( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv( 0.0, 0.0, 0.0 );

	// Compute coefficients for fundamental forms
	double E = DotProduct( dpdu, dpdu );
	double F = DotProduct( dpdu, dpdv );
	double G = DotProduct( dpdv, dpdv );

	Vector3D N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );


	double e = DotProduct( N, d2Pduu );
	double f = DotProduct( N, d2Pduv );
	double g = DotProduct( N, d2Pdvv );

	// Compute \dndu and \dndv from fundamental form coefficients
	double invEGF2 = 1.0 / (E*G - F*F);
	Vector3D dndu = (f*F - e*G) * invEGF2 * dpdu +
			        (e*F - f*E) * invEGF2 * dpdv;
	Vector3D dndv = (g*F - f*G) * invEGF2 * dpdu +
	                (f*F - g*E) * invEGF2 * dpdv;

	// Initialize _DifferentialGeometry_ from parametric information
	*dg = DifferentialGeometry( hitPoint ,
		                        dpdu,
								dpdv,
		                        dndu,
								dndv,
		                        -1, -1, 0 );

	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;

}
