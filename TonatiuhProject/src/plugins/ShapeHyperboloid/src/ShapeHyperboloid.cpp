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
#include "gf.h"
#include "ShapeHyperboloid.h"

TNodeType ShapeHyperboloid::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > ShapeHyperboloid::CreateInstance( )
{
	struct EnableShapeHyperboloid : public ShapeHyperboloid { using ShapeHyperboloid::ShapeHyperboloid; };
	return std::make_shared<EnableShapeHyperboloid>();
}

/*!
 * Initializes ShapeHyperboloid type.
 */
void ShapeHyperboloid::Init()
{
	ShapeHyperboloid::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), "ShapeHyperboloid", &ShapeHyperboloid::CreateInstance );
}

/*!
 * Default constructor, initializes node instance.
 */
ShapeHyperboloid::ShapeHyperboloid( )
:TShape(),
 m_focusLegthLabel( "focusLegth" ),
 m_distanceTwoFocusLabel( "distanceTwoFocus" ),
 m_reflectorMaxDiameterLabel( "reflectorMaxDiameter" )
{
	SetName(GetType().GetName() );

	//Parameters
	m_pParametersList->Append<double>( m_focusLegthLabel, 0.1, true );
	m_pParametersList->Append<double>( m_distanceTwoFocusLabel, 10.0, true );
	m_pParametersList->Append<double>( m_reflectorMaxDiameterLabel, 1.0, true );
}

/*!
 * Destructor.
 */
ShapeHyperboloid::~ShapeHyperboloid()
{

}

/*!
 * Creates a copy of shape node.
 */
std::shared_ptr< TNode > ShapeHyperboloid::Copy() const
{
	struct EnableCreateShapeHyperboloid : public ShapeHyperboloid { using ShapeHyperboloid::ShapeHyperboloid; };
	std::shared_ptr<ShapeHyperboloid> shapeNode = std::make_unique<EnableCreateShapeHyperboloid>();
	if( shapeNode == nullptr )	return ( 0  );

	//Coping the parameters.
	shapeNode->m_pParametersList->SetValue( m_focusLegthLabel, GetParameterValue<double>( m_focusLegthLabel ) );
	shapeNode->m_pParametersList->SetValue( m_distanceTwoFocusLabel, GetParameterValue<double>( m_distanceTwoFocusLabel ) );
	shapeNode->m_pParametersList->SetValue( m_reflectorMaxDiameterLabel, GetParameterValue<double>( m_reflectorMaxDiameterLabel ) );

	return ( shapeNode );
}

/*!
 * Returns the filename that stores the shape icon.
 */
std::string ShapeHyperboloid::GetIcon() const
{

	return ( ":/icons/shapehyperboloid.png" );
}

/*!
 * Returns the bounding box for the defined parameters values.
 */
BBox ShapeHyperboloid::GetBondingBox() const
{
	double focusLength = GetParameterValue<double>( m_focusLegthLabel );
	double distanceTwoFocus = GetParameterValue<double>( m_distanceTwoFocusLabel );
	double reflectorMaxDiameter = GetParameterValue<double>( m_reflectorMaxDiameterLabel );

	double cConic = fabs( distanceTwoFocus /2 );
	double aConic = cConic - focusLength;
	double bConic = sqrt( fabs( cConic * cConic - aConic * aConic ) );

	double xmin = - ( reflectorMaxDiameter / 2 );
	double xmax = reflectorMaxDiameter / 2;
	double r = reflectorMaxDiameter / 2;
	double ymax = -aConic
							+( sqrt(  aConic * aConic * bConic * bConic
										*  ( bConic * bConic + r * r) )
					/ ( bConic * bConic ) );

	double ymin  = 0.0;

	double zmin = - ( reflectorMaxDiameter / 2 );
	double zmax = reflectorMaxDiameter / 2;

	return ( BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) ) );

}

/*!
 * Returns the type of node.
 */
TNodeType ShapeHyperboloid::GetType() const
{
	return ( m_nodeType );
}

bool ShapeHyperboloid::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{
	double focusLength = GetParameterValue<double>( m_focusLegthLabel );
	double distanceTwoFocus = GetParameterValue<double>( m_distanceTwoFocusLabel );
	double reflectorMaxDiameter = GetParameterValue<double>( m_reflectorMaxDiameterLabel );

	double xo= objectRay.origin.x;
	double yo= objectRay.origin.y;
	double zo= objectRay.origin.z;

	double xd= objectRay.direction().x;
	double yd= objectRay.direction().y;
	double zd= objectRay.direction().z;

	double cConic = fabs( distanceTwoFocus /2 );
	double aConic = cConic - focusLength;
	double bConic = sqrt( fabs( cConic * cConic - aConic * aConic ) );

	double A =  ( bConic * bConic * yd * yd  - aConic * aConic * (xd * xd  + zd * zd ) );
	double B = 2 * (aConic  * bConic * bConic * yd + bConic * bConic * yd * yo - aConic * aConic * (xd * xo + zd * zo ) );
	double C = 2 * aConic * bConic * bConic * yo + bConic * bConic * yo * yo - aConic * aConic*  (xo * xo + zo * zo );

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double thit = ( t0 > objectRay.mint )? t0 : t1 ;
	if( thit > objectRay.maxt ) return false;

   //Compute possible hyperbola hit position
	Point3D hitPoint = objectRay( thit );

	//Evaluate Tolerance
	double tol = 0.00001;

	if( (thit - objectRay.mint) < tol ) return false;

	double r = reflectorMaxDiameter/ 2;
	double ymax = -aConic + ( sqrt( aConic * aConic * bConic * bConic
											*  ( bConic * bConic + r * r) )
								/ ( bConic * bConic ) );


	double ymin  = 0.0;

	// Test intersection against clipping parameters
	double yradius = sqrt(hitPoint.x * hitPoint.x + hitPoint.z * hitPoint.z);
	if( hitPoint.y < ymin || hitPoint.y > ymax ||  yradius > r )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		hitPoint = objectRay( thit );
		yradius = sqrt(hitPoint.x * hitPoint.x + hitPoint.z * hitPoint.z);
		if( hitPoint.y < ymin || hitPoint.y > ymax ||  yradius > r ) return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	//if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	//else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function Cylinder::Intersect(...) called with null pointers" );

	// Find parametric representation of hyperbola hit
	double u = yradius / ( reflectorMaxDiameter / 2 );
	double phi = atan2( hitPoint.z , hitPoint.x );
	if( phi < 0.0 ) phi = phi + gc::TwoPi;
	double v = phi / gc::TwoPi;


	Vector3D dpdu = Dpdu( u, v );
	Vector3D dpdv = Dpdv( u, v );

	// Compute cylinder \dndu and \dndv
	Vector3D d2Pduu( 0.0,
					(2 * pow( aConic, 4) * pow( bConic, 4 ) *  reflectorMaxDiameter *  reflectorMaxDiameter )
					/ ( pow( aConic, 2 )* pow( bConic, 2)
							* pow( 4 *  bConic * bConic + reflectorMaxDiameter * reflectorMaxDiameter * u * u , 3 / 2.0 ) ),
					0.0 );

	Vector3D d2Pduv( - reflectorMaxDiameter*gc::Pi * sin( gc::TwoPi * v ),
					0.0,
					reflectorMaxDiameter *gc::Pi * cos( gc::TwoPi * v ) );
	Vector3D d2Pdvv( -2.0 * reflectorMaxDiameter * gc::Pi * gc::Pi * u * cos( gc::TwoPi * v),
					0.0,
					-2.0 * reflectorMaxDiameter * gc::Pi * gc::Pi * u * sin( gc::TwoPi * v ) );

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
void ShapeHyperboloid::Draw() const
{

}

/*!
 * Computes the partial derivative of the point surface respect the parameter 'u'.
 */
Vector3D ShapeHyperboloid::Dpdu( double u, double v ) const
{
	double focusLength = GetParameterValue<double>( m_focusLegthLabel );
	double distanceTwoFocus = GetParameterValue<double>( m_distanceTwoFocusLabel );
	double reflectorMaxDiameter = GetParameterValue<double>( m_reflectorMaxDiameterLabel );

	double cConic = fabs( distanceTwoFocus /2 );
	double aConic = cConic - focusLength;
	double bConic = sqrt( fabs( cConic * cConic - aConic * aConic ) );

	Vector3D dpdu = Vector3D( 0.5 * reflectorMaxDiameter * cos( gc::TwoPi *  v ),
			( pow( aConic, 2 ) * pow( reflectorMaxDiameter, 2 ) * u )
				/ ( 2 * sqrt( pow( aConic, 2 ) * pow( bConic, 2 ) *
						( 4 * pow( bConic, 2 )  +  ( pow( reflectorMaxDiameter, 2 ) * u * u ) ) ) ),
			0.5 * reflectorMaxDiameter * sin( gc::TwoPi * v ) );
	return dpdu;
}

/*!
 * Computes the partial derivative of the point surface respect the parameter 'v'.
 */
Vector3D ShapeHyperboloid::Dpdv( double u, double v ) const
{
	double reflectorMaxDiameter = GetParameterValue<double>( m_reflectorMaxDiameterLabel );
	Vector3D dpdv = Vector3D( - reflectorMaxDiameter * gc::Pi * u * sin( gc::TwoPi * v ),
			0.0,
			reflectorMaxDiameter * gc::Pi * u * cos( gc::TwoPi * v ) );
	return dpdv;
}
