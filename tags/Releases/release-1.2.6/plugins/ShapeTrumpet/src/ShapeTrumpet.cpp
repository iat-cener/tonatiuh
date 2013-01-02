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

/*!
 * \class ShapeTrumpet.
 * \brief ShapeTrumpet class the trumpet surface representation.
 *
 * \section Defaults
 *
 * \param a 0.25
 * \param focusHyperbola	0.32
 * \param truncationHeight	0.0
 * \param hyperbolaHeight 1.0
 * \param activeSide OUTSIDE
 *
*/
#include <QMessageBox>
#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "BBox.h"
#include "gc.h"
#include "gf.h"
#include "NormalVector.h"
#include "Point3D.h"
#include "Ray.h"
#include "Vector3D.h"


#include "DifferentialGeometry.h"
#include "ShapeTrumpet.h"

SO_NODE_SOURCE(ShapeTrumpet);

/*!
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 */
void ShapeTrumpet::initClass()
{
	SO_NODE_INIT_CLASS( ShapeTrumpet, TShape, "TShape" );
}

/*!
 * Creates a new trumpet shape.
 */
ShapeTrumpet::ShapeTrumpet()
:m_bHyperbola( 0 ),
 m_lastApertureValue( 0.25 ),
 m_lastFocusHyperbola( 0.32 ),
 m_lastHyperbolaHeightValue( 1.0 ),
 m_lastTruncationHeightValue( 0.0 ),
 m_apertureSensor( 0 ),
 m_fHSensor( 0 ),
 m_heightSensor( 0 ),
 m_truncationSensor( 0 )
{
	SO_NODE_CONSTRUCTOR(ShapeTrumpet);
	SO_NODE_ADD_FIELD( a, (0.25) );
	SO_NODE_ADD_FIELD( focusHyperbola, (0.32) );
	SO_NODE_ADD_FIELD( truncationHeight, (0.0) );
	SO_NODE_ADD_FIELD( hyperbolaHeight, (1.0) );

	SO_NODE_DEFINE_ENUM_VALUE( Side, INSIDE );
	SO_NODE_DEFINE_ENUM_VALUE( Side, OUTSIDE );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
	SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );


	SetBHyperbola();

	m_apertureSensor = new SoFieldSensor(updateApertureValue, this);
	m_apertureSensor->setPriority( 1 );
	m_apertureSensor->attach( &a );
	m_fHSensor = new SoFieldSensor(updateFocusValue, this);
	m_fHSensor->setPriority( 1 );
	m_fHSensor->attach( &focusHyperbola );
	m_heightSensor = new SoFieldSensor(updateHeightValue, this);
	m_heightSensor->setPriority( 1 );
	m_heightSensor->attach( &hyperbolaHeight );
	m_truncationSensor = new SoFieldSensor(updateTruncationValue, this);
	m_truncationSensor->setPriority( 1 );
	m_truncationSensor->attach( &truncationHeight );

}

/*!
 * Destroys trumpet shape.
 */
ShapeTrumpet::~ShapeTrumpet()
{
	delete m_apertureSensor;
	delete m_fHSensor;
	delete m_heightSensor;
	delete m_truncationSensor;
}


/*!
 * Shape area. //Not yet implemented.
 */
double ShapeTrumpet::GetArea() const
{
	return -1;
}

/*!
 * Returns shape bbox.
 */
BBox ShapeTrumpet::GetBBox() const
{
	double xMin= - sqrt( a.getValue()  * a.getValue()  * ( 1 +
			( ( hyperbolaHeight.getValue() * hyperbolaHeight.getValue() )
					/ ( m_bHyperbola * m_bHyperbola ) ) ) );
	double xMax = sqrt( a.getValue()  * a.getValue()  * ( 1 +
			( ( hyperbolaHeight.getValue() * hyperbolaHeight.getValue() )
					/ ( m_bHyperbola * m_bHyperbola ) ) ) );

	double yMin = truncationHeight.getValue();
	double yMax = hyperbolaHeight.getValue();

	double zMin = xMin;
	double zMax = xMax;
	return BBox( Point3D( xMin, yMin, zMin ), Point3D( xMax, yMax, zMax ) );
}

/*!
 * Returns shape icon file name.
 */
QString ShapeTrumpet::GetIcon() const
{
	return QString( ":/icons/ShapeTrumpet.png" );
}

/*!
 * Returns true if the \a objectRay intersects with the trumpet surface. Otherwise, returns false.
 *
 * The ray is transformed to surface coordinates system.
 * If the ray intersects with the surface \a tHit and \a dg arguments are changed with the intersection information.
 */
bool ShapeTrumpet::Intersect(const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	double a0 = a.getValue();
	double tH = truncationHeight.getValue();
	double hH = hyperbolaHeight.getValue();

	double b = m_bHyperbola;

	double A = ( ( b * b ) * ( objectRay.direction().x * objectRay.direction().x + objectRay.direction().z * objectRay.direction().z ) )
				- ( ( objectRay.direction().y * objectRay.direction().y ) * ( a0 * a0 ) );

	double B = ( (  2.0 * objectRay.origin.x * objectRay.direction().x +  2.0 *objectRay.origin.z * objectRay.direction().z ) * ( b * b ) )
						- ( (  2.0 * objectRay.origin.y * objectRay.direction().y ) * ( a0 * a0 ) );

	double C = ( ( objectRay.origin.x * objectRay.origin.x + objectRay.origin.z * objectRay.origin.z ) * ( b * b ) )
			 - ( ( objectRay.origin.y * objectRay.origin.y ) * ( a0 * a0 )  )
			 - ( ( a0 * a0 ) * ( b * b ) );

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double thit = ( t0 > objectRay.mint )? t0 : t1 ;
	if( thit > objectRay.maxt ) return false;

	//Evaluate Tolerance
	double tol = 0.000001;

	// Compute ShapeSphere hit position and $\phi$
	Point3D hitPoint = objectRay( thit );

	double rMin = sqrt( a0 * a0 * ( 1 +
			( ( tH * tH )
					/ ( b * b ) ) ) );

	double rMax = sqrt( a0 * a0 * ( 1 +
			( ( hH * hH )
					/ ( b * b ) ) ) );

	double length = sqrt( hitPoint.x * hitPoint.x + hitPoint.z * hitPoint.z );


	// Test intersection against clipping parameters
	if( (thit - objectRay.mint) < tol
			|| length < rMin || length > rMax
			|| hitPoint.y < tH || hitPoint.y > hH )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		// Compute ShapeSphere hit position and $\phi$
		hitPoint = objectRay( thit );
		length = sqrt( hitPoint.x * hitPoint.x + hitPoint.z * hitPoint.z );

		if( (thit - objectRay.mint) < tol
				|| length < rMin || length > rMax
				|| hitPoint.y < tH || hitPoint.y > hH )	return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );

	// Find parametric representation of CPC concentrator hit
	double u = ( length - rMin  ) / ( rMax - rMin );
	double phi = atan2( -hitPoint.z, hitPoint.x );
	if( phi < 0 ) phi += gc::TwoPi;
	double v = phi/ gc::TwoPi;


	// Compute  \dpdu and \dpdv
	Vector3D dpdu = GetDPDU( u, v );
	Vector3D dpdv = GetDPDV( u, v );

	// Compute cylinder \dndu and \dndv
	double h2 = hH;
	double h1 = tH;
	double aux1 = - sqrt( a0 * a0 * (1 + ( ( h1 * h1 )/ ( m_bHyperbola * m_bHyperbola ) ) ) );
	double aux2 = sqrt( a0 * a0 * (1 + ( ( h2 * h2 )/ ( m_bHyperbola * m_bHyperbola ) ) ) );

	double aux = aux1 + aux2;

	double d2PduuY = - ( ( m_bHyperbola * m_bHyperbola * m_bHyperbola * m_bHyperbola * aux * aux * ( -aux1 + u * aux ) * ( -aux1 + u * aux ) )
						/ ( a0 * a0 * a0 * a0 *
								pow( m_bHyperbola * m_bHyperbola *
										( -1 + ( ( ( -aux1 + u * aux ) * ( -aux1 + u * aux ) )
												/ ( a0 * a0 ) ) ), 3.0/ 2 ) ) )
					+ ( ( m_bHyperbola * m_bHyperbola * aux * aux )
						/ ( a0 * a0  *
								sqrt( m_bHyperbola * m_bHyperbola *
										( -1 + ( ( ( -aux1 + u * aux ) * ( -aux1 + u * aux ) )
												/ ( a0 * a0 ) ) ) ) ) );
	Vector3D d2Pduu(0 , d2PduuY, 0);
	Vector3D d2Pduv( - gc::TwoPi * sin( gc::TwoPi * v ) * aux,
			0.0,
			- gc::TwoPi * cos( gc::TwoPi * v ) * aux );
	Vector3D d2Pdvv( - gc::TwoPi * gc::TwoPi * cos( gc::TwoPi * v ) * ( -aux1 + u * aux ),
			0.0,
			gc::TwoPi * gc::TwoPi * sin( gc::TwoPi * v ) * ( -aux1 + u * aux ) );

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

/*!
 * Returns true if the \a objectRay instersects with the trumpet surface. Otherwise, retruns false.
 */
bool ShapeTrumpet::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}

/*!
 * Returns surface point coordinates defined for \a u and \a v parameters.
 */
Point3D ShapeTrumpet::Sample( double u, double v ) const
{
	if( OutOfRange( u, v ) ) 	gf::SevereError("Function ShapeTrumpet::Sample called with invalid parameters" );

	double rmin = sqrt(  a.getValue() * a.getValue() * ( 1 + ( ( truncationHeight.getValue() * truncationHeight.getValue() ) / ( m_bHyperbola * m_bHyperbola ) ) ) );
	double rmax = sqrt(  a.getValue() * a.getValue() * ( 1 + ( ( hyperbolaHeight.getValue() * hyperbolaHeight.getValue() ) / ( m_bHyperbola* m_bHyperbola ) ) ) );
	double r = u * (rmax - rmin) + rmin;
	double phi = v * gc::TwoPi;
	double x = cos( phi ) * r;
	double y = sqrt( ( ( ( r * r ) / ( a.getValue() * a.getValue() ) ) - 1 ) * m_bHyperbola * m_bHyperbola );
	double z = -sin( phi ) * r;
	return Point3D( x, y, z  );
}

/*!
 * Checks whether the defined aperture parameter is a valid parameter.
 */
void ShapeTrumpet::updateApertureValue( void *data, SoSensor *)
{
	ShapeTrumpet* shape = (ShapeTrumpet *) data;
	if( shape->a.getValue() < 0 )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Trumpet a must take positive value. ") );
		shape->a.setValue( shape->m_lastApertureValue );
	}
	else
		shape->m_lastApertureValue = shape->a.getValue();


	shape->SetBHyperbola();
}

/*!
 * Checks whether the defined focus parameter is a valid parameter.
 */
void ShapeTrumpet::updateFocusValue( void *data, SoSensor *)
{
	ShapeTrumpet* shape = (ShapeTrumpet *) data;
	if( ( shape->focusHyperbola.getValue() < 0 ) ||
			( shape->focusHyperbola.getValue() <=  shape->a.getValue() ) )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Trumpet focus value must take values on the ( a, Infinity ) range. ") );
		shape->focusHyperbola.setValue( shape->m_lastFocusHyperbola );
	}
	else
		shape->m_lastFocusHyperbola = shape->focusHyperbola.getValue();

	shape->SetBHyperbola();
}

/*!
 * Checks whether the defined height parameter is a valid parameter.
 */
void ShapeTrumpet::updateHeightValue( void *data, SoSensor *)
{
	ShapeTrumpet* shape = (ShapeTrumpet *) data;
	if( shape->hyperbolaHeight.getValue() < 0 )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Trumpet height must take positive value. ") );
		shape->hyperbolaHeight.setValue( shape->m_lastHyperbolaHeightValue );
	}
	else if( shape->hyperbolaHeight.getValue() <=  shape->truncationHeight.getValue() )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Trumpet  height must take values on the ( truncationHeight, Infinity ) range. ") );
		shape->hyperbolaHeight.setValue( shape->m_lastHyperbolaHeightValue );
	}
	else
		shape->m_lastHyperbolaHeightValue = shape->hyperbolaHeight.getValue();


	shape->SetBHyperbola();
}

/*!
 * Checks whether the defined truncation height parameter is a valid parameter.
 */
void ShapeTrumpet::updateTruncationValue( void *data, SoSensor *)
{
	ShapeTrumpet* shape = (ShapeTrumpet *) data;
	if( ( shape->truncationHeight.getValue() < 0 ) || ( shape->truncationHeight.getValue() >= shape->hyperbolaHeight.getValue() ) )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "Trumpet truncation must take values on the [0, hyperbolaHeight ) range. ") );
		shape->truncationHeight.setValue( shape->m_lastTruncationHeightValue );
	}
	else
		shape->m_lastTruncationHeightValue = shape->truncationHeight.getValue();

	shape->SetBHyperbola();
}

/*!
 * Returns surface normal to the point defined with \a u and \a v parameters.
 */
NormalVector ShapeTrumpet::GetNormal( double u, double v ) const
{
	if( OutOfRange( u, v ) ) 	gf::SevereError("Function ShapeTrumpet::GetNormal called with invalid parameters" );

	Vector3D dpdu = GetDPDU( u, v );
	Vector3D dpdv = GetDPDV( u, v );

	return Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

}

/*!
 * Returns true whether \a u and \a v are valid parameters.
 */
bool ShapeTrumpet::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

/*!
 * Computes the shape exact sizes. The bounding box is calculated and returned in the local coordinate system.
 */
void ShapeTrumpet::computeBBox( SoAction* /* action */, SbBox3f& box, SbVec3f& /* center */)
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
	SbVec3f min, max;

	min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
	max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );

	// Set the box to bound the two extreme points.
	box.setBounds(min, max);
}

/*!
 * The method implements action behavior for shape nodes for SoCallbackAction.
 */
void ShapeTrumpet::generatePrimitives( SoAction *action )
{

    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	const int rows = 100; // Number of points per row
    const int columns = 100; // Number of points per column
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

    		Point3D point = Sample(ui, vj);
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

/*!
 * Computes the asymptotic angle for the input parameters.
 */
void ShapeTrumpet::SetBHyperbola()
{
	m_bHyperbola = sqrt( -( a.getValue() * a.getValue() ) + ( focusHyperbola.getValue() * focusHyperbola.getValue() ) ) ;
}

/*!
 * Returns dpdu vector for \a u and \a v valid parameters.
 */
Vector3D ShapeTrumpet::GetDPDU( double u, double v ) const
{
	double a0 = a.getValue();
	double h2 = hyperbolaHeight.getValue();
	double h1 = truncationHeight.getValue();
	double aux1 = - sqrt( a0 * a0 * (1 + ( ( h1 * h1 )/ ( m_bHyperbola * m_bHyperbola ) ) ) );
	double aux2 = sqrt( a0 * a0 * (1 + ( ( h2 * h2 )/ ( m_bHyperbola * m_bHyperbola ) ) ) );
	double x = 	cos( gc::TwoPi * v ) * (  aux1 + aux2 );


	double y = ( m_bHyperbola * m_bHyperbola * ( aux1 + aux2 ) * ( -aux1 + u * ( aux1 + aux2 ) ) )
			/ ( a0 * a0 * sqrt( m_bHyperbola * m_bHyperbola * ( -1 + ( ( ( -aux1 + u * ( aux1 + aux2 ) )  * ( -aux1 + u * ( aux1 + aux2 ) ) ) / ( a0 * a0 ) ) ) ) );

	double z = 	-sin( gc::TwoPi * v ) * (  aux1 + aux2 );

	return Vector3D( x, y, z );
}

/*!
 * Returns dpdv vector for \a u and \a v valid parameters.
 */
Vector3D ShapeTrumpet::GetDPDV ( double u, double v ) const
{
	double a0 = a.getValue();
	double h2 = hyperbolaHeight.getValue();
	double h1 = truncationHeight.getValue();
	double aux1 = - sqrt( a0 * a0 * (1 + ( ( h1 * h1 )/ ( m_bHyperbola * m_bHyperbola ) ) ) );
	double aux2 = sqrt( a0 * a0 * (1 + ( ( h2 * h2 )/ ( m_bHyperbola * m_bHyperbola ) ) ) );

	double x = - gc::TwoPi * sin( gc::TwoPi * v ) * ( -aux1 + u * ( aux1 + aux2 ) );
	double y = 0;
	double z = - gc::TwoPi * cos( gc::TwoPi * v ) * ( -aux1 + u * ( aux1 + aux2 ) );

	return Vector3D( x, y , z );
}
