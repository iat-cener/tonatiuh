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
#include "gc.h"
#include "gf.h"

#include "ShapeSphericalPolygon.h"

TNodeType ShapeSphericalPolygon::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > ShapeSphericalPolygon::CreateInstance( )
{
	//shared_prt needs a public constructor
	struct EnableCreateShapeSphericalPolygon : public ShapeSphericalPolygon { using ShapeSphericalPolygon::ShapeSphericalPolygon; };
	return std::make_shared<EnableCreateShapeSphericalPolygon>();
}


/*!
 * Initializes ShapeSphericalPolygon type.
 */
void ShapeSphericalPolygon::Init()
{
	ShapeSphericalPolygon::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), "ShapeSphericalPolygon", &ShapeSphericalPolygon::CreateInstance );
}

/*!
 * ShapeSphericalPolygon : public TNode
 */
ShapeSphericalPolygon::ShapeSphericalPolygon()
:TShape( ),
 m_sphereRadiusLabel( "sphereRadius" ),
 m_radiusLabel( "radius" ),
 m_polygonSidesLabel( "m_polygonSidesLabel" )
{
	SetName(GetType().GetName() );

	//Translation
	m_pParametersList->Append<double>( m_sphereRadiusLabel, 0.5, true );
	m_pParametersList->Append<double>( m_radiusLabel, 0.5, true );
	m_pParametersList->Append<int>( m_polygonSidesLabel, 6, true );

}

/*!
 * Destructor.
 */
ShapeSphericalPolygon::~ShapeSphericalPolygon()
{

}

/*!
 * Creates a copy of shape node.
 */
std::shared_ptr< TNode > ShapeSphericalPolygon::Copy() const
{
	struct EnableCreateShapeSphericalPolygon : public ShapeSphericalPolygon { using ShapeSphericalPolygon::ShapeSphericalPolygon; };
	std::shared_ptr<ShapeSphericalPolygon> shapeNode = std::make_unique<EnableCreateShapeSphericalPolygon>();
	if( shapeNode == nullptr )	return ( 0  );


	//Coping the parameters.
	shapeNode->m_pParametersList->SetValue( m_sphereRadiusLabel, GetParameterValue<double>( m_sphereRadiusLabel ) );
	shapeNode->m_pParametersList->SetValue( m_radiusLabel, GetParameterValue<double>( m_radiusLabel ) );
	shapeNode->m_pParametersList->SetValue( m_polygonSidesLabel, GetParameterValue<int>( m_polygonSidesLabel ) );

	return ( shapeNode );
}

/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeSphericalPolygon::GetBondingBox() const
{
	double sphereRadius = GetParameterValue<double>( m_sphereRadiusLabel );
	double radius = GetParameterValue<double>( m_radiusLabel );

	double xmax = radius;
	double ymax = radius;
	double thetaMax = asin( radius / sphereRadius );

	double zmax = sphereRadius * ( 1 - cos( thetaMax ) );

	return BBox( Point3D( - xmax, - ymax, 0 ), Point3D( xmax, ymax, zmax) );
}

/*!
 * Returns the filename that stores the shape icon.
 */
std::string ShapeSphericalPolygon::GetIcon() const
{
	return ( ":icons/ShapeSphericalPolygon.png" );
}


/*!
 * Returns the type of node.
 */
TNodeType ShapeSphericalPolygon::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Computes if the \a objectRay intersects with current surface.
 * \a tHit, \a dg and \a isShapeFront return information about the intersection point.
 */
bool ShapeSphericalPolygon::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{
	double sphereRadius = GetParameterValue<double>( m_sphereRadiusLabel );
	double radius = GetParameterValue<double>( m_radiusLabel );
	int polygonSides = GetParameterValue<int>( m_polygonSidesLabel );

	double A = objectRay.direction().lengthSquared();
	double B = 2.0 * ( objectRay.origin.x *  objectRay.direction().x
					 + objectRay.origin.y *  objectRay.direction().y
					 + objectRay.origin.z *  objectRay.direction().z
					 - objectRay.direction().z * sphereRadius );

	double C = objectRay.origin.x * objectRay.origin.x
			 + objectRay.origin.y * objectRay.origin.y
			 + objectRay.origin.z * objectRay.origin.z
			 - 2 * objectRay.origin.z * sphereRadius;

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
	//double phi = atan2( hitPoint.y, hitPoint.x );
	//if ( phi < 0. ) phi += gc::TwoPi;

	Vector3D nR = Normalize( Vector3D( hitPoint.x, hitPoint.y, 0.0 ) );
	double phi = atan2( hitPoint.x, hitPoint.y );
	if( phi < 0.0 )	phi += gc::TwoPi;

	double u = phi / gc::TwoPi;
	double centralAngle = gc::TwoPi / polygonSides;

	double part  =  floor( phi / centralAngle );
	if( fabs( u - 1.0 ) < gc::Epsilon ) 	part = polygonSides - 1;

	double t2 = radius * cos( 0.5 * centralAngle  )
							* ( 1 / cos( 0.5 * centralAngle  - phi + centralAngle * part ) );
	double x = sin( phi ) * t2;
	double y = cos( phi )* t2;
	Vector3D tPoint = Vector3D( x, y,  sphereRadius- sqrt( sphereRadius * sphereRadius - x * x - y * y  ) );
	double zRadius = sqrt( hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y );

	if( (thit - objectRay.mint) < tol || zRadius > radius || Vector3D( hitPoint).length() > tPoint.length() )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;

		thit = t1;
		hitPoint = objectRay( thit );

		Vector3D nR = Normalize( Vector3D( hitPoint.x, hitPoint.y, 0.0 ) );
		phi = atan2( hitPoint.x, hitPoint.y );
		if( phi < 0.0 )	phi += gc::TwoPi;

		u = phi / gc::TwoPi;
		part  =  floor( phi / centralAngle );
		if( fabs( u - 1.0 ) < gc::Epsilon ) 	part = polygonSides - 1;

		t2 = radius * cos( 0.5 * centralAngle  )
										* ( 1 / cos( 0.5 * centralAngle  - phi + centralAngle * part ) );
		x = sin( phi ) * t2;
		y = cos( phi )* t2;
		tPoint = Vector3D( x, y,  sphereRadius- sqrt( sphereRadius * sphereRadius - x * x - y * y  ) );


		zRadius = sqrt( hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y );

		if( (thit - objectRay.mint) < tol || zRadius > radius || Vector3D( hitPoint).length() > tPoint.length() )	return false;
	}

	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );

	double v = Vector3D( hitPoint).length()/ tPoint.length();

	// Compute ShapeSphere \dpdu and \dpdv
	double theta = acos( ( sphereRadius - hitPoint.z ) / sphereRadius );
	double thetaMax =  asin( radius / sphereRadius ) ;

	// Compute sphere \dpdu and \dpdv
	Vector3D dpdu( gc::TwoPi * radius * cos( phi ) * sin( theta ),
					- gc::TwoPi * radius * sin( phi ) *sin( theta ),
					0.0 );
	Vector3D dpdv( radius * thetaMax * cos( theta ) * sin( phi ),
					radius * thetaMax * cos( phi ) * cos( theta ),
					radius * thetaMax * sin( theta ) );

	Vector3D d2Pduu( -4  * gc::Pi * gc::Pi * radius * sin( phi ) * sin( theta ),
					 -4  * gc::Pi * gc::Pi * radius * cos( phi ) * sin( theta ),
					 0.0 );
	Vector3D d2Pduv( gc::TwoPi * radius * thetaMax * cos( phi ) * cos( theta ),
					- gc::TwoPi * radius * thetaMax * sin( phi ) * cos( theta ),
						 0.0 );

	Vector3D d2Pdvv( -radius * thetaMax * thetaMax * sin( phi )* sin( theta ),
			-radius * thetaMax * thetaMax * cos( phi )* sin( theta ),
			radius * thetaMax * thetaMax * cos( theta ) );


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
void ShapeSphericalPolygon::Draw() const
{

}

/*
Point3D ShapeSphericalPolygon::GetPoint3D( double u, double v ) const
{
	if ( OutOfRange( u, v ) ) gf::SevereError( "Function ShapeSphericalPolygon::GetPoint3D called with invalid parameters" );

	double centralAngle = gc::TwoPi / polygonSides;
	double phi = u * gc::TwoPi;

	double part  =  floor( phi / centralAngle );
	if( fabs( u - 1.0 ) < gc::Epsilon ) 	part = polygonSides - 1;
	//double u1 = ( u - ( part * 1 / polygonSides ) )*polygonSides;

	Point3D aPoint = Point3D( sin( part* centralAngle ), cos(  part* centralAngle ), 0.0  ) * radius;
	Point3D bPoint = Point3D( sin( ( part + 1 ) * centralAngle ), cos( ( part + 1 ) * centralAngle ), 0.0  ) * radius;
	double t = 0.5 *  ( 1 / sin( 0.5 * centralAngle ) )
	               * (1 / cos( 0.5 * centralAngle - phi + centralAngle  * part ) )
	               * sin( phi - centralAngle * part );
	Point3D tPoint = aPoint + ( bPoint - aPoint) * t;

	double x = v * tPoint.x;
	double y = v * tPoint.y;
	double z = sphereRadius;
	if( fabs( sphereRadius *  sphereRadius - x  * x  - y * y  ) > gc::Epsilon )
		z = sphereRadius - sqrt( sphereRadius *  sphereRadius
												- x  * x  - y * y );

	return Point3D( x, y, z );
}

NormalVector ShapeSphericalPolygon::GetNormal(  double u, double v  ) const
{
	Point3D point = GetPoint3D( u, v );
	Vector3D vector( point.x, point.y, point.z - sphereRadius);
	return NormalVector( -point.x/ vector.length(), -point.y/vector.length(), -( point.z - sphereRadius )/vector.length() );
}

bool ShapeSphericalPolygon::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1 ) );
}
*/

