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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>

#include <QIcon>
#include <QObject>
#include <QMessageBox>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "BBox.h"
#include "gf.h"
#include "Ray.h"
#include "Vector3D.h"


#include "DifferentialGeometry.h"
#include "ShapeTroughParabola.h"



SO_NODE_SOURCE(ShapeTroughParabola);

void ShapeTroughParabola::initClass()
{
	SO_NODE_INIT_CLASS(ShapeTroughParabola, TShape, "TShape");
}

ShapeTroughParabola::ShapeTroughParabola()
{
	SO_NODE_CONSTRUCTOR(ShapeTroughParabola);
	SO_NODE_ADD_FIELD( focusLength, (0.125) );
	SO_NODE_ADD_FIELD( xMin, (-0.5) );
	SO_NODE_ADD_FIELD( xMax, (0.5) );
	SO_NODE_ADD_FIELD( lengthXMin, (1.0) );
	SO_NODE_ADD_FIELD( lengthXMax, (1.0) );

	SO_NODE_DEFINE_ENUM_VALUE( Side, INSIDE );
	SO_NODE_DEFINE_ENUM_VALUE( Side, OUTSIDE );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
	SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );


	SoFieldSensor* m_xMinSensor = new SoFieldSensor(updateXMinValues, this);
	m_xMinSensor->setPriority( 1 );
	m_xMinSensor->attach( &xMin );
	SoFieldSensor* m_xMaxSensor = new SoFieldSensor(updateXMaxValues, this);
	m_xMaxSensor->setPriority( 1 );
	m_xMaxSensor->attach( &xMax );

}

ShapeTroughParabola::~ShapeTroughParabola()
{
}


double ShapeTroughParabola::GetArea() const
{
	return -1;
}

BBox ShapeTroughParabola::GetBBox() const
{
	double xmin = xMin.getValue();
	double xmax = xMax.getValue();

	double y1 = ( xMin.getValue() * xMin.getValue() ) / ( 4 * focusLength.getValue() );
	double y2 = ( xMax.getValue() * xMax.getValue() ) / ( 4 * focusLength.getValue() );

	double ymin = 0.0;
	if( ( xMin.getValue() * xMax.getValue() ) > 0 ) ymin = std::min( y1, y2 );
	double ymax = std::max( y1, y2 );

	double zmin = 0.0;
	double zmax = std::max( lengthXMin.getValue(), lengthXMax.getValue() );
	return BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) );
}

QString ShapeTroughParabola::GetIcon() const
{
	return QLatin1String( ":/icons/ShapeTroughParabola.png" );
}

bool ShapeTroughParabola::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	// Compute quadratic parabolic cylinder coefficients
	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A = objectRay.direction().x*objectRay.direction().x;
    double B = 2.0 * ( objectRay.direction().x* objectRay.origin.x - 2 * focusLength.getValue() * objectRay.direction().y);
	double C = objectRay.origin.x * objectRay.origin.x - 4 * focusLength.getValue() * objectRay.origin.y;

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
    double thit = ( t0 > objectRay.mint )? t0 : t1 ;
    if( thit > objectRay.maxt ) return false;

	//Evaluate Tolerance
	double tol = 0.00001;
	//if( (thit - objectRay.mint) < tol ) return false;

	// Compute parabolic cylinder hit position
    Point3D hitPoint = objectRay( thit );

	// Test intersection against clipping parameters
	double xmin = xMin.getValue();
	double xmax = xMax.getValue();

	double zmax = std::max( lengthXMin.getValue(), lengthXMax.getValue() );
	double z1 = ( ( zmax - lengthXMin.getValue() ) / 2 ) + ( (lengthXMin.getValue() - lengthXMax.getValue() ) / ( 2 * ( xmax - xmin ) ) ) * ( hitPoint.x - xmin );
	double z2 = ( ( zmax + lengthXMin.getValue() ) / 2 )  + ( ( (lengthXMax.getValue() - lengthXMin.getValue() ) / ( 2 * ( xmax - xmin ) ) )  * ( hitPoint.x - xmin ) );

	double y1 = ( xmin * xmin ) / ( 4 * focusLength.getValue() );
	double y2 = ( xmax * xmax ) / ( 4 * focusLength.getValue() );

	double ymin = 0.0;
	if( ( xmin * xmax ) > 0 ) ymin = std::min( y1, y2 );
	double ymax = std::max( y1, y2 );

	if( ( thit - objectRay.mint) < tol ||
		hitPoint.z < z1 ||
		hitPoint.z > z2 ||
		hitPoint.y > ymax || hitPoint.y < ymin ||
		hitPoint.x > xmax || hitPoint.x < xmin )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		// Compute parabolic cylinder hit position
		hitPoint = objectRay( thit );

		z1 = ( ( zmax - lengthXMin.getValue() ) / 2 ) + ( (lengthXMin.getValue() - lengthXMax.getValue() ) / ( 2 * ( xmax - xmin ) ) ) * ( hitPoint.x - xmin );
		z2 = ( ( zmax + lengthXMin.getValue() ) / 2 )  + ( ( (lengthXMax.getValue() - lengthXMin.getValue() ) / ( 2 * ( xmax - xmin ) ) )  * ( hitPoint.x - xmin ) );

		if( ( thit - objectRay.mint) < tol ||
			hitPoint.z < z1 ||
			hitPoint.z > z2 ||
			hitPoint.y > ymax || hitPoint.y < ymin ||
			hitPoint.x > xmax || hitPoint.x < xmin)
			return false;
	}

	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) )	gf::SevereError( "Function ParabolicCyl::Intersect(...) called with null pointers" );

	// Compute definitive parabolic cylinder hit position
	hitPoint = objectRay( thit );

	// Find parametric representation of paraboloid hit
	double u =  hitPoint.x  / focusLength.getValue();

	z1 = ( ( zmax - lengthXMin.getValue() ) / 2 ) + ( (lengthXMin.getValue() - lengthXMax.getValue() ) / ( 2 * ( xmax - xmin ) ) ) * ( hitPoint.x - xmin );
	z2 = ( ( zmax + lengthXMin.getValue() ) / 2 )  + ( ( (lengthXMax.getValue() - lengthXMin.getValue() ) / ( 2 * ( xmax - xmin ) ) )  * ( hitPoint.x - xmin ) );

	double v = ( hitPoint.z - z1 ) / (z2 - z1);

	// Compute parabaloid \dpdu and \dpdv
	Vector3D dpdu(1.0, hitPoint.x /( 2.0 * focusLength.getValue() ), 0.0);
	Vector3D dpdv(0.0, 0.0, 1.0);

	// Compute parabaloid \dndu and \dndv
	Vector3D d2Pduu ( 0.0, 1.0/( 2.0* focusLength.getValue() ), 0.0 );
	Vector3D d2Pduv ( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv ( 0.0, 0.0, 0.0 );

	// Compute coefficients for fundamental forms
	double E = DotProduct(dpdu, dpdu);
	double F = DotProduct(dpdu, dpdv);
	double G = DotProduct(dpdv, dpdv);

	Vector3D N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

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
	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

	// Update _tHit_ for quadric intersection
	*tHit = thit;

	return true;
}

bool ShapeTroughParabola::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}

Point3D ShapeTroughParabola::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

void ShapeTroughParabola::updateXMinValues( void *data, SoSensor *)
{
	ShapeTroughParabola* shapeTroughParabola = (ShapeTroughParabola *) data;
	if( shapeTroughParabola->xMax.getValue() < shapeTroughParabola->xMin.getValue() )
	{
		QMessageBox::warning( 0, QLatin1String( "Tonatiuh" ),
				QObject::tr( "xMin must be smaller than xMax. ") );
		shapeTroughParabola->xMin.setValue( shapeTroughParabola->xMax.getValue() );
	}
}

void ShapeTroughParabola::updateXMaxValues( void *data, SoSensor *)
{
	ShapeTroughParabola* shapeTroughParabola = (ShapeTroughParabola *) data;
	if( shapeTroughParabola->xMax.getValue() < shapeTroughParabola->xMin.getValue() )
	{
		QMessageBox::warning( 0, QLatin1String( "Tonatiuh" ),
				QObject::tr( "xMax must be larger than xMin. ") );
		shapeTroughParabola->xMax.setValue( shapeTroughParabola->xMin.getValue()  );
	}
}

Point3D ShapeTroughParabola::GetPoint3D( double u, double v ) const
{
	if ( OutOfRange( u, v ) )	gf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );

	double xmin = xMin.getValue();
	double xmax = xMax.getValue();

	double x = ( xmax - xmin ) *  u + xmin;
	double y = ( x * x ) /( 4 * focusLength.getValue() );

	/*double n = 6;
	double lM = tan( tgc::Pi / n )* xmax;
	double a = ( ( xmax * xmax ) /( 4 * focusLength.getValue() ) / lM);

	double zmax = y / a;
	double z = zmax * ( 2 * v - 1 );*/
	double zmax = std::max( lengthXMin.getValue(), lengthXMax.getValue() );
	double z1 = ( ( zmax - lengthXMin.getValue() ) / 2 ) + ( (lengthXMin.getValue() - lengthXMax.getValue() ) / ( 2 * ( xmax - xmin ) ) ) * ( x - xmin );
	double z2 = ( ( zmax + lengthXMin.getValue() ) / 2 )  + ( ( (lengthXMax.getValue() - lengthXMin.getValue() ) / ( 2 * ( xmax - xmin ) ) )  * ( x - xmin ) );
	double z = ( z2 - z1 ) * v +z1;

	return Point3D (x, y, z);

}

NormalVector ShapeTroughParabola::GetNormal (double u ,double v) const
{
	Point3D point = GetPoint3D( u, v );
	Vector3D dpdu(1.0, point.x /( 2.0 * focusLength.getValue() ), 0.0);
	Vector3D dpdv(0.0, 0.0, 1.0);

	return Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

}

bool ShapeTroughParabola::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeTroughParabola::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
	SbVec3f min, max;

	min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
	max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );

	// Set the box to bound the two extreme points.
	box.setBounds(min, max);
}

void ShapeTroughParabola::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	const int rows = 20; // Number of points per row
    const int columns = 5; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid

    float vertex[totalPoints][6];

    int h = 0;
    double ui = 0;
	double vj = 0;

    for ( int i = 0; i < rows; ++i )
    {
    	ui =( 1.0 /(double)(rows-1) ) * i;

    	for ( int j = 0 ; j < columns ; ++j )
    	{

    		vj = ( 1.0 /(double)(columns-1) ) * j;

    		Point3D point = GetPoint3D(ui, vj);
    		NormalVector normal;
    		if( activeSide.getValue() == 0 )	normal = GetNormal(ui, vj);
    		else	normal = -GetNormal(ui, vj);

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
			//indices[k] = irow*columns + icolumn;
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
