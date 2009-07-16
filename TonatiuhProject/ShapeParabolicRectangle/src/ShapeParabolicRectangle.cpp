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
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeParabolicRectangle.h"
#include "tgf.h"
#include "Trace.h"
#include "Vector3D.h"


SO_NODE_SOURCE(ShapeParabolicRectangle);

void ShapeParabolicRectangle::initClass()
{
	Trace trace( "ShapeParabolicRectangle::initClass", false );
	SO_NODE_INIT_CLASS(ShapeParabolicRectangle, TShape, "TShape");
}

ShapeParabolicRectangle::ShapeParabolicRectangle()
{
	Trace trace( "ShapeParabolicRectangle::ShapeParabolicRectangle", false );

	SO_NODE_CONSTRUCTOR(ShapeParabolicRectangle);
	SO_NODE_ADD_FIELD(m_xlength, (2.0));
	SO_NODE_ADD_FIELD(m_ylength, (2.0));
	SO_NODE_ADD_FIELD(m_depth, (1.0));
}

ShapeParabolicRectangle::~ShapeParabolicRectangle()
{
	Trace trace( "ShapeParabolicRectangle::~ShapeParabolicRectangle", false );
}

double ShapeParabolicRectangle::GetArea() const
{
	Trace trace( "ShapeParabolicRectangle::GetArea", false );
	return -1;
}

QString ShapeParabolicRectangle::getIcon()
{
	Trace trace( "ShapeParabolicRectangle::getIcon", false );
	return ":/icons/ShapeParabolicRectangle.png";
}

bool ShapeParabolicRectangle::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	Trace trace( "ShapeParabolicRectangle::Intersect", false );

	// Compute quadratic coefficients for the 1st parabola XZ
	double xmin = -m_xlength.getValue()/2;
	double xmax = m_xlength.getValue()/2;
	double xFocus = ( xmax * xmax ) / ( 4 * m_depth.getValue() );

	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A1 = objectRay.direction.x*objectRay.direction.x;
	double B1 = 2.0 * ( objectRay.direction.x* objectRay.origin.x - 2 * xFocus * objectRay.direction.z);
	double C1 = objectRay.origin.x * objectRay.origin.x - 4 * xFocus* objectRay.origin.z;

	// Compute quadratic coefficients for the 2nd parabola YZ
	double ymin = -m_ylength.getValue()/2;
	double ymax = m_ylength.getValue()/2;
	double yFocus = ( ymax * ymax ) / ( 4 * m_depth.getValue() );


	double A2 = objectRay.direction.y*objectRay.direction.y;
	double B2 = 2.0 * ( objectRay.direction.y* objectRay.origin.y - 2 * yFocus * objectRay.direction.z);
	double C2 = objectRay.origin.y * objectRay.origin.y - 4 * yFocus * (objectRay.origin.z);

	double t10 = objectRay.mint -1.0, t11 = objectRay.mint -1.0;
	double t20 = objectRay.mint -1.0, t21 = objectRay.mint -1.0;
	bool firstIntersected = tgf::Quadratic( A1, B1, C1, &t10, &t11 );
	bool secondIntersected = tgf::Quadratic( A2, B2, C2, &t20, &t21 );
	if( !firstIntersected && !secondIntersected ) return false;

	// Compute intersection distance along ray for the 1st parabolic cylinder XZ
	double thit = objectRay.mint;
	bool intersectWithFirst = true;
	if( t10 > objectRay.maxt || t11 < objectRay.mint )
	{
		if ( t20 > objectRay.maxt || t21 < objectRay.mint ) return false;
		else
		{
			intersectWithFirst = false;
			thit = ( t20 > objectRay.mint )? t20 : t21 ;
		}
	}
	else
	{
		double thit1 = ( t10 > objectRay.mint )? t10 : t11 ;
		if ( t20 > objectRay.maxt || t21 < objectRay.mint )	thit = thit1 ;

		else
		{
			double thit2 = ( t20 > objectRay.mint )? t20 : t21 ;

			thit = ( thit1 <= thit2 )? thit1: thit2;

			intersectWithFirst = ( thit1 <= thit2 )? true: false;
		}

	}

    if( thit > objectRay.maxt ) return false;

    //Evaluate Tolerance
    double tol = 0.00001;
    if( (thit - objectRay.mint) < tol ) return false;

	// Compute possible parabolic cylinder hit position for the 1st parabolic cylinder XZ
    Point3D hitPoint = objectRay( thit );

    if( hitPoint.x < xmin || hitPoint.x > xmax || hitPoint.y > ymax || hitPoint.y < ymin ) return false;
    if( hitPoint.z < 0.0 || hitPoint.z > m_depth.getValue() ) return false;

    // Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function ParabolicCyl::Intersect(...) called with null pointers" );

///////////////////////////////////////////////////////////////////////////////////////

	// Compute possible parabola hit position

	double x = hitPoint.x;
	double y = hitPoint.y;


	// Find parametric representation of paraboloid hit
	double u = (x-xmin) / (xmax-xmin);
	double v = (y-ymin) / (ymax-ymin);

	// Compute parabaloid \dpdu and \dpdv

	Vector3D dpdu = (intersectWithFirst)? Vector3D( 1.0, 0.0, x/(2.0 *xFocus) )
					: Vector3D( 1.0, 0.0, 0.0 );
	Vector3D dpdv = (intersectWithFirst)?
					Vector3D( 0.0, 1.0, 0.0)
					: Vector3D( 0.0, 1.0, y/(2.0 * yFocus ) );

	// Compute parabaloid \dndu and \dndv
	Vector3D d2Pduu = (intersectWithFirst)?
					Vector3D( 0.0, 0.0, 1.0 /(2.0 *xFocus ) )
					: Vector3D( 0.0, 0.0, 0.0 );
	Vector3D d2Pduv ( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv = (intersectWithFirst)?
					Vector3D( 0.0, 0.0, 0.0)
					: Vector3D( 0.0, 0.0, 1.0/ (2.0 * yFocus ) );

	// Compute coefficients for fundamental forms
	double E = DotProduct(dpdu, dpdu);
	double F = DotProduct(dpdu, dpdv);
	double G = DotProduct(dpdv, dpdv);
	Vector3D N = Normalize( NormalVector( CrossProduct(dpdu, dpdv) ) );
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


///////////////////////////////////////////////////////////////////////////////////////

	// Update _tHit_ for quadric intersection
	*tHit = thit;
	return true;
}

bool ShapeParabolicRectangle::IntersectP( const Ray& objectRay ) const
{
	Trace trace( "ShapeParabolicRectangle::IntersectP", false );
	return Intersect( objectRay, 0, 0 );
}

Point3D ShapeParabolicRectangle::Sample( double u, double v ) const
{
	Trace trace( "ShapeParabolicRectangle::Sample", false );
	return GetPoint3D( u, v );
}

bool ShapeParabolicRectangle::OutOfRange( double u, double v ) const
{
	Trace trace( "ShapeParabolicRectangle::OutOfRange", false );
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

Point3D ShapeParabolicRectangle::GetPoint3D( double u, double v ) const
{
	Trace trace( "ShapeParabolicRectangle::GetPoint3D", false );

	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );

	double xmin = -m_xlength.getValue()/2;
	double xmax = m_xlength.getValue()/2;
	double xFocus = ( xmax * xmax ) / ( 4 * m_depth.getValue() );


	double ymin = -m_ylength.getValue()/2;
	double ymax = m_ylength.getValue()/2;
	double yFocus = ( ymax * ymax ) / ( 4 * m_depth.getValue() );

	double xpos = u * ( xmax - xmin ) + xmin;
	double ypos = v * ( ymax - ymin ) + ymin;

	double x = xpos;
	double y = ypos;
	double z = std::max( xpos*xpos /(4* xFocus), ypos*ypos /( 4*yFocus ) );

	return Point3D (x, y, z);

}

NormalVector ShapeParabolicRectangle::GetNormal( double u, double v ) const
{
	Trace trace( "ShapeParabolicRectangle::GetNormal", false );

	Point3D point = GetPoint3D( u, v );
	return NormalVector( -point.x/2, 1.0, 0.0 );
}
void ShapeParabolicRectangle::computeBBox( SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
	Trace trace( "ShapeParabolicRectangle::computeBBox", false );

	double xmin = -m_xlength.getValue()/2;
	double xmax = m_xlength.getValue()/2;

	double ymin = -m_ylength.getValue()/2;
	double ymax = m_ylength.getValue()/2;

	double zmin = 0.0;
	double zmax = m_depth.getValue();

	box.setBounds(SbVec3f( xmin, ymin, zmin ), SbVec3f( xmax, ymax, zmax ));
}

void ShapeParabolicRectangle::generatePrimitives(SoAction *action)
{
	Trace trace( "ShapeParabolicRectangle::generatePrimitives", false );

    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);


    SbVec3f  point;
 	const int rows = 100; // Number of points per row
    const int columns = 100; // Number of points per column
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
