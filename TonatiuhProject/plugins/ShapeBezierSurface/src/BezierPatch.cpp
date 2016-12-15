/***************************************************************************
Copyright (C) 2008 by the Ekhi Software Development Team.

This file is part of Ekhi.

Ekhi program is free software: you can redistribute it and/or modify
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

The development of Ekhi was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Ekhi under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Ekhi is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <algorithm>
#include <iostream>
#include <map>

//#include <QMap>
//#include <QVector>

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/fields/SoMFVec3d.h>

#include "BezierPatch.h"
#include "DifferentialGeometry.h"
#include "NormalVector.h"
#include "Ray.h"
#include "Vector3D.h"

BezierPatch::BezierPatch()
:m_nIterations( 100 )
{

}

BezierPatch::~BezierPatch()
{

}


Point3D BezierPatch::GetPoint3D (double u, double v) const
{
	Point3D point = Point3D( Vector3D(  m_controlPoints[0] * (-1 + u) * (-1 + u) * (-1 + u) * (-1 + v) * (-1 + v) * (-1 + v) )
		- Vector3D( 3 * m_controlPoints[4] * (-1 + u)* (-1 + u) * u * (-1 + v) * (-1 + v) * (-1 + v) )
		+ Vector3D( 3 * m_controlPoints[8] * (-1 + u) *  u * u * (-1 + v) * (-1 + v) * (-1 + v) )
		- Vector3D( m_controlPoints[12] * u * u * u * (-1 + v) * (-1 + v) * (-1 + v) )
		- Vector3D( 3 * m_controlPoints[1] * (-1 + u) * (-1 + u) * (-1 + u) * (-1 + v)* (-1 + v) * v )
		+ Vector3D( 9 * m_controlPoints[5] * (-1 + u) * (-1 + u) * u * (-1 + v)* (-1 + v) * v )
		- Vector3D( 9 * m_controlPoints[9] * (-1 + u) * u * u * (-1 + v)* (-1 + v) * v )
		+ Vector3D(3 * m_controlPoints[13] * u * u * u * (-1 + v) * (-1 + v) * v )
		+ Vector3D(3 * m_controlPoints[2] * (-1 + u)  * (-1 + u)  * (-1 + u)* (-1 + v) * v * v )
		- Vector3D( 9 * m_controlPoints[6] * (-1 + u)  * (-1 + u) * u * (-1 + v) * v * v )
		+ Vector3D(9 * m_controlPoints[10] * (-1 + u) * u * u * (-1 + v) * v * v )
		- Vector3D( 3 * m_controlPoints[14] * u * u * u * (-1 + v) * v * v )
		- Vector3D(  m_controlPoints[3] * (-1 + u) * (-1 + u) * (-1 + u) * v * v * v )
		+ Vector3D(3 * m_controlPoints[7] * (-1 + u) * (-1 + u)* u * v * v * v )
		- Vector3D( 3 * m_controlPoints[11] * (-1 + u) * u * u * v * v * v )
		+ Vector3D(m_controlPoints[15] * u * u * u * v * v * v ) );

	return ( point );
}

NormalVector BezierPatch::GetNormal( double u, double v ) const
{
	Vector3D dpdu = DPDU( u, v );
	Vector3D dpdv = DPDV( u, v );

	return ( NormalVector( Normalize( CrossProduct( dpdu, dpdv ) ) ) );

}
void BezierPatch::GeneratePrimitives( SoAction* /*action*/ )
{
	//SoCoordinateElement::set3(action->getState(), this, m_controlPoints.getNum(), m_controlPoints.getValues( 0 ) );
	//SoNurbsSurface::generatePrimitives( action );
}

bool BezierPatch::Intersect(const Ray& objectRay, double* tHit, DifferentialGeometry* dg, double bezierTol ) const
{
	//Generate planes u, v perpendicular between themself which intersection is the ray, and plane w perpendicular to the ray which contains the objectRay.origin
	Vector3D t;
	if( fabs(objectRay.direction().x )< fabs(objectRay.direction().y ) )
	{
		if( fabs(objectRay.direction().x )< fabs(objectRay.direction().z ) )	t = Vector3D( 1.0, 0.0, 0.0 );
		else	t = Vector3D( 0.0, 0.0, 1.0 );
	}
	else
	{
		if( fabs(objectRay.direction().y )< fabs(objectRay.direction().z )	) t = Vector3D( 0.0, 1.0, 0.0 );
		else	t = Vector3D( 0.0, 0.0, 1.0 );
	}

	//Nu and Nv are planes mutually perpendicular and contains ray direction
	Vector3D nu = Normalize( CrossProduct( t, objectRay.direction() ) );
	Vector3D nv = Normalize( CrossProduct( nu, objectRay.direction() ) );

	//Nw is a plane perpendicural to contains ray direction
	Vector3D nw = Normalize( objectRay.direction() );

	//The planes contains the ray origin .	Anx rox + Bny roy + Cmn roz + D == 0
	double au = nu.x;
	double av = nv.x;
	double bu = nu.y;
	double bv = nv.y;
	double cu = nu.z;
	double cv = nv.z;
	double du = DotProduct( -nu, Vector3D( objectRay.origin ) );
	double dv = DotProduct( -nv, Vector3D( objectRay.origin ) );
	double dw = DotProduct( -nw, Vector3D( objectRay.origin ) );

	std::vector< std::vector< Vector3D > > buffer1;
	std::vector< Vector3D > cPCopy;// = m_controlPoints;
	for( unsigned int p = 0; p < m_controlPoints.size(); p++ )
		cPCopy.push_back( Vector3D( m_controlPoints[p] ) );
	SplitIPatch( cPCopy, &buffer1 );

	std::vector < double > previousDistance;
	std::vector < double > currentDistance;
	double error = gc::Infinity;

	double tol = 0.0000000001; //tolerance for the intersection rutine
	double lengthBB =  Distance( m_bbox.pMin, m_bbox.pMax ) ;
	int nIterations = ceil( log( 1 /  ( tol/ lengthBB ) ) * log( 2 ) )+1;

	int iteration = 0;

	while( ( iteration < nIterations ) && ( error > tol ) )
	{
		//Split a subpatch in 4 pieces
		std::vector< std::vector< Vector3D > > buffer2;
		for( unsigned int patch = 0; patch < buffer1.size(); patch++ )
		{
			SplitIPatch( buffer1[patch], &buffer2 );
		}
		buffer1.clear();

		//Hull clasification
		for( unsigned int patch = 0; patch< buffer2.size(); patch++ )
		{
			std::vector< Vector3D > iPatch = buffer2[patch];
			double pu1, pu2, pv1, pv2;

			Point3D p00 = Point3D( iPatch[ 0 ] );
			pu2 = au * p00[0] + bu * p00[1] + cu * p00[2] + du;
			pv2 = av * p00[0] + bv * p00[1] + cv * p00[2]+ dv;

			double intersectsU = false;
			double intersectsV = false;
			for( int i = 0; i < 16; i++ )
			{
				Point3D iPoint = Point3D( iPatch[ i ] );
				pu1 = pu2;
				pu2 = au * iPoint[0] + bu * iPoint[1] + cu * iPoint[2] + du;
				pv1 = pv2;
				pv2 = av * iPoint[0] + bv * iPoint[1] + cv * iPoint[2]+ dv;
				if( ( pu1 * pu2 < 0 )  )	intersectsU = true;
				if(  pv1 * pv2 < 0 )	intersectsV = true;
			}

			//Compute patch center and distance to patch center point
			if( intersectsU && intersectsV )
			{
				//Projection in w plane of the more external patch vertex
				Vector3D proyVertex1 = iPatch[0] - nw * ( DotProduct( Vector3D( iPatch[0] ), nw ) + dw);
				Vector3D proyVertex2 = iPatch[3] - nw * ( DotProduct( Vector3D( iPatch[3]  ), nw ) + dw);
				Vector3D proyVertex3 = iPatch[12] - nw * ( DotProduct( Vector3D( iPatch[12] ), nw ) + dw);
				Vector3D proyVertex4 = iPatch[15] - nw * ( DotProduct( Vector3D( iPatch[15] ), nw ) + dw);

				//Determine if the objectRay.origin is inside the patch or not
				if( DotProduct( CrossProduct( Vector3D( proyVertex2 - proyVertex1 ), Vector3D( objectRay.origin - proyVertex1 ) ), CrossProduct( Vector3D( proyVertex3 - proyVertex1 ), Vector3D( objectRay.origin - proyVertex1 ) ) ) < 0 &&
						DotProduct( CrossProduct( Vector3D( proyVertex2 - proyVertex4 ), Vector3D( objectRay.origin - proyVertex4 ) ), CrossProduct( Vector3D( proyVertex3 - proyVertex4 ), Vector3D( objectRay.origin - proyVertex4 ) ) ) < 0 )
				{
					buffer1.push_back( iPatch );
					Point3D center = Point3D( ( 1 / 64.0 ) * ( iPatch[0] +  3 * iPatch[4]
													+ 3 * iPatch[8] + iPatch[12]
													+ 3 * iPatch[1] +  9 * iPatch[5]
													+ 9 * iPatch[9] +  3 * iPatch[13]
													+ 3 * iPatch[2] +  9 * iPatch[6]
													+ 9 * iPatch[10] +  3 * iPatch[14]
													+ iPatch[3] +  3 * iPatch[7]
													+ 3 * iPatch[11] + iPatch[15] ) );
					currentDistance.push_back( Distance( center, objectRay.origin ) );

					if( iteration == 0 ) previousDistance.push_back( gc::Infinity );
				}
			}
		}

		if( buffer1.size() == 0 ) return ( false );

		if ( currentDistance.size() == previousDistance.size() )
		{
			std::vector< double > errorList;
			for( unsigned e = 0; e < currentDistance.size(); e++ )
				errorList.push_back( fabs( previousDistance[e] - currentDistance[e] ) );

			error = *std::max_element( errorList.begin(), errorList.end() );
			previousDistance = currentDistance;
			currentDistance.clear();
			errorList.clear();
		}
		else return ( false );
		buffer2.clear();

		iteration++;
	}

	// Now check if the function is being called from IntersectP,
		// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) )	return ( true );

	std::vector < double > distances;
	for( unsigned int a = 0; a < previousDistance.size(); a++ )
	{
		double thitRay;
		if( objectRay.direction().x != 0 ) thitRay = ( objectRay( previousDistance[a] ).x - objectRay.origin.x ) / objectRay.direction().x;
		else if ( objectRay.direction().y != 0 ) thitRay = ( objectRay( previousDistance[a] ).y - objectRay.origin.y ) / objectRay.direction().y;
		else	thitRay = ( objectRay( previousDistance[a] ).z - objectRay.origin.z ) / objectRay.direction().z;
		distances.push_back( thitRay );
	}
	previousDistance.clear();

	std::map< double, std::vector< Vector3D > > distancesMap;
	for( unsigned int i = 0; i < buffer1.size(); i++ )
		distancesMap.insert( std::pair< double, std::vector< Vector3D > > ( distances[i], buffer1[i] ) );

	std::sort( distances.begin(), distances.end() );

	std::vector < double > thitList;
	for( unsigned int i = 0; i < distances.size(); ++i)
		if( ( distances[i] < objectRay.maxt ) && ( distances[i] > bezierTol ) ) thitList.push_back( distances[i] );

	if( thitList.size() == 0 ) return ( false );

	std::vector< Vector3D > iPatch = distancesMap[ thitList[0] ];
	double thit = thitList[0];

	buffer1.clear();
	distances.clear();
	distancesMap.clear();

	Vector3D dpdu = DPDU( 0.5, 0.5, &iPatch);
	Vector3D dpdv = DPDV( 0.5, 0.5, &iPatch);

	// Compute cylinder \dndu and \dndv
	Vector3D d2Pduu = D2PDUU( 0.5, 0.5, &iPatch );
	Vector3D d2Pduv= D2PDUV( 0.5, 0.5, &iPatch );
	Vector3D d2Pdvv= D2PDVV( 0.5, 0.5, &iPatch );

	// Compute coefficients for fundamental forms
	double E = DotProduct( dpdu, dpdu );
	double F = DotProduct( dpdu, dpdv );
	double G = DotProduct( dpdv, dpdv );
	Vector3D N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );;
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
	*dg = DifferentialGeometry( objectRay( thit ) ,
								dpdu,
								dpdv,
								dndu,
								dndv,
								0.5, 0.5, 0 );
	*tHit = thit;
	thitList.clear();
	return ( true );

}

void BezierPatch::SetControlPoints( std::vector< Point3D > boundedPoints )
{
	//derivates: qu00, qv00, quv00, qu01, qv01, quv01, qu10, qv10, quv10, qu11, qv11,quv11
	std::vector< Vector3D > derivates = CornerDerivates( boundedPoints );

	//m_controlPointsList << boundedPoints[0]; //p00
	Point3D p00  = boundedPoints[0];
	m_controlPoints.push_back( p00 );
	Point3D p01 = boundedPoints[0] + derivates[1]/ 3; //p01 = derivates[1]/3 + p00;
	m_controlPoints.push_back( p01 );
	Point3D p02 = boundedPoints[9] - derivates[4]/ 3; //	p02 = p03 - Qv01[curves]/3;
	m_controlPoints.push_back( p02 );
	Point3D p03 = boundedPoints[9];	//p03 = curves[[10]];
	m_controlPoints.push_back( p03 );

	Point3D p10  = boundedPoints[0] + derivates[0] / 3; //p10 = p00 + Qu00[curves]/3;
	m_controlPoints.push_back( p10 );
	Point3D p11  = boundedPoints[0] + derivates[2]/ 9 + ( derivates[0] + derivates[1] ) / 3; //p11 = Quv00[curves]/9 + (Qu00[curves] + Qv00[curves])/3 + p00;
	m_controlPoints.push_back( p11 );
	Point3D p12  = boundedPoints[9] + ( derivates[3] - derivates[4] ) / 3 - derivates[5]/ 9; //p12 = p03 + (Qu01[curves] - Qv01[curves])/3 - Quv01[curves]/9;
	m_controlPoints.push_back( p12 );
	Point3D p13  =  boundedPoints[9] + derivates[3] / 3; //p13 = Qu01[curves]/3 + p03;
	m_controlPoints.push_back( p13 );

	Point3D p20  = boundedPoints[3] - derivates[6] / 3; //p20 = p30 - Qu10[curves]/3;
	m_controlPoints.push_back( p20 );
	Point3D p21  = boundedPoints[3] + ( derivates[7] - derivates[6] ) / 3 - derivates[8] / 9; //p21 = p30 + (Qv10[curves] - Qu10[curves])/3 - Quv10[curves]/9;
	m_controlPoints.push_back( p21 );
	Point3D p22  = boundedPoints[6] + derivates[11] / 9 - ( derivates[9] + derivates[10] ) / 3; //p22 = Quv11[curves]/9 - (Qu11[curves] + Qv11[curves])/3 + p33;
	m_controlPoints.push_back( p22 );
	Point3D p23  = boundedPoints[6] - derivates[9] / 3; //p23 = p33 - Qu11[curves]/3;
	m_controlPoints.push_back( p23 );

	Point3D p30  = boundedPoints[3]; //p30
	m_controlPoints.push_back( p30 );
	Point3D p31  = boundedPoints[3] + derivates[7]/ 3; //p31 = Qv10[curves]/3 + p30;
	m_controlPoints.push_back( p31 );
	Point3D p32  = boundedPoints[6] - derivates[10]/ 3; //p32 = p33 - Qv11[curves]/3;
	m_controlPoints.push_back( p32 );
	Point3D p33  = boundedPoints[6]; //p33
	m_controlPoints.push_back( p33 );

	m_bbox = BBox();
	int nControlPoints = m_controlPoints.size();
	for (int i = 0; i < nControlPoints; i++)
		m_bbox = Union( m_bbox , m_controlPoints[i] );

	m_centoid = Point3D( m_bbox.pMin.x + 0.5 * ( m_bbox.pMax.x - m_bbox.pMin.x  ),
			m_bbox.pMin.y + 0.5 * ( m_bbox.pMax.y - m_bbox.pMin.y  ),
			m_bbox.pMin.z + 0.5 * ( m_bbox.pMax.z - m_bbox.pMin.z  ) );

}

std::vector< Vector3D >  BezierPatch::CornerDerivates( std::vector< Point3D > boundedPoints )
{
	std::vector< Vector3D > cornerDerivates;

	//qu00
	Vector3D qu00 = 3 * (  boundedPoints[1]- boundedPoints[0]);
	cornerDerivates.push_back( qu00 );

	//qv00
	Vector3D qv00 =  3 * ( boundedPoints[11] - boundedPoints[0]);
	cornerDerivates.push_back( qv00 );

	//quv00
	cornerDerivates.push_back( 0.0 );

	//qu01
	Vector3D qu01 = 3 * ( boundedPoints[8]- boundedPoints[9]);
	cornerDerivates.push_back( qu01 );

	//qv01
	Vector3D qv01 =  3 * ( boundedPoints[9]-  boundedPoints[10]);
	cornerDerivates.push_back( qv01 );

	//quv01
	cornerDerivates.push_back(  0.0 );

	//qu10
	Vector3D qu10 = 3 * ( boundedPoints[3] - boundedPoints[2]);
	cornerDerivates.push_back( qu10 );

	//qv10
	Vector3D qv10 =  3 * ( boundedPoints[4] - boundedPoints[3] );
	cornerDerivates.push_back( qv10 );

	//quv10
	cornerDerivates.push_back(  0.0 );

	//qu11
	Vector3D qu11 = 3 * ( boundedPoints[6] - boundedPoints[7]);
	cornerDerivates.push_back( qu11 );

	//qt11
	Vector3D qv11 =  3 * ( boundedPoints[6] - boundedPoints[5] );
	cornerDerivates.push_back( qv11 );

	//quv11
	cornerDerivates.push_back(  0.0 );

	return cornerDerivates;
}

Vector3D BezierPatch::DPDU( double u, double v, std::vector< Vector3D >* controlPoint ) const
{
	Vector3D dpdu;

	if( controlPoint == 0 )
	{
		dpdu = Vector3D( 3 * m_controlPoints[0] * pow(-1 + u, 2 ) * pow(-1 + v, 3 ) )
				- Vector3D( 3 * m_controlPoints[12] *  pow( u, 2 ) * pow(-1 + v, 3 ) )
				+ Vector3D( 3 * m_controlPoints[8] * u * (-2 + 3 * u ) * pow(-1 + v, 3 ) )
				- Vector3D( 3 * m_controlPoints[4] * (-1 + u) * (-1 + 3 * u) * pow(-1 + v, 3 ) )
				- Vector3D( 9 * m_controlPoints[1] * pow(-1 + u, 2 ) * pow(-1 + v, 2) * v )
				+ Vector3D( 9 * m_controlPoints[9] * (2 - 3 * u ) *  u * pow(-1 + v, 2) * v )
				+ Vector3D( 9 * m_controlPoints[13] * pow( u, 2 ) * pow(-1 + v, 2 ) * v )
				+ Vector3D( 9 * m_controlPoints[5] * (-1 + u) * (-1 + 3 * u) * pow(-1 + v, 2) * v )
				+ Vector3D( 9 * m_controlPoints[2] * pow(-1 + u, 2 ) * (-1 + v) * pow( v, 2 ) )
				- Vector3D( 9 * m_controlPoints[14] * u * u * (-1 + v) * v * v )
				+ Vector3D( 9 * m_controlPoints[10] * u * (-2 + 3 * u) * (-1 + v) * v * v )
				- Vector3D( 9 * m_controlPoints[6] * (-1 + u) * (-1 + 3 * u) * (-1 + v) * v * v )
				- Vector3D( 3 * m_controlPoints[3] * pow(-1 + u, 2) * v * v * v )
				+ Vector3D( 3 * m_controlPoints[11] * (2 - 3 * u) * u * v * v* v )
				+ Vector3D( 3 * m_controlPoints[15] * u * u * v * v * v )
				+ Vector3D( 3 * m_controlPoints[7] * (-1 + u) * (-1 + 3 * u) * v * v * v );
	}
	else
	{
		dpdu = Vector3D( 3 * controlPoint->at( 0 ) * pow(-1 + u, 2 ) * pow(-1 + v, 3 ) )
		- Vector3D( 3 * controlPoint->at( 12 ) *  pow( u, 2 ) * pow(-1 + v, 3 ) )
		+ Vector3D( 3 * controlPoint->at( 8 ) * u * (-2 + 3 * u ) * pow(-1 + v, 3 ) )
		- Vector3D( 3 * controlPoint->at( 4 ) * (-1 + u) * (-1 + 3 * u) * pow(-1 + v, 3 ) )
		- Vector3D( 9 * controlPoint->at( 1 ) * pow(-1 + u, 2 ) * pow(-1 + v, 2) * v )
		+ Vector3D( 9 * controlPoint->at( 9 )* (2 - 3 * u ) *  u * pow(-1 + v, 2) * v )
		+ Vector3D( 9 * controlPoint->at( 13 ) * pow( u, 2 ) * pow(-1 + v, 2 ) * v )
		+ Vector3D( 9 * controlPoint->at( 5 )* (-1 + u) * (-1 + 3 * u) * pow(-1 + v, 2) * v )
		+ Vector3D( 9 * controlPoint->at( 2 ) * pow(-1 + u, 2 ) * (-1 + v) * pow( v, 2 ) )
		- Vector3D( 9 * controlPoint->at( 14 ) * u * u * (-1 + v) * v * v )
		+ Vector3D( 9 * controlPoint->at( 10 ) * u * (-2 + 3 * u) * (-1 + v) * v * v )
		- Vector3D( 9 * controlPoint->at( 6 ) * (-1 + u) * (-1 + 3 * u) * (-1 + v) * v * v )
		- Vector3D( 3 * controlPoint->at( 3 ) * pow(-1 + u, 2) * v * v * v )
		+ Vector3D( 3 * controlPoint->at( 11 ) * (2 - 3 * u) * u * v * v* v )
		+ Vector3D( 3 * controlPoint->at( 15 )* u * u * v * v * v )
		+ Vector3D( 3 * controlPoint->at( 7 ) * (-1 + u) * (-1 + 3 * u) * v * v * v );
	}

	return ( dpdu );
}

Vector3D BezierPatch::DPDV( double u, double v, std::vector< Vector3D >* controlPoint ) const
{
	Vector3D dpdv;

	if( controlPoint == 0 )
	{
		dpdv = Vector3D( 3 * m_controlPoints[0] * pow(-1 + u, 3 ) * pow(-1 + v, 2 ) )
				- Vector3D( 9 * m_controlPoints[4] * pow(-1 + u, 2 ) * u * pow(-1 + v, 2 ) )
				+ Vector3D( 9 * m_controlPoints[8] * (-1 + u) * u * u * pow(-1 + v, 2 ) )
				- Vector3D( 3 * m_controlPoints[12] * u * u * u * pow(-1 + v, 2 ) )
				+ Vector3D( 3 * m_controlPoints[14] * u * u * u * (2 - 3 * v) * v )
				- Vector3D( 3 * m_controlPoints[3] * pow(-1 + u, 3 ) * v * v )
				+ Vector3D( 9 * m_controlPoints[7] * pow(-1 + u, 2 ) * u * v * v )
				- Vector3D( 9 * m_controlPoints[11] * (-1 + u) * u * u * v * v )
				+ Vector3D( 3 * m_controlPoints[15] * u * u * u * v * v )
				+ Vector3D( 3 * m_controlPoints[2] * pow(-1 + u, 3 ) * v * (-2 + 3 * v) )
				- Vector3D( 9 * m_controlPoints[6] * pow(-1 + u, 2 ) * u * v * (-2 + 3 * v) )
				+ Vector3D( 9 * m_controlPoints[10] * (-1 + u) * u * u * v * (-2 + 3 * v) )
				- Vector3D( 3 * m_controlPoints[1] * pow(-1 + u, 3 ) * (-1 + v) * (-1 + 3 * v) )
				+ Vector3D( 9 * m_controlPoints[5] * pow(-1 + u, 2) * u * (-1 + v) * (-1 + 3 *v) )
				- Vector3D( 9 * m_controlPoints[9] * (-1 + u) * u * u * (-1 + v) * (-1 + 3 * v) )
				+ Vector3D( 3 * m_controlPoints[13] * u * u * u * (-1 + v) * (-1 + 3 * v) );
	}
	else
	{
		dpdv = Vector3D( 3 * controlPoint->at(0)  * pow(-1 + u, 3 ) * pow(-1 + v, 2 ) )
			- Vector3D( 9 * controlPoint->at(4)  * pow(-1 + u, 2 ) * u * pow(-1 + v, 2 ) )
			+ Vector3D( 9 * controlPoint->at(8)  * (-1 + u) * u * u * pow(-1 + v, 2 ) )
			- Vector3D( 3 * controlPoint->at(12)  * u * u * u * pow(-1 + v, 2 ) )
			+ Vector3D( 3 * controlPoint->at(14)  * u * u * u * (2 - 3 * v) * v )
			- Vector3D( 3 * controlPoint->at(3)  * pow(-1 + u, 3 ) * v * v )
			+ Vector3D( 9 * controlPoint->at(7)  * pow(-1 + u, 2 ) * u * v * v )
			- Vector3D( 9 * controlPoint->at(11)  * (-1 + u) * u * u * v * v )
			+ Vector3D( 3 * controlPoint->at(15)  * u * u * u * v * v )
			+ Vector3D( 3 * controlPoint->at(2)  * pow(-1 + u, 3 ) * v * (-2 + 3 * v) )
			- Vector3D( 9 * controlPoint->at(6)  * pow(-1 + u, 2 ) * u * v * (-2 + 3 * v) )
			+ Vector3D( 9 * controlPoint->at(10)  * (-1 + u) * u * u * v * (-2 + 3 * v) )
			- Vector3D( 3 * controlPoint->at(1)  * pow(-1 + u, 3 ) * (-1 + v) * (-1 + 3 * v) )
			+ Vector3D( 9 * controlPoint->at(5)  * pow(-1 + u, 2) * u * (-1 + v) * (-1 + 3 *v) )
			- Vector3D( 9 * controlPoint->at(9)  * (-1 + u) * u * u * (-1 + v) * (-1 + 3 * v) )
			+ Vector3D( 3 * controlPoint->at(13)  * u * u * u * (-1 + v) * (-1 + 3 * v) );
	}

	return ( dpdv );
}

Vector3D BezierPatch::D2PDUU( double u, double v, std::vector< Vector3D >* controlPoints ) const
{
	Vector3D d2Pduu = 6 * controlPoints->at( 0 ) * (-1 + u) * pow(-1 + v, 3)
		- 6 * controlPoints->at( 12 ) * u * pow(-1 + v, 3)
		- 6* controlPoints->at( 4 ) * (-2 + 3 * u ) * pow(-1 + v, 3)
		+ 6 * controlPoints->at( 8 ) * (-1 + 3 * u) * pow(-1 + v, 3 )
		+ 3 * controlPoints->at( 9 ) * (6 - 18 * u) * pow(-1 + v, 2 ) * v
		- 18 * controlPoints->at( 1 ) * (-1 + u) * pow(-1 + v, 2) * v
		+ 18 * controlPoints->at( 13 ) * u * pow(-1 + v, 2) * v
		+ 18 * controlPoints->at( 5 ) * (-2 + 3 * u) * pow(-1 + v, 2) * v
		+ 18 * controlPoints->at( 2 ) * (-1 + u) * (-1 + v) * v * v
		- 18 * controlPoints->at( 14 ) * u * (-1 + v) * v * v
		- 18 * controlPoints->at( 6 ) * (-2 + 3 * u) * (-1 + v) * v * v
		+ 18 * controlPoints->at( 10 ) * (-1 + 3 * u) * (-1 + v) * v * v
		+ controlPoints->at( 11 ) * (6 - 18 * u) * v * v * v
		- 6 * controlPoints->at( 3 ) * (-1 + u) * v * v * v
		+ 6 * controlPoints->at( 15 ) * u * v * v * v
		+ 6 * controlPoints->at( 7 ) *(-2 + 3 * u) * v * v * v;


	return ( d2Pduu );
}

Vector3D BezierPatch::D2PDUV( double u, double v, std::vector< Vector3D >* controlPoints ) const
{
	Vector3D d2Pduv =  9 * controlPoints->at( 0 ) * pow(-1 + u, 2) * pow(-1 + v, 2)
	-  9 * controlPoints->at( 12 ) * u * u * pow(-1 + v, 2 )
	+  9 * controlPoints->at( 8 ) * u * (-2 + 3 * u) * pow(-1 + v, 2)
	- Vector3D( 9 * controlPoints->at( 4 ) * (-1 + u) * (-1 + 3 * u) * pow(-1 + v, 2) )
	+ Vector3D( 9 * controlPoints->at( 14 ) * u * u * (2 - 3 * v) * v )
	- Vector3D( 9 * controlPoints->at( 3 ) * pow(-1 + u, 2) * v * v )
	+ Vector3D( 9 * controlPoints->at( 11 ) * (2 - 3 * u) * u * v * v )
	+ Vector3D( 9 * controlPoints->at( 15 ) * u * u * v * v )
	+ Vector3D( 9 * controlPoints->at( 7 ) * (-1 + u) * (-1 + 3 * u) * v * v )
	+ Vector3D( 9 * controlPoints->at( 2 ) * pow(-1 + u, 2) * v * (-2 + 3 * v) )
	+ Vector3D( 9 * controlPoints->at( 10 ) * u * (-2 + 3 * u) * v * (-2 + 3 * v) )
	- Vector3D( 9 * controlPoints->at( 6 ) * (-1 + u) * (-1 + 3 * u) * v * (-2 + 3 * v) )
	- Vector3D( 9 * controlPoints->at( 1 ) * pow(-1 + u, 2) * (-1 + v) * (-1 + 3 * v) )
	+ Vector3D( 9 * controlPoints->at( 13 ) * u * u * (-1 + v) * (-1 + 3 * v) )
	- Vector3D( 9 * controlPoints->at( 9 ) * u * (-2 + 3 * u) * (-1 + v) * (-1 + 3 * v) )
	+ Vector3D( 9 * controlPoints->at( 5 ) * (-1 + u) * (-1 + 3 * u) * (-1 + v) * (-1 + 3 * v) );

	return ( d2Pduv ) ;
}

Vector3D BezierPatch::D2PDVV( double u, double v, std::vector< Vector3D >* controlPoints ) const
{
	Vector3D d2Pdvv = Vector3D( 6 * controlPoints->at( 14 ) * u * u * u * (1 - 3 * v) )
	+ Vector3D( 6 * controlPoints->at( 0 ) * pow(-1 + u, 3) * (-1 + v) )
	- Vector3D( 18 * controlPoints->at( 4 ) * pow(-1 + u, 2) * u *(-1 + v) )
	+ Vector3D( 18 * controlPoints->at( 8 ) * (-1 + u) * u * u * (-1 + v) )
	- Vector3D( 6 * controlPoints->at( 12 ) * u * u * u * (-1 + v) )
	- Vector3D( 6 * controlPoints->at( 3 ) * pow(-1 + u, 3) * v )
	+ Vector3D( 18 * controlPoints->at( 7 ) * pow(-1 + u, 2) * u * v )
	- Vector3D( 18 * controlPoints->at( 11 ) * (-1 + u)* u * u * v )
	+ Vector3D( 6 * controlPoints->at( 15 ) * u * u * u * v )
	- Vector3D( 6 * controlPoints->at( 1 ) * pow(-1 + u, 3) * (-2 + 3 * v) )
	+ Vector3D( 18 * controlPoints->at( 5 ) * pow(-1 + u, 2) * u * (-2 + 3 * v) )
	- Vector3D( 18 * controlPoints->at( 9 ) * (-1 + u) * u * u * (-2 + 3 * v) )
	+ Vector3D( 6 * controlPoints->at( 13 ) * u * u * u * (-2 + 3 * v) )
	+ Vector3D( 6 * controlPoints->at( 2 ) * pow(-1 + u, 3) * (-1 + 3 * v) )
	- Vector3D( 18 * controlPoints->at( 6 ) * pow(-1 + u, 2) * u * (-1 + 3 * v) )
	+ Vector3D( 18 * controlPoints->at( 10 ) * (-1 + u) * u *u * (-1 + 3 * v) );


	return Vector3D( d2Pdvv );
}

void BezierPatch::HullSplitU( std::vector< Vector3D > p, std::vector< Vector3D >* q, std::vector< Vector3D >* r ) const
{

	//Casteljau algorithm
	for( int iv = 0; iv < 4; iv++ )
	{
		Vector3D p0 = p[iv] ;
		q->at( iv ) = p0;

		Vector3D p1 = p[4 + iv ] ;
		Vector3D q1 = ( ( p0 + p1 ) / 2 );
		q->at( 4 + iv ) = q1;

		Vector3D p2 = p[8 + iv] ;
		Vector3D q2 = ( q1  / 2 ) + ( ( p1 + p2 ) / 4 );
		q->at( 8 + iv  ) = q2;


		Vector3D p3 = p[12 + iv ] ;
		r->at( 12 + iv ) = p3;

		Vector3D r2 = ( p2 + p3 ) / 2;
		r->at( 8 + iv ) = r2;

		Vector3D r1 = ( r2 / 2 ) + ( ( p1 + p2 ) / 4 );
		r->at( 4 + iv ) = r1;

		Vector3D q3 = ( q2 + r1 ) / 2 ;
		q->at( 12 + iv ) = q3;
		r->at( iv ) = q3;
	}

}

void BezierPatch::HullSplitV( std::vector< Vector3D >p, std::vector< Vector3D >* q, std::vector< Vector3D >* r ) const
{
	for( int iv = 0; iv < 4; iv++ )
	{
		Vector3D p0 = p[4 * iv] ;
		q->at( 4 * iv ) = p0;

		Vector3D p1 = p[4 * iv + 1] ;
		Vector3D q1 = ( ( p0 + p1 ) / 2 );
		q->at( 4 * iv + 1 ) =  q1;

		Vector3D p2 = p[4 * iv + 2] ;
		Vector3D q2 = ( q1  / 2 ) + ( ( p1 + p2 ) / 4 );
		q->at( 4 * iv + 2 ) = q2;

		Vector3D p3 = p[4 * iv + 3] ;
		r->at( 4 * iv + 3 ) = p3;

		Vector3D r2 = ( p2 + p3 ) / 2;
		r->at( 4 * iv + 2 ) = r2;

		Vector3D r1 = ( r2 / 2 ) + ( ( p1 + p2 ) / 4 );
		r->at( 4 * iv + 1 ) =  r1;

		Vector3D q3 = ( q2 + r1 ) / 2 ;
		q->at( 4 * iv + 3 ) =  q3;

		r->at( 4 * iv ) = q3;
	}
}

void BezierPatch::SplitIPatch( std::vector< Vector3D > p, std::vector< std::vector< Vector3D > >* iPatches ) const
{
	std::vector< Vector3D > q( 16, 0);
	std::vector< Vector3D > r( 16, 0);
	std::vector< Vector3D > q1( 16, 0);
	std::vector< Vector3D > r1( 16, 0);
	std::vector< Vector3D > q2( 16, 0);
	std::vector< Vector3D > r2( 16, 0);
	//, std::vector< Point3D > controlPoint
	HullSplitU( p, &q, &r );
	HullSplitV( q, &q1, &q2 );
	HullSplitV( r, &r1, &r2 );

	iPatches->push_back( q1 );
	iPatches->push_back( q2 );
	iPatches->push_back( r1 );
	iPatches->push_back( r2 );

}
