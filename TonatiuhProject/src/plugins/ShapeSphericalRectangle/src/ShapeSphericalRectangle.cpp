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

#include "gc.h"
#include "gf.h"

#include "ShapeSphericalRectangle.h"
#include "TParameterList.h"


TNodeType ShapeSphericalRectangle::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* ShapeSphericalRectangle::CreateInstance( )
{
  return ( new ShapeSphericalRectangle() );
}


/*!
 * Initializes ShapeSphericalRectangle type.
 */
void ShapeSphericalRectangle::Init()
{
	ShapeSphericalRectangle::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), QString( "ShapeSphericalRectangle" ), &ShapeSphericalRectangle::CreateInstance );
}

/*!
 * ShapeSphericalRectangle : public TNode
 */

ShapeSphericalRectangle::ShapeSphericalRectangle()
:TShape()
{
	setObjectName(GetType().GetName());

	//Translation
	m_parametersList->Append( QLatin1String("radius"), 0.75 );
	m_parametersList->Append( QLatin1String("widthX"), 1.0 );
	m_parametersList->Append( QLatin1String("widthZ"), 1.0 );

}


/*!
 * Destroys ShapeSphericalRectangle object.
 */
ShapeSphericalRectangle::~ShapeSphericalRectangle()
{

}

/*!
 * Returns the filename that stores the shape icon.
 */
QString ShapeSphericalRectangle::GetIcon()
{

	return QString( ":/icons/ShapeSphericalRectangle.png" );
}

/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeSphericalRectangle::GetBondingBox() const
{

	double radius = m_parametersList->GetValue( QLatin1String("radius") ).toDouble();
	double widthX = m_parametersList->GetValue( QLatin1String("widthX") ).toDouble();
	double widthZ = m_parametersList->GetValue( QLatin1String("widthZ") ).toDouble();

	double xmin = -widthX/2;
	double xmax = widthX/2;

	double zmin = -widthZ / 2;
	double zmax = widthZ / 2;

	double ymin = 0.0;
	double ymax = radius - sqrt( radius * radius - xmin * xmin - zmin * zmin );

	return BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) );
}

/*!
 * Returns the type of node.
 */
TNodeType ShapeSphericalRectangle::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Intersects the \a objectRay in surface coordinates with the surface and calculates the information of the surface in the intersection poit.
 * Returns true if an intersection occurs.
 */
bool ShapeSphericalRectangle::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{
	double r = m_parametersList->GetValue( QLatin1String("radius") ).toDouble();
	double wX = m_parametersList->GetValue( QLatin1String("widthX") ).toDouble();
	double wZ = m_parametersList->GetValue( QLatin1String("widthZ") ).toDouble();

	// Compute quadratic ShapeSphere coefficients
	double A =   objectRay.direction().x * objectRay.direction().x
			   + objectRay.direction().y * objectRay.direction().y
			   + objectRay.direction().z * objectRay.direction().z;

	double B = 2.0 * (   objectRay.origin.x * objectRay.direction().x
					   + objectRay.origin.y * objectRay.direction().y
					   + objectRay.origin.z * objectRay.direction().z
					   - objectRay.direction().y * r );

	double C =   objectRay.origin.x * objectRay.origin.x
			   + objectRay.origin.y * objectRay.origin.y
			   + objectRay.origin.z * objectRay.origin.z
			   - 2 * objectRay.origin.y * r;

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) )	return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double thit = ( t0 > objectRay.mint )? t0 : t1 ;
	if( thit > objectRay.maxt ) return false;

	//Evaluate Tolerance
	double tol = 0.00001;

	double ymax = r - sqrt( r* r - ( wX / 2 ) * ( wX / 2 )
													- ( wZ / 2 ) * ( wZ / 2 ) );

	//Compute possible hit position
	Point3D hitPoint = objectRay( thit );

	// Test intersection against clipping parameters
	if( (thit - objectRay.mint) < tol ||  hitPoint.x < ( - wX / 2 ) || hitPoint.x > ( wX / 2 ) ||
			hitPoint.z < ( - wZ / 2 ) || hitPoint.z > ( wZ / 2 ) || hitPoint.y < 0.0 || hitPoint.y > ymax  )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		hitPoint = objectRay( thit );
		if( (thit - objectRay.mint) < tol ||  hitPoint.x < ( - wX / 2 ) || hitPoint.x > ( wX / 2 ) ||
					hitPoint.z < ( - wZ / 2 ) || hitPoint.z > ( wZ / 2 ) || hitPoint.y < 0.0 || hitPoint.y > ymax  )
			return false;

	}

	// Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) )	gf::SevereError( "Function ShapeSphericalRectangle::Intersect(...) called with null pointers" );

///////////////////////////////////////////////////////////////////////////////////////
	// Compute possible parabola hit position

	// Find parametric representation of paraboloid hit
	double u =  ( hitPoint.x  / wX ) + 0.5;
	double v =  ( hitPoint.z  / wZ ) + 0.5;

	double aux = r * r
			- (-0.5 + u) * (-0.5 + u) * wX *wX
			- (-0.5 + v) * (-0.5 + v) * wZ *wZ;


	Vector3D dpdu( wX,
					( (-0.5 + u) * wX *  wX )/ sqrt( r * r
							- (-0.5 + u) * (-0.5 + u) * wX *wX
							- (-0.5 + v) * (-0.5 + v) * wZ *wZ ),
					0 );


	Vector3D dpdv( 0.0,
				( (-0.5 + v) * wZ *  wZ )/ sqrt( r * r
						- (-0.5 + u) * (-0.5 + u) * wX *wX
						- (-0.5 + v) * (-0.5 + v) * wZ *wZ ),
				wZ );

	// Compute parabaloid \dndu and \dndv
	Vector3D d2Pduu( 0.0,
			( ( (-0.5 + u) * (-0.5 + u) * wX *  wX * wX *  wX ) / sqrt( aux * aux * aux ) )
				+ ( ( wX *  wX )  / sqrt( aux ) ),
			0.0 );

	Vector3D d2Pduv( 0.0,
			( (-0.5 + u) * (-0.5 + v ) * wX *  wX * wZ *  wZ  )
				/ sqrt( aux * aux * aux ),
			0.0 );
	Vector3D d2Pdvv( 0.0,
				( ( (-0.5 + u) * (-0.5 + u) * wZ *  wZ * wZ *  wZ ) / sqrt( aux * aux * aux ) )
					+ ( ( wZ *  wZ )  / sqrt( aux ) ),
				0.0 );

	// Compute coefficients for fundamental forms
	double E = DotProduct(dpdu, dpdu);
	double F = DotProduct(dpdu, dpdv);
	double G = DotProduct(dpdv, dpdv);

	NormalVector N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

	double e = DotProduct(N, d2Pduu);
	double f = DotProduct(N, d2Pduv);
	double g = DotProduct(N, d2Pdvv);

	// Compute \dndu and \dndv from fundamental form coefficients
	double invEGF2 = 1.0 / (E*G - F*F);
	Vector3D dndu = (f*F - e*G) * invEGF2 * dpdu +
		(e*F - f*E) * invEGF2 * dpdv;
	Vector3D dndv = (g*F - f*G) * invEGF2 * dpdu +
		(f*F - g*E) * invEGF2 * dpdv;

	// Initialize _DifferentialGeometry_ from parametric information
	*dg = DifferentialGeometry(hitPoint,
							   dpdu,
							   dpdv,
							   dndu,
							   dndv,
							   u, v );

	*isShapeFront =  ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;


///////////////////////////////////////////////////////////////////////////////////////

	// Update _tHit_ for quadric intersection
	*tHit = thit;
	return ( true );
}

/*!
 * Not implemented yet.
 */
void ShapeSphericalRectangle::Draw() const
{

}
