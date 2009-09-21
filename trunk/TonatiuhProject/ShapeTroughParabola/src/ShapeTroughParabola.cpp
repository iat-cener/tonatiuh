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

#include <QIcon>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeTroughParabola.h"
#include "tgf.h"
#include "tgc.h"
#include "Trace.h"
#include "Vector3D.h"


SO_NODE_SOURCE(ShapeTroughParabola);

void ShapeTroughParabola::initClass()
{
	Trace trace( "ShapeTroughParabola::initClass", false );

	SO_NODE_INIT_CLASS(ShapeTroughParabola, TShape, "TShape");
}

ShapeTroughParabola::ShapeTroughParabola()
{
	Trace trace( "ShapeTroughParabola::ShapeTroughParabola", false );

	SO_NODE_CONSTRUCTOR(ShapeTroughParabola);
	SO_NODE_ADD_FIELD( focusLength, (0.125));
	SO_NODE_ADD_FIELD( length, (1.0));
	SO_NODE_ADD_FIELD( width, (1.0));
    SO_NODE_DEFINE_ENUM_VALUE( reverseOrientation, INSIDE);
    SO_NODE_DEFINE_ENUM_VALUE( reverseOrientation, OUTSIDE);
    SO_NODE_SET_SF_ENUM_TYPE( orientation, reverseOrientation );
    SO_NODE_ADD_FIELD( orientation, (INSIDE) );

}

ShapeTroughParabola::~ShapeTroughParabola()
{
	Trace trace( "ShapeTroughParabola::~ShapeTroughParabola", false );
}


double ShapeTroughParabola::GetArea() const
{
	Trace trace( "ShapeTroughParabola::GetArea", false );
	return -1;
}

QString ShapeTroughParabola::getIcon()
{
	Trace trace( "ShapeTroughParabola::getIcon", false );
	return ":/icons/ShapeTroughParabola.png";
}

bool ShapeTroughParabola::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	Trace trace( "ShapeTroughParabola::Intersect", false );

	// Compute quadratic parabolic cylinder coefficients
	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A = objectRay.direction.x*objectRay.direction.x;
    double B = 2.0 * ( objectRay.direction.x* objectRay.origin.x - 2 * focusLength.getValue() * objectRay.direction.y);
	double C = objectRay.origin.x * objectRay.origin.x - 4 * focusLength.getValue() * objectRay.origin.y;

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !tgf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
    double thit = ( t0 > objectRay.mint )? t0 : t1 ;
    if( thit > objectRay.maxt ) return false;

	//Evaluate Tolerance
	double tol = 0.00001;
	if( (thit - objectRay.mint) < tol ) return false;

	// Compute parabolic cylinder hit position
    Point3D hitPoint = objectRay( thit );

	// Test intersection against clipping parameters
	double xmin = -width.getValue() / 2;
	double xmax = width.getValue() / 2;

	double ymin = 0.0;
	double ymax = ( (width.getValue() / 2) * (width.getValue() / 2) ) / ( 4 * focusLength.getValue() );

	if( hitPoint.z < 0.0 ||
		hitPoint.z > length.getValue() ||
		hitPoint.y > ymax || hitPoint.y < ymin ||
		hitPoint.x > xmax || hitPoint.x < xmin )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		// Compute parabolic cylinder hit position
		hitPoint = objectRay( thit );

		if( hitPoint.z < 0.0 ||
			hitPoint.z > length.getValue() ||
			hitPoint.y > ymax || hitPoint.y < ymin ||
			hitPoint.x > xmax || hitPoint.x < xmin)
			return false;
	}

	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function ParabolicCyl::Intersect(...) called with null pointers" );

	// Compute definitive parabolic cylinder hit position
	hitPoint = objectRay( thit );
	double x = hitPoint.x;
	double y = hitPoint.y;

	// Find parametric representation of paraboloid hit
	double u =  x  / focusLength.getValue();
	double v = y / length.getValue();

	// Compute parabaloid \dpdu and \dpdv
	Vector3D dpdu(1.0, x /( 2.0 * focusLength.getValue() ), 0.0);
	Vector3D dpdv(0.0, 0.0, 1.0);

	// Compute parabaloid \dndu and \dndv
	Vector3D d2Pduu ( 0.0, 1.0/( 2.0* focusLength.getValue() ), 0.0 );
	Vector3D d2Pduv ( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv ( 0.0, 0.0, 0.0 );

	// Compute coefficients for fundamental forms
	double E = DotProduct(dpdu, dpdu);
	double F = DotProduct(dpdu, dpdv);
	double G = DotProduct(dpdv, dpdv);

	Vector3D N;
	if( orientation.getValue() == 1 )
	{
		N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
	}
	else
	{
		N = Normalize( NormalVector( CrossProduct( dpdv, dpdu ) ) );
	}
	if( DotProduct(N, objectRay.direction) > 0 )
	{
		return false;
	}
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
	*dg = DifferentialGeometry(hitPoint, dpdu, dpdv, dndu, dndv, u, v, this);

	// Update _tHit_ for quadric intersection
	*tHit = thit;
	return true;
}

bool ShapeTroughParabola::IntersectP( const Ray& objectRay ) const
{
	Trace trace( "ShapeTroughParabola::IntersectP", false );

	return Intersect( objectRay, 0, 0 );
}

Point3D ShapeTroughParabola::Sample( double u, double v ) const
{
	Trace trace( "ShapeTroughParabola::Sample", false );

	return GetPoint3D( u, v );
}

Point3D ShapeTroughParabola::GetPoint3D( double u, double v ) const
{
	Trace trace( "ShapeTroughParabola::GetPoint3D", false );

	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );

		double x = u * width.getValue() - width.getValue() / 2;
	double y = ( x * x ) /( 4 * focusLength.getValue() );
	double z = v * length.getValue();

	return Point3D (x, y, z);

}

NormalVector ShapeTroughParabola::GetNormal (double u ,double v) const
{
	Trace trace( "ShapeTroughParabola::GetNormal", false );

	Point3D point = GetPoint3D( u, v );
	Vector3D r( 0, 1, 0 );
	Vector3D v1 = Normalize( Vector3D( -point.x, focusLength.getValue() - point.y, 0 ) );
	NormalVector normal  = Normalize( NormalVector( r + v1 ) );

	return NormalVector( normal.x , normal.y , 0 );

}

bool ShapeTroughParabola::OutOfRange( double u, double v ) const
{
	Trace trace( "ShapeTroughParabola::OutOfRange", false );
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeTroughParabola::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
	Trace trace( "ShapeTroughParabola::computeBBox", false );

	double xmin = -width.getValue() / 2;
	double xmax = width.getValue() / 2;

	double ymin = 0.0;
	double ymax = ( (width.getValue() / 2) * (width.getValue() / 2) ) / ( 4 * focusLength.getValue() );

	double zmin = 0.0;
	double zmax = length.getValue();
	box.setBounds(SbVec3f( xmin, ymin, zmin ), SbVec3f( xmax, ymax, zmax ) );
}

void ShapeTroughParabola::generatePrimitives(SoAction *action)
{
	Trace trace( "ShapeTroughParabola::generatePrimitives", false );

    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	const int rows = 20; // Number of points per row
    const int columns = 20; // Number of points per column
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

	beginShape(action, QUADS);
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
