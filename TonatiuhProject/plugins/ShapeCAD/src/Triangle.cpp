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
 m_v3( v3 )
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
	if( m_v3.y > yMin )	yMax = m_v3.y;
	double zMax = - gc::Infinity;
	if( m_v1.z > zMax )	zMax = m_v1.z;
	if( m_v2.z > zMax )	zMax = m_v2.z;
	if( m_v3.z > zMax )	zMax = m_v3.z;

	m_bbox.pMin = Point3D( xMin, yMin, zMin );
	m_bbox.pMax  = Point3D( xMax, yMax, zMax );

	m_centoid = Point3D( m_bbox.pMin.x + 0.5 * ( m_bbox.pMax.x - m_bbox.pMin.x  ),
			m_bbox.pMin.y + 0.5 * ( m_bbox.pMax.y - m_bbox.pMin.y  ),
			m_bbox.pMin.z + 0.5 * ( m_bbox.pMax.z - m_bbox.pMin.z  ) );
}


/*!
 * Triangle intersection
 */
bool Triangle::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{

	Vector3D vAB = Vector3D( m_v2 - m_v1 );
	Vector3D vAC = Vector3D( m_v3 - m_v1 );

	Vector3D vN = CrossProduct( vAB, vAC );
	double d = -vN.x *  m_v1.x - vN.y * m_v1.y - vN.z *  m_v1.z;

	double thit = (-d - vN.x * objectRay.origin.x - vN.y * objectRay.origin.y - vN.z * objectRay.origin.z )
			/ (vN.x * objectRay.direction().x  + vN.y * objectRay.direction().y + vN.z * objectRay.direction().z );

	//Evaluate Tolerance
	double tol = 0.00001;
	if( thit > *tHit ) return false;
	if( (thit - objectRay.mint) < tol ) return false;

	Point3D hitPoint = objectRay( thit );

	/*// is hitPoint inside triangle?
	//double uu, uv, vv, wu, wv, D;
	double uu = DotProduct( vAB, vAB );
	double uv = DotProduct( vAB, vAC );
	double vv = DotProduct( vAC, vAC );
	Vector3D  w = Vector3D( hitPoint ) - Vector3D( m_v1 );
	double wu = DotProduct( w, vAB );
	double wv = DotProduct( w, vAC );
	double D = uv * uv - uu * vv;
	std::cout<<"uu: "<<uu<<"\tvv: "<<vv<<std::endl;

	// get and test parametric coords
	double u = (uv * wv - vv * wu) / D;
	std::cout<<"u: "<<u<<std::endl;
	if( u < 0.0 || u > 1.0 )	return false;

	double v = ( uv * wu - uu * wv ) / D;
	std::cout<<"v: "<<v<<std::endl;
	std::cout<<"( u + v): "<<( u + v)<<std::endl;
	if( v < 0.0 || ( u + v) > 1.0)	return false;
	*/
	Vector3D  v2 = Vector3D( hitPoint ) - Vector3D( m_v1 );

	double dot00 = DotProduct( vAC, vAC );
	double dot01 = DotProduct( vAC, vAB );
	double dot02 = DotProduct( vAC, v2 );
	double dot11 = DotProduct( vAB, vAB );
	double dot12 = DotProduct( vAB, v2 );
	double D = dot00 * dot11 - dot01 * dot01;

	double u = (dot11 * dot02 - dot01 * dot12) / D;
	if( u < 0.0 || u > 1.0 )	return false;

	double v = ( dot00 * dot12 - dot01 * dot02 ) / D;
	if( v < 0.0 || ( u + v) > 1.0)	return false;


	// Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	//if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	//else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeFlatTriangle::Intersect(...) called with null pointers" );

	Vector3D dpdu = vAB;
	Vector3D dpdv = vAC;

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
		                        u, v, 0 );

	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}
