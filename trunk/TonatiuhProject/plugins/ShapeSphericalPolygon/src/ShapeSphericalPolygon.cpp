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

#include <QMessageBox>
#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "BBox.h"
#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeSphericalPolygon.h"
#include "tgf.h"
#include "tgc.h"
#include "Vector3D.h"


SO_NODE_SOURCE(ShapeSphericalPolygon);

void ShapeSphericalPolygon::initClass()
{
	SO_NODE_INIT_CLASS( ShapeSphericalPolygon, TShape, "TShape" );
}

ShapeSphericalPolygon::ShapeSphericalPolygon()
:m_lastValidSidesValue( 6 ),
 m_lastValidSphereRadiusValue( 0.5 ),
 m_radiusSensor( 0 ),
 m_sidesSensor( 0 ),
 m_sphereRadiusSensor( 0 )
{
	SO_NODE_CONSTRUCTOR( ShapeSphericalPolygon );
	SO_NODE_ADD_FIELD( sphereRadius, (0.5) );
	SO_NODE_ADD_FIELD( radius, (0.5) );
	SO_NODE_ADD_FIELD( polygonSides, (6) );

	SO_NODE_DEFINE_ENUM_VALUE( Side, INSIDE );
	SO_NODE_DEFINE_ENUM_VALUE( Side, OUTSIDE );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
	SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );

    m_sphereRadiusSensor = new SoFieldSensor( SphereRadiusChanged, this );
    m_sphereRadiusSensor->setPriority( 0 );
    m_sphereRadiusSensor->attach( &sphereRadius );

    m_radiusSensor = new SoFieldSensor( RadiusChanged, this );
    m_radiusSensor->setPriority( 0 );
    m_radiusSensor->attach( &radius );

    m_sidesSensor = new SoFieldSensor( SidesChanged, this );
    m_sidesSensor->setPriority( 0 );
    m_sidesSensor->attach( &polygonSides );
}

ShapeSphericalPolygon::~ShapeSphericalPolygon()
{
	delete m_sphereRadiusSensor;
	delete m_radiusSensor;
	delete m_sidesSensor;
}

SoNode* ShapeSphericalPolygon::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	ShapeSphericalPolygon* newShapeSphericalPolygon = dynamic_cast< ShapeSphericalPolygon* >( SoNode::copy( copyConnections ) );

	return newShapeSphericalPolygon;
}


double ShapeSphericalPolygon::GetArea() const
{
	return -1;
}

BBox ShapeSphericalPolygon::GetBBox() const
{
	double xmax = radius.getValue();
	double ymax = radius.getValue();
	double thetaMax = asin( radius.getValue() / sphereRadius.getValue() );

	double zmax = sphereRadius.getValue() * ( 1 - cos( thetaMax ) );

	return BBox( Point3D( - xmax, - ymax, 0 ), Point3D( xmax, ymax, zmax) );
}

QString ShapeSphericalPolygon::GetIcon() const
{
	return ":/icons/ShapeSphericalPolygon.png";
}

bool ShapeSphericalPolygon::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	double A = objectRay.direction().lengthSquared();
	double B = 2.0 * ( objectRay.origin.x *  objectRay.direction().x
					 + objectRay.origin.y *  objectRay.direction().y
					 + objectRay.origin.z *  objectRay.direction().z
					 - objectRay.direction().z * sphereRadius.getValue() );

	double C = objectRay.origin.x * objectRay.origin.x
			 + objectRay.origin.y * objectRay.origin.y
			 + objectRay.origin.z * objectRay.origin.z
			 - 2 * objectRay.origin.z * sphereRadius.getValue();

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !tgf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double thit = ( t0 > objectRay.mint )? t0 : t1 ;
	if( thit > objectRay.maxt ) return false;

	//Evaluate Tolerance
	double tol = 0.00001;

	// Compute ShapeSphere hit position and $\phi$
	Point3D hitPoint = objectRay( thit );
	//double phi = atan2( hitPoint.y, hitPoint.x );
	//if ( phi < 0. ) phi += tgc::TwoPi;

	Vector3D nR = Normalize( Vector3D( hitPoint.x, hitPoint.y, 0.0 ) );
	double phi = atan2( hitPoint.x, hitPoint.y );
	if( phi < 0.0 )	phi += tgc::TwoPi;

	double u = phi / tgc::TwoPi;
	double centralAngle = tgc::TwoPi / polygonSides.getValue();

	double part  =  floor( phi / centralAngle );
	if( fabs( u - 1.0 ) < tgc::Epsilon ) 	part = polygonSides.getValue() - 1;

	double t2 = radius.getValue() * cos( 0.5 * centralAngle  )
							* ( 1 / cos( 0.5 * centralAngle  - phi + centralAngle * part ) );
	double x = sin( phi ) * t2;
	double y = cos( phi )* t2;
	Vector3D tPoint = Vector3D( x, y,  sphereRadius.getValue()- sqrt( sphereRadius.getValue() * sphereRadius.getValue() - x * x - y * y  ) );
	double zRadius = sqrt( hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y );

	if( (thit - objectRay.mint) < tol || zRadius > radius.getValue() || Vector3D( hitPoint).length() > tPoint.length() )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;

		thit = t1;
		hitPoint = objectRay( thit );

		Vector3D nR = Normalize( Vector3D( hitPoint.x, hitPoint.y, 0.0 ) );
		phi = atan2( hitPoint.x, hitPoint.y );
		if( phi < 0.0 )	phi += tgc::TwoPi;

		u = phi / tgc::TwoPi;
		part  =  floor( phi / centralAngle );
		if( fabs( u - 1.0 ) < tgc::Epsilon ) 	part = polygonSides.getValue() - 1;

		t2 = radius.getValue() * cos( 0.5 * centralAngle  )
										* ( 1 / cos( 0.5 * centralAngle  - phi + centralAngle * part ) );
		x = sin( phi ) * t2;
		y = cos( phi )* t2;
		tPoint = Vector3D( x, y,  sphereRadius.getValue()- sqrt( sphereRadius.getValue() * sphereRadius.getValue() - x * x - y * y  ) );


		zRadius = sqrt( hitPoint.x * hitPoint.x + hitPoint.y * hitPoint.y );

		if( (thit - objectRay.mint) < tol || zRadius > radius.getValue() || Vector3D( hitPoint).length() > tPoint.length() )	return false;
	}

	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );

	double v = Vector3D( hitPoint).length()/ tPoint.length();

	// Compute ShapeSphere \dpdu and \dpdv
	double theta = acos( ( sphereRadius.getValue() - hitPoint.z ) / sphereRadius.getValue() );
	double thetaMax =  asin( radius.getValue() / sphereRadius.getValue() ) ;

	// Compute sphere \dpdu and \dpdv
	Vector3D dpdu( tgc::TwoPi * radius.getValue() * cos( phi ) * sin( theta ),
					- tgc::TwoPi * radius.getValue() * sin( phi ) *sin( theta ),
					0.0 );
	Vector3D dpdv( radius.getValue() * thetaMax * cos( theta ) * sin( phi ),
					radius.getValue() * thetaMax * cos( phi ) * cos( theta ),
					radius.getValue() * thetaMax * sin( theta ) );

	Vector3D d2Pduu( -4  * tgc::Pi * tgc::Pi * radius.getValue() * sin( phi ) * sin( theta ),
					 -4  * tgc::Pi * tgc::Pi * radius.getValue() * cos( phi ) * sin( theta ),
					 0.0 );
	Vector3D d2Pduv( tgc::TwoPi * radius.getValue() * thetaMax * cos( phi ) * cos( theta ),
					- tgc::TwoPi * radius.getValue() * thetaMax * sin( phi ) * cos( theta ),
						 0.0 );

	Vector3D d2Pdvv( -radius.getValue() * thetaMax * thetaMax * sin( phi )* sin( theta ),
			-radius.getValue() * thetaMax * thetaMax * cos( phi )* sin( theta ),
			radius.getValue() * thetaMax * thetaMax * cos( theta ) );


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

bool ShapeSphericalPolygon::IntersectP( const Ray& worldRay ) const
{
	return Intersect( worldRay, 0, 0 );
}

Point3D ShapeSphericalPolygon::Sample( double u, double v) const
{
	return GetPoint3D( u , v );
}

Point3D ShapeSphericalPolygon::GetPoint3D( double u, double v ) const
{
	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function ShapeSphericalPolygon::GetPoint3D called with invalid parameters" );

	double centralAngle = tgc::TwoPi / polygonSides.getValue();
	double phi = u * tgc::TwoPi;

	double part  =  floor( phi / centralAngle );
	if( fabs( u - 1.0 ) < tgc::Epsilon ) 	part = polygonSides.getValue() - 1;
	//double u1 = ( u - ( part * 1 / polygonSides.getValue() ) )*polygonSides.getValue();

	Point3D aPoint = Point3D( sin( part* centralAngle ), cos(  part* centralAngle ), 0.0  ) * radius.getValue();
	Point3D bPoint = Point3D( sin( ( part + 1 ) * centralAngle ), cos( ( part + 1 ) * centralAngle ), 0.0  ) * radius.getValue();
	double t = 0.5 *  ( 1 / sin( 0.5 * centralAngle ) )
	               * (1 / cos( 0.5 * centralAngle - phi + centralAngle  * part ) )
	               * sin( phi - centralAngle * part );
	Point3D tPoint = aPoint + ( bPoint - aPoint) * t;

	double x = v * tPoint.x;
	double y = v * tPoint.y;
	double z = sphereRadius.getValue();
	if( fabs( sphereRadius.getValue() *  sphereRadius.getValue() - x  * x  - y * y  ) > tgc::Epsilon )
		z = sphereRadius.getValue() - sqrt( sphereRadius.getValue() *  sphereRadius.getValue()
												- x  * x  - y * y );

	return Point3D( x, y, z );
}

NormalVector ShapeSphericalPolygon::GetNormal(  double u, double v  ) const
{
	Point3D point = GetPoint3D( u, v );
	Vector3D vector( point.x, point.y, point.z - sphereRadius.getValue());
	return NormalVector( -point.x/ vector.length(), -point.y/vector.length(), -( point.z - sphereRadius.getValue() )/vector.length() );
}

bool ShapeSphericalPolygon::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1 ) );
}

void ShapeSphericalPolygon::RadiusChanged( void* data, SoSensor* )
{
	ShapeSphericalPolygon* polygon = static_cast< ShapeSphericalPolygon* >( data );

	if( ( polygon->radius.getValue() <= 0.0 ) || ( polygon->radius.getValue() > polygon->sphereRadius.getValue() ) )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "The polygon radius must be a positive value and less than radius value." ) );
		polygon->sphereRadius.setValue( polygon->m_lastValidSphereRadiusValue );

	}
	else
		polygon-> m_lastValidSphereRadiusValue = polygon->sphereRadius.getValue();
}

void ShapeSphericalPolygon::SidesChanged( void* data, SoSensor* )
{
    ShapeSphericalPolygon* polygon = static_cast< ShapeSphericalPolygon* >( data );
	if( polygon->polygonSides.getValue() < 3 )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "The polygon sides must be at least 3." ) );
		polygon->polygonSides.setValue( polygon->m_lastValidSidesValue );

	}
	else
		polygon-> m_lastValidSidesValue = polygon->polygonSides.getValue();
}

void ShapeSphericalPolygon::SphereRadiusChanged( void* data, SoSensor* )
{
	ShapeSphericalPolygon* polygon = static_cast< ShapeSphericalPolygon* >( data );
	if( ( polygon->sphereRadius.getValue() <= 0.0 ) || ( polygon->radius.getValue() > polygon->sphereRadius.getValue() ) )
	{
		QMessageBox::warning( 0, QString( "Tonatiuh" ), QString( "The polygon sphereRadius value must be a positive value and largest than radius value." ) );
		polygon->sphereRadius.setValue( polygon->m_lastValidSphereRadiusValue );

	}
	else
		polygon-> m_lastValidSphereRadiusValue = polygon->sphereRadius.getValue();
}


void ShapeSphericalPolygon::computeBBox(SoAction *, SbBox3f& box, SbVec3f& center)
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
	SbVec3f min, max;

	min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
	max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );

	// Set the box to bound the two extreme points.
	box.setBounds( min, max );
}


void ShapeSphericalPolygon::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;

    // Access the state from the action.
    SoState* state = action->getState();

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    // If we need to generate texture coordinates with a
    // function, we'll need an SoGLTextureCoordinateElement.
    // Otherwise, we'll set up the coordinates directly.
    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

    double partUMax = 1.0 /polygonSides.getValue();
    for( int side = 0.0; side < polygonSides.getValue(); side++ )
	{
		const int rows = 20; // Number of points per row
	    const int columns = 20; // Number of points per column
	    const int totalPoints = (rows)*(columns); // Total points in the grid

	    float vertex[totalPoints][6];

	    int h = 0;
	    double ui = 0;
		double vj = 0;

		for (int i = 0; i < rows; ++i )
		{
			ui = ( side * partUMax ) + ( partUMax / ( rows - 1 ) ) * i;

			for ( int j = 0 ; j < columns ; ++j )
			{

				vj = ( 1.0 /(double)(columns-1) ) * j;

				Point3D point = GetPoint3D(ui, vj);
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

}
