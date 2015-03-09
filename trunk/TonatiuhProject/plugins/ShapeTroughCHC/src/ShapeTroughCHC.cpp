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

#include <algorithm>
#include <vector>

#include <QIcon>
#include <QMap>
#include <QMessageBox>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/sensors/SoFieldSensor.h>


#include "BBox.h"
#include "gc.h"
#include "gf.h"
#include "Ray.h"
#include "Transform.h"
#include "Vector3D.h"

#include "DifferentialGeometry.h"
#include "ShapeTroughCHC.h"


double fPart( double alpha, double x, double r1, double theta, double r, double eccentricity )
{
	return -r1 + sin( alpha - theta ) * ( r/ ( 1 - eccentricity * cos( alpha ) ) ) - x ;
}

SO_NODE_SOURCE(ShapeTroughCHC);

void ShapeTroughCHC::initClass()
{
	SO_NODE_INIT_CLASS(ShapeTroughCHC, TShape, "TShape");
}

ShapeTroughCHC::ShapeTroughCHC()
{
	SO_NODE_CONSTRUCTOR(ShapeTroughCHC);
	SO_NODE_ADD_FIELD( r1, (0.2) );
	SO_NODE_ADD_FIELD( p1, (0.5) );
	SO_NODE_ADD_FIELD(lengthX1, (1.0) );
	SO_NODE_ADD_FIELD(lengthX2, (1.0) );
	SO_NODE_ADD_FIELD( height, (1.0) );

	SetInternalValues();

	SoFieldSensor* m_r1Sensor = new SoFieldSensor(updateInternalValues, this);
	m_r1Sensor->setPriority( 1 );
	m_r1Sensor->attach( &r1 );
	SoFieldSensor* m_p1Sensor = new SoFieldSensor(updateInternalValues, this);
	m_p1Sensor->setPriority( 1 );
	m_p1Sensor->attach( &p1 );
	SoFieldSensor* m_heightSensor = new SoFieldSensor(updateInternalValues, this);
	m_heightSensor->setPriority( 1 );
	m_heightSensor->attach( &height );


}

ShapeTroughCHC::~ShapeTroughCHC()
{
}


double ShapeTroughCHC::GetArea() const
{
	return -1;
}

BBox ShapeTroughCHC::GetBBox() const
{
	double xMin = r1.getValue();
	double xMax = p1.getValue();
	double yMin = 0.0;
	double yMax = height.getValue();

	double maxlength = std::max( lengthX1.getValue(), lengthX2.getValue() );
	double zMin = - maxlength /2;
	double zMax = maxlength /2;
	return BBox( Point3D( xMin, yMin, zMin ), Point3D( xMax, yMax, zMax ) );
}

QString ShapeTroughCHC::GetIcon() const
{
	return ":/icons/ShapeTroughCHC.png";
}

bool ShapeTroughCHC::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	double a = ( m_s - height.getValue() )/( cos( m_theta ) * 2 *  m_eccentricity);
	double b = sqrt( ( m_eccentricity * m_eccentricity - 1 ) *  a * a );

	double angle = -( 0.5 * gc::Pi ) + m_theta;

	Transform hTransform( cos( angle ), -sin( angle ), 0.0, -r1.getValue() + a * m_eccentricity * sin( m_theta ),
			sin( angle ), cos( angle ), 0.0, - a * m_eccentricity * cos( m_theta ),
	           0.0, 0.0, 1.0, 0.0,
	           0.0, 0.0, 0.0, 1.0 );


	Ray transformedRay = hTransform.GetInverse()( objectRay );

	double A =   ( transformedRay.direction().x * transformedRay.direction().x ) / ( a * a )
			   - ( transformedRay.direction().y * transformedRay.direction().y ) / ( b * b );

    double B = 2.0 * ( ( ( transformedRay.origin.x * transformedRay.direction().x ) / ( a * a ) )
    				- ( ( transformedRay.origin.y * transformedRay.direction().y ) / ( b * b ) ) );

	double C = ( ( transformedRay.origin.x * transformedRay.origin.x ) / ( a * a ) )
				- ( ( transformedRay.origin.y * transformedRay.origin.y ) / ( b * b ) )
				- 1;

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double thit = ( t0 > objectRay.mint )? t0 : t1 ;
	if( thit > objectRay.maxt ) return false;

	//Evaluate Tolerance
	double tol = 0.00001;

	// Compute ShapeTroughCHC hit position and $\phi$
	Point3D hitPoint = objectRay( thit );

	// Test intersection against clipping parameters
	double m =  ( lengthX2.getValue() / 2- lengthX1.getValue() / 2 ) / ( p1.getValue() - r1.getValue() );
	double zmax = ( lengthX1.getValue()  / 2 ) + m * ( hitPoint.x - r1.getValue() );
	double zmin = - zmax;

	// Test intersection against clipping parameters
	if( (thit - objectRay.mint) < tol
			|| hitPoint.x < r1.getValue() || hitPoint.x > p1.getValue()
			|| hitPoint.y < 0.0 || hitPoint.y > height.getValue()
			|| hitPoint.z < zmin ||  hitPoint.z > zmax )
	{

		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		// Compute ShapeSphere hit position and $\phi$
		hitPoint = objectRay( thit );

		zmax = ( lengthX1.getValue()  / 2 ) + m * ( hitPoint.x - r1.getValue() );
		zmin = - zmax;

		if( (thit - objectRay.mint) < tol
					|| hitPoint.x < r1.getValue() || hitPoint.x > p1.getValue()
					|| hitPoint.y < 0.0 || hitPoint.y > height.getValue()
					|| hitPoint.z < zmin ||  hitPoint.z > zmax )	return false;
	}

	// Find parametric representation of CHC concentrator hit

	double sup = m_theta + 0.5* gc::Pi;
	double inf = m_theta + m_phi;

	double alpha;
	bool isAlpha = findRoot( fPart, hitPoint.x,  m_eccentricity,  r1.getValue(), m_theta, inf, sup, 0.5*( sup-inf), 500, &alpha );
	if( !isAlpha )return false;
	double u = ( alpha - inf ) / ( sup - inf );

	zmax = (lengthX1.getValue() / 2 ) + m* ( hitPoint.x - r1.getValue() );
	double v = ( ( hitPoint.z / zmax ) + 1 )/ 2;


	// Compute  \dpdu and \dpdv
	Vector3D dpdu = GetDPDU( u, v );
	Vector3D dpdv = GetDPDV( u, v );

	// Compute cylinder \dndu and \dndv

	//Not yet implemented
	Vector3D d2Pduu( 0.0, 0.0, 0.0 );

	Vector3D d2Pduv( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv( 0.0, 0.0, 0.0 );

	// Compute coefficients for fundamental forms
	double E = DotProduct( dpdu, dpdu );
	double F = DotProduct( dpdu, dpdv );
	double G = DotProduct( dpdv, dpdv );
	Vector3D N = Normalize( CrossProduct( dpdu, dpdv ) );

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

    dg->shapeFrontSide = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

	return true;

}

bool ShapeTroughCHC::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}

Point3D ShapeTroughCHC::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

void ShapeTroughCHC::updateInternalValues( void *data, SoSensor *)
{
	ShapeTroughCHC* shapeTroughCHC = (ShapeTroughCHC *) data;
	shapeTroughCHC->SetInternalValues();
}

Point3D ShapeTroughCHC::GetPoint3D( double u, double v ) const
{
	if ( OutOfRange( u, v ) )	gf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );

	double alpha = u * ( ( m_theta + 0.5* gc::Pi ) - ( m_theta + m_phi ) ) + ( m_theta + m_phi );

	double r  =  2 * r1.getValue() *( 1 - m_eccentricity * cos( m_theta + 0.5* gc::Pi ) );
	double radius  =  r / ( 1 - m_eccentricity * cos ( alpha )  );

	double x = - r1.getValue() + sin( alpha - m_theta ) * radius;
	double y = cos( alpha - m_theta ) * radius;


	double m =  ( ( lengthX2.getValue()/ 2 )- ( lengthX1.getValue()/ 2 ) ) / ( p1.getValue() - r1.getValue() );
	double zMax = ( lengthX1.getValue()/ 2 ) + m * ( x - r1.getValue() );

	double z = zMax * ( 2 * v -1 );

	return Point3D (x, y, z);

}

NormalVector ShapeTroughCHC::GetNormal (double u ,double v) const
{
	Vector3D dpdu = GetDPDU( u, v );
	Vector3D dpdv = GetDPDV( u, v );

	return Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

}

bool ShapeTroughCHC::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeTroughCHC::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
	SbVec3f min, max;

	min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
	max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );

	// Set the box to bound the two extreme points.
	box.setBounds(min, max);
}

void ShapeTroughCHC::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	const int rows = 20; // Number of points per row
    const int columns = 3; // Number of points per column
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

Vector3D ShapeTroughCHC::GetDPDU( double u, double v ) const
{
	double alphaMax = m_theta + 0.5* gc::Pi;
	double alphaMin = m_theta + m_phi;
	double alpha = u * ( alphaMax - alphaMin ) + alphaMin;

	double r  =  2 * r1.getValue() *( 1 - m_eccentricity * cos( m_theta + 0.5* gc::Pi ) );

	double x = ( ( alphaMax - alphaMin) * r * cos( alpha - m_theta )
					/ (1 - m_eccentricity * cos( alpha ) ) )
			- ( ( ( alphaMax - alphaMin ) * m_eccentricity * r * sin( alpha ) *sin( alpha - m_theta ) )
				 / ( (1 - m_eccentricity * cos( alpha ) ) * (1 - m_eccentricity * cos( alpha ) ) ) );

	double y = - ( ( (alphaMax - alphaMin)* m_eccentricity * r * cos( alpha - m_theta ) * sin( alpha ) )
					/ ( (1 - m_eccentricity * cos( alpha ) ) * (1 - m_eccentricity * cos( alpha ) ) ) )
				- ( ( ( alphaMax - alphaMin) * r * sin( alpha - m_theta ) )
					/ ( 1 - m_eccentricity * cos( alpha ) ) );
	double z = 	( 1 / ( p1.getValue() - r1.getValue() ) ) * ( 0.5 * lengthX2.getValue() - 0.5 * lengthX1.getValue() ) * ( -1 + 2 *  v)
				* ( ( ( ( alphaMax - alphaMin ) * r * cos( alpha - m_theta ) )
						/ (1 - m_eccentricity * cos( alpha ) ) )
					- ( ( ( alphaMax - alphaMin ) * m_eccentricity * r * sin( alpha ) * sin ( alpha - m_theta ) )
			     	 	 /( ( 1 - m_eccentricity * cos( alpha ) ) * (1 - m_eccentricity * cos( alpha ) ) ) ) );

	return Vector3D( x, y, z );
}


Vector3D ShapeTroughCHC::GetDPDV( double u, double /* v */ ) const
{
   double alphaMax = m_theta + 0.5 * gc::Pi;
   double alphaMin = m_theta + m_phi;
   double alpha = u * ( alphaMax - alphaMin ) + alphaMin;

   double r  =  2 * r1.getValue() *( 1 - m_eccentricity * cos( m_theta + 0.5* gc::Pi ) );

   double x = 0.0;
   double y = 0.0;
   double z =  2* ( 0.5 * lengthX1.getValue()
                    + (
                         (
                              ( 0.5 * lengthX2.getValue() - 0.5 * lengthX1.getValue() )
                            * ( -2 * r1.getValue() +  ( ( r * sin( alpha - m_theta ) ) / ( 1 - m_eccentricity * cos( alpha ) ) ) )
                         ) / ( p1.getValue() - r1.getValue() )
                      )
                   );

   return Vector3D( x, y, z );
}

bool ShapeTroughCHC::findRoot( double (*funcion)( double, double, double, double, double, double ), double xCoord, double e, double r1, double theta, double a, double b, double y0, int max, double* alpha )
{

	double r  =  2 * r1 *( 1 - e * cos( theta + 0.5* gc::Pi ) );


	double y1 = funcion( a, xCoord, r1, theta, r, e );
	double y2 =funcion( b, xCoord, r1, theta, r, e );

	double delta = fabs( a - b ) / 100;
	while( ( ( y1 * y2 ) > 0.0 ) && ( a < b ) )
	{
		a += delta;
		y1 = funcion(  a, xCoord, r1, theta, r, e );
		//funcion( a, r, thetaI, ray  );
	}

	if( ( ( y1 * y2 ) > 0.0 ) || ( a > b ) ) return false;

	int iterations = 0;
	while( ( fabs( a - b ) > 0.000000000001 ) && ( iterations < max ) )
	{
		double x = ( a + b ) / 2.0 ;
		//double y = funcion( x, r, thetaI, ray );
		double y = funcion( x, xCoord, r1, theta, r, e );

		if( y == y0 )
		{
			*alpha = x;
			return true;
		}

		if( ( y > y0 ) && ( y2 > y1 ) ) 	b= x;
		else if( ( y > y0 ) && ( y2 < y1 ) ) 		a = x;
		else if( ( y < y0 ) && ( y2 > y1 ) ) 		a= x;
		else	b = x;
		iterations++;

	}

	if( iterations >= max ) return false;

	*alpha  = ( a + b ) / 2 ;
	return true;
}

/*!
 * Computes and sets \a m_phi, \a m_s, \a m_theta and \a m_eccentricity.
 */
void ShapeTroughCHC::SetInternalValues()
{
	m_phi = atan2( r1.getValue() + p1.getValue(), height.getValue() );
	m_s = - ( ( p1.getValue() * p1.getValue() - r1.getValue() * r1.getValue() ) * cos( m_phi ) )
				/ (r1.getValue() - p1.getValue() * sin( m_phi ) ) ;
	m_theta = atan( tan( m_phi) + ( ( 2 * r1.getValue() ) / (m_s - height.getValue() ) ) );

	m_eccentricity = sqrt( ( r1.getValue() + p1.getValue() ) * ( r1.getValue() + p1.getValue() )
								* ( 1 - sin( m_phi ) ) * ( 1 - sin( m_phi ) ) * cos( m_phi ) * cos( m_phi )
							+ ( p1.getValue() - r1.getValue() ) * ( p1.getValue() - r1.getValue() )
								* ( 1 + sin( m_phi ) ) * ( 1 + sin( m_phi ) ) * sin( m_phi ) * sin( m_phi ) )
	                  /( 2 * p1.getValue() * cos( m_phi ) * cos( m_phi ) - ( p1.getValue() - r1.getValue() ) * ( 1 + sin( m_phi ) ) );

}
