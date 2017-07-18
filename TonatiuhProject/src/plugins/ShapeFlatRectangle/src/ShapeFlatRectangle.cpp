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

#include "DifferentialGeometry.h"

#include "ShapeFlatRectangle.h"
#include "TParameterList.h"


TNodeType ShapeFlatRectangle::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* ShapeFlatRectangle::CreateInstance( )
{
  return ( new ShapeFlatRectangle() );
}

/*!
 * Initializes ShapeFlatDisk type.
 */
void ShapeFlatRectangle::Init()
{
	ShapeFlatRectangle::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), QString( "ShapeFlatRectangle" ), &ShapeFlatRectangle::CreateInstance );
}

/*!
 * Creates ShapeFlatRectangle object
 */
ShapeFlatRectangle::ShapeFlatRectangle()
:TShape()//,
 //m_nodeType(TNodeType::FromName( QLatin1String( "ShapeFlatRectangle" ) ) )
{
	setObjectName(GetType().GetName());

	//Parameters
	m_parametersList->Append( QLatin1String("width"), 1.0 );
	m_parametersList->Append( QLatin1String("height"), 1.0 );
}

/*!
 * Destructor.
 */
ShapeFlatRectangle::~ShapeFlatRectangle()
{

}

/*!
 * Returns the filename that stores the shape icon.
 */
QString ShapeFlatRectangle::GetIcon()
{

	return QLatin1String( ":/icons/ShapeFlatRectangle.png" );
}


/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeFlatRectangle::GetBondingBox() const
{

	double width = m_parametersList->GetValue( QLatin1String("width") ).toDouble();
	double height = m_parametersList->GetValue( QLatin1String("height") ).toDouble();

	Point3D min = Point3D( -height/2, 0.0, -width/2);
	Point3D max = Point3D( height/2, 0.0, width/2);
	return BBox( min, max );
}

/*!
 * Returns the type of node.
 */
TNodeType ShapeFlatRectangle::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Calculates the intersection of the \a objectRay with this surface. The ray is in the surface local coordinates system.
 * Returns false if the ray does not intersect with the surface. \a tHit, \a dg and \a isShapeFront is the information generated about the intersection.
 */
bool ShapeFlatRectangle::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{
	double width = m_parametersList->GetValue( QLatin1String("width") ).toDouble();
	double height = m_parametersList->GetValue( QLatin1String("height") ).toDouble();

	// Solve equation for _t_ value
	if ( ( objectRay.origin.y == 0 ) && ( objectRay.direction().y == 0 ) ) return ( false );
	double t = -objectRay.origin.y * objectRay.invDirection().y;

	// Compute intersection distance along ray
	if( t > objectRay.maxt || t < objectRay.mint ) return ( false );

	//Evaluate Tolerance
	double tol = 0.00001;
	if( (t - objectRay.mint) < tol ) return ( false );

	// Compute rectangle hit position
	Point3D hitPoint = objectRay( t );

	// Test intersection against clipping parameters
	if( hitPoint.x < -height/2 || hitPoint.x > height/2 || hitPoint.z < -width/2 || hitPoint.z > width/2 )
		return ( false );

	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	//if( ( tHit == 0 ) && ( dg == 0 ) ) return ( true );
	//else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function Sphere::Intersect(...) called with null pointers" );


	// Find parametric representation of the rectangle hit point
	double u = ( hitPoint.x + height/2 ) / ( height );
	double v = ( hitPoint.z + width/2 ) / ( width );


	// Compute rectangle \dpdu and \dpdv
	Vector3D dpdu ( 0.0, 0.0, height );
	Vector3D dpdv ( width, 0.0, 0.0 );

	NormalVector N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

	// Compute \dndu and \dndv from fundamental form coefficients
	Vector3D dndu ( 0.0, 0.0, 0.0 );
	Vector3D dndv ( 0.0, 0.0, 0.0 );

	// Initialize _DifferentialGeometry_ from parametric information
	*dg = DifferentialGeometry( hitPoint ,
			                        dpdu,
									dpdv,
			                        dndu,
									dndv,
			                        u, v );

	*isShapeFront = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;

	// Update _tHit_ for quadric intersection
	*tHit = t;
	return ( true );
}

/*!
 * Not implemented yet.
 */
void ShapeFlatRectangle::Draw() const
{

}
