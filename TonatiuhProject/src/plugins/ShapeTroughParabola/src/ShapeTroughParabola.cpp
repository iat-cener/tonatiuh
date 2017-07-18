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

#include "DifferentialGeometry.h"
#include "gc.h"
#include "gf.h"

#include "ShapeTroughParabola.h"
#include "TParameterList.h"


TNodeType ShapeTroughParabola::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* ShapeTroughParabola::CreateInstance( )
{
  return ( new ShapeTroughParabola() );
}


/*!
 * Initializes ShapeTroughParabola type.
 */
void ShapeTroughParabola::Init()
{
	ShapeTroughParabola::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), QString( "ShapeTroughParabola" ), &ShapeTroughParabola::CreateInstance );
}

/*!
 * ShapeSphericalRectangle : public TNode
 */

ShapeTroughParabola::ShapeTroughParabola()
:TShape()
{
	setObjectName(GetType().GetName());

	//Translation
	m_parametersList->Append( QLatin1String("focusLength"), 0.125);
	m_parametersList->Append( QLatin1String("xMin" ), -0.5 );
	m_parametersList->Append( QLatin1String("xMax" ), 0.5 );
	m_parametersList->Append( QLatin1String("lengthXMin" ), 1.0 );
	m_parametersList->Append( QLatin1String("lengthXMax" ), 1.0 );
}

/*!
 * Destroys ShapeSphericalRectangle object.
 */
ShapeTroughParabola::~ShapeTroughParabola()
{

}


/*!
 * Returns the filename that stores the shape icon.
 */
QString ShapeTroughParabola::GetIcon()
{
	return ( QLatin1String( ":/icons/ShapeTroughParabola.png" ) );
}

/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeTroughParabola::GetBondingBox() const
{

	double focusLength = m_parametersList->GetValue( QLatin1String("focusLength") ).toDouble();
	double xmin = m_parametersList->GetValue( QLatin1String("xMin") ).toDouble();
	double xmax = m_parametersList->GetValue( QLatin1String("xMax") ).toDouble();
	double lengthXMin = m_parametersList->GetValue( QLatin1String("lengthXMin") ).toDouble();
	double lengthXMax = m_parametersList->GetValue( QLatin1String("lengthXMax") ).toDouble();

	double y1 = ( xmin * xmin ) / ( 4 * focusLength );
	double y2 = ( xmax * xmax ) / ( 4 * focusLength );

	double ymin = 0.0;
	if( ( xmin * xmax ) > 0 )
		ymin = std::min( y1, y2 );
	double ymax = std::max( y1, y2 );

	double zmin = 0.0;
	double zmax = std::max( lengthXMin, lengthXMax);
	return BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) );


	return ( BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) ) );
}

/*!
 * Returns the type of node.
 */
TNodeType ShapeTroughParabola::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Intersects the \a objectRay in surface coordinates with the surface and calculates the information of the surface in the intersection poit.
 * Returns true if an intersection occurs.
 */
bool ShapeTroughParabola::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{

	double focusLength = m_parametersList->GetValue( QLatin1String("focusLength") ).toDouble();
	double xmin = m_parametersList->GetValue( QLatin1String("xMin") ).toDouble();
	double xmax = m_parametersList->GetValue( QLatin1String("xMax") ).toDouble();
	double lengthXMin = m_parametersList->GetValue( QLatin1String("lengthXMin") ).toDouble();
	double lengthXMax = m_parametersList->GetValue( QLatin1String("lengthXMax") ).toDouble();


	// Compute quadratic parabolic cylinder coefficients
	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A = objectRay.direction().x*objectRay.direction().x;
	double B = 2.0 * ( objectRay.direction().x* objectRay.origin.x - 2 * focusLength * objectRay.direction().y);
	double C = objectRay.origin.x * objectRay.origin.x - 4 * focusLength * objectRay.origin.y;

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
	double zmax = std::max( lengthXMin, lengthXMax );
	double z1 = ( ( zmax - lengthXMin ) / 2 ) + ( (lengthXMin - lengthXMax ) / ( 2 * ( xmax - xmin ) ) ) * ( hitPoint.x - xmin );
	double z2 = ( ( zmax + lengthXMin) / 2 )  + ( ( (lengthXMax - lengthXMin ) / ( 2 * ( xmax - xmin ) ) )  * ( hitPoint.x - xmin ) );

	double y1 = ( xmin * xmin ) / ( 4 * focusLength );
	double y2 = ( xmax * xmax ) / ( 4 * focusLength );

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

		z1 = ( ( zmax - lengthXMin) / 2 ) + ( (lengthXMin - lengthXMax ) / ( 2 * ( xmax - xmin ) ) ) * ( hitPoint.x - xmin );
		z2 = ( ( zmax + lengthXMin ) / 2 )  + ( ( (lengthXMax - lengthXMin ) / ( 2 * ( xmax - xmin ) ) )  * ( hitPoint.x - xmin ) );

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
	double u =  (hitPoint.x -xmin ) / (xmax -xmin ) ;

	z1 = ( ( zmax - lengthXMin ) / 2 ) + ( (lengthXMin - lengthXMax) / ( 2 * ( xmax - xmin ) ) ) * ( hitPoint.x - xmin );
	z2 = ( ( zmax + lengthXMin) / 2 )  + ( ( (lengthXMax - lengthXMin) / ( 2 * ( xmax - xmin ) ) )  * ( hitPoint.x - xmin ) );

	double v = ( hitPoint.z - z1 ) / (z2 - z1);


	// Compute parabaloid \dpdu and \dpdv
	Vector3D dpdu(1.0, hitPoint.x /( 2.0 * focusLength ), 0.0);
	Vector3D dpdv(0.0, 0.0, 1.0);

	// Compute parabaloid \dndu and \dndv
	Vector3D d2Pduu ( 0.0, 1.0/( 2.0* focusLength ), 0.0 );
	Vector3D d2Pduv ( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv ( 0.0, 0.0, 0.0 );


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
							   u, v );

	*isShapeFront =  ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;


///////////////////////////////////////////////////////////////////////////////////////

	// Update _tHit_ for quadric intersection
	*tHit = thit;
	return ( true );
}

/*!
 * Not implemented yet.
 */
void ShapeTroughParabola::Draw() const
{

}

/*
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
*/
