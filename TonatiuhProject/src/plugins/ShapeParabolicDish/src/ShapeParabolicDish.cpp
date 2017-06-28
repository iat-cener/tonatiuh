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

#include "ShapeParabolicDish.h"
#include "TParameterList.h"


TNodeType ShapeParabolicDish::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* ShapeParabolicDish::CreateInstance( )
{
  return ( new ShapeParabolicDish() );
}


/*!
 * Initializes ShapeFlatDisk type.
 */
void ShapeParabolicDish::Init()
{
	ShapeParabolicDish::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), QString( "ShapeParabolicDish" ), &ShapeParabolicDish::CreateInstance );
}

/*!
 * ShapeParabolicDish : public TNode
 */
ShapeParabolicDish::ShapeParabolicDish()
:TShape()
//,
// m_nodeType(TNodeType::FromName( QLatin1String( "ShapeParabolicDish" ) ) )
{
	setObjectName(GetType().GetName());

	//Translation
	m_parametersList->Append( QLatin1String("focusLength"), 0.125 );
	m_parametersList->Append( QLatin1String("dishMinRadius"), 0.1 );
	m_parametersList->Append( QLatin1String("dishMaxRadius"), 0.5 );
	m_parametersList->Append( QLatin1String("phiMax"), gc::TwoPi );

}

/*!
 * Destructor.
 */
ShapeParabolicDish::~ShapeParabolicDish()
{

}

/*!
 * Returns the filename that stores the shape icon.
 */
QString ShapeParabolicDish::GetIcon()
{

	return QLatin1String( ":/icons/ShapeParabolicDish.png" );
}


/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeParabolicDish::GetBondingBox() const
{
	double phiMax = m_parametersList->Get( QLatin1String("phiMax") ).toDouble();
	double dishMaxRadius = m_parametersList->Get( QLatin1String("dishMaxRadius") ).toDouble();
	double dishMinRadius = m_parametersList->Get( QLatin1String("dishMinRadius") ).toDouble();
	double focusLength = m_parametersList->Get( QLatin1String("focusLength") ).toDouble();

	double cosPhiMax = cos( phiMax );
	double sinPhiMax = sin( phiMax );

	double xmin = ( phiMax >= gc::Pi  ) ?
			( phiMax >= 1.5 * gc::Pi  ) ? -dishMaxRadius : sinPhiMax* dishMaxRadius
			: 0.0;
	double xmax = ( phiMax >= gc::Pi / 2 ) ? dishMaxRadius : sinPhiMax* dishMaxRadius;

	double ymin = ( dishMinRadius > 0.0 ) ? dishMinRadius * dishMinRadius /( 4*focusLength ) : 0.0;
	double ymax = dishMaxRadius *dishMaxRadius /( 4*focusLength );

	double zmin = ( phiMax > gc::Pi ) ? -dishMaxRadius
			: std::min( dishMinRadius * cosPhiMax, dishMaxRadius * cosPhiMax );
	double zmax = dishMaxRadius;

	return BBox( Point3D( xmin, ymin, zmin), Point3D( xmax, ymax, zmax) );
}

/*!
 * Returns the type of node.
 */
TNodeType ShapeParabolicDish::GetType() const
{
	return ( m_nodeType );
}


/*!
 * Not implemented yet.
 */
bool ShapeParabolicDish::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{

	double pMax = m_parametersList->Get( QLatin1String("phiMax") ).toDouble();
	//double phiMax = m_parametersList->Get( QLatin1String("phiMax") ).toDouble();
	double dishMaxRadius = m_parametersList->Get( QLatin1String("dishMaxRadius") ).toDouble();
	double dishMinRadius = m_parametersList->Get( QLatin1String("dishMinRadius") ).toDouble();
	double focusLength = m_parametersList->Get( QLatin1String("focusLength") ).toDouble();


	double A = objectRay.direction().x*objectRay.direction().x + objectRay.direction().z * objectRay.direction().z;
	double B = 2.0 * ( objectRay.direction().x* objectRay.origin.x + objectRay.direction().z * objectRay.origin.z - 2 * focusLength * objectRay.direction().y );
	double C = objectRay.origin.x * objectRay.origin.x + objectRay.origin.z * objectRay.origin.z - 4 * focusLength * objectRay.origin.y;

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double thit = ( t0 > objectRay.mint )? t0 : t1 ;
	if( thit > objectRay.maxt ) return false;

	//Evaluate Tolerance
	double tol = 0.00001;

	//Compute possible hit position
	Point3D hitPoint = objectRay( thit );

	double radius = sqrt(hitPoint.x*hitPoint.x + hitPoint.z*hitPoint.z) ;
	double phi;
	if( ( hitPoint.z == 0.0 ) &&( hitPoint.x ==0.0 ) ) phi = 0.0;
	else if( hitPoint.x > 0 ) phi = atan2( hitPoint.x, hitPoint.z );
	else phi = gc::TwoPi + atan2( hitPoint.x, hitPoint.z );

	// Test intersection against clipping parameters
	if( (thit - objectRay.mint) < tol ||  radius < dishMinRadius || radius > dishMaxRadius || phi > pMax )
		{
			if ( thit == t1 ) return false;
			if ( t1 > objectRay.maxt ) return false;
			thit = t1;

			hitPoint = objectRay( thit );
			radius = sqrt(hitPoint.x*hitPoint.x + hitPoint.z*hitPoint.z) ;
			if( ( hitPoint.z == 0.0 ) &&( hitPoint.x ==0.0 ) ) phi = 0.0;
			else if( hitPoint.x > 0 ) phi = atan2( hitPoint.x, hitPoint.z );
			else phi = gc::TwoPi + atan2( hitPoint.x, hitPoint.z );

			if( (thit - objectRay.mint) < tol ||  radius < dishMinRadius || radius > dishMaxRadius || phi > pMax ) return false;
		}

	// Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ParabolicCyl::Intersect(...) called with null pointers" );

///////////////////////////////////////////////////////////////////////////////////////

	// Find parametric representation of paraboloid hit
	double u = phi / pMax;
	double v = ( radius - dishMinRadius )  /( dishMaxRadius- dishMinRadius );

	// Compute Circular Parabolic Facet \dpdu and \dpdv
	double r = v * ( dishMaxRadius - dishMinRadius ) + dishMinRadius;
	Vector3D dpdu( pMax * r * cos( pMax * u ),
					0,
					-pMax * r * sin( pMax * u ) );

	Vector3D dpdv( ( dishMaxRadius - dishMinRadius )  * sin( pMax * u ),
				   ( ( dishMaxRadius - dishMinRadius ) * r  )
							   / ( 2 * focusLength ),
					( dishMaxRadius - dishMinRadius ) * cos( pMax * u ) );


	// Compute Circular Parabolic Facet \dndu and \dndv
	Vector3D d2Pduu ( -pMax * pMax * r * sin( pMax * u ),
			0.0,
			-pMax* pMax * r * cos( pMax * u ) );
	Vector3D d2Pduv ( pMax* ( dishMaxRadius- dishMinRadius ) * cos( pMax * u ),
					0.0,
					-pMax * ( dishMaxRadius- dishMinRadius ) * sin( pMax * u ) );
	Vector3D d2Pdvv (0, ( ( dishMaxRadius- dishMinRadius ) * ( dishMaxRadius- dishMinRadius ) ) /(2 * focusLength ), 0 );


	// Compute coefficients for fundamental forms
	double E = DotProduct(dpdu, dpdu);
	double F = DotProduct(dpdu, dpdv);
	double G = DotProduct(dpdv, dpdv);

	Vector3D N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

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
	*isShapeFront = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;


///////////////////////////////////////////////////////////////////////////////////////

	// Update _tHit_ for quadric intersection
	*tHit = thit;

	return ( true );
}

/*!
 * Not implemented yet.
 */
void ShapeParabolicDish::Draw() const
{

}

