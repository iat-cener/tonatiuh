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
***************************************************************************/

#include <vector>
#include <algorithm>

#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "gf.h"

#include "BBox.h"
#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeParabolicRectangle.h"
#include "Vector3D.h"


SO_NODE_SOURCE(ShapeParabolicRectangle);

void ShapeParabolicRectangle::initClass()
{
	SO_NODE_INIT_CLASS(ShapeParabolicRectangle, TShape, "TShape");
}

ShapeParabolicRectangle::ShapeParabolicRectangle()
{
	SO_NODE_CONSTRUCTOR(ShapeParabolicRectangle);
	SO_NODE_ADD_FIELD( focusLength, (0.125) );
	SO_NODE_ADD_FIELD( widthX, (1.0) );
	SO_NODE_ADD_FIELD( widthZ, (1.0) );

	SO_NODE_DEFINE_ENUM_VALUE( Side, INSIDE );
	SO_NODE_DEFINE_ENUM_VALUE( Side, OUTSIDE );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
	SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );
}

ShapeParabolicRectangle::~ShapeParabolicRectangle()
{
}

double ShapeParabolicRectangle::GetArea() const
{
	return -1;
}

BBox ShapeParabolicRectangle::GetBBox() const
{
	double xmin = -widthX.getValue()/2;
	double xmax = widthX.getValue()/2;

	double ymin = 0.0;
	double ymax = ( ( widthX.getValue() * widthX.getValue() ) + ( widthZ.getValue() * widthZ.getValue() ) )
			/ ( 16 * focusLength.getValue() );

	double zmin = -widthZ.getValue() / 2;
	double zmax = widthZ.getValue() / 2;

	return BBox( Point3D( xmin, ymin, zmin), Point3D( xmax, ymax, zmax) );

}

QString ShapeParabolicRectangle::GetIcon() const
{
	return ":/icons/ShapeParabolicRectangle.png";
}

bool ShapeParabolicRectangle::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	double focus = focusLength.getValue();
	double wX = widthX.getValue();
	double wZ = widthZ.getValue();

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
							   u, v, this);
	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;


///////////////////////////////////////////////////////////////////////////////////////

	// Update _tHit_ for quadric intersection
	*tHit = thit;
	return true;
}

bool ShapeParabolicRectangle::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}

Point3D ShapeParabolicRectangle::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

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
void ShapeParabolicRectangle::computeBBox( SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
	SbVec3f min, max;

	min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
	max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );;

	// Set the box to bound the two extreme points.
	box.setBounds(min, max);
}

void ShapeParabolicRectangle::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);


    SbVec3f  point;
 	const int rows = 12; // Number of points per row
    const int columns = 12; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid

    float vertex[totalPoints][6];

    int h = 0;
    double ui = 0;
	double vj = 0;

    for (int i = 0; i < rows; ++i )
    {
    	ui =( 1.0 /(double)(rows-1) ) * i;

    	for ( int j = 0 ; j < columns ; ++j )
    	{
    		vj = ( 1.0 /(double)(columns-1) ) * j;

    		Point3D point = GetPoint3D(ui, vj);
    		NormalVector normal;
    		if( activeSide.getValue() == 0 )	normal = -GetNormal(ui, vj);
    		else	normal = GetNormal(ui, vj);

    		vertex[h][0] = point.x;
    		vertex[h][1] = point.y;
    		vertex[h][2] = point.z;
    		vertex[h][3] = normal.x;
    		vertex[h][4] = normal.y;
    		vertex[h][5] = normal.z;

    		pv.setPoint( vertex[h][0], vertex[h][1], vertex[h][2] );
    		h++; //Increase h to the next point.

    	}
    }

    float u = 1;
    float v = 1;
    beginShape(action, QUADS );
	for( int irow = 0; irow < (rows-1); ++irow )
	{
		for( int icolumn = 0; icolumn < (columns-1); ++icolumn )
		{
			int index0 = irow*columns + icolumn;
			SbVec3f  point0( vertex[index0][0], vertex[index0][1],  vertex[index0][2] );
			SbVec3f normal0(vertex[index0][3], vertex[index0][4], vertex[index0][5] );
			SbVec4f texCoord0 = useTexFunc ? tce->get(point0, normal0): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point0);
			pv.setNormal(normal0);
			pv.setTextureCoords(texCoord0);
			shapeVertex(&pv);

			int index1 = index0 + 1;
			SbVec3f  point1( vertex[index1][0], vertex[index1][1],  vertex[index1][2] );
			SbVec3f normal1(vertex[index1][3], vertex[index1][4], vertex[index1][5] );
			SbVec4f texCoord1 = useTexFunc ? tce->get(point1, normal1): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point1);
			pv.setNormal(normal1);
			pv.setTextureCoords(texCoord1);
			shapeVertex(&pv);

			int index3 = index0 + columns;
			int index2 = index3 + 1;

			SbVec3f  point2( vertex[index2][0], vertex[index2][1],  vertex[index2][2] );
			SbVec3f normal2(vertex[index2][3], vertex[index2][4], vertex[index2][5] );
			SbVec4f texCoord2 = useTexFunc ? tce->get(point2, normal2): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point2);
			pv.setNormal(normal2);
			pv.setTextureCoords(texCoord2);
			shapeVertex(&pv);

			SbVec3f  point3( vertex[index3][0], vertex[index3][1],  vertex[index3][2] );
			SbVec3f normal3(vertex[index3][3], vertex[index3][4], vertex[index3][5] );
			SbVec4f texCoord3 = useTexFunc ? tce->get(point3, normal3): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point3);
			pv.setNormal(normal3);
			pv.setTextureCoords(texCoord3);
			shapeVertex(&pv);

		}
	}

	endShape();

}
