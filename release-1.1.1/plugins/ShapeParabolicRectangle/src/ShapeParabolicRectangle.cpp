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

#include <vector>
#include <algorithm>

#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeParabolicRectangle.h"
#include "tgf.h"
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

QString ShapeParabolicRectangle::GetIcon() const
{
	return ":/icons/ShapeParabolicRectangle.png";
}

bool ShapeParabolicRectangle::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	// Compute quadratic coefficients
	double A = objectRay.direction.x * objectRay.direction.x + objectRay.direction.z * objectRay.direction.z;
	double B = 2.0 * ( objectRay.direction.x * objectRay.origin.x + objectRay.direction.z * objectRay.origin.z  - 2 * focusLength.getValue() * objectRay.direction.y );
	double C = objectRay.origin.x * objectRay.origin.x + objectRay.origin.z * objectRay.origin.z - 4 * focusLength.getValue()* objectRay.origin.y;

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !tgf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
    double thit = ( t0 > objectRay.mint )? t0 : t1 ;
    if( thit > objectRay.maxt ) return false;

    //Evaluate Tolerance
	double tol = 0.00001;

	//Compute possible hit position
	Point3D hitPoint = objectRay( thit );

	// Test intersection against clipping parameters
	if( (thit - objectRay.mint) < tol ||  hitPoint.x < ( - widthX.getValue() / 2 ) || hitPoint.x > ( widthX.getValue() / 2 ) ||
			hitPoint.z < ( - widthZ.getValue() / 2 ) || hitPoint.z > ( widthZ.getValue() / 2 ) )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		hitPoint = objectRay( thit );
		if( (thit - objectRay.mint) < tol ||  hitPoint.x < ( - widthX.getValue() / 2 ) || hitPoint.x > ( widthX.getValue() / 2 ) ||
					hitPoint.z < ( - widthZ.getValue() / 2 ) || hitPoint.z > ( widthZ.getValue() / 2 ) )	return false;

	}

    // Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function ParabolicCyl::Intersect(...) called with null pointers" );

///////////////////////////////////////////////////////////////////////////////////////

	// Compute possible parabola hit position

	// Find parametric representation of paraboloid hit
	double u =  ( hitPoint.x  / widthX.getValue() ) + 0.5;
	double v =  ( hitPoint.z  / widthZ.getValue() ) + 0.5;

	Vector3D dpdu( widthX.getValue(), ( (-0.5 + u) * widthX.getValue() *  widthX.getValue() )/(2 * focusLength.getValue()), 0 );
	Vector3D dpdv( 0.0, (( -0.5 + v) * widthZ.getValue() *  widthZ.getValue() ) /( 2 * focusLength.getValue() ), widthZ.getValue() );

	// Compute parabaloid \dndu and \dndv
	Vector3D d2Pduu( 0.0,  (widthX.getValue() *  widthX.getValue() ) /( 2 * focusLength.getValue() ), 0.0 );
	Vector3D d2Pduv( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv( 0.0,  (widthZ.getValue() *  widthZ.getValue() ) /( 2 * focusLength.getValue() ), 0.0 );

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
	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction ) > 0 ) ? false : true;


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
	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );

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
	double xmin = -widthX.getValue()/2;
	double xmax = widthX.getValue()/2;

	double ymin = 0.0;
	double ymax = ( ( widthX.getValue() * widthX.getValue() ) + ( widthZ.getValue() * widthZ.getValue() ) )
			/ ( 16 * focusLength.getValue() );

	double zmin = -widthZ.getValue() / 2;
	double zmax = widthZ.getValue() / 2;

	box.setBounds(SbVec3f( xmin, ymin, zmin ), SbVec3f( xmax, ymax, zmax ));
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
 	const int rows = 10; // Number of points per row
    const int columns = 10; // Number of points per column
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

	const int totalIndices  = (rows-1)*(columns-1)*4;
    int32_t* indices = new int32_t[totalIndices];
    int k = 0;
    for( int irow = 0; irow < (rows-1); ++irow )
           for( int icolumn = 0; icolumn < (columns-1); ++icolumn )
           {
           	indices[k] = irow*columns + icolumn;
        	indices[k+1] = indices[k] + 1;
        	indices[k+3] = indices[k] + columns;
        	indices[k+2] = indices[k+3] + 1;

        	k+=4; //Set k to the first point of the next face.
           }

    float finalvertex[totalIndices][6];
    for(int ivert = 0; ivert<totalIndices; ++ivert )
    {
    	finalvertex[ivert][0] = vertex[indices[ivert]][0];
    	finalvertex[ivert][1] = vertex[indices[ivert]][1];
    	finalvertex[ivert][2] = vertex[indices[ivert]][2];
    	finalvertex[ivert][3] = vertex[indices[ivert]][3];
    	finalvertex[ivert][4] = vertex[indices[ivert]][4];
    	finalvertex[ivert][5] = vertex[indices[ivert]][5];
    }
    delete[] indices;

    float u = 1;
    float v = 1;

	beginShape(action, QUADS );
    for( int i = 0; i < totalIndices; ++i )
    {
    	SbVec3f  point( finalvertex[i][0], finalvertex[i][1],  finalvertex[i][2] );
    	SbVec3f normal(finalvertex[i][3],finalvertex[i][4], finalvertex[i][5] );
		SbVec4f texCoord = useTexFunc ? tce->get(point, normal): SbVec4f( u,v, 0.0, 1.0 );

		pv.setPoint(point);
		pv.setNormal(normal);
		pv.setTextureCoords(texCoord);
		shapeVertex(&pv);
    }
    endShape();
}
