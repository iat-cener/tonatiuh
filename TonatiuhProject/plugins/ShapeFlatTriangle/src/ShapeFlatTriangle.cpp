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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Iñigo Pagola, Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "gf.h"
#include "ParameterValueException.h"
#include "ShapeFlatTriangle.h"

SO_NODE_SOURCE(ShapeFlatTriangle);

/**
 * Sets up initialization for data common to all instances of this class.
 */
void ShapeFlatTriangle::initClass()
{
	SO_NODE_INIT_CLASS(ShapeFlatTriangle, TShape, "TShape");
}

/**
 * Default constructor, initializes node instance.
 */
ShapeFlatTriangle::ShapeFlatTriangle(  )
{
	SO_NODE_CONSTRUCTOR(ShapeFlatTriangle);
	SO_NODE_ADD_FIELD( a, (-0.5, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( b, ( 0.5, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( c, ( 0.0, 1.0, 0.0) );
}

/**
 * Destructor.
 */
ShapeFlatTriangle::~ShapeFlatTriangle()
{
}

/*!
 * Return the shape bounding box.
 */
BBox ShapeFlatTriangle::GetBBox() const
{
	double xmin = std::min( a.getValue()[0], std::min( b.getValue()[0], c.getValue()[0] ) );
	double xmax = std::max( a.getValue()[0], std::max( b.getValue()[0], c.getValue()[0] ) );
	double ymin = std::min( a.getValue()[1], std::min( b.getValue()[1], c.getValue()[1] ) );
	double ymax = std::max( a.getValue()[1], std::max( b.getValue()[1], c.getValue()[1] ) );
	double zmin = std::min( a.getValue()[2], std::min( b.getValue()[2], c.getValue()[2] ) );
	double zmax = std::max( a.getValue()[2], std::max( b.getValue()[2], c.getValue()[2] ) );

	return BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) );
}

std::string ShapeFlatTriangle::GetIcon() const
{
	return ( ":/icons/ShapeFlatTriangle.png" );
}

bool ShapeFlatTriangle::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	Vector3D vAB = Vector3D( b.getValue()[0], b.getValue()[1], b.getValue()[2] ) - Vector3D( a.getValue()[0], a.getValue()[1], a.getValue()[2] );
	Vector3D vAC = Vector3D( c.getValue()[0], c.getValue()[1], c.getValue()[2] ) - Vector3D( a.getValue()[0], a.getValue()[1], a.getValue()[2] );

	Vector3D vN = CrossProduct( vAB, vAC );
	double d = -vN.x * a.getValue()[0] - vN.y * a.getValue()[1] - vN.z * a.getValue()[2];

	double thit = (-d - vN.x * objectRay.origin.x - vN.y * objectRay.origin.y - vN.z * objectRay.origin.z )
			/ (vN.x * objectRay.direction().x  + vN.y * objectRay.direction().y + vN.z * objectRay.direction().z );

	//Evaluate Tolerance
	double tol = 0.00001;
	if( thit > objectRay.maxt ) return false;
	if( (thit - objectRay.mint) < tol ) return false;

	Point3D hitPoint = objectRay( thit );

	// is hitPoint inside triangle?
	double uu = DotProduct( vAB, vAB );
	double uv = DotProduct( vAB, vAC );
	double vv = DotProduct( vAC, vAC );
	Vector3D  w = Vector3D( hitPoint ) - Vector3D( a.getValue()[0], a.getValue()[1], a.getValue()[2] );
	double wu = DotProduct( w, vAB );
	double wv = DotProduct( w, vAC );
	double D = uv * uv - uu * vv;

	// get and test parametric coords
	double u = (uv * wv - vv * wu) / D;
	if( u < 0.0 || u > 1.0 )	return false;

	double v = ( uv * wu - uu * wv ) / D;
	if( v < 0.0 || ( u + v) > 1.0)	return false;

	// Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeFlatTriangle::Intersect(...) called with null pointers" );

	Vector3D dpdu = vAB;
	Vector3D dpdv = vAC;

	// Compute \dndu and \dndv
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
		                        u, v, this );

	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeFlatTriangle::IntersectP( const Ray& worldRay ) const
{
	return Intersect( worldRay, 0, 0 );
}

Point3D ShapeFlatTriangle::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

/*!
* Checks the \a value for parameter \a name.
*/
bool ShapeFlatTriangle::ValidateParamaterValue( std::string name, std::string value ) const
{
	trt::TONATIUH_REALVECTOR3 newValue;
	newValue.set( value.c_str() );  

	Point3D v0( a.getValue()[0], a.getValue()[1], a.getValue()[2]);
	Point3D v1( b.getValue()[0], b.getValue()[1], b.getValue()[2] );
	Point3D v2( c.getValue()[0], c.getValue()[1], c.getValue()[2] );

	if( name == "a" )
	{
		v0 = Point3D( newValue.getValue()[0], newValue.getValue()[1], newValue.getValue()[2] );
		if( newValue == b )
			throw ParameterValueException( "a", "'a' and 'b' vertex coordinates cannot be the same" );
		else if(  newValue == c )
			throw ParameterValueException( "a", "'a' and 'c' vertex coordinates cannot be the same" );
	}
	
	if( name == "b" )
	{
		v1 = Point3D( newValue.getValue()[0], newValue.getValue()[1], newValue.getValue()[2] );
		if( newValue == a )
			throw ParameterValueException( "b", "'a' and 'b' vertex coordinates cannot be the same" );
		if( newValue == c )
			throw ParameterValueException( "b", "'b' and 'c' vertex coordinates cannot be the same" );
	}
	if( name == "c" )
	{
		v2 = Point3D( newValue.getValue()[0], newValue.getValue()[1], newValue.getValue()[2] );
		if( newValue == a )
			throw ParameterValueException( "c", "'a' and 'c' vertex coordinates cannot be the same" );
		if( newValue == b )
		 	throw ParameterValueException( "c", "'b' and 'c' vertex coordinates cannot be the same" );
	}

	Vector3D edge1 = Normalize( v1 - v0 );
	Vector3D edge2 = Normalize( v2 - v0 );
	if( fabs(fabs( DotProduct( edge1, edge2 ) )  - 1.0 ) < 0.00000001 ) 
	{
		throw ParameterValueException( name, "Defined three vertex, are colinear points." );
	}
	return true;
}

Point3D ShapeFlatTriangle::GetPoint3D (double u, double v) const
{
	if ( OutOfRange( u, v ) ) gf::SevereError( "Function ShapeFlatTriangle::GetPoint3D called with invalid parameters" );

	Point3D v0( a.getValue()[0], a.getValue()[1], a.getValue()[2]);
	Point3D v1( b.getValue()[0], b.getValue()[1], b.getValue()[2] );
	Point3D v2( c.getValue()[0], c.getValue()[1], c.getValue()[2] );

	Vector3D edge1 = v1 - v0;
	Vector3D edge2 = v2 - v0;

	Point3D p = v0 + edge1 * u + edge2 * v;
	return p;
}

NormalVector ShapeFlatTriangle::GetNormal( double /* u */ ,double /* v */ ) const
{
   Point3D v0( a.getValue()[0], a.getValue()[1], a.getValue()[2]);
   Point3D v1( b.getValue()[0], b.getValue()[1], b.getValue()[2] );
   Point3D v2( c.getValue()[0], c.getValue()[1], c.getValue()[2] );

   Vector3D dpdu = v1 - v0;
   Vector3D dpdv = v2 - v0;
   NormalVector normal = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
   return normal;
}

bool ShapeFlatTriangle::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeFlatTriangle::computeBBox(SoAction *, SbBox3f &box, SbVec3f& /*center*/)
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
    SbVec3f min, max;

    min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
    max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );;

    // Set the box to bound the two extreme points.
    box.setBounds(min, max);
}

void ShapeFlatTriangle::generatePrimitives(SoAction *action)
{

    SoPrimitiveVertex   pv;

    // Access the state from the action.
    SoState  *state = action->getState();

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    // If we need to generate texture coordinates with a
    // function, we'll need an SoGLTextureCoordinateElement.
    // Otherwise, we'll set up the coordinates directly.
    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);



	beginShape(action, TRIANGLES );
	SbVec3f aPoint( a.getValue()[0], a.getValue()[1], a.getValue()[2] );
	SbVec3f bPoint( b.getValue()[0], b.getValue()[1], b.getValue()[2] );
	SbVec3f cPoint( c.getValue()[0], c.getValue()[1], c.getValue()[2] );

	SbVec3f dpdu =  bPoint - aPoint;
	SbVec3f dpdv = cPoint - aPoint;
	SbVec3f normal = dpdu.cross( dpdv );
	normal.normalize();


	//a Point
	SbVec4f aTexCoord = useTexFunc ? tce->get(aPoint, normal): SbVec4f( 0.0 ,0.0, 0.0, 1.0 );
	pv.setPoint(aPoint);
	pv.setNormal(normal);
	pv.setTextureCoords(aTexCoord);
	shapeVertex(&pv);

	//b Point
	SbVec4f bTexCoord = useTexFunc ? tce->get(bPoint, normal): SbVec4f( 1.0, 0.0, 0.0, 1.0 );
	pv.setPoint(bPoint);
	pv.setNormal(normal);
	pv.setTextureCoords(bTexCoord);
	shapeVertex(&pv);

	//a Point
	SbVec4f cTexCoord = useTexFunc ? tce->get(cPoint, normal): SbVec4f( 0.0, 1.0, 0.0, 1.0 );
	pv.setPoint(cPoint);
	pv.setNormal(normal);
	pv.setTextureCoords(cTexCoord);
	shapeVertex(&pv);

    endShape();
}
