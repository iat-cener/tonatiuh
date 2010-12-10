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

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeTroughHyperbola.h"
#include "tgf.h"
#include "tgc.h"
#include "Vector3D.h"


SO_NODE_SOURCE(ShapeTroughHyperbola);

void ShapeTroughHyperbola::initClass()
{
	SO_NODE_INIT_CLASS(ShapeTroughHyperbola, TShape, "TShape");
}

ShapeTroughHyperbola::ShapeTroughHyperbola()
:m_asymptoticAngle( 0 ),
 m_lastApertureValue( 0.25 ),
 m_lastFocusHyperbola( 0.32 ),
 m_lastHyperbolaHeightValue( 1.0 ),
 m_lastTruncationHeightValue( 0.0 ),
 m_lastZLengthXMinValue(1.0),
 m_lastZLengthXMaxValue(1.0)
{

	SO_NODE_CONSTRUCTOR(ShapeTroughHyperbola);
	SO_NODE_ADD_FIELD( a0, (m_lastApertureValue) );
	SO_NODE_ADD_FIELD( focusHyperbola, ( m_lastFocusHyperbola ) );
	SO_NODE_ADD_FIELD( truncationHeight, ( m_lastTruncationHeightValue ) );
	SO_NODE_ADD_FIELD( hyperbolaHeight, ( m_lastHyperbolaHeightValue ) );
	SO_NODE_ADD_FIELD( zLengthXMin, (m_lastZLengthXMinValue) );
	SO_NODE_ADD_FIELD( zLengthXMax, (m_lastZLengthXMaxValue) );


	SetAsymptoticAngle();

	SoFieldSensor* m_apertureSensor = new SoFieldSensor(updateApertureValue, this);
	m_apertureSensor->attach( &a0 );
	SoFieldSensor* m_fHSensor = new SoFieldSensor(updateFocusValue, this);
	m_fHSensor->attach( &focusHyperbola );
	SoFieldSensor* lengthSensor1 = new SoFieldSensor(updateLengthValues, this);
	lengthSensor1->attach( &zLengthXMin );
	SoFieldSensor* lengthSensor2 = new SoFieldSensor(updateLengthValues, this);
	lengthSensor2->attach( &zLengthXMax );
	SoFieldSensor* m_heightSensor = new SoFieldSensor(updateHeightValue, this);
	m_heightSensor->attach( &hyperbolaHeight );
	SoFieldSensor* m_truncationSensor = new SoFieldSensor(updateTruncationValue, this);
	m_truncationSensor->attach( &truncationHeight );


}

ShapeTroughHyperbola::~ShapeTroughHyperbola()
{
}


double ShapeTroughHyperbola::GetArea() const
{
	return -1;
}

QString ShapeTroughHyperbola::GetIcon() const
{
	return ":/icons/ShapeTroughHyperbola.png";
}

bool ShapeTroughHyperbola::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	double a = a0.getValue();
	double b = a / tan( m_asymptoticAngle );

	double A = ( ( b * b ) * ( objectRay.direction.x * objectRay.direction.x ) )
				- ( ( objectRay.direction.y * objectRay.direction.y ) * ( a * a ) );
	double B = 2.0 * ( ( (  objectRay.origin.x * objectRay.direction.x ) * ( b * b ) )
						- ( ( objectRay.origin.y * objectRay.direction.y ) * ( a * a ) ) );
	double C = ( ( objectRay.origin.x * objectRay.origin.x ) * ( b * b ) )
			 - ( ( objectRay.origin.y * objectRay.origin.y ) * ( a * a )  )
			 - ( ( a * a ) * ( b * b ) );

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !tgf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double thit = ( t0 > objectRay.mint )? t0 : t1 ;
	if( thit > objectRay.maxt ) return false;

	//Evaluate Tolerance
	double tol = 0.000001;

	// Compute ShapeSphere hit position and $\phi$
	Point3D hitPoint = objectRay( thit );

	double xMin = sqrt( a * a *
							( 1 + ( ( truncationHeight.getValue() * truncationHeight.getValue() )
									/ ( b* b ) ) ) );
	double xMax = sqrt( a * a * ( 1 +
				( ( hyperbolaHeight.getValue() * hyperbolaHeight.getValue() )
						/ ( b * b ) ) ) );

	// Test intersection against clipping parameters
	double m =  ( zLengthXMax.getValue() / 2- zLengthXMin.getValue() / 2 ) / ( xMax - xMin );
	double zmax = ( zLengthXMin.getValue()  / 2 ) + m * ( hitPoint.x - xMin );
	double zmin = - zmax;


	// Test intersection against clipping parameters
	if( (thit - objectRay.mint) < tol
			|| hitPoint.x < xMin || hitPoint.x > xMax
			|| hitPoint.y < truncationHeight.getValue() || hitPoint.y > hyperbolaHeight.getValue()
			|| hitPoint.z < zmin ||  hitPoint.z > zmax )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		// Compute ShapeSphere hit position and $\phi$
		hitPoint = objectRay( thit );
		zmax = ( zLengthXMin.getValue() * 0.5 ) + m * ( hitPoint.x - xMin );
		zmin = - zmax;

		if( (thit - objectRay.mint) < tol
				|| hitPoint.x < xMin || hitPoint.x > xMax
				|| hitPoint.y < truncationHeight.getValue() || hitPoint.y > hyperbolaHeight.getValue()
				|| hitPoint.z < zmin ||  hitPoint.z > zmax )	return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );


	// Find parametric representation of CPC concentrator hit
	double u = ( hitPoint.x - xMin  ) / ( xMax - xMin );
	double v = ( ( hitPoint.z / zmax ) + 1 )/ 2;

	// Compute  \dpdu and \dpdv
	Vector3D dpdu = GetDPDU( u, v );
	Vector3D dpdv = GetDPDV( u, v );

	// Compute cylinder \dndu and \dndv
	double tanAngle = tan( m_asymptoticAngle );
	double h = hyperbolaHeight.getValue();
	double t = truncationHeight.getValue();
	double cotAngle = 1 / tanAngle;
	double aux1 = sqrt( a * a * (1 + ( ( h * h * tanAngle * tanAngle )/ ( a * a ) ) ) );
	double aux2 = sqrt( a * a * (1 + ( ( t * t * tanAngle * tanAngle )/ ( a * a ) ) ) );
	double aux = aux1 - aux2;

	double d2PduuY = - ( ( cotAngle * cotAngle * cotAngle * cotAngle * aux * aux * ( aux2 + u * aux ) )
						/ pow( ( a * a * cotAngle * cotAngle *
								( -1 + ( ( ( a + u * aux ) * ( a + u * aux ) )
										/ ( a * a ) ) ) ), 3.0/ 2 ) )
					+ ( ( cotAngle * cotAngle * aux * aux )
						/ sqrt( a * a * cotAngle * cotAngle *
								( -1 + ( ( ( a + u * aux ) * ( a + u * aux ) )
										/ ( a * a ) ) ) ) );
	Vector3D d2Pduu(0 , d2PduuY, 0);
	Vector3D d2Pduv( 0.0, 0.0, 2 * ( -0.5 * zLengthXMin.getValue() + 0.5 * zLengthXMax.getValue() ) );
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
		                        dpdv,
								dpdu,
		                        dndu,
								dndv,
		                        u, v, this );

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeTroughHyperbola::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}

Point3D ShapeTroughHyperbola::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

void ShapeTroughHyperbola::updateApertureValue( void *data, SoSensor *)
{
	ShapeTroughHyperbola* shape = (ShapeTroughHyperbola *) data;
	if( shape->a0.getValue() < 0 )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Hyperbola Trough a0 must take positive value. ") );
		shape->a0.setValue( shape->m_lastApertureValue );
	}
	else
		shape->m_lastApertureValue = shape->a0.getValue();


	shape->SetAsymptoticAngle();
}

void ShapeTroughHyperbola::updateFocusValue( void *data, SoSensor *)
{
	ShapeTroughHyperbola* shape = (ShapeTroughHyperbola *) data;
	if( ( shape->focusHyperbola.getValue() < 0 ) ||
			( shape->focusHyperbola.getValue() <=  shape->a0.getValue() ) )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Hyperbola Trough focus value must take values on the ( a0, Infinity ) range. ") );
		shape->focusHyperbola.setValue( shape->m_lastFocusHyperbola );
	}
	else
		shape->m_lastFocusHyperbola = shape->focusHyperbola.getValue();

	shape->SetAsymptoticAngle();
}

void ShapeTroughHyperbola::updateLengthValues( void *data, SoSensor *)
{
	ShapeTroughHyperbola* shape = (ShapeTroughHyperbola *) data;
	if( ( shape->zLengthXMin.getValue() < 0 ) || ( shape->zLengthXMax.getValue() < 0 ) )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Hyperbola Trough z length variables must take positive values. ") );
		shape->zLengthXMin.setValue( shape->m_lastZLengthXMinValue );
		shape->zLengthXMax.setValue( shape->m_lastZLengthXMaxValue );
	}
	else
	{
		shape->m_lastZLengthXMinValue = shape->zLengthXMin.getValue();
		shape->m_lastZLengthXMaxValue = shape->zLengthXMax.getValue();
	}
}

void ShapeTroughHyperbola::updateHeightValue( void *data, SoSensor *)
{
	ShapeTroughHyperbola* shape = (ShapeTroughHyperbola *) data;
	if( shape->hyperbolaHeight.getValue() < 0 )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Hyperbola Trough height must take positive value. ") );
		shape->hyperbolaHeight.setValue( shape->m_lastHyperbolaHeightValue );
	}
	else if( shape->hyperbolaHeight.getValue() <=  shape->truncationHeight.getValue() )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Hyperbola Trough  height must take values on the ( truncationHeight, Infinity ) range. ") );
		shape->hyperbolaHeight.setValue( shape->m_lastHyperbolaHeightValue );
	}
	else
		shape->m_lastHyperbolaHeightValue = shape->hyperbolaHeight.getValue();


	shape->SetAsymptoticAngle();
}
void ShapeTroughHyperbola::updateTruncationValue( void *data, SoSensor *)
{
	ShapeTroughHyperbola* shape = (ShapeTroughHyperbola *) data;
	if( ( shape->truncationHeight.getValue() < 0 ) || ( shape->truncationHeight.getValue() >= shape->hyperbolaHeight.getValue() ) )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Hyperbola Trough truncation must take values on the [0, hyperbolaHeight ) range. ") );
		shape->truncationHeight.setValue( shape->m_lastTruncationHeightValue );
	}
	else
		shape->m_lastTruncationHeightValue = shape->truncationHeight.getValue();
}


Point3D ShapeTroughHyperbola::GetPoint3D( double u, double v ) const
{
	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );

	double a = a0.getValue();
	double b = a / tan( m_asymptoticAngle );

	double xmin =  sqrt( a * a * ( 1 +
			( ( truncationHeight.getValue() * truncationHeight.getValue() )
					/ ( b* b ) ) ) );
	double xmax = sqrt( a * a * ( 1 +
			( ( hyperbolaHeight.getValue() * hyperbolaHeight.getValue() )
					/ ( b* b ) ) ) );
	double x = u * ( xmax - xmin ) + xmin;
	double y = sqrt( ( ( ( x * x ) / ( a * a ) ) - 1 ) * b * b );
	double m = ( 0.5 * zLengthXMax.getValue() - 0.5 * zLengthXMin.getValue() )
			/ ( xmax - xmin);
	double zMax = ( 0.5 * zLengthXMin.getValue() ) + m * ( x - xmin );
	double z = zMax * ( 2 * v - 1 );
	return Point3D (x, y, z);

}

NormalVector ShapeTroughHyperbola::GetNormal (double u ,double v) const
{
	Vector3D dpdu = GetDPDU( u, v );
	Vector3D dpdv = GetDPDV( u, v );

	return Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

}

bool ShapeTroughHyperbola::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeTroughHyperbola::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
	double a = a0.getValue();
	double b = a / tan( m_asymptoticAngle );

	double xMin =  sqrt( a * a * ( 1 +
			( ( truncationHeight.getValue() * truncationHeight.getValue() )
					/ ( b* b ) ) ) );
	double xMax = sqrt( a * a * ( 1 +
			( ( hyperbolaHeight.getValue() * hyperbolaHeight.getValue() )
					/ ( b* b ) ) ) );

	double yMin = truncationHeight.getValue();
	double yMax = hyperbolaHeight.getValue();

	double zLengthMax = std::max( zLengthXMin.getValue(), zLengthXMax.getValue() );
	double zMin = - zLengthMax /2;
	double zMax = zLengthMax /2;
	box.setBounds(SbVec3f( xMin, yMin, zMin ), SbVec3f( xMax, yMax, zMax ) );
}

void ShapeTroughHyperbola::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	const int rows = 20; // Number of points per row
    const int columns = 10; // Number of points per column
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
    for( int ivert = 0; ivert<totalIndices; ++ivert )
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
    for( int i = 0; i < totalIndices; ++i )
    {
    	SbVec3f  point( finalvertex[i][0], finalvertex[i][1],  finalvertex[i][2] );
    	SbVec3f normal(-finalvertex[i][3],-finalvertex[i][4], -finalvertex[i][5] );
		SbVec4f texCoord = useTexFunc ? tce->get(point, normal): SbVec4f( u,v, 0.0, 1.0 );

		pv.setPoint(point);
		pv.setNormal(normal);
		pv.setTextureCoords(texCoord);
		shapeVertex(&pv);
    }
    endShape();
}

void ShapeTroughHyperbola::SetAsymptoticAngle()
{
	m_asymptoticAngle = asin( a0.getValue() / focusHyperbola.getValue() );
}

Vector3D ShapeTroughHyperbola::GetDPDU( double u, double v ) const
{

	double a = a0.getValue();
	double tanAngle = tan( m_asymptoticAngle );
	double h2 = hyperbolaHeight.getValue();
	double h1 = truncationHeight.getValue();
	double cotAngle = 1 / tanAngle;
	double aux1 = - sqrt( a * a * (1 + ( ( h1 * h1 * tanAngle * tanAngle )/ ( a * a ) ) ) );
	double aux2 = sqrt( a * a * (1 + ( ( h2 * h2 * tanAngle * tanAngle )/ ( a * a ) ) ) );
	double x = aux1 + aux2;


	double y = ( cotAngle * cotAngle * x * ( -aux1 + u * x ) )
			/ sqrt( a * a * cotAngle * cotAngle * ( -1 + ( ( ( -aux1 + u * x )  * ( -aux1 + u * x ) ) / ( a * a ) ) ) );


	double z = (-0.5 * zLengthXMin.getValue() + 0.5 * zLengthXMax.getValue() ) * ( -1 + 2 * v );


	return Vector3D( x, y, z );


}

Vector3D ShapeTroughHyperbola::GetDPDV ( double u, double v ) const
{
	return Vector3D( 0.0, 0.0, 2 * ( 0.5 * zLengthXMin.getValue()
											+ ( -0.5 * zLengthXMin.getValue() + 0.5 * zLengthXMax.getValue() ) *  u ) );
}
