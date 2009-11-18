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

#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeCylinder.h"
#include "tgf.h"
#include "tgc.h"
#include "Trace.h"
#include "Vector3D.h"

using tgc::Pi;
using tgc::TwoPi;

SO_NODE_SOURCE(ShapeCylinder);

void ShapeCylinder::initClass()
{
	Trace trace( "ShapeCylinder::initClass", false );
	SO_NODE_INIT_CLASS(ShapeCylinder, TShape, "TShape");
}

ShapeCylinder::ShapeCylinder( )
{
	Trace trace( "ShapeCylinder::ShapeCylinder", false );

	SO_NODE_CONSTRUCTOR(ShapeCylinder);
	SO_NODE_ADD_FIELD( radius, (0.5) );
	SO_NODE_ADD_FIELD( length, (1.0) );
	SO_NODE_ADD_FIELD( phiMax, (TwoPi) );
	SO_NODE_DEFINE_ENUM_VALUE( reverseOrientation, INSIDE );
  	SO_NODE_DEFINE_ENUM_VALUE( reverseOrientation, OUTSIDE );
  	SO_NODE_SET_SF_ENUM_TYPE( activeSide, reverseOrientation );
	SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );
}

ShapeCylinder::~ShapeCylinder()
{
	Trace trace( "ShapeCylinder::~ShapeCylinder", false );
}

double ShapeCylinder::GetArea() const
{
	Trace trace( "ShapeCylinder::getIcon", false );

	return (2 * tgc::Pi * radius.getValue() * length.getValue() );

}

QString ShapeCylinder::getIcon()
{
	Trace trace( "ShapeCylinder::getIcon", false );
	return ":/icons/ShapeCylinder.png";
}

bool ShapeCylinder::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	Trace trace( "ShapeCylinder::Intersect", false );

	// Compute quadratic cylinder coefficients
	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A = objectRay.direction.x*objectRay.direction.x + objectRay.direction.y*objectRay.direction.y;
    double B = 2.0 * ( objectRay.direction.x* objectRay.origin.x + objectRay.direction.y * objectRay.origin.y);
	double C = objectRay.origin.x * objectRay.origin.x + objectRay.origin.y * objectRay.origin.y - radius.getValue() * radius.getValue();

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !tgf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
    double thit = ( t0 > objectRay.mint )? t0 : t1 ;
    if( thit > objectRay.maxt ) return false;

   //Compute possible cylinder hit position and $\phi
    Point3D hitPoint = objectRay( thit );
	double phi = atan2( hitPoint.y, hitPoint.x );
	if ( phi < 0. ) phi += TwoPi;

	//Evaluate Tolerance
	double tol = 0.00001;
	if( (thit - objectRay.mint) < tol ) return false;

	double zmin = 0.0;
	double zmax = length.getValue();

	// Test intersection against clipping parameters
	if( hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax.getValue() )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		hitPoint = objectRay( thit );
		phi = atan2( hitPoint.y, hitPoint.x );
		if ( hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax.getValue() ) return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function Cylinder::Intersect(...) called with null pointers" );

	// Compute definitive cylinder hit position and $\phi$
    hitPoint = objectRay( thit );
	phi = atan2( hitPoint.y, hitPoint.x );
	//if ( phi < 0. ) phi += TwoPi;

	// Find parametric representation of Cylinder hit
	double u = phi / phiMax.getValue();
	double v = hitPoint.z /length.getValue();

	// Compute cylinder \dpdu and \dpdv
	//double zradius = sqrt( hitPoint.x*hitPoint.x + hitPoint.y*hitPoint.y );
	//double invzradius = 1.0 / zradius;

	Vector3D dpdu( -phiMax.getValue() * radius.getValue() * sin ( phiMax.getValue() * u ),
						phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * u ),
						0.0 );
	Vector3D dpdv( 0.0, 0.0, length.getValue() );

	// Compute cylinder \dndu and \dndv
	Vector3D d2Pduu( -phiMax.getValue() * phiMax.getValue() * radius.getValue()
							* cos( phiMax.getValue() * u ),
						-phiMax.getValue() * phiMax.getValue() * radius.getValue()
							* sin( phiMax.getValue() * u ),
						0.0 );
	Vector3D d2Pduv( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv( 0.0, 0.0, 0.0 );

	// Compute coefficients for fundamental forms
	double E = DotProduct( dpdu, dpdu );
	double F = DotProduct( dpdu, dpdv );
	double G = DotProduct( dpdv, dpdv );
	Vector3D N;

	if( activeSide.getValue() == 0 )
		N = Normalize( NormalVector( CrossProduct( dpdv, dpdu ) ) );
	else
		N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

	if( DotProduct(N, -objectRay.direction) < 0 )	return false;

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

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeCylinder::IntersectP( const Ray& worldRay ) const
{
	Trace trace( "ShapeCylinder::IntersectP", false );

	return Intersect( worldRay, 0, 0 );
}

Point3D ShapeCylinder::Sample( double u, double v ) const
{
	Trace trace( "ShapeCylinder::Sample", false );

	return GetPoint3D( u, v );
}

bool ShapeCylinder::OutOfRange( double u, double v ) const
{
	Trace trace( "ShapeCylinder::OutOfRange", false );

	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

Point3D ShapeCylinder::GetPoint3D (double u, double v) const
{
	Trace trace( "ShapeCylinder::GetPoint3D", false );

	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function Function Poligon::GetPoint3D called with invalid parameters" );

	double phi = u * phiMax.getValue();
	double ilength = v * length.getValue();

	double x = radius.getValue()*cos (phi);
	double y = radius.getValue()*sin (phi);
	double z = ilength;

	return Point3D (x, y, z);
}

NormalVector ShapeCylinder::GetNormal (double u, double /* v */) const
{
	Trace trace( "ShapeCylinder::GetNormal", false );


	Vector3D dpdu( -phiMax.getValue() * radius.getValue() * sin ( phiMax.getValue() * u ),
					phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * u ),
					0.0 );
	Vector3D dpdv( 0.0, 0.0, length.getValue() );

	NormalVector normal;
	if( activeSide.getValue() == 0 )
		normal = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
	else
		normal = Normalize( NormalVector( CrossProduct( dpdv, dpdu ) ) );

	return normal;

}

void ShapeCylinder::computeBBox(SoAction *, SbBox3f &box, SbVec3f& /*center*/ )
{
	Trace trace( "ShapeCylinder::computeBBox", false );

	double cosPhiMax = cos( phiMax.getValue() );
	double sinPhiMax = sin( phiMax.getValue() );

	double xmin = ( phiMax.getValue() >= Pi ) ? -radius.getValue() : radius.getValue() * cosPhiMax;
	double xmax = radius.getValue();
	double ymin = 0.0;
	if( phiMax.getValue() > Pi ) ymin = ( phiMax.getValue() < 1.5*Pi ) ? radius.getValue() * sinPhiMax : -radius.getValue();
	double ymax = ( phiMax.getValue() < Pi/2.0 )? radius.getValue() * sinPhiMax : radius.getValue();

	double zmin = 0.0;
	double zmax = length.getValue();
	box.setBounds( SbVec3f( xmin, ymin, zmin ), SbVec3f( xmax, ymax, zmax ) );
}

void ShapeCylinder::generatePrimitives(SoAction *action)
{
	Trace trace( "ShapeCylinder::generatePrimitives", false );

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


	const int rows = 35; // Number of points per row
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
