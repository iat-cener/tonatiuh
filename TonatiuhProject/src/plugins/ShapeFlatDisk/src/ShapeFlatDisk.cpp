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
#include "gf.h"
#include "NormalVector.h"

#include "ShapeFlatDisk.h"


TNodeType ShapeFlatDisk::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > ShapeFlatDisk::CreateInstance( )
{
	//shared_prt needs a public constructor
	struct EnableCreatShapeFlatDisk : public ShapeFlatDisk { using ShapeFlatDisk::ShapeFlatDisk; };
	return std::make_shared<EnableCreatShapeFlatDisk>();
}

/*!
 * Initializes ShapeFlatDisk type.
 */
void ShapeFlatDisk::Init()
{
	ShapeFlatDisk::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Shape" ), "ShapeFlatDisk", &ShapeFlatDisk::CreateInstance );
}

/*!
 * ShapeFlatDisk : public TShape
 */
ShapeFlatDisk::ShapeFlatDisk()
:TShape(),
 m_radiusLabel( "radius" )
{
	SetName(GetType().GetName() );

	//Translation
	m_pParametersList->Append<double>( m_radiusLabel, 0.5, true );
}

/*!
 * Destructor.
 */
ShapeFlatDisk::~ShapeFlatDisk()
{

}

/*!
 * Creates a copy of shape node.
 */
std::shared_ptr< TNode > ShapeFlatDisk::Copy() const
{
	struct EnableCreatShapeFlatDisk : public ShapeFlatDisk { using ShapeFlatDisk::ShapeFlatDisk; };
	std::shared_ptr<ShapeFlatDisk> shapeNode = std::make_unique<EnableCreatShapeFlatDisk>();
	if( shapeNode == nullptr )	return ( 0  );

	//Coping node parts.
	//NO parts

	//Coping the parameters.
	shapeNode->m_pParametersList->SetValue( m_radiusLabel, GetParameterValue<double>( m_radiusLabel ) );

	return ( shapeNode );
}

/*!
 * Returns the filename that stores the shape icon.
 */
std::string ShapeFlatDisk::GetIcon() const
{
	return ( ":/icons/ShapeFlatDisk.png" );
}


/*!
 * Returns disk bounding box for the defined parameters values.
 */
BBox ShapeFlatDisk::GetBondingBox() const
{

	double radius = GetParameterValue<double>( m_radiusLabel );
	Point3D min = Point3D( -radius, 0.0, -radius );
	Point3D max = Point3D( radius, 0.0, radius );

	return BBox( min, max );
}

/*!
 * Returns the type of node.
 */
TNodeType ShapeFlatDisk::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Calculates the intersection of the \a objectRay with this surface. The ray is in the surface local coordinates system.
 * Returns false if the ray does not intersect with the surface. \a tHit, \a dg and \a isShapeFront is the information generated about the intersection.
 */
bool ShapeFlatDisk::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const
{
	double radius = GetParameterValue<double>( m_radiusLabel );

	// Solve equation for _t_ value
	if ( ( objectRay.origin.y == 0 ) && ( objectRay.direction().y == 0 ) ) return false;
	double t = -objectRay.origin.y * objectRay.invDirection().y;
	// Compute intersection distance along ray
	if( t > objectRay.maxt || t < objectRay.mint ) return false;

	//Evaluate Tolerance
	double tol = 0.00001;
	if( (t - objectRay.mint) < tol ) return false;

	// Compute rectangle hit position
	Point3D hitPoint = objectRay( t );

	// Test intersection against clipping parameters
	if( sqrt( hitPoint.x*hitPoint.x + hitPoint.z*hitPoint.z ) > radius ) return ( false );

	// Now check if the funtion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	//if( ( tHit == 0 ) && ( dg == 0 ) ) return ( true );
	//else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function Sphere::Intersect(...) called with null pointers" );

	// Find parametric representation of the rectangle hit point
	double phi = atan2( hitPoint.z, hitPoint.x );
	if ( phi < 0. ) phi += gc::TwoPi;
	double iradius = sqrt( hitPoint.x*hitPoint.x + hitPoint.z*hitPoint.z );

	double u = phi/gc::TwoPi;
	double v = iradius/radius;

	// Compute rectangle \dpdu and \dpdv
	Vector3D dpdu ( -v * radius * sin( u * gc::TwoPi ) * gc::TwoPi, 0.0, v * radius * cos( u * gc::TwoPi ) * gc::TwoPi );
	Vector3D dpdv ( radius * cos( u * gc::TwoPi ), 0.0,  radius * sin( u * gc::TwoPi ) );

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
void ShapeFlatDisk::Draw() const
{

}
