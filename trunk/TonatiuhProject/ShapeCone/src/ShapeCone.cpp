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
#include "ShapeCone.h"
#include "tgf.h"
#include "tgc.h"
#include "Trace.h"


SO_NODE_SOURCE(ShapeCone);

void ShapeCone::initClass()
{
	Trace trace( "ShapeConeFactory::initClass", false );
	SO_NODE_INIT_CLASS(ShapeCone, TShape, "TShape");
}

ShapeCone::ShapeCone(  )
{
	Trace trace( "ShapeConeFactory::ShapeCone", false );

	SO_NODE_CONSTRUCTOR(ShapeCone);
	SO_NODE_ADD_FIELD(m_baseradius, (10.0));
	SO_NODE_ADD_FIELD(m_topradius, (0.0));
	SO_NODE_ADD_FIELD( m_height, (10.0));
	SO_NODE_ADD_FIELD(m_phiMax, (1.5* tgc::Pi));
}

ShapeCone::~ShapeCone()
{
	Trace trace( "ShapeConeFactory::~ShapeCone", false );
}

QString ShapeCone::getIcon()
{
	Trace trace( "ShapeConeFactory::getIcon", false );
	return ":/icons/ShapeCone.png";
}

bool ShapeCone::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	Trace trace( "ShapeConeFactory::Intersect", false );

	// Compute quadratic ShapeCone coefficients
	double theta = atan2( m_height.getValue(), ( m_baseradius.getValue()-m_topradius.getValue() ) );
	double invTan = 1 / tan(theta);

	double A = ( objectRay.direction.x * objectRay.direction.x )
				+ ( objectRay.direction.y * objectRay.direction.y  )
				- ( objectRay.direction.z * objectRay.direction.z * invTan * invTan);
	double B = 2.0 * ( ( objectRay.origin.x * objectRay.direction.x )
						+ ( objectRay.origin.y * objectRay.direction.y )
						+ ( m_baseradius.getValue() * invTan * objectRay.direction.z )
						- ( invTan * invTan * objectRay.origin.z * objectRay.direction.z ) );
	double C = ( objectRay.origin.x * objectRay.origin.x )
				+ ( objectRay.origin.y * objectRay.origin.y )
				- ( m_baseradius.getValue() * m_baseradius.getValue() )
				+ ( 2 * m_baseradius.getValue() * invTan * objectRay.origin.z )
				- ( invTan * invTan * objectRay.origin.z * objectRay.origin.z );

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

	// Compute possible ShapeCone hit position and $\phi$
    Point3D hitPoint = objectRay( thit );
	double phi = atan2( hitPoint.y, hitPoint.x );
	if ( phi < 0. ) phi += tgc::TwoPi;


	// Test intersection against clipping parameters
	if( hitPoint.z < 0 || hitPoint.z > m_height.getValue() || phi > m_phiMax.getValue() )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		hitPoint = objectRay( thit );

		if ( hitPoint.z < 0 || hitPoint.z > m_height.getValue() || phi > m_phiMax.getValue() ) return false;
	}
	// Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function Cylinder::Intersect(...) called with null pointers" );

	// Compute definitive ShapeCone hit position and $\phi$
    hitPoint = objectRay( thit );
	phi = atan2( hitPoint.y, hitPoint.x );
	if ( phi < 0. ) phi += tgc::TwoPi;

	// Find parametric representation of ShapeCone hit
	double zradius = sqrt( hitPoint.x*hitPoint.x + hitPoint.y*hitPoint.y );
	double maxradius = std::max( m_baseradius.getValue(), m_topradius.getValue());
	double minradius = std::min( m_baseradius.getValue(), m_topradius.getValue());
	double u = phi / m_phiMax.getValue();
	double v = (zradius-minradius)/(maxradius-minradius);

	// Compute ShapeCone \dpdu and \dpdv
	double invzradius = 1.0 / zradius;
	double cosphi = hitPoint.x * invzradius;
	double sinphi = hitPoint.y * invzradius;
	Vector3D dpdu( -zradius*sinphi, zradius*cosphi, 0.0);
	Vector3D dpdv( ((maxradius-minradius)*cosphi)-(minradius*sinphi), ((maxradius-minradius)*sinphi)+(minradius*cosphi) , ( ( -m_height.getValue() )*(maxradius-minradius))/(m_baseradius.getValue()-m_topradius.getValue()) );

	// Compute ShapeCone \dndu and \dndv
	Vector3D d2Pduu( -zradius*cosphi, -zradius*sinphi, 0.0);
	Vector3D d2Pduv( -(maxradius-minradius)*sinphi, -(maxradius-minradius)*cosphi, 0.0 );
	Vector3D d2Pdvv( 0.0, 0.0, 0.0 );

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
		                        dpdu,
								dpdv,
		                        dndu,
								dndv,
		                        u, v, this );

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeCone::IntersectP( const Ray& worldRay ) const
{
	Trace trace( "ShapeConeFactory::IntersectP", false );
	return Intersect( worldRay, 0, 0 );
}

Point3D ShapeCone::Sample( double u, double v ) const
{
	Trace trace( "ShapeConeFactory::Sample", false );
	return GetPoint3D( u, v );
}

Point3D ShapeCone::GetPoint3D (double u, double v) const
{
	Trace trace( "ShapeConeFactory::OutOfRange", false );
	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function ShapeCone::GetPoint3D called with invalid parameters" );

	double maxradius = std::max( m_baseradius.getValue(), m_topradius.getValue());
	double minradius = std::min( m_baseradius.getValue(), m_topradius.getValue());

	double phi = u * m_phiMax.getValue();
	double radius = minradius + v * (maxradius-minradius);

	double x = radius*cos (phi);
	double y = radius*sin (phi);
	double z = ( ( ( m_height.getValue() ) / (m_baseradius.getValue() - m_topradius.getValue() ) ) * ( m_baseradius.getValue() - radius ) );

	return Point3D (x, y, z);
}

NormalVector ShapeCone::GetNormal( double u ,double v ) const
{
	Trace trace( "ShapeConeFactory::GetNormal", false );

	Point3D point = GetPoint3D( u, v );

	NormalVector vector_aux( point.x, point.y, 0 );
	NormalVector vector ( -point.x, -point.y, -( ( m_baseradius.getValue() - m_topradius.getValue() )/( m_height.getValue() ) ) * vector_aux.Length() );
	if ( vector.Length() > 0 )	vector = vector/vector.Length();

	return vector;

}

bool ShapeCone::OutOfRange( double u, double v ) const
{
	Trace trace( "ShapeConeFactory::OutOfRange", false );
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeCone::computeBBox(SoAction *, SbBox3f &box, SbVec3f& /*center*/)
{
	Trace trace( "ShapeConeFactory::computeBBox", false );

	double cosPhiMax = cos( m_phiMax.getValue() );
	double sinPhiMax = sin( m_phiMax.getValue() );
	double maxradius = std::max( m_baseradius.getValue(), m_topradius.getValue());

	double xmin = ( m_phiMax.getValue() >= tgc::Pi ) ? -maxradius : maxradius * cosPhiMax;
	double xmax = maxradius;
	double ymin = ( m_phiMax.getValue() > tgc::Pi ) ?
					( ( m_phiMax.getValue() < 1.5*tgc::Pi ) ? maxradius * sinPhiMax : -maxradius ) :
					0.0;
	double ymax = ( m_phiMax.getValue() < tgc::Pi/2.0 )? maxradius * sinPhiMax : maxradius;

	box.setBounds(SbVec3f( xmin, ymin, 0 ), SbVec3f( xmax, ymax,  m_height.getValue() ) );

}

void ShapeCone::generatePrimitives(SoAction *action)
{
	Trace trace( "ShapeConeFactory::generatePrimitives", false );

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

