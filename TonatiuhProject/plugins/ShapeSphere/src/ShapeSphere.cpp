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
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "gc.h"
#include "gf.h"
#include "ParameterValueException.h"
#include "Ray.h"
#include "ShapeSphere.h"

SO_NODE_SOURCE(ShapeSphere);

void ShapeSphere::initClass()
{
	SO_NODE_INIT_CLASS(ShapeSphere, TShape, "TShape");
}

ShapeSphere::ShapeSphere( )
{
	SO_NODE_CONSTRUCTOR(ShapeSphere);
	SO_NODE_ADD_FIELD( radius, (0.5) );
	SO_NODE_ADD_FIELD( yMin, (-0.5) );
	SO_NODE_ADD_FIELD( yMax, (0.5) );
	SO_NODE_ADD_FIELD( phiMax, ( gc::TwoPi) );

	SO_NODE_DEFINE_ENUM_VALUE( Side, INSIDE );
	SO_NODE_DEFINE_ENUM_VALUE( Side, OUTSIDE );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
	SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );
}

ShapeSphere::~ShapeSphere()
{
}

BBox ShapeSphere::GetBBox() const
{
	double cosPhiMax = cos( phiMax.getValue() );
   	double sinPhiMax = sin( phiMax.getValue() );

   	double thetaMin = acos( yMax.getValue() / radius.getValue() );
   	double thetaMax = acos( yMin.getValue()/radius.getValue() );
   	double maxRadius = ( yMax.getValue() * yMin.getValue() > 0.0 ) ?  std::max( sin(thetaMin) * radius.getValue() , sin(thetaMax) * radius.getValue() )
																	: radius.getValue();
   	double minRadius = std::min( sin(thetaMin) * radius.getValue(), sin(thetaMax) * radius.getValue() );
   	double xmin = ( phiMax.getValue() < gc::Pi  ) ?  0.0
								: ( phiMax.getValue() < 1.5 * gc::Pi  ) ? sinPhiMax * maxRadius
										: -maxRadius;
   	double xmax = ( phiMax.getValue() >= gc::Pi / 2 ) ? maxRadius : sinPhiMax * maxRadius;

   	double ymin = yMin.getValue();
	double ymax = yMax.getValue();

	double zmin = ( phiMax.getValue() > gc::Pi ) ? -maxRadius
					:( phiMax.getValue() > gc::Pi / 2 ) ? maxRadius* cosPhiMax : std::min( maxRadius * cosPhiMax, minRadius * cosPhiMax );
	double zmax = maxRadius;

	return BBox( Point3D( xmin, ymin, zmin), Point3D( xmax, ymax, zmax) );
}

std::string ShapeSphere::GetIcon() const
{
	return ( ":/icons/ShapeSphere.png" );
}

bool ShapeSphere::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	// Compute quadratic ShapeSphere coefficients
	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A = objectRay.direction().lengthSquared();
    double B = 2.0 * DotProduct( vObjectRayOrigin, objectRay.direction() );
	double C = vObjectRayOrigin.lengthSquared() - radius.getValue() * radius.getValue();

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
    double thit = ( t0 > objectRay.mint )? t0 : t1 ;
    if( thit > objectRay.maxt ) return false;

    //Evaluate Tolerance
	double tol = 0.00001;

	// Compute ShapeSphere hit position and $\phi$
    Point3D hitPoint = objectRay( thit );
	double phi = atan2( hitPoint.x, hitPoint.z );
	if ( phi < 0. ) phi += gc::TwoPi;

	// Test intersection against clipping parameters
	if( (thit - objectRay.mint) < tol || hitPoint.y < yMin.getValue() || hitPoint.y > yMax.getValue() || phi > phiMax.getValue() )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		// Compute ShapeSphere hit position and $\phi$
		hitPoint = objectRay( thit );
		phi = atan2( hitPoint.x, hitPoint.z );
	    if ( phi < 0. ) phi += gc::TwoPi;

		if ( (thit - objectRay.mint) < tol || hitPoint.y < yMin.getValue() || hitPoint.y > yMax.getValue() || phi > phiMax.getValue() )	return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );

	// Find parametric representation of ShapeSphere hit
	double theta = acos( hitPoint.y / radius.getValue() );
	double thetaMin = acos( yMax.getValue() / radius.getValue() );
	double thetaMax = acos( yMin.getValue()/radius.getValue() );
	double u = ( theta - thetaMin ) / ( thetaMax - thetaMin );
	double v = phi / phiMax.getValue();

	// Compute ShapeSphere \dpdu and \dpdv
	Vector3D dpdu( radius.getValue() * ( -thetaMin + thetaMax ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) * sin( phiMax.getValue() * v ),
					radius.getValue() * ( -thetaMin + thetaMax ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					radius.getValue() * ( -thetaMin + thetaMax ) * cos( phiMax.getValue() * v ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) );

	Vector3D dpdv( -phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					0.0,
					phiMax.getValue() * radius.getValue() * sin( phiMax.getValue() * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ) );

	// Compute ShapeSphere \dndu and \dndv
	Vector3D d2Pduu(  -radius.getValue() * ( thetaMin - thetaMax ) * ( -thetaMin + thetaMax ) * sin( phiMax.getValue() * v ) * sin( (-1 + u) * thetaMin - u * thetaMax ),
					radius.getValue() * ( thetaMin - thetaMax ) * ( -thetaMin + thetaMax ) * cos( (-1 + u) * thetaMin - u * thetaMax ),
					-radius.getValue() * ( thetaMin - thetaMax ) * ( -thetaMin + thetaMax ) * cos( phiMax.getValue() * v ) * sin( (-1 + u) * thetaMin -u * thetaMax )  );

	Vector3D d2Pduv( phiMax.getValue() * radius.getValue() * ( -thetaMin + thetaMax ) * cos( phiMax.getValue() * v ) * cos( (-1 + u ) * thetaMin - u  * thetaMax ),
					0.0,
					-phiMax.getValue() * radius.getValue() * ( -thetaMin + thetaMax ) * cos( (-1 + u ) * thetaMin - u  * thetaMax ) * sin( phiMax.getValue() * v ) );

	Vector3D d2Pdvv( phiMax.getValue() * phiMax.getValue() * radius.getValue() * sin( phiMax.getValue() * v ) * sin( (-1 + u) * thetaMin - u * thetaMax ),
					0.0,
					phiMax.getValue() * phiMax.getValue() *  radius.getValue() * cos( phiMax.getValue() * v ) * sin( (-1 + u) * thetaMin - u * thetaMax ) );

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
		                        u, v, this );

	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction() ) > 0 ) ? false : true;
    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeSphere::IntersectP( const Ray& ray ) const
{
	return Intersect( ray, 0, 0 );
}

/*!
* Returns the 3D coordintates por parameteric coordinates \a u and \a v
*/
Point3D ShapeSphere::Sample( double u, double v ) const
{
	if ( OutOfRange( u, v ) ) gf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );


	double thetaMin = acos( yMax.getValue() / radius.getValue() );
	double thetaMax = acos( yMin.getValue()/radius.getValue() );
	double theta = u *( thetaMax - thetaMin ) + thetaMin;
	double phi = v * phiMax.getValue();

	double x = radius.getValue() * sin( theta ) * sin( phi );
	double y = radius.getValue() * cos( theta );
	double z = radius.getValue() * sin( theta ) * cos( phi );

	return Point3D (x, y, z);
}

/*!
* Checks the cone parameters values. Checks the \a value of parameter \a name .
*/
bool ShapeSphere::ValidateParamaterValue( std::string name, std::string value ) const
{
    if( name == "radius" && std::stod( value ) < 0 ) 
		throw ParameterValueException( "radius", "The radius of the sphere, must be a positive number" );
	else if( name == "radius" && std::stod( value ) < std::max( std::fabs( yMin.getValue() ), std::fabs( yMax.getValue() ) ) ) 
		throw ParameterValueException( "radius", "The sphere radius must equal or greater than y values" );
		
	else if( name == "yMin" && std::stod( value ) > ( yMax.getValue() ) )
		throw ParameterValueException( "yMin", "The sphere's minimum y-value must be less than its maximum y-value" );
	else if( name == "yMin" && std::stod( value ) < ( -radius.getValue() ) )
		throw ParameterValueException( "yMin", "The sphere's minimum y-value must lie within the range [-radius,radius]" );
	else if( name == "yMin" && std::stod( value ) > ( radius.getValue() ) )
		throw ParameterValueException( "yMin", "The sphere's minimum y-value must lie within the range [-radius,radius]" );
		
	else if( name == "yMax" && std::stod( value ) < ( yMin.getValue() ) )
		throw ParameterValueException( "yMax", "The sphere's maximum y-value must be greater than its minimum y-value" );
	else if( name == "yMax" && std::stod( value ) < ( -radius.getValue() ) )
		throw ParameterValueException( "yMax", "The sphere's maximum y-value must lie within the range [-radius,radius]" );
	else if( name == "yMax" && std::stod( value ) > ( radius.getValue() ) )
		throw ParameterValueException( "yMax", "The sphere's maximum y-value must lie within the range [-radius,radius]" );
		
	else if( name == "phiMax" && std::stod( value ) < 0 )
		throw ParameterValueException( "phiMax", "The sphere’s generation angle must be a positive value" );

	return true;
}

NormalVector ShapeSphere::GetNormal(double u, double v ) const
{
	double thetaMin = acos( yMax.getValue() / radius.getValue() );
	double thetaMax = acos( yMin.getValue()/radius.getValue() );

	Vector3D dpdu( radius.getValue() * ( -thetaMin + thetaMax ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) * sin( phiMax.getValue() * v ),
					radius.getValue() * ( -thetaMin + thetaMax ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					radius.getValue() * ( -thetaMin + thetaMax ) * cos( phiMax.getValue() * v ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) );

	Vector3D dpdv( -phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					0.0,
					phiMax.getValue() * radius.getValue() * sin( phiMax.getValue() * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ) );
	NormalVector normal = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
	return normal;
}

bool ShapeSphere::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeSphere::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
	SbVec3f min, max;

	min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
	max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );

	// Set the box to bound the two extreme points.
	box.setBounds(min, max);
}

void ShapeSphere::generatePrimitives(SoAction *action)
{
	SoPrimitiveVertex   pv;
	SoState  *state = action->getState();

	SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
						  SoTextureCoordinateElement::FUNCTION );

	const SoTextureCoordinateElement* tce = 0;
	if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	SbVec3f  point;
	int rows = 50; // Number of points per row
	int columns = 50; // Number of points per column
	int totalPoints = rows * columns; // Total points in the grid

	float vertex[totalPoints][6];

	int h = 0;
	double ui = 0;
	double vj = 0;

	for (int i = 0; i < rows; ++i )
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

	const int totalIndices  = (rows-1) * (columns-1) * 4;
	int32_t* indices = new int32_t[totalIndices];
	int k = 0;
	for( int irow = 0; irow < (rows-1); ++irow )
		for( int icolumn = 0; icolumn < (columns-1); ++icolumn )
		{

			indices[k] = irow * columns + icolumn;
			indices[k+1] = irow * columns + icolumn + 1;
			indices[k+2] = irow * columns + icolumn + columns + 1;
			indices[k+3] = irow * columns + icolumn + columns;

			k+=4; //Set k to the first point of the next face.
		}


	float finalvertex[totalIndices][6];
	for(int ivert = 0; ivert<totalIndices; ++ivert )
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
	for( int i = 0; i < totalIndices; ++i )
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
