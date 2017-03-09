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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/

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
 m_vE1( Vector3D() ),
 m_vE2( Vector3D() ),
 m_vW1( Vector3D() ),
 m_tol( 0.000001 )
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


	/*
	 m_vB = Vector3D( m_v1 - m_v3 );
	 m_vC = Vector3D( m_v2 - m_v3 );
	 m_vW1 = CrossProduct( m_vB, m_vC );
	 */
	 m_vE1 = Vector3D( m_v2 - m_v1 );
	 m_vE2 = Vector3D( m_v3 - m_v1 );
	 m_vW1 = CrossProduct( m_vE1, m_vE2 );

	 m_tol = m_vE1.length()*m_vE2.length()/1000000;
}


/*!
 * Triangle intersection
 */
bool Triangle::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{

	//e1 = B - A
	//e2 = C - A
	Vector3D pVector = CrossProduct( objectRay.direction(), m_vE2 );
	double det = DotProduct( m_vE1, pVector );
	double inv_det = 1/det;

	//std::cout<<"det: "<<det<<std::endl;

	//Vector3D tVector = Vector3D( objectRay.origin ) – Vector3D(m_v1) ;
	Vector3D tVector = Vector3D( objectRay.origin - m_v1 );
	Vector3D qVec = CrossProduct( tVector ,  m_vE1 );
	double thit;
	//double tol = 0.000001;
	if (det > m_tol)
	{
		double u = DotProduct(  tVector,pVector );
		if (u < 0 || u > det )	return ( false );

		double v = DotProduct( objectRay.direction(), qVec );
		if (v < 0 || ( v + u ) > det) return ( false );

		double t = DotProduct( m_vE2, qVec );
		thit  =  t * inv_det;
		u *= inv_det;
		v *= inv_det;
	}
	else if (det < - m_tol)
	{

		double u = DotProduct( tVector, pVector ) * inv_det;
		if (u < 0.0 || u > 1.0)	return ( false );

		double v = DotProduct( objectRay.direction(), qVec )  * inv_det;
		if (v < 0.0 || ( ( v+u ) > 1.0 ) ) 	return ( false );


		double t = DotProduct( m_vE2, qVec ) * inv_det;
		thit  =  t;
	}
	else
		return ( false );


	//return intersection == true , t, P

	/*//Jimenez algorithm
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
	*/


	if( thit > *tHit ) return false;
	if( (thit - objectRay.mint) < m_tol ) return false;


	Point3D hitPoint = objectRay( thit );


	Vector3D dpdu = Normalize( m_vE1 );
	Vector3D dpdv = Normalize( m_vE2 );

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
