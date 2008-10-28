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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

#include "DifferentialGeometry.h"
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
	SO_NODE_ADD_FIELD( m_sphereRadius, (10.0) );
	SO_NODE_ADD_FIELD( m_radius, (3.0) );
	SO_NODE_ADD_FIELD( m_sides, (6) );

    m_thetaMax = asin( m_radius.getValue() / m_sphereRadius.getValue() );
    m_phiMax = tgc::Pi/m_sides.getValue();
    m_xMax = m_radius.getValue() * cos( m_phiMax );

	m_sphereRadiusSensor = new SoFieldSensor( SphereRadiusChanged, this );
	m_sphereRadiusSensor->attach( &m_sphereRadius );

	m_radiusSensor = new SoFieldSensor( RadiusChanged, this );
	m_radiusSensor->attach( &m_radius );

	m_sidesSensor = new SoFieldSensor( SidesChanged, this );
	m_sidesSensor->attach( &m_sides );
}

ShapeSphericalPolygon::~ShapeSphericalPolygon()
{
	Trace trace( "ShapeSphericalPolygon::~ShapeSphericalPolygon", false );
}


bool ShapeSphericalPolygon::OutOfRange( double u, double v ) const
{
	Trace trace( "ShapeSphericalPolygon::OutOfRange", false );

	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1 ) );
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

	double x = m_sphereRadius.getValue() * sintheta * cos( phi );
	double y = m_sphereRadius.getValue() * sintheta * sin( phi );
	double z =  m_sphereRadius.getValue() * ( 1 - cos( m_thetaMax ) );

	Point3D point = Point3D( x, y, z );

	return point;
}

bool ShapeSphericalPolygon::IsInside( double u, double v ) const
{
	Trace trace( "ShapeSphericalPolygon::IsInside", false );

	double phi = u * tgc::TwoPi;
	double theta = v * m_thetaMax;

	double ratio = phi/m_phiMax;
	double integerPart;
	double fractionalPart = modf( ratio, &integerPart );
	double phiReduced =  tgf::IsOdd( integerPart ) ? ((1.0-fractionalPart) * m_phiMax ) : ( fractionalPart * m_phiMax ) ;

	double xReduced = m_sphereRadius.getValue() * sin( theta ) * cos( phiReduced );
	return ( xReduced > m_xMax ) ? false : true;
}


SbVec3f ShapeSphericalPolygon::GetNormal(  double u, double v  ) const
{
	Trace trace( "ShapeSphericalPolygon::GetNormal", false );

	Point3D point = GetPoint3D( u, v );
	SbVec3f vector( point.x, point.y, point.z - m_sphereRadius.getValue());
	return SbVec3f ( -point.x/vector.length(), -point.y/vector.length(), -(point.z-m_sphereRadius.getValue())/vector.length() );
}

void ShapeSphericalPolygon::generatePrimitives(SoAction *action)
{
	Trace trace( "ShapeSphericalPolygon::generatePrimitives", false );

    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    SbVec4f texCoord;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);
    else
    {
        texCoord[2] = 0.0;
        texCoord[3] = 1.0;
    }

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

	int totalIndex = triangleIndex * 2 * m_sides.getValue();
	float finalVertex[totalIndex][2];

	double twoPhiMax = 2*m_phiMax;

	for (int i = 0 ; i < m_sides.getValue() ; i++)
	{
		for ( int j = 0 ; j < triangleIndex ; j++ )
		{
			finalVertex[ j + 2 * i * triangleIndex ][0] = triangleVertex[ index[j] ].first + i * twoPhiMax;
   			finalVertex[ j + 2 * i * triangleIndex ][1] = triangleVertex[ index[j] ].second;
   			finalVertex[ j + (2*i + 1) * triangleIndex ][0] = triangleVertex[ index[j] ].first + i * twoPhiMax + 2 * (m_phiMax - triangleVertex[ index[j] ].first);
   			finalVertex[ j + (2*i + 1) * triangleIndex ][1] = triangleVertex[ index[j] ].second;
		}
	}

	SbVec3f  point;

#define GEN_VERTEX( pv , phi , theta , s , t ) 								 							\
																										\
	 double sintheta = sin( theta );																	\
	 double x = m_sphereRadius.getValue() * sintheta * cos( phi );										\
	 double y = m_sphereRadius.getValue() * sintheta * sin( phi );										\
	 double z = -m_sphereRadius.getValue() * cos( theta ) + m_sphereRadius.getValue();					\
																										\
     point.setValue( x , y , z );                  														\
     SbVec3f vector( point[0], point[1], point[2] - m_sphereRadius.getValue() );													\
	 SbVec3f normal( -point[0]/vector.length(), -point[1]/vector.length(), - ( point[2] - m_sphereRadius.getValue() ) /vector.length() );	\
     if (useTexFunc)                            														\
       texCoord = tce->get(point, normal);      														\
     else {                                     														\
       texCoord[0] = s;                         														\
       texCoord[1] = t;                         														\
     }                                          														\
     pv.setPoint(point);                        														\
     pv.setNormal(normal);                      														\
     pv.setTextureCoords(texCoord);             														\
     shapeVertex(&pv)


    float u = 1;
    float v = 1;
	beginShape(action, QUADS );
	for ( int i = 0; i < totalIndex; i++ )
	{
    	GEN_VERTEX(pv , finalVertex[i][0] , finalVertex[i][1] , u ,  v );
    }
    endShape();

}

std::vector< std::pair<double,double> > ShapeSphericalPolygon::MeshTriangle( const std::vector<double>& distribution ) const
{
	Trace trace( "ShapeSphericalPolygon::MeshTriangle", false );

	double num = distribution.size();
	std::vector< std::pair<double,double> > points;
	double xmax = m_radius.getValue() * cos( m_phiMax );
	double ymax = m_radius.getValue() * sin( m_phiMax );

	for (int i = 0 ; i < num ; i++)
		{
		double x = xmax * distribution[i];
		for (int j = 0 ; j <= i ; j++)
			{
			double y = ymax * distribution[j];
			double z = sqrt( m_sphereRadius.getValue() * m_sphereRadius.getValue() - (x*x + y*y));

			double phi = ( x > 0 )? atan2( y, x ) : 0 ;
			double theta = acos( z / m_sphereRadius.getValue() );
			std::pair<double,double> newpoint(phi,theta);
			points.push_back(newpoint);
			}
		}
	return points;
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

void ShapeSphericalPolygon::SphereRadiusChanged( void* data, SoSensor* )
{
	Trace trace( "ShapeSphericalPolygon::SphereRadiusChanged", false );

	ShapeSphericalPolygon* polygon = static_cast< ShapeSphericalPolygon* >( data );
	polygon->m_thetaMax = asin(  polygon->m_radius.getValue() /  polygon->m_sphereRadius.getValue() );
}

void ShapeSphericalPolygon::RadiusChanged( void* data, SoSensor* )
{
	Trace trace( "ShapeSphericalPolygon::RadiusChanged", false );

	ShapeSphericalPolygon* polygon = static_cast< ShapeSphericalPolygon* >( data );
	polygon->m_thetaMax = asin(  polygon->m_radius.getValue() /  polygon->m_sphereRadius.getValue() );
	polygon->m_xMax = polygon->m_radius.getValue() * cos( polygon->m_phiMax );
}

void ShapeSphericalPolygon::SidesChanged( void* data, SoSensor* )
{
	Trace trace( "ShapeSphericalPolygon::SidesChanged", false );

    ShapeSphericalPolygon* polygon = static_cast< ShapeSphericalPolygon* >( data );
    polygon->m_phiMax = tgc::Pi/ polygon->m_sides.getValue();
    polygon->m_xMax =  polygon->m_radius.getValue() * cos(  polygon->m_phiMax );
}

void ShapeSphericalPolygon::computeBBox(SoAction *, SbBox3f& box, SbVec3f& center)
{
	Trace trace( "ShapeSphericalPolygon::computeBBox", false );

	double xmax = m_radius.getValue();
	double ymax = m_radius.getValue();
	double zmax = m_sphereRadius.getValue() * ( 1 - cos( m_thetaMax ) );

	center.setValue(0,0,zmax);

	box.setBounds(SbVec3f( - xmax, - ymax, 0 ), SbVec3f( xmax, ymax, zmax));
}

SoNode* ShapeSphericalPolygon::copy( SbBool copyConnections ) const
{
	Trace trace( "ShapeSphericalPolygon::copy", false );

	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	ShapeSphericalPolygon* newShapeSphericalPolygon = dynamic_cast< ShapeSphericalPolygon* >( SoNode::copy( copyConnections ) );

	return newShapeSphericalPolygon;
}

QString ShapeSphericalPolygon::getIcon()
{
	Trace trace( "ShapeSphericalPolygon::getIcon", false );

	return ":/icons/ShapeSphericalPolygon.png";
}

bool ShapeSphericalPolygon::Intersect( const Ray& worldRay, double* tHit, DifferentialGeometry* dg ) const
{
	Trace trace( "ShapeSphericalPolygon::Intersect", false );

	// Traslation of the coordinate system from the local coordinates with origin in the bottom of the sphere to the sphere center.
	Ray objectRay = worldRay ;
	objectRay.origin.z -= m_sphereRadius.getValue();

	// Compute quadratic sphere coefficients
	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A = objectRay.direction.LengthSquared();
    double B = 2.0 * DotProduct( vObjectRayOrigin, objectRay.direction );
	double C = vObjectRayOrigin.LengthSquared() - m_sphereRadius.getValue()*m_sphereRadius.getValue();

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !tgf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
    double thit = ( t0 > objectRay.mint )? t0 : t1 ;
    if( thit > objectRay.maxt ) return false;

    //Evaluate Tolerance
	double tol = 0.0001;
	if( (thit - objectRay.mint) < tol ) return false;

	// Compute sphere hit position and $\phi$
    Point3D hitPoint = objectRay( thit );
	double phi = atan2( hitPoint.y, hitPoint.x );
	if ( phi < 0. ) phi += tgc::TwoPi;

	// Test intersection against ray clipping parameters
	if( hitPoint.z > 0 )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		// Compute sphere hit position and $\phi$
		hitPoint = objectRay( thit );
		phi = atan2( hitPoint.y, hitPoint.x );
	    if ( phi < 0. ) phi += tgc::TwoPi;

		if ( hitPoint.z > 0 ) return false;
	}

	// Find parametric representation of sphere hit

	double u = phi / tgc::TwoPi;
	double theta = acos( -hitPoint.z / m_sphereRadius.getValue() );
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
	Vector3D dpdu( -tgc::TwoPi * hitPoint.y, tgc::TwoPi * hitPoint.x, 0.0 );
	Vector3D dpdv = ( tgc::Pi - m_thetaMax ) *
	                  Vector3D( hitPoint.z * cosphi, hitPoint.z * sinphi, -m_sphereRadius.getValue() * sin( theta ) );

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
	hitPoint.z += m_sphereRadius.getValue();

	*dg = DifferentialGeometry( hitPoint ,
		                        dpdu,
								dpdv,
		                        dndu,
								dndv,
		                        u, v, this );

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeSphericalPolygon::IntersectP( const Ray& worldRay ) const
{
	Trace trace( "ShapeSphericalPolygon::IntersectP", false );

	return Intersect( worldRay, 0, 0 );
}
