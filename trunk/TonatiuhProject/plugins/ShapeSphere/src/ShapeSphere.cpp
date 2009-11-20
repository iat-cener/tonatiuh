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
#include <Inventor/sensors/SoFieldSensor.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeSphere.h"
#include "tgc.h"
#include "tgf.h"
#include "Trace.h"

SO_NODE_SOURCE(ShapeSphere);

void ShapeSphere::initClass()
{
	Trace trace( "ShapeSphere::initClass", false );
	SO_NODE_INIT_CLASS(ShapeSphere, TShape, "TShape");
}

ShapeSphere::ShapeSphere( )
{
	Trace trace( "ShapeSphere::ShapeSphere", false );

	SO_NODE_CONSTRUCTOR(ShapeSphere);
	SO_NODE_ADD_FIELD( radius, (0.5) );
	SO_NODE_ADD_FIELD( z1, (-0.5) );
	SO_NODE_ADD_FIELD( z2, (0.5) );
	SO_NODE_ADD_FIELD( phiMax, ( tgc::TwoPi) );

	double zmin = std::min( z1.getValue(), z2.getValue() );
	double zmax = std::max( z1.getValue(), z2.getValue() );

	m_thetaMin = acos( zmax/radius.getValue() );
	m_thetaMax = acos( zmin/radius.getValue() );

	SoFieldSensor* m_z1Sensor = new SoFieldSensor(updateMinMaxTheta, this);
	m_z1Sensor->attach( &z1 );
	SoFieldSensor* m_z2Sensor = new SoFieldSensor(updateMinMaxTheta, this);
	m_z2Sensor->attach( &z2 );
	SoFieldSensor* m_radiusSensor = new SoFieldSensor(updateMinMaxTheta, this);
	m_radiusSensor->attach( &radius );
	SoFieldSensor* m_phiMaxSensor = new SoFieldSensor(updatePhiMax, this);
	m_phiMaxSensor->attach( &phiMax );
}

ShapeSphere::~ShapeSphere()
{
	Trace trace( "ShapeSphere::~ShapeSphere", false );
}

SoNode* ShapeSphere::copy( SbBool copyConnections ) const
{
	Trace trace( "ShapeSphere::copy", false );

	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	ShapeSphere* newShapeSphere = dynamic_cast< ShapeSphere* >( SoNode::copy( copyConnections ) );

	// Copy the m_thetaMin, m_thetaMax private members explicitly
	newShapeSphere->m_thetaMin = m_thetaMin;
	newShapeSphere->m_thetaMax = m_thetaMax;

	return newShapeSphere;
}

double ShapeSphere::GetArea() const
{
	Trace trace( "ShapeSphere::GetArea", false );
	return ( 4 * tgc::Pi * radius.getValue() * radius.getValue() );
}

QString ShapeSphere::getIcon()
{
	Trace trace( "ShapeSphere::getIcon", false );
	return QString( ":/icons/ShapeSphere.png" );
}

bool ShapeSphere::Intersect( const Ray& ray, double* tHit, DifferentialGeometry* dg ) const
{
	Trace trace( "ShapeSphere::Intersect", false );

	Ray objectRay = ray ;

	// Compute quadratic ShapeSphere coefficients
	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
	double A = objectRay.direction.LengthSquared();
    double B = 2.0 * DotProduct( vObjectRayOrigin, objectRay.direction );
	double C = vObjectRayOrigin.LengthSquared() - radius.getValue() * radius.getValue();

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !tgf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
    double thit = ( t0 > objectRay.mint )? t0 : t1 ;
    if( thit > objectRay.maxt ) return false;

    //Evaluate Tolerance
	double tol = 0.00001;
	if( (thit - objectRay.mint) < tol ) return false;

	// Compute ShapeSphere hit position and $\phi$
    Point3D hitPoint = objectRay( thit );
	double phi = atan2( hitPoint.y, hitPoint.x );
	if ( phi < 0. ) phi += tgc::TwoPi;

	double zmin = std::min( z1.getValue(), z2.getValue() );
	double zmax = std::max( z1.getValue(), z2.getValue() );

	// Test intersection against clipping parameters
	if( hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax.getValue() )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		// Compute ShapeSphere hit position and $\phi$
		hitPoint = objectRay( thit );
		phi = atan2( hitPoint.y, hitPoint.x );
	    if ( phi < 0. ) phi += tgc::TwoPi;

		if ( hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax.getValue() ) return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );

	// Find parametric representation of ShapeSphere hit
	double theta = acos( hitPoint.z / radius.getValue() );
	double u = ( theta - m_thetaMin ) / ( m_thetaMax - m_thetaMin );
	double v = phi / phiMax.getValue();

	// Compute ShapeSphere \dpdu and \dpdv
	Vector3D dpdu( radius.getValue() * ( m_thetaMax - m_thetaMin ) * cos( phiMax.getValue() * v )
						* cos( m_thetaMin + m_thetaMax * u - m_thetaMin * u ),
					radius.getValue() * ( m_thetaMax - m_thetaMin )
						* cos( m_thetaMin + m_thetaMax * u - m_thetaMin * u ) * sin( phiMax.getValue() * v ),
					radius.getValue() * ( -m_thetaMax + m_thetaMin )
						* sin( m_thetaMin + m_thetaMax * u - m_thetaMin * u ) );

	Vector3D dpdv( -phiMax.getValue() * radius.getValue() *sin( phiMax.getValue() * v )
							* sin( m_thetaMin + m_thetaMax * u - m_thetaMin * u ),
						phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * v)
							* sin( m_thetaMin + m_thetaMax * u - m_thetaMin * u ),
						0.0 );

	// Compute ShapeSphere \dndu and \dndv
	Vector3D d2Pduu( -radius.getValue() * (m_thetaMax - m_thetaMin) * (m_thetaMax - m_thetaMin) * cos( phiMax.getValue() * v )
							* sin( m_thetaMin + m_thetaMax * u - m_thetaMin * u ),
						-radius.getValue() * (m_thetaMax - m_thetaMin) * (m_thetaMax - m_thetaMin)
							* sin(m_thetaMin + m_thetaMax * u - m_thetaMin * u ) * sin(phiMax.getValue()* v ),
						radius.getValue() * (m_thetaMax - m_thetaMin) *(-m_thetaMax + m_thetaMin) * cos(m_thetaMin + m_thetaMax * u - m_thetaMin * u) );
	Vector3D d2Pduv( -phiMax.getValue() *  radius.getValue() *  (m_thetaMax - m_thetaMin) * cos(m_thetaMax + m_thetaMax * u - m_thetaMin * u )
							* sin( phiMax.getValue() *  v ),
						phiMax.getValue() *  radius.getValue() *  (m_thetaMax - m_thetaMin) * cos(m_thetaMin + m_thetaMax * u - m_thetaMin * u )
						    * cos( phiMax.getValue() *  v),
						0.0 );
	Vector3D d2Pdvv( -phiMax.getValue() * phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * v )
						* sin( m_thetaMin + m_thetaMax * u - m_thetaMin * u ),
					-phiMax.getValue() * phiMax.getValue() * radius.getValue() * sin(m_thetaMin + m_thetaMax * u - m_thetaMin * u )
						* sin(phiMax.getValue() * v ),
					0.0 );

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

	dg->shapeFrontSide = ( DotProduct( N, ray.direction ) > 0 ) ? false : true;
    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeSphere::IntersectP( const Ray& ray ) const
{
	Trace trace( "ShapeSphere::IntersectP", false );
	return Intersect( ray, 0, 0 );
}

Point3D ShapeSphere::Sample( double u1, double u2 ) const
{
	Trace trace( "ShapeSphere::Sample", false );
	return GetPoint3D( u1, u2 );
}

void ShapeSphere::updateMinMaxTheta( void *data, SoSensor * )
{
	Trace trace( "ShapeSphere::updateMinMaxTheta", false );

	ShapeSphere* shapeSphere = (ShapeSphere *) data;

	//double zmin, zmax;
	double zmin = std::min( shapeSphere->z1.getValue(), shapeSphere->z2.getValue() );
	double zmax = std::max( shapeSphere->z1.getValue(), shapeSphere->z2.getValue() );


	if( shapeSphere->radius.getValue() < fabs( zmin ) )
		zmin = -shapeSphere->radius.getValue();

	if(shapeSphere->radius.getValue() < zmax )
		zmax = shapeSphere->radius.getValue();

	shapeSphere->m_thetaMin = acos( zmax/shapeSphere->radius.getValue() );
	shapeSphere->m_thetaMax = acos( zmin/shapeSphere->radius.getValue() );
}


void ShapeSphere::updatePhiMax( void *data, SoSensor* )
{
	Trace trace( "ShapeSphere::updatePhiMax", false );

	ShapeSphere* shapeSphere = (ShapeSphere *) data;

	if( shapeSphere->phiMax.getValue() < 0.0 ) shapeSphere->phiMax = 0.0;
	if( shapeSphere->phiMax.getValue() > tgc::TwoPi ) shapeSphere->phiMax = tgc::TwoPi;

}

Point3D ShapeSphere::GetPoint3D( double u, double v ) const
{
	Trace trace( "ShapeSphere::GetPoint3D", false );

	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );


	double theta = u * ( m_thetaMax - m_thetaMin ) + m_thetaMin;
	double phi = v * phiMax.getValue();

	double cosphi = cos (phi);
	double sinphi = sin (phi);
	double costheta = cos (theta);
	double sintheta = sin (theta);

	double x = radius.getValue() * sintheta * cosphi;
	double y = radius.getValue() * sintheta * sinphi;
	double z = radius.getValue() * costheta;

	return Point3D (x, y, z);

}

NormalVector ShapeSphere::GetNormal(double u, double v ) const
{
	Trace trace( "ShapeSphere::GetNormal", false );

	Vector3D dpdu( radius.getValue() * ( m_thetaMax - m_thetaMin ) * cos( phiMax.getValue() * v )
						* cos( m_thetaMin + m_thetaMax * u - m_thetaMin * u ),
					radius.getValue() * ( m_thetaMax - m_thetaMin )
						* cos( m_thetaMin + m_thetaMax * u - m_thetaMin * u ) * sin( phiMax.getValue() * v ),
					radius.getValue() * ( -m_thetaMax + m_thetaMin )
						* sin( m_thetaMin + m_thetaMax * u - m_thetaMin * u ) );

	Vector3D dpdv( -phiMax.getValue() * radius.getValue() *sin( phiMax.getValue() * v )
							* sin( m_thetaMin + m_thetaMax * u - m_thetaMin * u ),
						phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * v)
							* sin( m_thetaMin + m_thetaMax * u - m_thetaMin * u ),
						0.0 );
	NormalVector normal = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
	return normal;
}

bool ShapeSphere::OutOfRange( double u, double v ) const
{
	Trace trace( "ShapeSphere::OutOfRange", false );
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeSphere::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
	Trace trace( "ShapeSphere::computeBBox", false );

   	double sinThetaMin = sin( m_thetaMin );
   	double sinThetaMax = sin( m_thetaMax );
   	double cosPhiMax = cos( phiMax.getValue() );
   	double sinPhiMax = sin( phiMax.getValue() );

   	double xmin = ( ( m_thetaMin <= tgc::Pi/2.0 ) && ( m_thetaMax >= tgc::Pi/2.0 ) ) ?
						( phiMax.getValue() <=tgc::Pi / 2 ) ? radius.getValue() * std::min( sinThetaMin, sinThetaMax ) * cosPhiMax :
   							( phiMax.getValue() >= tgc::Pi ) ? - radius.getValue() : radius.getValue() * cosPhiMax :
   						( phiMax.getValue() >= tgc::Pi ) ? - radius.getValue() * std::max( sinThetaMin, sinThetaMax ):
   							( phiMax.getValue() <=tgc::Pi / 2 ) ? radius.getValue() * std::min( sinThetaMin, sinThetaMax ) * cosPhiMax :
								radius.getValue() * std::max( sinThetaMin, sinThetaMax ) * cosPhiMax;
   	double xmax = ( ( m_thetaMin <= tgc::Pi/2.0 ) && ( m_thetaMax >= tgc::Pi/2.0 ) )? radius.getValue() : radius.getValue() * std::max( sinThetaMin, sinThetaMax );

   	double ymin = ( ( m_thetaMin <= tgc::Pi/2.0 ) && ( m_thetaMax >= tgc::Pi/2.0 ) ) ?
					( ( phiMax.getValue() <= tgc::Pi ) ? 0.0 :
   									( phiMax.getValue() < 1.5 * tgc::Pi ) ? radius.getValue() * sinPhiMax : -radius.getValue() ) :
					( (( phiMax.getValue() <= tgc::Pi ) ? 0.0 :
						( phiMax.getValue() < 1.5 * tgc::Pi ) ? std::min( radius.getValue()*sinThetaMin* sinPhiMax, radius.getValue()*sinThetaMax* sinPhiMax ) :
								std::min( -radius.getValue()*sinThetaMin, -radius.getValue()*sinThetaMax ) ) );
	double ymax = ( ( m_thetaMin <= tgc::Pi/2.0 ) && ( m_thetaMax >= tgc::Pi/2.0 ) ) ?
					( phiMax.getValue() >= tgc::Pi/2.0 ) ? radius.getValue() : radius.getValue() * sinPhiMax:
					( phiMax.getValue() >= tgc::Pi/2.0 ) ? radius.getValue() *  std::max( sinThetaMin, sinThetaMax ) : radius.getValue() * std::max( sinThetaMin* sinPhiMax, sinThetaMax* sinPhiMax );

   	double zmin = std::max( -radius.getValue(), std::min( z1.getValue(), z2.getValue() ) );
	double zmax = std::min( radius.getValue(), std::max( z1.getValue(), z2.getValue() ) );

	box.setBounds(SbVec3f( xmin, ymin, zmin ), SbVec3f( xmax, ymax, zmax ) );
}

void ShapeSphere::generatePrimitives(SoAction *action)
{
	Trace trace( "ShapeSphere::generatePrimitives", false );

    SoPrimitiveVertex   pv;

    // Access the state from the action.
    SoState  *state = action->getState();

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    // If we need to generate texture coordinates with a
    // function, we'll need an SoGLTextureCoordinateElement.
    // Otherwise, we'll set up the coordinates directly.
    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	const int rows = 100; // Number of points per row
    const int columns = 100; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid

    float vertex[totalPoints][6];

    int h = 0;
    double ui = 0;
	double vj = 0;

	for (int i = 0; i < rows; i++)
    {
    	ui =( 1.0 /(double)(rows-1) ) * i;

    	for ( int j = 0 ; j < columns ; j++ )
    	{

    		vj = ( 1.0 /(double)(columns-1) ) * j;

    		Point3D point = GetPoint3D(ui, vj);
    		NormalVector normal = GetNormal(ui, vj);

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
    for(int irow = 0; irow < (rows-1); irow++)
           for(int icolumn = 0; icolumn < (columns-1); icolumn++)
           {
           	indices[k] = irow*columns + icolumn;
        	indices[k+1] = indices[k] + 1;
        	indices[k+3] = indices[k] + columns;
        	indices[k+2] = indices[k+3] + 1;

        	k+=4; //Set k to the first point of the next face.
           }

    float finalvertex[totalIndices][6];
    for(int ivert = 0; ivert<totalIndices;ivert++)
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
    for( int i = 0; i < totalIndices; i++ )
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
