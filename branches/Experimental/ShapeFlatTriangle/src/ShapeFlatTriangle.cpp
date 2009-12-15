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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <vector>
#include <algorithm>

#include <QString>
#include <QVector>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>


#include "DifferentialGeometry.h"
#include "NormalVector.h"
#include "Point3D.h"
#include "Ray.h"
#include "ShapeFlatTriangle.h"
#include "tgf.h"
#include "Vector3D.h"

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
	SO_NODE_ADD_FIELD( a, (-0.5, 0.0 ) );
	SO_NODE_ADD_FIELD( b, ( 0.5, 0.0) );
	SO_NODE_ADD_FIELD( c, ( 0.0, 1.0) );
}

/**
 * Destructor.
 */
ShapeFlatTriangle::~ShapeFlatTriangle()
{
}


double ShapeFlatTriangle::GetArea() const
{
	return -1;
}

QString ShapeFlatTriangle::getIcon()
{
	return ":/icons/ShapeFlatTriangle.png";
}

bool ShapeFlatTriangle::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{

	if( objectRay.direction.z ==0.0 ) return false;

	double thit = - objectRay.origin.z / objectRay.direction.z;

	//Evaluate Tolerance
	double tol = 0.00001;
	if( thit > objectRay.maxt ) return false;
	if( (thit - objectRay.mint) < tol ) return false;

	Point3D hitPoint = objectRay( thit );
	Vector3D vAB = Vector3D( b.getValue()[0], b.getValue()[1], 0.0 ) - Vector3D( a.getValue()[0], a.getValue()[1], 0.0 );
	Vector3D vAC = Vector3D( c.getValue()[0], c.getValue()[1], 0.0 ) - Vector3D( a.getValue()[0], a.getValue()[1], 0.0 );

	// is hitPoint inside triangle?
	//double uu, uv, vv, wu, wv, D;
	double uu = DotProduct( vAB, vAB );
	double uv = DotProduct( vAB, vAC );
	double vv = DotProduct( vAC, vAC );
	Vector3D  w = Vector3D( hitPoint ) - Vector3D( a.getValue()[0], a.getValue()[1], 0.0 );
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
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function ShapeFlatTriangle::Intersect(...) called with null pointers" );

	Vector3D dpdu = vAB;
	Vector3D dpdv = vAC;

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
		                        u, v, this );

	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction ) > 0 ) ? false : true;

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

Point3D ShapeFlatTriangle::GetPoint3D (double u, double v) const
{
	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function ShapeFlatTriangle::GetPoint3D called with invalid parameters" );

	Point3D v0( a.getValue()[0], a.getValue()[1], 0.0 );
	Point3D v1( b.getValue()[0], b.getValue()[1], 0.0 );
	Point3D v2( c.getValue()[0], c.getValue()[1], 0.0 );

	Vector3D edge1 = v1 - v0;
	Vector3D edge2 = v2 - v0;

	Point3D p = v0 + edge1 * u + edge2 * v;
	return p;
}

NormalVector ShapeFlatTriangle::GetNormal( double u ,double v ) const
{

	Point3D v0( a.getValue()[0], a.getValue()[1], 0.0 );
	Point3D v1( b.getValue()[0], b.getValue()[1], 0.0 );
	Point3D v2( c.getValue()[0], c.getValue()[1], 0.0 );

	Vector3D dpdu =  v1 - v0;
	Vector3D dpdv = v2 - v0;
	NormalVector vector = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
	return vector;
}

bool ShapeFlatTriangle::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeFlatTriangle::computeBBox(SoAction *, SbBox3f &box, SbVec3f& /*center*/)
{
	double xmin = std::min( a.getValue()[0], std::min( b.getValue()[0], c.getValue()[0] ) );
	double xmax = std::max( a.getValue()[0], std::max( b.getValue()[0], c.getValue()[0] ) );
	double ymin = std::min( a.getValue()[1], std::min( b.getValue()[1], c.getValue()[1] ) );
	double ymax = std::max( a.getValue()[1], std::max( b.getValue()[1], c.getValue()[1] ) );
	double zmin = 0.0;
	double zmax = 0.0;

	box.setBounds(SbVec3f( xmin, ymin, zmin ), SbVec3f( xmax, ymax, zmax ) );

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
	SbVec3f aPoint( a.getValue()[0], a.getValue()[1], 0.0 );
	SbVec3f bPoint( b.getValue()[0], b.getValue()[1], 0.0 );
	SbVec3f cPoint( c.getValue()[0], c.getValue()[1], 0.0 );

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

