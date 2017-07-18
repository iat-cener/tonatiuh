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

#include "Point3D.h"
#include "DifferentialGeometry.h"

#include "ShapeFlatTriangle.h"
#include "TParameterList.h"
#include "TParameterPoint3D.h"


TNodeType ShapeFlatTriangle::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* ShapeFlatTriangle::CreateInstance( )
{
  return ( new ShapeFlatTriangle() );
}

/*!
 * Initializes ShapeFlatDisk type.
 */
void ShapeFlatTriangle::Init()
{
	ShapeFlatTriangle::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), QString( "ShapeFlatTriangle" ), &ShapeFlatTriangle::CreateInstance );
}

/*!
 * Default constructor, initializes node instance.
 */
ShapeFlatTriangle::ShapeFlatTriangle(  )
:TShape()
{
	setObjectName(GetType().GetName());

	//Parameters
	QVariant aParameter;
	aParameter.setValue( new TParameterPoint3D( Point3D( -0.5, 0.0, 0.0 ) ) );
	m_parametersList->Append( QLatin1String("a"), aParameter );

	QVariant bParameter;
	bParameter.setValue( new TParameterPoint3D( Point3D( 0.5, 0.0, 0.0 ) ) );
	m_parametersList->Append( QLatin1String("b"), bParameter );

	QVariant cParameter;
	cParameter.setValue( new TParameterPoint3D( Point3D( 0.0, 1.0, 0.0 ) ) );
	m_parametersList->Append( QLatin1String("c"), cParameter );
}

/*!
 * Destructor.
 */
ShapeFlatTriangle::~ShapeFlatTriangle()
{
	TParameterPoint3D* aPoint = m_parametersList->GetValue( QLatin1String("a") ).value<TParameterPoint3D*>();
	delete aPoint;
	aPoint = 0;

	TParameterPoint3D* bPoint = m_parametersList->GetValue( QLatin1String("b") ).value<TParameterPoint3D*>();
	delete bPoint;
	bPoint = 0;

	TParameterPoint3D* cPoint = m_parametersList->GetValue( QLatin1String("c") ).value<TParameterPoint3D*>();
	delete cPoint;
	cPoint = 0;

}

/*!
 * Returns the filename that stores the shape icon.
 */
QString ShapeFlatTriangle::GetIcon()
{

	return QLatin1String( ":/icons/ShapeFlatTriangle.png" );
}


/*!
 * Returns the bounding box for the defined parameters values.
 */
BBox ShapeFlatTriangle::GetBondingBox() const
{

	Point3D aPoint = m_parametersList->GetValue( QLatin1String("a") ).value<TParameterPoint3D*>()->GetPoint3D();
	Point3D bPoint = m_parametersList->GetValue( QLatin1String("b") ).value<TParameterPoint3D*>()->GetPoint3D();
	Point3D cPoint = m_parametersList->GetValue( QLatin1String("c") ).value<TParameterPoint3D*>()->GetPoint3D();

	double xmin = std::min( aPoint.x, std::min( bPoint.x, cPoint.x ) );
	double xmax = std::max( aPoint.x, std::max( bPoint.x, cPoint.x ) );
	double ymin = std::min( aPoint.y, std::min( bPoint.y, cPoint.y ) );
	double ymax = std::max( aPoint.y, std::max( bPoint.y, cPoint.y ) );
	double zmin = std::min( aPoint.z, std::min( bPoint.z, cPoint.z ) );
	double zmax = std::max( aPoint.z, std::max( bPoint.z, cPoint.z ) );

	return BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) );
}

/*!
 * Returns the type of node.
 */
TNodeType ShapeFlatTriangle::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Calculates the intersection of the \a objectRay with this surface. The ray is in the surface local coordinates system.
 * Returns false if the ray does not intersect with the surface. \a tHit, \a dg and \a isShapeFront is the information generated about the intersection.
 */
bool ShapeFlatTriangle::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{

	Point3D aPoint = m_parametersList->GetValue( QLatin1String("a") ).value<TParameterPoint3D*>()->GetPoint3D();
	Point3D bPoint = m_parametersList->GetValue( QLatin1String("b") ).value<TParameterPoint3D*>()->GetPoint3D();
	Point3D cPoint = m_parametersList->GetValue( QLatin1String("c") ).value<TParameterPoint3D*>()->GetPoint3D();


	Vector3D vAB = Vector3D( bPoint - aPoint );
	Vector3D vAC = Vector3D( cPoint - aPoint );

	Vector3D vN = CrossProduct( vAB, vAC );
	double d = -vN.x * aPoint.x - vN.y * aPoint.y - vN.z * aPoint.z;
	//if( objectRay.direction.z ==0.0 ) return false;

	double thit = (-d - vN.x * objectRay.origin.x - vN.y * objectRay.origin.y - vN.z * objectRay.origin.z )
			/ (vN.x * objectRay.direction().x  + vN.y * objectRay.direction().y + vN.z * objectRay.direction().z );
	//double thit = - objectRay.origin.z / objectRay.direction.z;

	//Evaluate Tolerance
	double tol = 0.00001;
	if( thit > objectRay.maxt ) return false;
	if( (thit - objectRay.mint) < tol ) return false;

	Point3D hitPoint = objectRay( thit );

	// is hitPoint inside triangle?
	//double uu, uv, vv, wu, wv, D;
	double uu = DotProduct( vAB, vAB );
	double uv = DotProduct( vAB, vAC );
	double vv = DotProduct( vAC, vAC );
	Vector3D  w = Vector3D( hitPoint - aPoint );
	double wu = DotProduct( w, vAB );
	double wv = DotProduct( w, vAC );
	double D = uv * uv - uu * vv;

	// get and test parametric coords
	double u = (uv * wv - vv * wu) / D;
	if( u < 0.0 || u > 1.0 )	return false;

	double v = ( uv * wu - uu * wv ) / D;
	if( v < 0.0 || ( u + v) > 1.0)	return false;

	// Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	//if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	//else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeFlatTriangle::Intersect(...) called with null pointers" );

	Vector3D dpdu = vAB;
	Vector3D dpdv = vAC;

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
			                        u, v );

	*isShapeFront = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

	// Update _tHit_ for quadric intersection
	*tHit = thit;
	return ( true );
}

/*!
 * Not implemented yet.
 */
void ShapeFlatTriangle::Draw() const
{

}

