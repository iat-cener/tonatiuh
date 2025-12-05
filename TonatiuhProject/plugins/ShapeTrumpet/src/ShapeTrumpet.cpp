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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Iñigo Pagola, Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
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

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "gf.h"
#include "ParameterValueException.h"
#include "Ray.h"
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
}

/*!
 * Destroys trumpet shape.
 */
ShapeTrumpet::~ShapeTrumpet()
{

}

/*!
 * Returns shape bbox.
 */
BBox ShapeTrumpet::GetBBox() const
{
	double bHyperbola = sqrt( -( a.getValue() * a.getValue() ) + ( focusHyperbola.getValue() * focusHyperbola.getValue() ) ) ;

	double xMin= - sqrt( a.getValue()  * a.getValue()  * ( 1 +
			( ( hyperbolaHeight.getValue() * hyperbolaHeight.getValue() ) / ( bHyperbola * bHyperbola ) ) ) );
	double xMax = sqrt( a.getValue()  * a.getValue()  * ( 1 +
			( ( hyperbolaHeight.getValue() * hyperbolaHeight.getValue() ) / ( bHyperbola * bHyperbola ) ) ) );

	double yMin = truncationHeight.getValue();
	double yMax = hyperbolaHeight.getValue();

	double zMin = xMin;
	double zMax = xMax;
	return BBox( Point3D( xMin, yMin, zMin ), Point3D( xMax, yMax, zMax ) );
}

/*!
 * Returns shape icon file name.
 */
std::string ShapeTrumpet::GetIcon() const
{
	return ( ":/icons/ShapeTrumpet.png" );
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

	double bHyperbola = sqrt( -( a.getValue() * a.getValue() ) + ( focusHyperbola.getValue() * focusHyperbola.getValue() ) ) ;
	
	double A = ( ( bHyperbola * bHyperbola ) * ( objectRay.direction().x * objectRay.direction().x + objectRay.direction().z * objectRay.direction().z ) )
				- ( ( objectRay.direction().y * objectRay.direction().y ) * ( a0 * a0 ) );

	double B = ( (  2.0 * objectRay.origin.x * objectRay.direction().x +  2.0 *objectRay.origin.z * objectRay.direction().z ) * ( bHyperbola * bHyperbola) )
						- ( (  2.0 * objectRay.origin.y * objectRay.direction().y ) * ( a0 * a0 ) );

	double C = ( ( objectRay.origin.x * objectRay.origin.x + objectRay.origin.z * objectRay.origin.z ) * ( bHyperbola * bHyperbola ) )
			 - ( ( objectRay.origin.y * objectRay.origin.y ) * ( a0 * a0 )  )
			 - ( ( a0 * a0 ) * ( bHyperbola * bHyperbola ) );

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double thit = ( t0 > objectRay.mint )? t0 : t1 ;
	if( thit > objectRay.maxt ) return false;

	//Evaluate Tolerance
	double tol = 0.000001;

	// Compute hit position and $\phi$
	Point3D hitPoint = objectRay( thit );

	double rMin = sqrt( a0 * a0 * ( 1 + ( ( tH * tH ) / ( bHyperbola * bHyperbola ) ) ) );

	double rMax = sqrt( a0 * a0 * ( 1 + ( ( hH * hH ) / ( bHyperbola * bHyperbola ) ) ) );

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
	double aux1 = - sqrt( a0 * a0 * (1 + ( ( h1 * h1 )/ ( bHyperbola * bHyperbola ) ) ) );
	double aux2 = sqrt( a0 * a0 * (1 + ( ( h2 * h2 )/ ( bHyperbola * bHyperbola ) ) ) );

	double aux = aux1 + aux2;

	double d2PduuY = - ( ( bHyperbola * bHyperbola * bHyperbola * bHyperbola * aux * aux * ( -aux1 + u * aux ) * ( -aux1 + u * aux ) )
						/ ( a0 * a0 * a0 * a0 *
								pow( bHyperbola * bHyperbola *
										( -1 + ( ( ( -aux1 + u * aux ) * ( -aux1 + u * aux ) )
												/ ( a0 * a0 ) ) ), 3.0/ 2 ) ) )
					+ ( ( bHyperbola * bHyperbola * aux * aux )
						/ ( a0 * a0  *
								sqrt( bHyperbola * bHyperbola *
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

	double bHyperbola = sqrt( -( a.getValue() * a.getValue() ) + ( focusHyperbola.getValue() * focusHyperbola.getValue() ) ) ;

	double rmin = sqrt(  a.getValue() * a.getValue() * ( 1 + ( ( truncationHeight.getValue() * truncationHeight.getValue() ) / ( bHyperbola * bHyperbola ) ) ) );
	double rmax = sqrt(  a.getValue() * a.getValue() * ( 1 + ( ( hyperbolaHeight.getValue() * hyperbolaHeight.getValue() ) / ( bHyperbola* bHyperbola ) ) ) );
	double r = u * (rmax - rmin) + rmin;
	double phi = v * gc::TwoPi;
	double x = cos( phi ) * r;
	double y = sqrt( ( ( ( r * r ) / ( a.getValue() * a.getValue() ) ) - 1 ) * bHyperbola * bHyperbola );
	double z = -sin( phi ) * r;
	return Point3D( x, y, z  );
}

/*!
* Checks the cone parameters values. Checks the \a value of parameter \a name .
*/
bool ShapeTrumpet::ValidateParamaterValue( std::string name, std::string value ) const
{
    if( name == "a" && std::stod( value ) < 0 ) 
		throw ParameterValueException( "a", "The 'a' must be a positive number" );

    if( ( name == "focusHyperbola" ) && ( ( std::stod( value ) < 0 )  || ( std::stod( value ) < a.getValue() ) ) )
		throw ParameterValueException( "focusHyperbola", "Trumpet focus value must take values on the ( a, Infinity ) range" );
		
    if( ( name == "hyperbolaHeight" ) && ( std::stod( value ) < 0 ) )
		throw ParameterValueException( "hyperbolaHeight", "Trumpet 'hyperbolaHeight' must take positive value" );

    if( ( name == "hyperbolaHeight" ) && ( std::stod( value ) <= truncationHeight.getValue() ) )
		throw ParameterValueException( "hyperbolaHeight", "Trumpet 'hyperbolaHeight' must take values on the ( truncationHeight, Infinity ) range" );

    if( ( name == "truncationHeight" ) && ( std::stod( value ) >= hyperbolaHeight.getValue() ) )
		throw ParameterValueException( "truncationHeight", "Trumpet 'truncationHeight' must take values on the [0, hyperbolaHeight ) range" );

	return true;	
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

/*!
 * Returns dpdu vector for \a u and \a v valid parameters.
 */
Vector3D ShapeTrumpet::GetDPDU( double u, double v ) const
{
	
	double bHyperbola = sqrt( -( a.getValue() * a.getValue() ) + ( focusHyperbola.getValue() * focusHyperbola.getValue() ) ) ;

	double a0 = a.getValue();
	double h2 = hyperbolaHeight.getValue();
	double h1 = truncationHeight.getValue();
	double aux1 = - sqrt( a0 * a0 * (1 + ( ( h1 * h1 )/ ( bHyperbola * bHyperbola ) ) ) );
	double aux2 = sqrt( a0 * a0 * (1 + ( ( h2 * h2 )/ ( bHyperbola * bHyperbola ) ) ) );
	double x = 	cos( gc::TwoPi * v ) * (  aux1 + aux2 );


	double y = ( bHyperbola * bHyperbola * ( aux1 + aux2 ) * ( -aux1 + u * ( aux1 + aux2 ) ) )
			/ ( a0 * a0 * sqrt( bHyperbola * bHyperbola * ( -1 + ( ( ( -aux1 + u * ( aux1 + aux2 ) )  * ( -aux1 + u * ( aux1 + aux2 ) ) ) / ( a0 * a0 ) ) ) ) );

	double z = 	-sin( gc::TwoPi * v ) * (  aux1 + aux2 );

	return Vector3D( x, y, z );
}

/*!
 * Returns dpdv vector for \a u and \a v valid parameters.
 */
Vector3D ShapeTrumpet::GetDPDV ( double u, double v ) const
{
	double bHyperbola = sqrt( -( a.getValue() * a.getValue() ) + ( focusHyperbola.getValue() * focusHyperbola.getValue() ) ) ;
	double a0 = a.getValue();
	double h2 = hyperbolaHeight.getValue();
	double h1 = truncationHeight.getValue();
	double aux1 = - sqrt( a0 * a0 * (1 + ( ( h1 * h1 )/ ( bHyperbola * bHyperbola ) ) ) );
	double aux2 = sqrt( a0 * a0 * (1 + ( ( h2 * h2 )/ ( bHyperbola * bHyperbola ) ) ) );

	double x = - gc::TwoPi * sin( gc::TwoPi * v ) * ( -aux1 + u * ( aux1 + aux2 ) );
	double y = 0;
	double z = - gc::TwoPi * cos( gc::TwoPi * v ) * ( -aux1 + u * ( aux1 + aux2 ) );

	return Vector3D( x, y , z );
}
