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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "DifferentialGeometry.h"
#include "NormalVector.h"
#include "gf.h"

#include "ShapeCone.h"
#include "TParameterList.h"

TNodeType ShapeCone::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* ShapeCone::CreateInstance( )
{
  return ( new ShapeCone() );
}

/*!
 * Initializes ShapeCone type.
 */
void ShapeCone::Init()
{
	ShapeCone::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), "ShapeCone", &ShapeCone::CreateInstance );
}

/*!
 * ShapeFlatDisk : public TShape
 */
ShapeCone::ShapeCone(  )
:TShape()
{
	//setObjectName(GetType().GetName().c_str() );
	SetName(GetType().GetName() );

	//Parameters
	m_parametersList->Append( "baseRadius", 0.5 );
	m_parametersList->Append( "topRadius", 0.0 );
	m_parametersList->Append( "height", 1.0 );
	m_parametersList->Append( "phiMax", gc::TwoPi );

}

/*!
 * Destructor.
 */
ShapeCone::~ShapeCone()
{

}

/*!
 * Returns the filename that stores the shape icon.
 */
std::string ShapeCone::GetIcon() const
{

	return ( ":/icons/ShapeCone.png" );
}


/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeCone::GetBondingBox() const
{
	double baseRadius = m_parametersList->GetValue( "baseRadius" ).toDouble();
	double topRadius = m_parametersList->GetValue( "topRadius" ).toDouble();
	double height = m_parametersList->GetValue( "height" ).toDouble();
	double phiMax = m_parametersList->GetValue( "phiMax" ).toDouble();

	double cosPhiMax = cos( phiMax );
	double sinPhiMax = sin( phiMax );
	double minradius = std::min( baseRadius, topRadius );
	double maxradius = std::max( baseRadius, topRadius );


	double xmin = ( phiMax > gc::Pi ) ?
						( ( phiMax < 1.5*gc::Pi ) ? maxradius * sinPhiMax : -maxradius ) :
						0.0;
	double xmax = ( phiMax < gc::Pi/2.0 )?  maxradius * sinPhiMax : maxradius;
	double ymin = 0.0;
	double ymax = height;
	double zmin = ( phiMax >= gc::Pi ) ? -maxradius : std::min( minradius * cosPhiMax, maxradius * cosPhiMax );
	double zmax = maxradius;

	return BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) );
}

/*!
 * Returns the type of node.
 */
TNodeType ShapeCone::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Calculates the intersection of the \a objectRay with this surface. The ray is in the surface local coordinates system.
 * Returns false if the ray does not intersect with the surface. \a tHit, \a dg and \a isShapeFront is the information generated about the intersection.
 */
bool ShapeCone::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{
	double baseRadius = m_parametersList->GetValue( "baseRadius" ).toDouble();
	double topRadius = m_parametersList->GetValue( "topRadius" ).toDouble();
	double height = m_parametersList->GetValue( "height" ).toDouble();
	double phiMax = m_parametersList->GetValue( "phiMax" ).toDouble();


	// Compute quadratic ShapeCone coefficients
	double theta = atan2( height, ( baseRadius - topRadius ) );
	double invTan = 1 / tan( theta );

	double A = (     objectRay.direction().x * objectRay.direction().x )
				 + ( objectRay.direction().z * objectRay.direction().z )
				 - ( objectRay.direction().y * objectRay.direction().y * invTan * invTan );

	double B = 2.0 * ( (    objectRay.origin.x * objectRay.direction().x )
						+ ( objectRay.origin.z * objectRay.direction().z )
						+ ( baseRadius * invTan * objectRay.direction().y )
						- ( invTan * invTan * objectRay.origin.y * objectRay.direction().y ) );

	double C = (    objectRay.origin.x * objectRay.origin.x )
				+ ( objectRay.origin.z * objectRay.origin.z )
				- ( baseRadius * baseRadius )
				+ ( 2 * baseRadius * invTan * objectRay.origin.y )
				- ( invTan * invTan * objectRay.origin.y * objectRay.origin.y );

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;


	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;

    double thit = ( t0 > objectRay.mint )? t0 : t1 ;
    if( thit > objectRay.maxt ) return false;

    //Evaluate Tolerance
	double tol = 0.00001;
	if( (thit - objectRay.mint) < tol ) return false;

	// Compute possible ShapeCone hit position and $\phi$
    Point3D hitPoint = objectRay( thit );
	double phi = atan2( hitPoint.x, hitPoint.z );

	// Test intersection against clipping parameters
	if( hitPoint.y < 0 || hitPoint.y > height || phi > phiMax )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		hitPoint = objectRay( thit );
		phi = atan2( hitPoint.x, hitPoint.z );
		if ( hitPoint.y < 0 || hitPoint.y > height || phi > phiMax ) return false;
	}
	// Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) )	gf::SevereError( "Function Cylinder::Intersect(...) called with null pointers" );

	// Compute definitive ShapeCone hit position and $\phi$
    hitPoint = objectRay( thit );

	// Find parametric representation of ShapeCone hit
	double u = phi / phiMax;
	double v = hitPoint.y / height;

	// Compute ShapeCone \dpdu and \dpdv
	Vector3D dpdu( phiMax * ( baseRadius - baseRadius * v + topRadius * v )
						* cos( phiMax * u ),
					0.0,
					-phiMax * ( baseRadius - baseRadius * v + topRadius * v )
						* sin( phiMax * u ) );

	Vector3D dpdv(  -height / tan ( atan2( height, ( baseRadius - topRadius ) ) )
							* sin( phiMax *  u ),
							height,
					-height * cos( phiMax* u )
							 / tan ( atan2( height, ( baseRadius - topRadius ) ) ) );

	// Compute ShapeCone \dndu and \dndv

	Vector3D d2Pduu( -phiMax * phiMax * ( baseRadius - baseRadius * v + topRadius * v )
			* sin( phiMax * u ),
	   0.0,
	   -phiMax * phiMax * ( baseRadius - baseRadius * v + topRadius * v )
		   * cos( phiMax * u ) );

	Vector3D d2Pduv( phiMax * ( -baseRadius + topRadius ) * cos( phiMax * u ),
			0.0,
			phiMax * ( baseRadius - topRadius ) * sin( phiMax* u ) );

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
		                        u, v );

	*isShapeFront = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

/*!
 * Not implemented yet.
 */
void ShapeCone::Draw() const
{

}
