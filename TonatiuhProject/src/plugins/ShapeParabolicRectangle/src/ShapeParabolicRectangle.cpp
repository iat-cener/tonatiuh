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

#include "ShapeParabolicRectangle.h"


TNodeType ShapeParabolicRectangle::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > ShapeParabolicRectangle::CreateInstance( )
{
	//shared_prt needs a public constructor
	struct EnableCreateShapeParabolicRectangle : public ShapeParabolicRectangle { using ShapeParabolicRectangle::ShapeParabolicRectangle; };
	return std::make_shared<EnableCreateShapeParabolicRectangle>();
}


/*!
 * Initializes ShapeParabolicRectangle type.
 */
void ShapeParabolicRectangle::Init()
{
	ShapeParabolicRectangle::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), "ShapeParabolicRectangle", &ShapeParabolicRectangle::CreateInstance );
}

/*!
 * ShapeParabolicRectangle : public TNode
 */
ShapeParabolicRectangle::ShapeParabolicRectangle()
:TShape(),
 m_focusLengthLabel( "focusLength" ),
 m_widthXLabel( "widthX" ),
 m_widthZLabel( "widthZ" )
{
	SetName(GetType().GetName() );

	//Translation
	m_pParametersList->Append<double>( m_focusLengthLabel, 0.125, true );
	m_pParametersList->Append<double>( m_widthXLabel, 1.0, true );
	m_pParametersList->Append<double>( m_widthZLabel, 1.0, true );
}

/*!
 * Destructor.
 */
ShapeParabolicRectangle::~ShapeParabolicRectangle()
{

}

/*!
 * Creates a copy of shape node.
 */
std::shared_ptr< TNode > ShapeParabolicRectangle::Copy() const
{
	struct EnableCreateShapeParabolicRectangle : public ShapeParabolicRectangle { using ShapeParabolicRectangle::ShapeParabolicRectangle; };
	std::shared_ptr<ShapeParabolicRectangle> shapeNode = std::make_unique<EnableCreateShapeParabolicRectangle>();
	if( shapeNode == nullptr )	return ( 0  );


	//Coping the parameters.
	shapeNode->m_pParametersList->SetValue( m_focusLengthLabel, GetParameterValue<double>( m_focusLengthLabel ) );
	shapeNode->m_pParametersList->SetValue( m_widthXLabel, GetParameterValue<double>( m_widthXLabel ) );
	shapeNode->m_pParametersList->SetValue( m_widthZLabel, GetParameterValue<double>( m_widthZLabel ) );

	return ( shapeNode );
}

/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeParabolicRectangle::GetBondingBox() const
{
	double focusLength = GetParameterValue<double>( m_focusLengthLabel );
	double widthX= GetParameterValue<double>( m_widthXLabel );
	double widthZ = GetParameterValue<double>( m_widthZLabel );

	double xmin = -widthX/2;
	double xmax = widthX/2;

	double ymin = 0.0;
	double ymax = ( ( widthX * widthX ) + ( widthZ * widthZ ) )
			/ ( 16 * focusLength );

	double zmin = -widthZ / 2;
	double zmax = widthZ / 2;

	return BBox( Point3D( xmin, ymin, zmin), Point3D( xmax, ymax, zmax) );

}

/*!
 * Returns the filename that stores the shape icon.
 */
std::string ShapeParabolicRectangle::GetIcon() const
{
	return ( ":icons/ShapeParabolicRectangle.png" );
}


/*!
 * Returns the type of node.
 */
TNodeType ShapeParabolicRectangle::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Computes if the \a objectRay intersects with current surface.
 * \a tHit, \a dg and \a isShapeFront return information about the intersection point.
 */
bool ShapeParabolicRectangle::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{
	double focus = GetParameterValue<double>( m_focusLengthLabel );
	double wX= GetParameterValue<double>( m_widthXLabel );
	double wZ = GetParameterValue<double>( m_widthZLabel );


	// Compute quadratic coefficients
	double A = objectRay.direction().x * objectRay.direction().x + objectRay.direction().z * objectRay.direction().z;
	double B = 2.0 * ( objectRay.direction().x * objectRay.origin.x + objectRay.direction().z * objectRay.origin.z  - 2 * focus * objectRay.direction().y );
	double C = objectRay.origin.x * objectRay.origin.x + objectRay.origin.z * objectRay.origin.z - 4 * focus * objectRay.origin.y;

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

	// Test intersection against clipping parameters
	if( (thit - objectRay.mint) < tol ||  hitPoint.x < ( - wX / 2 ) || hitPoint.x > ( wX / 2 ) ||
			hitPoint.z < ( - wZ / 2 ) || hitPoint.z > ( wZ / 2 ) )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		hitPoint = objectRay( thit );
		if( (thit - objectRay.mint) < tol ||  hitPoint.x < ( - wX / 2 ) || hitPoint.x > ( wX / 2 ) ||
					hitPoint.z < ( - wZ / 2 ) || hitPoint.z > ( wZ / 2 ) )	return false;

	}

    // Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) )	gf::SevereError( "Function ParabolicCyl::Intersect(...) called with null pointers" );

///////////////////////////////////////////////////////////////////////////////////////

	// Compute possible parabola hit position

	// Find parametric representation of paraboloid hit
	double u =  ( hitPoint.x  / wX ) + 0.5;
	double v =  ( hitPoint.z  / wZ ) + 0.5;

	Vector3D dpdu( wX, ( (-0.5 + u) * wX *  wX ) / ( 2 * focus ), 0 );
	Vector3D dpdv( 0.0, (( -0.5 + v) * wZ *  wZ ) /( 2 * focus ), wZ );

	// Compute parabaloid \dndu and \dndv
	Vector3D d2Pduu( 0.0,  (wX *  wX ) /( 2 * focus ), 0.0 );
	Vector3D d2Pduv( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv( 0.0,  (wZ *  wZ ) /( 2 * focus ), 0.0 );

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
							   u, v);
	*isShapeFront = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

///////////////////////////////////////////////////////////////////////////////////////

	// Update _tHit_ for quadric intersection
	*tHit = thit;
	return true;
}

/*!
 * Not implemented yet.
 */
void ShapeParabolicRectangle::Draw() const
{

}

/*
bool ShapeParabolicRectangle::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

Point3D ShapeParabolicRectangle::GetPoint3D( double u, double v ) const
{
	if ( OutOfRange( u, v ) ) gf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );

	double x = ( u - 0.5 )* widthX.getValue();
	double z = ( v - 0.5 )* widthZ.getValue();
	double y = ( x * x + z * z)/( 4 * focusLength.getValue() );
	return Point3D (x, y, z);

}

NormalVector ShapeParabolicRectangle::GetNormal( double u, double v ) const
{
	Vector3D dpdu( widthX.getValue(), ( (-0.5 + u) * widthX.getValue() *  widthX.getValue() )/(2 * focusLength.getValue()), 0 );
	Vector3D dpdv( 0.0, (( -0.5 + v) * widthZ.getValue() *  widthZ.getValue() ) /( 2 * focusLength.getValue() ), widthZ.getValue() );
	return Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
}
*/
