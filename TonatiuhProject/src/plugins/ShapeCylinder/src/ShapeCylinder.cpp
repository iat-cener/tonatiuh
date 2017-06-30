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

#include "gf.h"
#include "NormalVector.h"

#include "ShapeCylinder.h"
#include "TParameterList.h"


TNodeType ShapeCylinder::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* ShapeCylinder::CreateInstance( )
{
  return ( new ShapeCylinder() );
}

/*!
 * Initializes ShapeCylinder type.
 */
void ShapeCylinder::Init()
{
	ShapeCylinder::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), QString( "ShapeCylinder" ), &ShapeCylinder::CreateInstance );
}

/*!
 * ShapeFlatDisk : public TShape
 */
ShapeCylinder::ShapeCylinder()
:TShape()
{
	setObjectName(GetType().GetName());

	//Translation
	m_parametersList->Append( QLatin1String("radius"), 0.5 );
	m_parametersList->Append( QLatin1String("length"), 1.0 );
	m_parametersList->Append( QLatin1String("phiMax"), gc::TwoPi );
}

/*!
 * Destructor.
 */
ShapeCylinder::~ShapeCylinder()
{

}

/*!
 * Returns the filename that stores the shape icon.
 */
QString ShapeCylinder::GetIcon()
{

	return QLatin1String( ":/icons/ShapeCylinder.png" );
}


/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeCylinder::GetBondingBox() const
{
	double radius = m_parametersList->GetValue( QLatin1String("radius") ).toDouble();
	double phiMax = m_parametersList->GetValue( QLatin1String("phiMax") ).toDouble();
	double length = m_parametersList->GetValue( QLatin1String("length") ).toDouble();

	double cosPhiMax = cos( phiMax );
	double sinPhiMax = sin( phiMax );

	double xmin = ( phiMax >= gc::Pi ) ? -radius : radius * cosPhiMax;
	double xmax = radius;

	double ymin = 0.0;
	if( phiMax > gc::Pi )
		ymin = ( phiMax < ( 1.5 * gc::Pi ) ) ? radius * sinPhiMax : -radius;
	double ymax = ( phiMax < ( gc::Pi / 2.0 ) )? radius * sinPhiMax : radius;

	double zmin = 0.0;
	double zmax = length;

	return BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) );
}

/*!
 * Returns the type of node.
 */
TNodeType ShapeCylinder::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Calculates the intersection of the \a objectRay with this surface. The ray is in the surface local coordinates system.
 * Returns false if the ray does not intersect with the surface. \a tHit, \a dg and \a isShapeFront is the information generated about the intersection.
 */
bool ShapeCylinder::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{

	double radius = m_parametersList->GetValue( QLatin1String("radius") ).toDouble();
	double phiMax = m_parametersList->GetValue( QLatin1String("phiMax") ).toDouble();
	double length = m_parametersList->GetValue( QLatin1String("length") ).toDouble();

	// Compute quadratic cylinder coefficients
	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A = objectRay.direction().x*objectRay.direction().x + objectRay.direction().y*objectRay.direction().y;
	double B = 2.0 * ( objectRay.direction().x* objectRay.origin.x + objectRay.direction().y * objectRay.origin.y);
	double C = objectRay.origin.x * objectRay.origin.x + objectRay.origin.y * objectRay.origin.y - radius * radius;

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double t = ( t0 > objectRay.mint )? t0 : t1 ;
	if( t > objectRay.maxt ) return false;

   //Compute possible cylinder hit position and $\phi
	Point3D hitPoint = objectRay( t );
	double phi = atan2( hitPoint.y, hitPoint.x );
	if ( phi < 0. ) phi += gc::TwoPi;

	//Evaluate Tolerance
	double tol = 0.00001;
	double zmin = 0.0;
	double zmax = length;


	// Test intersection against clipping parameters
	if( (t - objectRay.mint) < tol  || hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax )
	{
		if ( t == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		t = t1;

		hitPoint = objectRay( t );
		phi = atan2( hitPoint.y, hitPoint.x );
		if ( phi < 0. ) phi += gc::TwoPi;
		if ( (t - objectRay.mint) < tol  || hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax ) return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) )
		gf::SevereError( "Function Cylinder::Intersect(...) called with null pointers" );


	// Find parametric representation of Cylinder hit
	double u = phi / phiMax;
	double v = hitPoint.z /length;

	Vector3D dpdu( -phiMax * radius*  sin ( phiMax * u ),
						phiMax * radius * cos( phiMax * u ),
						0.0 );
	Vector3D dpdv( 0.0, 0.0, length );

	// Compute cylinder \dndu and \dndv
	Vector3D d2Pduu( -phiMax * phiMax * radius
							* cos( phiMax * u ),
						-phiMax * phiMax * radius
							* sin( phiMax * u ),
						0.0 );
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
								u, v );


	*isShapeFront = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

	// Update _tHit_ for quadratic intersection
	*tHit = t;

	return ( true );
}

/*!
 * Not implemented yet.
 */
void ShapeCylinder::Draw() const
{

}
