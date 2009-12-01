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

#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeSphericalPolygon.h"
#include "tgf.h"
#include "tgc.h"
#include "Trace.h"
#include "Vector3D.h"


SO_NODE_SOURCE(ShapeSphericalPolygon);

void ShapeSphericalPolygon::initClass()
{
	Trace trace( "ShapeSphericalPolygon::initClass", false );
	SO_NODE_INIT_CLASS( ShapeSphericalPolygon, TShape, "TShape" );
}

ShapeSphericalPolygon::ShapeSphericalPolygon()
{
	Trace trace( "ShapeSphericalPolygon::ShapeSphericalPolygon", false );

	SO_NODE_CONSTRUCTOR( ShapeSphericalPolygon );
	SO_NODE_ADD_FIELD( sphereRadius, (0.5) );
	SO_NODE_ADD_FIELD( radius, (0.5) );
	SO_NODE_ADD_FIELD( polygonSides, (6) );

    m_thetaMax = asin( radius.getValue() / sphereRadius.getValue() );
    m_phiMax = tgc::Pi/polygonSides.getValue();
    m_xMax = radius.getValue() * cos( m_phiMax );

	m_sphereRadiusSensor = new SoFieldSensor( SphereRadiusChanged, this );
	m_sphereRadiusSensor->attach( &sphereRadius );

	m_radiusSensor = new SoFieldSensor( RadiusChanged, this );
	m_radiusSensor->attach( &radius );

	m_sidesSensor = new SoFieldSensor( SidesChanged, this );
	m_sidesSensor->attach( &polygonSides );
}

ShapeSphericalPolygon::~ShapeSphericalPolygon()
{
	Trace trace( "ShapeSphericalPolygon::~ShapeSphericalPolygon", false );
}

SoNode* ShapeSphericalPolygon::copy( SbBool copyConnections ) const
{
	Trace trace( "ShapeSphericalPolygon::copy", false );

	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	ShapeSphericalPolygon* newShapeSphericalPolygon = dynamic_cast< ShapeSphericalPolygon* >( SoNode::copy( copyConnections ) );

	return newShapeSphericalPolygon;
}


double ShapeSphericalPolygon::GetArea() const
{
	Trace trace( "ShapeSphericalPolygon::GetArea", false );
	return -1;
}

QString ShapeSphericalPolygon::getIcon()
{
	Trace trace( "ShapeSphericalPolygon::getIcon", false );
	return ":/icons/ShapeSphericalPolygon.png";
}

bool ShapeSphericalPolygon::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	Trace trace( "ShapeSphericalPolygon::Intersect", false );

	// Traslation of the coordinate system from the local coordinates with origin in the bottom of the sphere to the sphere center.
	Ray originBottomRay = objectRay;
	originBottomRay.origin.z -= sphereRadius.getValue();

	// Compute quadratic sphere coefficients
	Vector3D vObjectRayOrigin = Vector3D( originBottomRay.origin );
	double A = originBottomRay.direction.lengthSquared();
    double B = 2.0 * DotProduct( vObjectRayOrigin, originBottomRay.direction );
	double C = vObjectRayOrigin.lengthSquared() - sphereRadius.getValue() * sphereRadius.getValue();

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !tgf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > originBottomRay.maxt || t1 < originBottomRay.mint ) return false;
    double thit = ( t0 > originBottomRay.mint )? t0 : t1 ;
    if( thit > originBottomRay.maxt ) return false;

    //Evaluate Tolerance
	double tol = 0.0001;
	if( (thit - originBottomRay.mint) < tol ) return false;

	// Compute sphere hit position and $\phi$
    Point3D hitPoint = originBottomRay( thit );
	double phi = atan2( hitPoint.y, hitPoint.x );
	if ( phi < 0. ) phi += tgc::TwoPi;

	// Test intersection against ray clipping parameters
	if( hitPoint.z > 0 )
	{
		if ( thit == t1 ) return false;
		if ( t1 > originBottomRay.maxt ) return false;
		thit = t1;

		// Compute sphere hit position and $\phi$
		hitPoint = originBottomRay( thit );
		phi = atan2( hitPoint.y, hitPoint.x );
	    if ( phi < 0. ) phi += tgc::TwoPi;

		if ( hitPoint.z > 0 ) return false;
	}

	// Find parametric representation of sphere hit

	double u = phi / tgc::TwoPi;
	double theta = acos( -hitPoint.z / sphereRadius.getValue() );
	double v = theta  / m_thetaMax ;

	// Test intersection against polygon clipping parameters
	if(!IsInside(u,v)) return false;

	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function Hexagon::Intersect(...) called with null pointers" );


	// Compute sphere \dpdu and \dpdv
	double zradius = sqrt( hitPoint.x*hitPoint.x + hitPoint.y*hitPoint.y );
	double invzradius = 1.0 / zradius;
	double cosphi = hitPoint.x * invzradius;
	double sinphi = hitPoint.y * invzradius;
	Vector3D dpdu= ( tgc::Pi - m_thetaMax ) *
	                  Vector3D( hitPoint.z * cosphi, hitPoint.z * sinphi, -sphereRadius.getValue() * sin( theta ) );

	Vector3D dpdv( -tgc::TwoPi * hitPoint.y, tgc::TwoPi * hitPoint.x, 0.0 );

	// Compute sphere \dndu and \dndv
	Vector3D d2Pduu = -tgc::TwoPi *tgc::TwoPi * Vector3D( hitPoint.x, hitPoint.y, 0 );
	Vector3D d2Pduv = ( tgc::Pi - m_thetaMax ) * hitPoint.z * tgc::TwoPi * Vector3D( -sinphi, cosphi, 0. );
	Vector3D d2Pdvv = -( tgc::Pi - m_thetaMax ) * ( tgc::Pi - m_thetaMax ) * Vector3D( hitPoint.x, hitPoint.y, hitPoint.z );

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

	// Back to the Shape coordinate sistem with the origin in the botom of the sphere.
	hitPoint.z += sphereRadius.getValue();

	*dg = DifferentialGeometry( hitPoint ,
		                        dpdu,
								dpdv,
		                        dndu,
								dndv,
		                        u, v, this );
	dg->shapeFrontSide = ( DotProduct( N, objectRay.direction ) > 0 ) ? false : true;

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeSphericalPolygon::IntersectP( const Ray& worldRay ) const
{
	Trace trace( "ShapeSphericalPolygon::IntersectP", false );
	return Intersect( worldRay, 0, 0 );
}

Point3D ShapeSphericalPolygon::Sample( double u, double v) const
{
	Trace trace( "ShapeSphericalPolygon::Sample", false );
	return GetPoint3D( u , v );
}

Point3D ShapeSphericalPolygon::GetPoint3D( double u, double v ) const
{
	Trace trace( "ShapeSphericalPolygon::GetPoint3D", false );

	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function ShapeSphericalPolygon::GetPoint3D called with invalid parameters" );
	if( !IsInside( u, v ) ) tgf::SevereError( "Function ShapeSphericalPolygon::GetPoint3D called with parameters outside the polygon" );

	double phi = u * tgc::TwoPi;
	double theta = v * m_thetaMax;
	double sintheta = sin( theta );

	double x = sphereRadius.getValue() * sintheta * cos( phi );
	double y = sphereRadius.getValue() * sintheta * sin( phi );
	double z = sphereRadius.getValue() * ( 1 - cos( m_thetaMax ) );

	Point3D point = Point3D( x, y, z );

	return point;
}

NormalVector ShapeSphericalPolygon::GetNormal(  double u, double v  ) const
{
	Trace trace( "ShapeSphericalPolygon::GetNormal", false );

	Point3D point = GetPoint3D( u, v );
	Vector3D vector( point.x, point.y, point.z - sphereRadius.getValue());
	return NormalVector( -point.x/ vector.length(), -point.y/vector.length(), -( point.z - sphereRadius.getValue() )/vector.length() );
}

bool ShapeSphericalPolygon::IsInside( double u, double v ) const
{
	Trace trace( "ShapeSphericalPolygon::IsInside", false );

	double phi = u * tgc::TwoPi;
	double theta = v * m_thetaMax;

	double ratio = phi/m_phiMax;
	double integerPart;
	double fractionalPart = modf( ratio, &integerPart );
	double phiReduced =  (tgf::IsOdd( integerPart ) ? ( (1.0 - fractionalPart) * m_phiMax ) : ( fractionalPart * m_phiMax ) ) ;

	double xReduced = sphereRadius.getValue() * sin( theta ) * cos( phiReduced );
	return ( xReduced > m_xMax ) ? false : true;
}


bool ShapeSphericalPolygon::OutOfRange( double u, double v ) const
{
	Trace trace( "ShapeSphericalPolygon::OutOfRange", false );
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1 ) );
}

std::vector<double> ShapeSphericalPolygon::Distribution( const double num ) const
{
	Trace trace( "ShapeSphericalPolygon::Distribution", false );

	std::vector<double> distribution;
	for (int i = 0 ; i < num ; i++)
	{
		double value = ( 1 / ( num - 1 ) ) * (i);
		if ( value > 1 ) value = 1.0;
		if ( value < 0 ) value = 0.0;
		distribution.push_back(value);
	}
	return distribution;
}

std::vector< std::pair<double,double> > ShapeSphericalPolygon::MeshTriangle( const std::vector<double>& distribution ) const
{
	Trace trace( "ShapeSphericalPolygon::MeshTriangle", false );

	double num = distribution.size();
	std::vector< std::pair<double,double> > points;
	double xmax = radius.getValue() * cos( m_phiMax );
	double ymax = radius.getValue() * sin( m_phiMax );

	for (int i = 0 ; i < num ; i++)
		{
		double x = xmax * distribution[i];
		for (int j = 0 ; j <= i ; j++)
			{
			double y = ymax * distribution[j];
			double z = sqrt( sphereRadius.getValue() * sphereRadius.getValue() - (x*x + y*y));

			double phi = ( x > 0 )? atan2( y, x ) : 0 ;
			double theta = acos( z / sphereRadius.getValue() );
			std::pair<double,double> newpoint(phi,theta);
			points.push_back(newpoint);
			}
		}
	return points;
}

void ShapeSphericalPolygon::RadiusChanged( void* data, SoSensor* )
{
	Trace trace( "ShapeSphericalPolygon::RadiusChanged", false );

	ShapeSphericalPolygon* polygon = static_cast< ShapeSphericalPolygon* >( data );
	//if( polygon->radius.getValue() > polygon->sphereRadius.getValue() )
	polygon->m_thetaMax = asin(  polygon->radius.getValue() /  polygon->sphereRadius.getValue() );
	polygon->m_xMax = polygon->radius.getValue() * cos( polygon->m_phiMax );
}

void ShapeSphericalPolygon::SidesChanged( void* data, SoSensor* )
{
	Trace trace( "ShapeSphericalPolygon::SidesChanged", false );

    ShapeSphericalPolygon* polygon = static_cast< ShapeSphericalPolygon* >( data );
    polygon->m_phiMax = tgc::Pi/ polygon->polygonSides.getValue();
    polygon->m_xMax =  polygon->radius.getValue() * cos(  polygon->m_phiMax );
}

void ShapeSphericalPolygon::SphereRadiusChanged( void* data, SoSensor* )
{
	Trace trace( "ShapeSphericalPolygon::SphereRadiusChanged", false );

	ShapeSphericalPolygon* polygon = static_cast< ShapeSphericalPolygon* >( data );
	polygon->m_thetaMax = asin(  polygon->radius.getValue() /  polygon->sphereRadius.getValue() );
}

void ShapeSphericalPolygon::generatePrimitives(SoAction *action)
{
	Trace trace( "ShapeSphericalPolygon::generatePrimitives", false );

    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);


	// Get the points in Phi,Theta correspondig with the first triangle of the polygon.
	int num = 10;
	std::vector<double> distribution = Distribution ( num );

    std::vector< std::pair<double,double> > triangleVertex = MeshTriangle( distribution );

	int triangleQuads = 0;
    for ( int i = 1; i <= num; i++) triangleQuads += (i-1);
    int triangleIndex = triangleQuads * 4;
    int index[triangleIndex];
    int k = 0;

	for(int irow = 0; irow < (num-1); irow++)
		{
			int c = 0;
			for (int i = 0; i < irow; i++ ) c += ( i + 1 );
			//Crear los "cuadrados" de la columna
			for(int icolumn = 0; icolumn < irow; icolumn++)
				{
					index[k] = c + icolumn;
					index[k+1] = index[k] + ( irow + 1 );
					index[k+2] = index[k+1] + 1;
					index[k+3] = index[k] + 1;
					k+=4; //Set k to the first point of the next face.
				}
			//Crear el "triangulo" (como un cuadrado con dos vertices coincidentes) de la columna
			index[k] = c + irow;
			index[k+1] = index[k] + ( irow + 1 );
			index[k+2] = index[k+1] + 1;
			index[k+3] = index[k];
			k+=4; //Set k to the first point of the next face.
		}
	//Once we have the triangle points and indexes, proceed to compute the whole polygon points
	// and indexes

	int totalIndex = triangleIndex * 2 * polygonSides.getValue();
	float finalVertex[totalIndex][2];

	double twoPhiMax = 2*m_phiMax;

	for (int i = 0 ; i < polygonSides.getValue() ; i++)
	{
		for ( int j = 0 ; j < triangleIndex ; j++ )
		{
			finalVertex[ j + 2 * i * triangleIndex ][0] = triangleVertex[ index[j] ].first + i * twoPhiMax;
   			finalVertex[ j + 2 * i * triangleIndex ][1] = triangleVertex[ index[j] ].second;
   			finalVertex[ j + (2*i + 1) * triangleIndex ][0] = triangleVertex[ index[j] ].first + i * twoPhiMax + 2 * (m_phiMax - triangleVertex[ index[j] ].first);
   			finalVertex[ j + (2*i + 1) * triangleIndex ][1] = triangleVertex[ index[j] ].second;
		}
	}

    float u = 1;
    float v = 1;

    beginShape(action, QUADS );
	for ( int i = 0; i < totalIndex; i++ )
	{
		double x = sphereRadius.getValue() * sin(  finalVertex[i][1] ) * cos( finalVertex[i][0] );
		double y = sphereRadius.getValue() * sin(  finalVertex[i][1] ) * sin( finalVertex[i][0] );
		double z = -sphereRadius.getValue() * cos(  finalVertex[i][1] ) + sphereRadius.getValue();

		SbVec3f point( x, y ,z );
		SbVec3f vector( point[0], point[1], point[2] - sphereRadius.getValue() );
		SbVec3f normal( point[0]/vector.length(), point[1]/vector.length(),  ( point[2] - sphereRadius.getValue() ) /vector.length() );

		SbVec4f texCoord = useTexFunc ? tce->get(point, normal) : SbVec4f( u, v, 0.0, 1.0 );

	     pv.setPoint(point);
	     pv.setNormal(normal);
	     pv.setTextureCoords(texCoord);
	     shapeVertex(&pv);

    }
    endShape();

}

void ShapeSphericalPolygon::computeBBox(SoAction *, SbBox3f& box, SbVec3f& center)
{
	Trace trace( "ShapeSphericalPolygon::computeBBox", false );

	double xmax = radius.getValue();
	double ymax = radius.getValue();
	double zmax = sphereRadius.getValue() * ( 1 - cos( m_thetaMax ) );

	center.setValue(0,0,zmax);

	box.setBounds(SbVec3f( - xmax, - ymax, 0 ), SbVec3f( xmax, ymax, zmax));
}
