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
#include "ShapeSegment.h"
#include "tgf.h"
#include "Vector3D.h"

SO_NODE_SOURCE(ShapeSegment);

/**
 * Sets up initialization for data common to all instances of this class.
 */
void ShapeSegment::initClass()
{
	SO_NODE_INIT_CLASS(ShapeSegment, TShape, "TShape");
}

/**
 * Default constructor, initializes node instance.
 */
ShapeSegment::ShapeSegment(  )
{
	SO_NODE_CONSTRUCTOR(ShapeSegment);
	SO_NODE_ADD_FIELD( startPoint, (-0.5, 0.0 ) );
	SO_NODE_ADD_FIELD( endPoint, ( 0.5, 0.0) );
	SO_NODE_ADD_FIELD( length, (1.0 ) );
}

/**
 * Destructor.
 */
ShapeSegment::~ShapeSegment()
{

}


double ShapeSegment::GetArea() const
{
	return -1;
}

QString ShapeSegment::getIcon()
{
	return ":/icons/ShapeSegment.png";
}

bool ShapeSegment::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	double sx = startPoint.getValue()[0];
	double sy = startPoint.getValue()[1];
	double ex = endPoint.getValue()[0];
	double ey = endPoint.getValue()[1];

	double Mx = ex - sx;
	double My =ey - sy;

	 if( objectRay.direction.y * Mx == objectRay.direction.x * My ) return false;
	 double p = ( objectRay.direction.y * (sx - objectRay.origin.x) - objectRay.direction.x *(sy - objectRay.origin.y) )
			 /( objectRay.direction.x *My - Mx* objectRay.direction.y);

	 if( p < 0 || p > 1 ) return false;

	 double thit;
	 if(objectRay.direction.x == 0 )	thit = (sy - objectRay.origin.y + p * My ) / objectRay.direction.y;
	 else thit =(sx - objectRay.origin.x + p * Mx ) / objectRay.direction.x;

	//Evaluate Tolerance
	double tol = 0.00001;
	if( thit > objectRay.maxt ) return false;
	if( (thit - objectRay.mint) < tol ) return false;

	Point3D hitPoint = objectRay( thit );
	if( hitPoint.z < 0 || hitPoint.z > length.getValue() ) return false;


	// Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function ShapeSegment::Intersect(...) called with null pointers" );

	// Find parametric representation of ShapeCone hit
	double u = ( Vector3D( hitPoint.x, hitPoint.z, 0.0 ) - Vector3D( sx, sy, 0.0 ) ).length()
				/ ( Vector3D( ex, ey, 0.0 ) - Vector3D( sx, sy, 0.0 ) ).length();
	double v = hitPoint.z / length.getValue();

	Vector3D dpdu = Vector3D( endPoint.getValue()[0] - startPoint.getValue()[0],  endPoint.getValue()[1] - startPoint.getValue()[1], 0.0 );
	Vector3D dpdv(  0.0, 0.0, length.getValue() );

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

bool ShapeSegment::IntersectP( const Ray& worldRay ) const
{
	return Intersect( worldRay, 0, 0 );
}

Point3D ShapeSegment::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

Point3D ShapeSegment::GetPoint3D (double u, double v) const
{
	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function ShapeSegment::GetPoint3D called with invalid parameters" );

	SbVec2d p( startPoint.getValue() + (endPoint.getValue() - startPoint.getValue() ) * u );

	return Point3D( p[0], p[1] , v* length.getValue() );
}

NormalVector ShapeSegment::GetNormal( double u ,double v ) const
{
	Vector3D dpdu = Vector3D( endPoint.getValue()[0] - startPoint.getValue()[0],  endPoint.getValue()[1] - startPoint.getValue()[1], 0.0 );
	Vector3D dpdv(  0.0, 0.0, length.getValue() );
	NormalVector vector = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
	return vector;
}

bool ShapeSegment::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}


bool ShapeSegment::IntersectSegment( const Ray& objectRay, double sx, double sy, double ex, double ey, double* tHit ) const
{
	double Mx = ex - sx;
	double My =ey - sy;

	 if( objectRay.direction.y * Mx == objectRay.direction.x * My ) return false;
	 double p = ( objectRay.direction.y * (sx - objectRay.origin.x) - objectRay.direction.x *(sy - objectRay.origin.y) )
			 /( objectRay.direction.x *My - Mx* objectRay.direction.y);

	 if( p < 0 || p > 1 ) return false;

	 double t;
	 if(objectRay.direction.x == 0 )	t = (sy - objectRay.origin.y + p * My ) / objectRay.direction.y;
	 else t =(sx - objectRay.origin.x + p * Mx ) / objectRay.direction.x;

	 *tHit = t;
	 /*if( t <)
	  If[t < 0, "invalid", {segment, p, ray, t}]]*/

	  return true;

}
void ShapeSegment::computeBBox(SoAction *, SbBox3f &box, SbVec3f& /*center*/)
{
	double xmin = std::min( startPoint.getValue()[0], endPoint.getValue()[0] );
	double xmax = std::max( startPoint.getValue()[0], endPoint.getValue()[0] );
	double ymin = std::min( startPoint.getValue()[1], endPoint.getValue()[1] );
	double ymax = std::max( startPoint.getValue()[1], endPoint.getValue()[1] );
	double zmin = 0.0;
	double zmax = length.getValue();

	box.setBounds(SbVec3f( xmin, ymin, zmin ), SbVec3f( xmax, ymax, zmax ) );

}

void ShapeSegment::generatePrimitives(SoAction *action)
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

	const int rows = 16; // Number of points per row
    const int columns = 10; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid

    float vertex[totalPoints][6];

    int h = 0;
    double ui = 0;
	double vj = 0;

	for (int i = 0; i < rows; i++)
    {
    	ui =( 1.0 /(double)(rows-1) ) * i;

    	for ( int j = 0 ; j < columns ; j++ )
    	{

    		vj = ( 1.0 /(double)(columns-1) ) * j;
    		Point3D point = GetPoint3D(ui, vj);
    		NormalVector normal = GetNormal(ui, vj);

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
    for(int irow = 0; irow < (rows-1); irow++)
           for(int icolumn = 0; icolumn < (columns-1); icolumn++)
           {
           	indices[k] = irow*columns + icolumn;
        	indices[k+1] = indices[k] + 1;
        	indices[k+3] = indices[k] + columns;
        	indices[k+2] = indices[k+3] + 1;

        	k+=4; //Set k to the first point of the next face.
           }

    float finalvertex[totalIndices][6];
    for(int ivert = 0; ivert<totalIndices;ivert++)
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
    for( int i = 0; i < totalIndices; i++ )
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

