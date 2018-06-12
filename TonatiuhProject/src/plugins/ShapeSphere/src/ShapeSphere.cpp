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
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "DifferentialGeometry.h"
#include "gc.h"
#include "gf.h"

#include "ShapeSphere.h"


TNodeType ShapeSphere::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > ShapeSphere::CreateInstance( )
{
	//shared_prt needs a public constructor
	struct EnableCreateShapeSphere : public ShapeSphere { using ShapeSphere::ShapeSphere; };
	return std::make_shared<EnableCreateShapeSphere>();
}


/*!
 * Initializes ShapeSphere type.
 */
void ShapeSphere::Init()
{
	ShapeSphere::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), "ShapeSphere", &ShapeSphere::CreateInstance );
}

/*!
 * ShapeSphere : public TNode
 */
ShapeSphere::ShapeSphere( )
:TShape(),
 m_radiusLabel( "radius"),
 m_yMinLabel( "yMin" ),
 m_yMaxLabel( "yMax" ),
 m_phiMaxLabel( "phiMax" )
{
	SetName(GetType().GetName() );

	//Translation
	m_pParametersList->Append<double>( m_radiusLabel, 0.5, true );
	m_pParametersList->Append<double>( m_yMinLabel, -0.5, true );
	m_pParametersList->Append<double>( m_yMaxLabel, 0.5, true );
	m_pParametersList->Append<double>( m_phiMaxLabel, ( gc::TwoPi), true );
}


/*!
 * Destructor.
 */
ShapeSphere::~ShapeSphere()
{

}

/*!
 * Creates a copy of shape node.
 */
std::shared_ptr< TNode > ShapeSphere::Copy() const
{
	struct EnableCreateShapeSphere : public ShapeSphere { using ShapeSphere::ShapeSphere; };
	std::shared_ptr<ShapeSphere> shapeNode = std::make_unique<EnableCreateShapeSphere>();
	if( shapeNode == nullptr )	return ( 0  );


	//Coping the parameters.
	shapeNode->m_pParametersList->SetValue( m_radiusLabel, GetParameterValue<double>( m_radiusLabel ) );
	shapeNode->m_pParametersList->SetValue( m_yMinLabel, GetParameterValue<double>( m_yMinLabel ) );
	shapeNode->m_pParametersList->SetValue( m_yMaxLabel, GetParameterValue<double>( m_yMaxLabel ) );
	shapeNode->m_pParametersList->SetValue( m_phiMaxLabel, GetParameterValue<double>( m_phiMaxLabel ) );

	return ( shapeNode );
}

/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeSphere::GetBondingBox() const
{
	double radius= GetParameterValue<double>( m_radiusLabel );
	double yMin= GetParameterValue<double>( m_yMinLabel );
	double yMax= GetParameterValue<double>( m_yMaxLabel );
	double phiMax = GetParameterValue<double>( m_phiMaxLabel );

	double cosPhiMax = cos( phiMax );
   	double sinPhiMax = sin( phiMax );

   	double thetaMin = acos( yMax/ radius );
   	double thetaMax = acos( yMin/radius );
   	double maxRadius = ( yMax * yMin > 0.0 ) ?  std::max( sin(thetaMin) * radius , sin(thetaMax) * radius )
																	: radius;
   	double minRadius = std::min( sin(thetaMin) * radius, sin(thetaMax) * radius );
   	double xmin = ( phiMax < gc::Pi  ) ?  0.0
								: ( phiMax < 1.5 * gc::Pi  ) ? sinPhiMax * maxRadius
										: -maxRadius;
   	double xmax = ( phiMax >= gc::Pi / 2 ) ? maxRadius : sinPhiMax * maxRadius;

   	double ymin = yMin;
	double ymax = yMax;

	double zmin = ( phiMax > gc::Pi ) ? -maxRadius
					:( phiMax > gc::Pi / 2 ) ? maxRadius* cosPhiMax : std::min( maxRadius * cosPhiMax, minRadius * cosPhiMax );
	double zmax = maxRadius;

	return BBox( Point3D( xmin, ymin, zmin), Point3D( xmax, ymax, zmax) );
}

/*!
 * Returns the filename that stores the shape icon.
 */
std::string ShapeSphere::GetIcon() const
{
	return ( ":icons/ShapeSphere.png" );
}


/*!
 * Returns the type of node.
 */
TNodeType ShapeSphere::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Computes if the \a objectRay intersects with current surface.
 * \a tHit, \a dg and \a isShapeFront return information about the intersection point.
 */
bool ShapeSphere::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{

	double radius = GetParameterValue<double>( m_radiusLabel );
	double yMin= GetParameterValue<double>( m_yMinLabel );
	double yMax= GetParameterValue<double>( m_yMaxLabel );
	double phiMax = GetParameterValue<double>( m_phiMaxLabel );

	// Compute quadratic ShapeSphere coefficients
	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A = objectRay.direction().lengthSquared();
    double B = 2.0 * DotProduct( vObjectRayOrigin, objectRay.direction() );
	double C = vObjectRayOrigin.lengthSquared() - radius * radius;

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
    double thit = ( t0 > objectRay.mint )? t0 : t1 ;
    if( thit > objectRay.maxt ) return false;

    //Evaluate Tolerance
	double tol = 0.00001;

	// Compute ShapeSphere hit position and $\phi$
    Point3D hitPoint = objectRay( thit );
	double phi = atan2( hitPoint.x, hitPoint.z );
	if ( phi < 0. ) phi += gc::TwoPi;

	// Test intersection against clipping parameters
	if( (thit - objectRay.mint) < tol || hitPoint.y < yMin || hitPoint.y > yMax || phi > phiMax )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		// Compute ShapeSphere hit position and $\phi$
		hitPoint = objectRay( thit );
		phi = atan2( hitPoint.x, hitPoint.z );
	    if ( phi < 0. ) phi += gc::TwoPi;

		if ( (thit - objectRay.mint) < tol || hitPoint.y < yMin || hitPoint.y > yMax || phi > phiMax )	return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );

	// Find parametric representation of ShapeSphere hit
	double theta = acos( hitPoint.y / radius );
	double thetaMin = acos( yMax / radius );
	double thetaMax = acos( yMin/radius );
	double u = ( theta - thetaMin ) / ( thetaMax - thetaMin );
	double v = phi / phiMax;

	// Compute ShapeSphere \dpdu and \dpdv
	Vector3D dpdu( radius * ( -thetaMin + thetaMax ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) * sin( phiMax * v ),
					radius * ( -thetaMin + thetaMax ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					radius * ( -thetaMin + thetaMax ) * cos( phiMax * v ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) );

	Vector3D dpdv( -phiMax * radius * cos( phiMax * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					0.0,
					phiMax * radius * sin( phiMax * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ) );

	// Compute ShapeSphere \dndu and \dndv
	Vector3D d2Pduu(  -radius * ( thetaMin - thetaMax ) * ( -thetaMin + thetaMax ) * sin( phiMax * v ) * sin( (-1 + u) * thetaMin - u * thetaMax ),
					radius * ( thetaMin - thetaMax ) * ( -thetaMin + thetaMax ) * cos( (-1 + u) * thetaMin - u * thetaMax ),
					-radius * ( thetaMin - thetaMax ) * ( -thetaMin + thetaMax ) * cos( phiMax * v ) * sin( (-1 + u) * thetaMin -u * thetaMax )  );

	Vector3D d2Pduv( phiMax * radius * ( -thetaMin + thetaMax ) * cos( phiMax * v ) * cos( (-1 + u ) * thetaMin - u  * thetaMax ),
					0.0,
					-phiMax * radius * ( -thetaMin + thetaMax ) * cos( (-1 + u ) * thetaMin - u  * thetaMax ) * sin( phiMax * v ) );

	Vector3D d2Pdvv( phiMax * phiMax * radius * sin( phiMax * v ) * sin( (-1 + u) * thetaMin - u * thetaMax ),
					0.0,
					phiMax * phiMax *  radius * cos( phiMax * v ) * sin( (-1 + u) * thetaMin - u * thetaMax ) );

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
void ShapeSphere::Draw() const
{

}

/*
Point3D ShapeSphere::GetPoint3D( double u, double v ) const
{
	if ( OutOfRange( u, v ) ) gf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );


	double thetaMin = acos( yMax / radius );
	double thetaMax = acos( yMin/radius );
	double theta = u *( thetaMax - thetaMin ) + thetaMin;
	double phi = v * phiMax;

	double x = radius * sin( theta ) * sin( phi );
	double y = radius * cos( theta );
	double z = radius * sin( theta ) * cos( phi );

	return Point3D (x, y, z);

}

NormalVector ShapeSphere::GetNormal(double u, double v ) const
{
	double thetaMin = acos( yMax / radius );
	double thetaMax = acos( yMin/radius );

	Vector3D dpdu( radius * ( -thetaMin + thetaMax ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) * sin( phiMax * v ),
					radius * ( -thetaMin + thetaMax ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					radius * ( -thetaMin + thetaMax ) * cos( phiMax * v ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) );

	Vector3D dpdv( -phiMax * radius * cos( phiMax * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					0.0,
					phiMax * radius * sin( phiMax * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ) );
	NormalVector normal = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
	return normal;
}

bool ShapeSphere::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}
*/

