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

#include <algorithm>
#include <iomanip>
#include <map>
#include <vector>

#include <QIcon>
#include <QMap>
#include <QMessageBox>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "BBox.h"
#include "gc.h"
#include "gf.h"
#include "Ray.h"

#include "DifferentialGeometry.h"
#include "ShapeTroughAsymmetricCPC.h"

double fPart( double alpha, double x, double r1, double theta, double r, double eccentricity )
{
	return -r1 + sin( alpha - theta ) * ( r/ ( 1 - eccentricity * cos( alpha ) ) ) - x ;
}

SO_NODE_SOURCE(ShapeTroughAsymmetricCPC);

void ShapeTroughAsymmetricCPC::initClass()
{
	SO_NODE_INIT_CLASS(ShapeTroughAsymmetricCPC, TShape, "TShape");
}

ShapeTroughAsymmetricCPC::ShapeTroughAsymmetricCPC()
{
	SO_NODE_CONSTRUCTOR(ShapeTroughAsymmetricCPC);
	SO_NODE_ADD_FIELD( rInt, ( 0.0185 ) );
	SO_NODE_ADD_FIELD( rExt, ( 0.0253 ) );
	SO_NODE_ADD_FIELD( acceptanceAngleCW, ( gc::Pi / 6 ) );
	SO_NODE_ADD_FIELD( acceptanceAngleCCW, ( gc::Pi / 6 ) );
	SO_NODE_ADD_FIELD( truncationAngle, ( 0 ) );
	SO_NODE_ADD_FIELD( truncationOrigin, ( 1 ) );
	SO_NODE_ADD_FIELD( length, ( 1 ) );

	SetInternalValues();

	SoFieldSensor* m_rIntSensor = new SoFieldSensor(updateInternalValues, this);
	m_rIntSensor->setPriority( 1 );
	m_rIntSensor->attach( &rInt );
	SoFieldSensor* m_rExtSensor = new SoFieldSensor(updateInternalValues, this);
	m_rExtSensor->setPriority( 1 );
	m_rExtSensor->attach( &rExt );
	SoFieldSensor* m_acceptanceAngleCWSensor = new SoFieldSensor(updateInternalValues, this);
	m_acceptanceAngleCWSensor->setPriority( 1 );
	m_acceptanceAngleCWSensor->attach( &acceptanceAngleCW );
	SoFieldSensor* m_acceptanceAngleCCWSensor = new SoFieldSensor(updateInternalValues, this);
	m_acceptanceAngleCCWSensor->setPriority( 1 );
	m_acceptanceAngleCCWSensor->attach( &acceptanceAngleCCW );
	SoFieldSensor* m_lengthSensor = new SoFieldSensor(updateInternalValues, this);
	m_lengthSensor->setPriority( 1 );
	m_lengthSensor->attach( &length );
	SoFieldSensor* m_truncationAngleWSensor = new SoFieldSensor(updateInternalValues, this);
	m_truncationAngleWSensor->setPriority( 1 );
	m_truncationAngleWSensor->attach( &truncationAngle );
	SoFieldSensor* m_truncationOriginSensor = new SoFieldSensor(updateInternalValues, this);
	m_truncationOriginSensor->setPriority( 1 );
	m_truncationOriginSensor->attach( &truncationOrigin );

}

ShapeTroughAsymmetricCPC::~ShapeTroughAsymmetricCPC()
{
}


double ShapeTroughAsymmetricCPC::GetArea() const
{
	return -1;
}

BBox ShapeTroughAsymmetricCPC::GetBBox() const
{
	double xMin = ConcentratorProfileX( std::max( -( 3*gc::Pi/2 - acceptanceAngleCW.getValue() - m_tangentAngle ) , m_thetaMin ) );
	double xMax = ConcentratorProfileX( std::min( 3*gc::Pi/2 - acceptanceAngleCCW.getValue() - m_tangentAngle , m_thetaMax ) );
	double yMin = std::min( ConcentratorProfileY( ( gc::Pi / 2 ) - m_tangentAngle ) ,  ConcentratorProfileY( ( - gc::Pi / 2 ) - m_tangentAngle ) );
	double yMax = std::max( ConcentratorProfileY( m_thetaMax ) , ConcentratorProfileY( m_thetaMin ) );
	double zMin = 0.0;
	double zMax = length.getValue();

	return BBox( Point3D( xMin, yMin, zMin ), Point3D( xMax, yMax, zMax ) );

}

QString ShapeTroughAsymmetricCPC::GetIcon() const
{
	return ":/icons/ShapeTroughAsymmetricCPC.png";
}


double ShapeTroughAsymmetricCPC::GetVolume() const
{
	return 0.0;
}

bool ShapeTroughAsymmetricCPC::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	std::vector<double> roots = FindRoots( objectRay );
	if ( roots.size() == 0 ) return false;
	std::vector<double> tHits = FindThits( objectRay , roots );
	QMap<double,double> intersectionsMap;
	for ( unsigned int i = 0; i < roots.size(); i++)
		intersectionsMap.insert( tHits[ i ] , roots[ i ] );

	std::sort( tHits.begin(), tHits.end() );

	//For each tHit check tolerance and ray limits.
	double tol = 0.00001;
	unsigned int i = 0;
	bool valid = false;
	Point3D hitPoint;
	double thit;
	while(  ( i < tHits.size() ) && !valid )
	{
		thit = tHits[ i ];
		if( ( fabs( thit ) < tol ) || ( thit > objectRay.maxt ) || ( thit < objectRay.mint ) )
			valid = false;
		else
		{
			// Compute possible collector hit position. Only Z must be checked, as find root only computes roots within the shape limits.
			hitPoint = objectRay( thit );
			if( hitPoint.z < 0.0 || hitPoint.z > length.getValue() )	valid = false;
			else	valid = true;
		}
		i++;
	}
	if( !valid ) return false;
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) )	gf::SevereError( "Function Cylinder::Intersect(...) called with null pointers" );

	// Find parametric representation of CPC concentrator hit
	double thetaHit = intersectionsMap.value( thit );
	double u = ( thetaHit - m_thetaMin ) / ( m_thetaMax - m_thetaMin );
	double v = hitPoint.z / length.getValue();

	Vector3D dpdu = GetDPDU( u , v );
	Vector3D dpdv( 0.0 , 0.0 , 1.0 );
	Vector3D d2pduu = GetD2PDUU( u , v );
	Vector3D d2pduv( 0.0 , 0.0 , 0.0 );
	Vector3D d2pdvv( 0.0 , 0.0 , 0.0 );

	// Compute coefficients for fundamental forms
	double E = DotProduct( dpdu, dpdu );
	double F = DotProduct( dpdu, dpdv );
	double G = DotProduct( dpdv, dpdv );
	Vector3D N = Normalize( CrossProduct( dpdu, dpdv ) );
	double e = DotProduct( N, d2pduu );
	double f = DotProduct( N, d2pduv );
	double g = DotProduct( N, d2pdvv );

	// Compute \dndu and \dndv from fundamental form coefficients
	double invEGF2 = 1.0 / ( E * G - F * F );
	Vector3D dndu = ( f * F - e * G ) * invEGF2 * dpdu + ( e * F - f * E ) * invEGF2 * dpdv;
	Vector3D dndv = ( g * F - f * G ) * invEGF2 * dpdu + ( f * F - g * E ) * invEGF2 * dpdv;

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

bool ShapeTroughAsymmetricCPC::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}

Point3D ShapeTroughAsymmetricCPC::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

void ShapeTroughAsymmetricCPC::updateInternalValues( void *data, SoSensor *)
{
	ShapeTroughAsymmetricCPC* shapeTroughAsymmetricCPC = (ShapeTroughAsymmetricCPC *) data;
	shapeTroughAsymmetricCPC->SetInternalValues();
}

Point3D ShapeTroughAsymmetricCPC::GetPoint3D( double u, double v ) const
{
	double theta = m_thetaMin + u * ( m_thetaMax - m_thetaMin );
	double x = ConcentratorProfileX( theta );
	double y = ConcentratorProfileY( theta );
	double z = v * length.getValue();
	return Point3D( x, y, z );
}

NormalVector ShapeTroughAsymmetricCPC::GetNormal (double u ,double v) const
{
	Vector3D dpdu = GetDPDU( u, v );
	Vector3D dpdv( 0.0 , 0.0 , 1.0 );

	return Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
}

bool ShapeTroughAsymmetricCPC::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeTroughAsymmetricCPC::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
	double xMin = ConcentratorProfileX( std::max( -( 3*gc::Pi/2 - acceptanceAngleCW.getValue() - m_tangentAngle ) , m_thetaMin ) );
	double xMax = ConcentratorProfileX( std::min( 3*gc::Pi/2 - acceptanceAngleCCW.getValue() - m_tangentAngle , m_thetaMax ) );
	double yMin = std::min( ConcentratorProfileY( ( gc::Pi / 2 ) - m_tangentAngle ) ,  ConcentratorProfileY( ( - gc::Pi / 2 ) - m_tangentAngle ) );
	double yMax = std::max( ConcentratorProfileY( m_thetaMax ) , ConcentratorProfileY( m_thetaMin ) );
	double zMin = 0.0;
	double zMax = length.getValue();

	box.setBounds(SbVec3f( xMin, yMin, zMin ), SbVec3f( xMax, yMax, zMax ) );
}

void ShapeTroughAsymmetricCPC::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	const int rows = 100; // Number of points per row
    const int columns = 5; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid

    float vertex[totalPoints][6];

    int h = 0;
    double ui = 0;
	double vj = 0;

    for ( int i = 0; i < rows; ++i )
    {
    	ui =( 1.0 /(double)(rows-1) ) * i;

    	for ( int j = 0 ; j < columns ; ++j )
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

    float u = 1;
    float v = 1;
    beginShape(action, QUADS );
	for( int irow = 0; irow < (rows-1); ++irow )
	{
		for( int icolumn = 0; icolumn < (columns-1); ++icolumn )
		{
			int index0 = irow*columns + icolumn;
			SbVec3f  point0( vertex[index0][0], vertex[index0][1],  vertex[index0][2] );
			SbVec3f normal0(vertex[index0][3], vertex[index0][4], vertex[index0][5] );
			SbVec4f texCoord0 = useTexFunc ? tce->get(point0, normal0): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point0);
			pv.setNormal(normal0);
			pv.setTextureCoords(texCoord0);
			shapeVertex(&pv);

			int index1 = index0 + 1;
			SbVec3f  point1( vertex[index1][0], vertex[index1][1],  vertex[index1][2] );
			SbVec3f normal1(vertex[index1][3], vertex[index1][4], vertex[index1][5] );
			SbVec4f texCoord1 = useTexFunc ? tce->get(point1, normal1): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point1);
			pv.setNormal(normal1);
			pv.setTextureCoords(texCoord1);
			shapeVertex(&pv);

			int index3 = index0 + columns;
			int index2 = index3 + 1;

			SbVec3f  point2( vertex[index2][0], vertex[index2][1],  vertex[index2][2] );
			SbVec3f normal2(vertex[index2][3], vertex[index2][4], vertex[index2][5] );
			SbVec4f texCoord2 = useTexFunc ? tce->get(point2, normal2): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point2);
			pv.setNormal(normal2);
			pv.setTextureCoords(texCoord2);
			shapeVertex(&pv);

			SbVec3f  point3( vertex[index3][0], vertex[index3][1],  vertex[index3][2] );
			SbVec3f normal3(vertex[index3][3], vertex[index3][4], vertex[index3][5] );
			SbVec4f texCoord3 = useTexFunc ? tce->get(point3, normal3): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point3);
			pv.setNormal(normal3);
			pv.setTextureCoords(texCoord3);
			shapeVertex(&pv);

		}
	}

	endShape();


}

Vector3D ShapeTroughAsymmetricCPC::GetDPDURight( double acceptanceAngle, double theta ) const
{
	double involuteLimit = gc::Pi/2 + acceptanceAngle - m_tangentAngle;
	double x;
	double y;
	if( theta > involuteLimit )
	{
		x =   ( rInt.getValue() * ( 2*acceptanceAngle + gc::Pi + 2*( theta + m_tangentAngle ) - 4*m_thetaZero - 2*cos( acceptanceAngle - ( theta + m_tangentAngle ) ) )*( cos( acceptanceAngle ) + sin( ( theta + m_tangentAngle ) ) ) ) / ( 2 * pow( -1 + sin( acceptanceAngle - ( theta + m_tangentAngle ) ) , 2 ) );
		y = - ( rInt.getValue() * ( 2*acceptanceAngle + gc::Pi + 2*( theta + m_tangentAngle ) - 4*m_thetaZero - 2*cos( acceptanceAngle - ( theta + m_tangentAngle ) ) )*( cos( ( theta + m_tangentAngle ) ) - sin( acceptanceAngle ) ) ) / ( 2 * pow( -1 + sin( acceptanceAngle - ( theta + m_tangentAngle ) ) , 2 ) );
	}
	else
	{
		x = rInt.getValue() * ( theta + m_tangentAngle - m_thetaZero) * sin( theta + m_tangentAngle );
		y = - rInt.getValue() * ( theta + m_tangentAngle - m_thetaZero) * cos( theta + m_tangentAngle );
	}

	return Vector3D( x , y , 0.0 );
}

Vector3D ShapeTroughAsymmetricCPC::GetDPDU( double u , double /* v */ ) const
{
	double theta = m_thetaMin + u * ( m_thetaMax - m_thetaMin );
	Vector3D dpdu;

	if( theta >= 0.0 )
		dpdu = GetDPDURight( acceptanceAngleCCW.getValue() , theta ) * ( m_thetaMax - m_thetaMin );
	else
	{
		dpdu = GetDPDURight( acceptanceAngleCW.getValue() , - theta ) * ( m_thetaMax - m_thetaMin );
		dpdu = Vector3D( -dpdu.x , dpdu.y, 0.0 );
	}

	return dpdu;
}

Vector3D ShapeTroughAsymmetricCPC::GetD2PDUURight( double acceptanceAngle, double theta ) const
{
	double involuteLimit = gc::Pi/2 + acceptanceAngle - m_tangentAngle;
	double x;
	double y;
	if( theta > involuteLimit )
	{
		x = ( rInt.getValue() * (
				( 2 * acceptanceAngle + gc::Pi + 2 * ( theta + m_tangentAngle - 2 * m_thetaZero ) * ( cos( theta + m_tangentAngle ) + 2 * sin( acceptanceAngle ) ) )
				- 4 * sin( 2 * acceptanceAngle - ( theta + m_tangentAngle ) )
				- 5 * sin( theta + m_tangentAngle )
				- 4 * cos( acceptanceAngle ) ) ) /
				( -3 + cos( 2 * ( acceptanceAngle - ( theta + m_tangentAngle ) ) ) + 4 * sin( acceptanceAngle - ( theta + m_tangentAngle ) ));

		y = ( rInt.getValue() * (
				( 2 * acceptanceAngle + gc::Pi + 2 * ( theta + m_tangentAngle - 2 * m_thetaZero ) * ( 2 * cos( acceptanceAngle )- sin( theta + m_tangentAngle ) ) )
				+ cos( 2 * acceptanceAngle - ( theta + m_tangentAngle ) )
				+ 5 * cos( theta + m_tangentAngle )
				- 4 * sin( acceptanceAngle )
				) ) /
				( -3 + cos( 2 * ( acceptanceAngle - ( theta + m_tangentAngle ) ) ) + 4 * sin( acceptanceAngle - ( theta + m_tangentAngle ) ));
	}
	else
	{
		x = rInt.getValue() * ( ( theta + m_tangentAngle - m_thetaZero) * cos( theta + m_tangentAngle ) + sin( theta + m_tangentAngle ) );
		y = rInt.getValue() * ( ( theta + m_tangentAngle - m_thetaZero) * sin( theta + m_tangentAngle ) - cos( theta + m_tangentAngle ) );
	}

	return Vector3D( x , y , 0.0 );
}

Vector3D ShapeTroughAsymmetricCPC::GetD2PDUU( double u , double /* v */ ) const
{
	double theta = m_thetaMin + u * ( m_thetaMax - m_thetaMin );
	Vector3D d2pduu;

	if( theta >= 0.0 )
		d2pduu = GetD2PDUURight( acceptanceAngleCCW.getValue() , theta ) * pow( m_thetaMax - m_thetaMin , 2 );
	else
	{
		d2pduu = GetD2PDUURight( acceptanceAngleCW.getValue() , - theta ) * pow( m_thetaMax - m_thetaMin , 2 );
		d2pduu = Vector3D( -d2pduu.x , d2pduu.y, 0.0 );
	}

	return d2pduu;
}

/*!
 * Computes and sets \a m_phi, \a m_s, \a m_theta and \a m_eccentricity.
 */
void ShapeTroughAsymmetricCPC::SetInternalValues()
{
	m_tangentAngle = acos( rInt.getValue() / rExt.getValue() );
	m_thetaZero = m_tangentAngle - ( ( rExt.getValue()/rInt.getValue() ) * sin( m_tangentAngle ) );

	// Full length limits
	m_thetaMax = 3*gc::Pi/2 - acceptanceAngleCW.getValue() - m_tangentAngle;
	m_thetaMin = -( 3*gc::Pi/2 - acceptanceAngleCCW.getValue() - m_tangentAngle );

	// Truncation
	double thetaMaxTruncated = m_thetaMax;
	double thetaMinTruncated = m_thetaMin;

	Point3D truncationOr = Point3D( 0.0 , truncationOrigin.getValue() , 0.0 );
	Vector3D truncationDir = Vector3D( cos( truncationAngle.getValue() ) , sin( truncationAngle.getValue() ) , 0.0 );
	Ray truncationLine = Ray( truncationOr , truncationDir );
	std::vector<double> intersections = FindRoots( truncationLine );
	if( intersections.size() > 0 )
	{
		if( intersections[ 0 ] > 0.0 ) thetaMaxTruncated = intersections[ 0 ];
		if( intersections[ intersections.size() - 1 ] < 0.0 ) thetaMinTruncated = intersections[ intersections.size() - 1 ];
	}
	m_thetaMax = std::min( m_thetaMax , thetaMaxTruncated );
	m_thetaMin = std::max( m_thetaMin , thetaMinTruncated );

}

double ShapeTroughAsymmetricCPC::ConcentratorProfileX( double theta ) const
{
	double x;
	if( theta >= 0.0 )
	{
		double acceptanceAngle = acceptanceAngleCCW.getValue();
		double involuteLimit = gc::Pi/2 + acceptanceAngle - m_tangentAngle;
		double ro;
		if( theta < involuteLimit )
			ro = rInt.getValue() * ( theta + m_tangentAngle - m_thetaZero );
		else
			ro = ( rInt.getValue() * ( ( theta + m_tangentAngle + acceptanceAngle + ( gc::Pi / 2 ) - ( 2 * m_thetaZero ) ) -  cos( theta + m_tangentAngle - acceptanceAngle )  ) ) / ( 1 + sin( theta + m_tangentAngle - acceptanceAngle ) );

		x = ( rInt.getValue() * sin( theta + m_tangentAngle ) + ro * sin( theta + m_tangentAngle - ( gc::Pi / 2 ) ) );
	}
	else
	{
		double negTheta = -theta;
		double acceptanceAngle = acceptanceAngleCW.getValue();
		double involuteLimit = gc::Pi/2 + acceptanceAngle - m_tangentAngle;
		double ro;
		if( negTheta < involuteLimit )
			ro = rInt.getValue() * ( negTheta + m_tangentAngle - m_thetaZero );
		else
			ro = ( rInt.getValue() * ( ( negTheta + m_tangentAngle + acceptanceAngle + ( gc::Pi / 2 ) - ( 2 * m_thetaZero ) ) -  cos( negTheta + m_tangentAngle - acceptanceAngle )  ) ) / ( 1 + sin( negTheta + m_tangentAngle - acceptanceAngle ) );

		x = - ( rInt.getValue() * sin( negTheta + m_tangentAngle ) + ro * sin( negTheta + m_tangentAngle - ( gc::Pi / 2 ) ) );
	}

	return x;
}

double ShapeTroughAsymmetricCPC::ConcentratorProfileY( double theta ) const
{
	double y;
	if( theta >= 0.0 )
	{
		double acceptanceAngle = acceptanceAngleCCW.getValue();
		double involuteLimit = gc::Pi/2 + acceptanceAngle - m_tangentAngle;
		double ro;
		if( theta < involuteLimit )
			ro = rInt.getValue() * ( theta + m_tangentAngle - m_thetaZero );
		else
			ro = ( rInt.getValue() * ( ( theta + m_tangentAngle + acceptanceAngle + ( gc::Pi / 2 ) - ( 2 * m_thetaZero ) ) -  cos( theta + m_tangentAngle - acceptanceAngle )  ) ) / ( 1 + sin( theta + m_tangentAngle - acceptanceAngle ) );

		y = ( - rInt.getValue() * cos( theta + m_tangentAngle ) - ro * cos( theta + m_tangentAngle - ( gc::Pi / 2 ) ) );
	}
	else
	{
		double negTheta = -theta;
		double acceptanceAngle = acceptanceAngleCW.getValue();
		double involuteLimit = gc::Pi/2 + acceptanceAngle - m_tangentAngle;
		double ro;
		if( negTheta < involuteLimit )
			ro = rInt.getValue() * ( negTheta + m_tangentAngle - m_thetaZero );
		else
			ro = ( rInt.getValue() * ( ( negTheta + m_tangentAngle + acceptanceAngle + ( gc::Pi / 2 ) - ( 2 * m_thetaZero ) ) -  cos( negTheta + m_tangentAngle - acceptanceAngle )  ) ) / ( 1 + sin( negTheta + m_tangentAngle - acceptanceAngle ) );

		y = ( - rInt.getValue() * cos( negTheta + m_tangentAngle ) - ro * cos( negTheta + m_tangentAngle - ( gc::Pi / 2 ) ) );
	}

	return y;
}

std::vector<double> ShapeTroughAsymmetricCPC::FindRigthRoots( const Ray ray, double thetaStart , double thetaEnd , double acceptanceAngle ) const
{
	std::vector<double> roots;
	//Set general algorithm parameters
	double maxIterations = 500;
	double tolerance = 0.00000001;

	//Initialize starting and ending point
	double theta0 = thetaStart;
	double theta1 = thetaEnd;

	//Set step for going through the space in case YDistance(start) * YDistance(end) > 0 (both positives or negatives)
	double delta = fabs( theta1 - theta0 ) / 100;
	// Define an auxiliar value with -Infinity. This variable will hold the closest theta value next to the not computed root in case there are two roots
	double thetaAux = - gc::Infinity;

	//Compute extreme points
	double tDeviation0 = TDeviation( ray, theta0 , acceptanceAngle );
	double tDeviation1 = TDeviation( ray, theta1 , acceptanceAngle );

	//If YDistance(start) * YDistance(end) > 0 add the defined step to the starting point
	// until YDistance(start) * YDistance(end) < 0 condition is satisfied.
	while( ( ( tDeviation0 * tDeviation1 ) > 0.0 ) && ( theta0 < theta1 ) )
	{
		//Set theta aux if while is entered.
		thetaAux = theta0;
		theta0 += delta;
		tDeviation0 = TDeviation( ray, theta0 , acceptanceAngle );
	}
	// If YDistance(start) * YDistance(end) < 0 condition is NOT satisfied, there are no roots --> return.
	if( ( ( tDeviation0 * tDeviation1 ) > 0.0 ) || ( theta0 > theta1 ) ) return roots;

	// Once YDistance(start) * YDistance(end) < 0 iterate by the bisection method
	int iterations = 0;
	while( ( fabs( theta1 - theta0 ) > tolerance ) && ( iterations < maxIterations ) )
	{
		double thetaMiddle = ( theta0 + theta1 ) / 2.0 ;
		double tDeviationMiddle = TDeviation( ray, thetaMiddle , acceptanceAngle );

		if( ( tDeviationMiddle > 0.0 ) && ( tDeviation1 > tDeviation0 ) ) theta1 = thetaMiddle;
		else if( ( tDeviationMiddle > 0.0 ) && ( tDeviation1 < tDeviation0 ) ) theta0 = thetaMiddle;
		else if( ( tDeviationMiddle < 0.0 ) && ( tDeviation1 > tDeviation0 ) ) theta0 = thetaMiddle;
		else theta1 = thetaMiddle;

		iterations++;
	}

	//If max iterations has been reached --> return, else insert the root (mean) into the roots vector.
	if( iterations >= maxIterations ) return roots;
	roots.push_back( ( theta0 + theta1 ) / 2.0 );

	// If thetaAux has been used (no more -Infinity value), this should be close to the non-computed root. Update theta0 and theta1 and repeat iteration.
	if( thetaAux < thetaStart ) return roots;

	theta0 = std::max( thetaStart , thetaAux - delta );
	theta1 = std::min( thetaAux + delta , thetaEnd );

	tDeviation0 = TDeviation( ray, theta0 , acceptanceAngle );
	tDeviation1 = TDeviation( ray, theta1 , acceptanceAngle );

	if( ( tDeviation0 * tDeviation1 ) > 0.0 ) return roots;

	iterations = 0;
	while( ( fabs( theta1 - theta0 ) > tolerance ) && ( iterations < maxIterations ) )
	{
		double thetaMiddle = ( theta0 + theta1 ) / 2.0 ;
		double tDeviationMiddle = TDeviation( ray, thetaMiddle , acceptanceAngle );

		if( ( tDeviationMiddle > 0.0 ) && ( tDeviation1 > tDeviation0 ) ) theta1 = thetaMiddle;
		else if( ( tDeviationMiddle > 0.0 ) && ( tDeviation1 < tDeviation0 ) ) theta0 = thetaMiddle;
		else if( ( tDeviationMiddle < 0.0 ) && ( tDeviation1 > tDeviation0 ) ) theta0 = thetaMiddle;
		else theta1 = thetaMiddle;

		iterations++;
	}


	if( iterations >= maxIterations ) return roots;
	roots.push_back( ( theta0 + theta1 ) / 2.0 );

	return roots;
}

std::vector<double> ShapeTroughAsymmetricCPC::FindRoots( const Ray ray ) const
{
	// Right branch roots
	std::vector<double> roots = FindRigthRoots( ray, 0 , m_thetaMax , acceptanceAngleCCW.getValue() );

	// Left branch hits calculated with the symmetric ray.
	Point3D symmetricRayOrigin = Point3D( - ray.origin.x , ray.origin.y , ray.origin.z );
	Vector3D symmetricRayDirection = Vector3D( - ray.direction().x , ray.direction().y , ray.direction().z );
	Ray symmetricRay = Ray( symmetricRayOrigin , symmetricRayDirection );

	std::vector<double> rootsLeft = FindRigthRoots( symmetricRay, 0 , - m_thetaMin , acceptanceAngleCW.getValue() );

	// Change sign to roots thetaRoot --> -thetaRoot and append to roots
	for ( unsigned int i = 0; i < rootsLeft.size(); i++)
		roots.push_back( -1.0 * rootsLeft[i] );

	return roots;
}


double ShapeTroughAsymmetricCPC::TDeviation( const Ray ray, double theta, double acceptanceAngle ) const
{
	double dx = ray.direction().x;
	double ox = ray.origin.x;
	double dy = ray.direction().y;
	double oy = ray.origin.y;

	double involuteLimit = gc::Pi/2 + acceptanceAngle - m_tangentAngle;
	if( theta < involuteLimit )
		return -dy * ox + dx * oy + rInt.getValue() * ( dx - dy * ( theta + m_tangentAngle - m_thetaZero ) ) * cos( theta + m_tangentAngle ) + rInt.getValue() * ( dy + dx * ( theta + m_tangentAngle - m_thetaZero ) ) * sin( theta + m_tangentAngle );
	else
		return ( ( -2 * dy * rInt.getValue() * cos( acceptanceAngle ) )
			+ rInt.getValue() * ( -2 * dx + dy * ( 2 * acceptanceAngle + gc::Pi + 2 * ( theta + m_tangentAngle - 2 * m_thetaZero ) ) ) * cos( theta + m_tangentAngle )
			+ 2 * ( dx * rInt.getValue() * sin( acceptanceAngle ) - ( dy * ox - dx * oy ) * ( -1 + sin( acceptanceAngle - theta - m_tangentAngle ) ) )
			- rInt.getValue() * ( 2*dy + dx * ( 2 * acceptanceAngle + gc::Pi + 2 * ( theta + m_tangentAngle - 2 * m_thetaZero ) ) ) * sin( theta + m_tangentAngle ) ) /
			( 2*( -1 + sin( acceptanceAngle - theta - m_tangentAngle ) ) );

}

std::vector<double> ShapeTroughAsymmetricCPC::FindThits( const Ray ray, const std::vector<double> roots ) const
{
	double tolerance = 0.000001;
	std::vector<double> tHits;
	for( unsigned int i = 0 ; i < roots.size(); ++i )
	{
		double x = ConcentratorProfileX( roots[ i ] );
		double y = ConcentratorProfileY( roots[ i ] );

		double a = 1 - ray.direction().z * ray.direction().z;
		double b = 0;
		double c = - ( x - ray.origin.x ) * ( x - ray.origin.x ) - ( y - ray.origin.y ) * ( y - ray.origin.y );

		double t0;
		double t1;

		gf::Quadratic( a, b, c, &t0, &t1 );

		if (  std::fabs( ray( t0 ).y - y ) < tolerance ) tHits.push_back( t0 );
		else tHits.push_back( t1 );
	}

	return tHits;
}
