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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <algorithm>
#include <iostream>

#include <QMap>
#include <QVector>

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/fields/SoMFVec3d.h>

#include "BBox.h"
#include "BezierPatch.h"
#include "DifferentialGeometry.h"
#include "NormalVector.h"
#include "Ray.h"
#include "Vector3D.h"

SO_NODE_SOURCE(BezierPatch);

void BezierPatch::initClass()
{
	SO_NODE_INIT_CLASS(BezierPatch, SoNurbsSurface, "NurbsSurface");
}


BezierPatch::BezierPatch()
{
	SO_NODE_CONSTRUCTOR(BezierPatch);



	numUControlPoints = 4;
	numVControlPoints = 4 ;

	float uKnots[ 8 ] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
	float vKnots[ 8 ] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
	uKnotVector.setValues(0, 8, uKnots);
	vKnotVector.setValues(0, 8, vKnots);

}

BezierPatch::~BezierPatch()
{

}

BBox BezierPatch::GetComputeBBox()
{
	BBox box;
	int nControlPoints = m_controlPoints.getNum();
	for (int i = 0; i < nControlPoints; i++)
		box = Union( box , Point3D( m_controlPoints[i][0], m_controlPoints[i][1],m_controlPoints[i][2] )  );
	return box;
}


SoMFVec3f BezierPatch::GetControlPoints(  )
{
	return m_controlPoints;
}


void BezierPatch::SetControlPoints( QVector< Point3D > boundedPoints )
{
	//derivates: qu00, qv00, quv00, qu01, qv01, quv01, qu10, qv10, quv10, qu11, qv11,quv11
	QVector< Vector3D > derivates = CornerDerivates( boundedPoints );

	//m_controlPointsList << boundedPoints[0]; //p00
	Point3D p00  = boundedPoints[0];
	Point3D p01 = boundedPoints[0] + derivates[1]/ 3; //p01 = derivates[1]/3 + p00;
	Point3D p02 = boundedPoints[9] - derivates[4]/ 3; //	p02 = p03 - Qv01[curves]/3;
	Point3D p03 = boundedPoints[9];	//p03 = curves[[10]];

	Point3D p10  = boundedPoints[0] + derivates[0] / 3; //p10 = p00 + Qu00[curves]/3;
	Point3D p11  = boundedPoints[0] + derivates[2]/ 9 + ( derivates[0] + derivates[1] ) / 3; //p11 = Quv00[curves]/9 + (Qu00[curves] + Qv00[curves])/3 + p00;
	Point3D p12  = boundedPoints[9] + ( derivates[3] - derivates[4] ) / 3 - derivates[5]/ 9; //p12 = p03 + (Qu01[curves] - Qv01[curves])/3 - Quv01[curves]/9;
	Point3D p13  =  boundedPoints[9] + derivates[3] / 3; //p13 = Qu01[curves]/3 + p03;

	Point3D p20  = boundedPoints[3] - derivates[6] / 3; //p20 = p30 - Qu10[curves]/3;
	Point3D p21  = boundedPoints[3] + ( derivates[7] - derivates[6] ) / 3 - derivates[8] / 9; //p21 = p30 + (Qv10[curves] - Qu10[curves])/3 - Quv10[curves]/9;
	Point3D p22  = boundedPoints[6] + derivates[11] / 9 - ( derivates[9] + derivates[10] ) / 3; //p22 = Quv11[curves]/9 - (Qu11[curves] + Qv11[curves])/3 + p33;
	Point3D p23  = boundedPoints[6] - derivates[9] / 3; //p23 = p33 - Qu11[curves]/3;

	Point3D p30  = boundedPoints[3]; //p30
	Point3D p31  = boundedPoints[3] + derivates[7]/ 3; //p31 = Qv10[curves]/3 + p30;
	Point3D p32  = boundedPoints[6] - derivates[10]/ 3; //p32 = p33 - Qv11[curves]/3;
	Point3D p33  = boundedPoints[6]; //p33

	m_controlPoints.set1Value( 0, SbVec3f( p00.x, p00.y, p00.z ) );
	m_controlPoints.set1Value( 1, SbVec3f( p01.x, p01.y, p01.z ) );
	m_controlPoints.set1Value( 2, SbVec3f( p02.x, p02.y, p02.z ) );
	m_controlPoints.set1Value( 3, SbVec3f( p03.x, p03.y, p03.z ) );

	m_controlPoints.set1Value( 4, SbVec3f( p10.x, p10.y, p10.z ) );
	m_controlPoints.set1Value( 5, SbVec3f( p11.x, p11.y, p11.z ) );
	m_controlPoints.set1Value( 6, SbVec3f( p12.x, p12.y, p12.z ) );
	m_controlPoints.set1Value( 7, SbVec3f( p13.x, p13.y, p13.z ) );

	m_controlPoints.set1Value( 8, SbVec3f( p20.x, p20.y, p20.z ) );
	m_controlPoints.set1Value( 9, SbVec3f( p21.x, p21.y, p21.z ) );
	m_controlPoints.set1Value( 10, SbVec3f( p22.x, p22.y, p22.z ) );
	m_controlPoints.set1Value( 11, SbVec3f( p23.x, p23.y, p23.z ) );

	m_controlPoints.set1Value( 12, SbVec3f( p30.x, p30.y, p30.z ) );
	m_controlPoints.set1Value( 13, SbVec3f( p31.x, p31.y, p31.z ) );
	m_controlPoints.set1Value( 14, SbVec3f( p32.x, p32.y, p32.z ) );
	m_controlPoints.set1Value( 15, SbVec3f( p33.x, p33.y, p33.z ) );


	/*std::cout<<"---m_controlPoints---"<<std::endl;
	for( int i = 0; i < m_controlPoints.size() ; i++ )
		std::cout<<"m_controlPoints: "<<m_controlPoints[i]<<std::endl;*/
}

void BezierPatch::GeneratePrimitives( SoAction* action )
{
	generatePrimitives( action );
}

void BezierPatch::GLRender (SoGLRenderAction* action)
{
	SoCoordinateElement::set3(action->getState(), this, m_controlPoints.getNum(), m_controlPoints.getValues( 0 ) );
	SoNurbsSurface::GLRender( action );
}

bool BezierPatch::Intersect(const Ray& objectRay, double* tHit, DifferentialGeometry* dg) const
{
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
	Vector3D nu = CrossProduct( t, objectRay.direction() );
	Vector3D nv = CrossProduct( nu, objectRay.direction() );


	double au = nu[0];
	double av = nv[0];
	double bu = nu[1];
	double bv = nv[1];
	double cu = nu[2];
	double cv = nv[2];
	double du = DotProduct( -nu, Vector3D( objectRay.origin ) );
	double dv = DotProduct( -nv, Vector3D( objectRay.origin ) );

	SoMFVec3d p;
	p.set1Value( 0, SbVec3d( m_controlPoints[0] ) );
	p.set1Value( 1, SbVec3d( m_controlPoints[1] ) );
	p.set1Value( 2, SbVec3d( m_controlPoints[2] ) );
	p.set1Value( 3, SbVec3d( m_controlPoints[3] ) );

	p.set1Value( 4, SbVec3d( m_controlPoints[4] ) );
	p.set1Value( 5, SbVec3d( m_controlPoints[5] ) );
	p.set1Value( 6, SbVec3d( m_controlPoints[6] ) );
	p.set1Value( 7, SbVec3d( m_controlPoints[7] ) );

	p.set1Value( 8, SbVec3d( m_controlPoints[8] ) );
	p.set1Value( 9, SbVec3d( m_controlPoints[9] ) );
	p.set1Value( 10, SbVec3d( m_controlPoints[10] ) );
	p.set1Value( 11, SbVec3d( m_controlPoints[11] ) );

	p.set1Value( 12, SbVec3d( m_controlPoints[12] ) );
	p.set1Value( 13, SbVec3d( m_controlPoints[13] ) );
	p.set1Value( 14, SbVec3d( m_controlPoints[14] ) );
	p.set1Value( 15, SbVec3d( m_controlPoints[15] ) );

	SoMFVec3d q;
	SoMFVec3d r;
	SoMFVec3d* q1 = new SoMFVec3d;
	SoMFVec3d* r1 = new SoMFVec3d;
	SoMFVec3d* q2 = new SoMFVec3d;
	SoMFVec3d* r2 = new SoMFVec3d;

	HullSplitU( p, &q, &r );
	HullSplitV( q, q1, q2 );
	HullSplitV( r, r1, r2 );


	QVector< SoMFVec3d* > buffer1;
	buffer1.push_back( q1 );
	buffer1.push_back( q2 );
	buffer1.push_back( r1 );
	buffer1.push_back( r2 );


	int nIterations = 100;
	double previousDistance = 0;
	double currentDistance = tgc::Infinity;

	double tol = 0.000000001;
	int iteration = 0;
	while( ( iteration < nIterations ) && ( fabs( previousDistance - currentDistance ) > tol ) )
	{

		QVector< SoMFVec3d* > buffer2;
		for( int patch = 0; patch < buffer1.size(); patch++ )
		{
			SoMFVec3d* ip = buffer1[patch];
			if( !ip )	return false;
			SoMFVec3d iq;
			SoMFVec3d ir;
			SoMFVec3d* iq1 = new SoMFVec3d;
			SoMFVec3d* ir1 = new SoMFVec3d;
			SoMFVec3d* iq2 = new SoMFVec3d;
			SoMFVec3d* ir2 = new SoMFVec3d;

			HullSplitU( *ip, &iq, &ir );
			HullSplitV( iq, iq1, iq2 );
			HullSplitV( ir, ir1, ir2 );


			buffer2.push_back( iq1 );
			buffer2.push_back( iq2 );
			buffer2.push_back( ir1 );
			buffer2.push_back( ir2 );

		}

		//Hull clasification
		QMap< double, SoMFVec3d* > distancesMap;

		for( int patch = 0; patch< buffer2.size(); patch++ )
		{

			SoMFVec3d* iPatch = buffer2.value( patch );

			if( !iPatch )	return false;
			double pu1, pu2, pv1, pv2;

			SbVec3d p00 = iPatch->operator[]( 0 );
			pu2 = au * p00[0] + bu * p00[1] + cu * p00[2] + du;
			pv2 = av * p00[0] + bv * p00[1] + cv * p00[2]+ dv;

			double intersectsU = false;
			double intersectsV = false;
			for( int i = 0; i < 16; i++ )
			{
				SbVec3d iPoint = iPatch->operator[]( i );
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
				SbVec3d vCenter = ( 1 / 64.0 ) * ( iPatch->operator[]( 0 ) +  3 * iPatch->operator[]( 4 )
								+ 3 * iPatch->operator[]( 8 ) + iPatch->operator[]( 12 )
								+ 3 * iPatch->operator[]( 1 ) +  9 * iPatch->operator[]( 5 )
								+ 9 * iPatch->operator[]( 9 ) +  3 * iPatch->operator[]( 13 )
								+ 3 * iPatch->operator[]( 2 ) +  9 * iPatch->operator[]( 6 )
								+ 9 * iPatch->operator[]( 10 ) +  3 * iPatch->operator[]( 14 )
								+ iPatch->operator[]( 3 ) +  3 * iPatch->operator[]( 7 )
								+ 3 * iPatch->operator[]( 11 ) + iPatch->operator[]( 15 ) );
				Point3D center( vCenter[0], vCenter[1], vCenter[2] );
				if( DotProduct( Vector3D( center - objectRay.origin ), objectRay.direction() ) >= 0 )
				{
					SoMFVec3d* intersectedPatch = new SoMFVec3d;
					intersectedPatch->copyFrom( *iPatch );
					distancesMap.insert( Distance( center, objectRay.origin ),intersectedPatch );
				}
			}

		}
		for( int index = 0; index < buffer2.size(); index++ )
		{
			delete buffer2[index];
		}

		std::vector< double > distances = QVector< double >::fromList( distancesMap.keys() ).toStdVector();
		std::sort( distances.begin(), distances.end() );

		for (int i = 0; i < buffer1.size(); ++i)
		{
			delete 	buffer1[i];
		}
		buffer1.clear();

		unsigned int patches = 4;
		if( patches > distances.size() )	patches = distances.size();
		for (int i = 0; i < patches; ++i)
		{
			if( distances[i] != tgc::Infinity ) buffer1.push_back( distancesMap.value( distances[i] ) );

		}
		for (int i = patches; i < distances.size(); ++i)
		{
			delete distancesMap.value( distances[i] );
		}
		if( buffer1.size() == 0 )	return false;

		previousDistance = currentDistance;
		currentDistance = distances[0];
		iteration++;
	}

	SoMFVec3d* iPatch = buffer1.value( 0 );
	if( !iPatch )
	{
		for (int i = 0; i < buffer1.size(); ++i)
		{
			delete 	buffer1[i];
		}
		return false;
	}

	// Now check if the fucntion is being called from IntersectP,
		// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) )
	{
		for (int i = 0; i < buffer1.size(); ++i)
		{
			delete 	buffer1[i];
		}
		return true;
	}

	SbVec3d vCenter = ( 1 / 64.0 ) * ( iPatch->operator[]( 0 ) +  3 * iPatch->operator[]( 4 )
									+ 3 * iPatch->operator[]( 8 ) + iPatch->operator[]( 12 )
									+ 3 * iPatch->operator[]( 1 ) +  9 * iPatch->operator[]( 5 )
									+ 9 * iPatch->operator[]( 9 ) +  3 * iPatch->operator[]( 13 )
									+ 3 * iPatch->operator[]( 2 ) +  9 * iPatch->operator[]( 6 )
									+ 9 * iPatch->operator[]( 10 ) +  3 * iPatch->operator[]( 14 )
									+ iPatch->operator[]( 3 ) +  3 * iPatch->operator[]( 7 )
									+ 3 * iPatch->operator[]( 11 ) + iPatch->operator[]( 15 ) );

	Point3D intersectionPoint( vCenter[0], vCenter[1], vCenter[2] );


	double thit;
	if( objectRay.direction().x != 0 ) thit = ( intersectionPoint.x - objectRay.origin.x ) / objectRay.direction().x;
	else if ( objectRay.direction().y != 0 ) thit = ( intersectionPoint.y - objectRay.origin.y ) / objectRay.direction().y;
	else	thit = ( intersectionPoint.z - objectRay.origin.z ) / objectRay.direction().z;

	if( thit < 0.001 ) return false;
	if( ( thit > objectRay.maxt ) || ( (thit - objectRay.mint) < tol ) )
	{
		for (int i = 0; i < buffer1.size(); ++i)
		{
			delete 	buffer1[i];
		}
		return false;
	}

	Vector3D dpdu = DPDU( 0.5, 0.5, *iPatch);
	Vector3D dpdv = DPDV( 0.5, 0.5, *iPatch);

	// Compute cylinder \dndu and \dndv
	Vector3D d2Pduu = D2PDUU( 0.5, 0.5, *iPatch );
	Vector3D d2Pduv= D2PDUV( 0.5, 0.5, *iPatch );
	Vector3D d2Pdvv= D2PDVV( 0.5, 0.5, *iPatch );

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
	*dg = DifferentialGeometry( intersectionPoint,
								dpdu,
								dpdv,
								dndu,
								dndv,
								0.5, 0.5, 0 );
	*tHit = thit;


	for (int i = 0; i < buffer1.size(); ++i)	delete 	buffer1[i];

	return true;

}

bool BezierPatch::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}


Point3D BezierPatch::GetPoint3D (double u, double v) const
{
	SbVec3f point = m_controlPoints[0] * (-1 + u) * (-1 + u) * (-1 + u) * (-1 + v) * (-1 + v) * (-1 + v)
		- 3 * m_controlPoints[4] * (-1 + u)* (-1 + u) * u * (-1 + v) * (-1 + v) * (-1 + v)
		+ 3 * m_controlPoints[8] * (-1 + u) *  u * u * (-1 + v) * (-1 + v) * (-1 + v)
		 - m_controlPoints[12] * u * u * u * (-1 + v) * (-1 + v) * (-1 + v)
		 - 3 * m_controlPoints[1] * (-1 + u) * (-1 + u) * (-1 + u) * (-1 + v)* (-1 + v) * v
		 + 9 * m_controlPoints[5] * (-1 + u) * (-1 + u) * u * (-1 + v)* (-1 + v) * v
		 - 9 * m_controlPoints[9] * (-1 + u) * u * u * (-1 + v)* (-1 + v) * v
		 + 3 * m_controlPoints[13] * u * u * u * (-1 + v) * (-1 + v) * v
		 + 3 * m_controlPoints[2] * (-1 + u)  * (-1 + u)  * (-1 + u)* (-1 + v) * v * v
		 - 9 * m_controlPoints[6] * (-1 + u)  * (-1 + u) * u * (-1 + v) * v * v
		 + 9 * m_controlPoints[10] * (-1 + u) * u * u * (-1 + v) * v * v
		 - 3 * m_controlPoints[14] * u * u * u * (-1 + v) * v * v
		 - m_controlPoints[3] * (-1 + u) * (-1 + u) * (-1 + u) * v * v * v
		 + 3 * m_controlPoints[7] * (-1 + u) * (-1 + u)* u * v * v * v
		 - 3 * m_controlPoints[11] * (-1 + u) * u * u * v * v * v
		 + m_controlPoints[15] * u * u * u * v * v * v;

	return Point3D( point[0], point[1], point[2] );
}

NormalVector BezierPatch::GetNormal( double u, double v ) const
{
	SoMFVec3d p;
	p.set1Value( 0, SbVec3d( m_controlPoints[0] ) );
	p.set1Value( 1, SbVec3d( m_controlPoints[1] ) );
	p.set1Value( 2, SbVec3d( m_controlPoints[2] ) );
	p.set1Value( 3, SbVec3d( m_controlPoints[3] ) );

	p.set1Value( 4, SbVec3d( m_controlPoints[4] ) );
	p.set1Value( 5, SbVec3d( m_controlPoints[5] ) );
	p.set1Value( 6, SbVec3d( m_controlPoints[6] ) );
	p.set1Value( 7, SbVec3d( m_controlPoints[7] ) );

	p.set1Value( 8, SbVec3d( m_controlPoints[8] ) );
	p.set1Value( 9, SbVec3d( m_controlPoints[9] ) );
	p.set1Value( 10, SbVec3d( m_controlPoints[10] ) );
	p.set1Value( 11, SbVec3d( m_controlPoints[11] ) );

	p.set1Value( 12, SbVec3d( m_controlPoints[12] ) );
	p.set1Value( 13, SbVec3d( m_controlPoints[13] ) );
	p.set1Value( 14, SbVec3d( m_controlPoints[14] ) );
	p.set1Value( 15, SbVec3d( m_controlPoints[15] ) );

	Vector3D dpdu = DPDU( u, v, p );
	Vector3D dpdv = DPDV( u, v, p );

	return NormalVector( Normalize( CrossProduct( dpdu, dpdv ) ) );
}

void BezierPatch::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
	box.makeEmpty();
	int nControlPoints = m_controlPoints.getNum();
	for (int i = 0; i < nControlPoints; i++)
	{
	  box.extendBy( SbVec3f(m_controlPoints[i] ) );
	}
}

void BezierPatch::generatePrimitives( SoAction *action )
{
	SoCoordinateElement::set3(action->getState(), this, m_controlPoints.getNum(), m_controlPoints.getValues( 0 ) );
	SoNurbsSurface::generatePrimitives( action );
}


QVector< Vector3D > BezierPatch::CornerDerivates( QVector< Point3D > boundedPoints )
{
	QVector< Vector3D > cornerDerivates;

	//qu00
	Vector3D qu00 = 3 * (  boundedPoints[1]- boundedPoints[0]);
	cornerDerivates << qu00;

	//qv00
	Vector3D qv00 =  3 * ( boundedPoints[11] - boundedPoints[0]);
	cornerDerivates << qv00;

	//quv00
	cornerDerivates << 0.0;

	//qu01
	Vector3D qu01 = 3 * ( boundedPoints[8]- boundedPoints[9]);
	cornerDerivates << qu01;

	//qv01
	Vector3D qv01 =  3 * ( boundedPoints[9]-  boundedPoints[10]);
	cornerDerivates << qv01;

	//quv01
	cornerDerivates <<  0.0;

	//qu10
	Vector3D qu10 = 3 * ( boundedPoints[3] - boundedPoints[2]);
	cornerDerivates << qu10;

	//qv10
	Vector3D qv10 =  3 * ( boundedPoints[4] - boundedPoints[3] );
	cornerDerivates << qv10;

	//quv10
	cornerDerivates <<  0.0/* CrossProduct( qs10, qt10 )*/;

	//qu11
	Vector3D qu11 = 3 * ( boundedPoints[6] - boundedPoints[7]);
	cornerDerivates << qu11;

	//qt11
	Vector3D qv11 =  3 * ( boundedPoints[6] - boundedPoints[5] );
	cornerDerivates << qv11;

	//quv11
	cornerDerivates <<  0.0/* CrossProduct( qs11, qt11 )*/;

	return cornerDerivates;
}

Vector3D BezierPatch::DPDU( double u, double v, SoMFVec3d& controlPoints ) const
{
	SbVec3d dpdu = 3 * controlPoints[0] * pow(-1 + u, 2 ) * pow(-1 + v, 3 )
		- 3 * controlPoints[12] *  pow( u, 2 ) * pow(-1 + v, 3 )
		+ 3 * controlPoints[8] * u * (-2 + 3 * u ) * pow(-1 + v, 3 )
		- 3 * controlPoints[4] * (-1 + u) * (-1 + 3 * u) * pow(-1 + v, 3 )
		- 9 * controlPoints[1] * pow(-1 + u, 2 ) * pow(-1 + v, 2) * v
		+ 9 * controlPoints[9]* (2 - 3 * u ) *  u * pow(-1 + v, 2) * v
		+ 9 * controlPoints[13] * pow( u, 2 ) * pow(-1 + v, 2 ) * v
		+ 9 * controlPoints[5]* (-1 + u) * (-1 + 3 * u) * pow(-1 + v, 2) * v
		+ 9 * controlPoints[2] * pow(-1 + u, 2 ) * (-1 + v) * pow( v, 2 )
		- 9 * controlPoints[14] * u * u * (-1 + v) * v * v
		+ 9 * controlPoints[10] * u * (-2 + 3 * u) * (-1 + v) * v * v
		- 9 * controlPoints[6] * (-1 + u) * (-1 + 3 * u) * (-1 + v) * v * v
		- 3 * controlPoints[3] * pow(-1 + u, 2) * v * v * v
		+ 3 * controlPoints[11] * (2 - 3 * u) * u * v * v* v
		+ 3 * controlPoints[15]* u * u * v * v * v
		+ 3 * controlPoints[7] * (-1 + u) * (-1 + 3 * u) * v * v * v;

	return Vector3D( dpdu[0], dpdu[1], dpdu[2] );
}

Vector3D BezierPatch::DPDV( double u, double v, SoMFVec3d& controlPoints ) const
{
	SbVec3d dpdv = 3 * controlPoints[0]  * pow(-1 + u, 3 ) * pow(-1 + v, 2 )
	- 9 * controlPoints[4]  * pow(-1 + u, 2 ) * u * pow(-1 + v, 2 )
	+ 9 * controlPoints[8]  * (-1 + u) * u * u * pow(-1 + v, 2 )
	- 3 * controlPoints[12]  * u * u * u * pow(-1 + v, 2 )
	+ 3 * controlPoints[14]  * u * u * u * (2 - 3 * v) * v
	- 3 * controlPoints[3]  * pow(-1 + u, 3 ) * v * v
	+ 9 * controlPoints[7]  * pow(-1 + u, 2 ) * u * v * v
	- 9 * controlPoints[11]  * (-1 + u) * u * u * v * v
	+ 3 * controlPoints[15]  * u * u * u * v * v
	+ 3 * controlPoints[2]  * pow(-1 + u, 3 ) * v * (-2 + 3 * v)
	- 9 * controlPoints[6]  * pow(-1 + u, 2 ) * u * v * (-2 + 3 * v)
	+ 9 * controlPoints[10]  * (-1 + u) * u * u * v * (-2 + 3 * v)
	- 3 * controlPoints[1]  * pow(-1 + u, 3 ) * (-1 + v) * (-1 + 3 * v)
	+ 9 * controlPoints[5]  * pow(-1 + u, 2) * u * (-1 + v) * (-1 + 3 *v)
	- 9 * controlPoints[9]  * (-1 + u) * u * u * (-1 + v) * (-1 + 3 * v)
	+ 3 * controlPoints[13]  * u * u * u * (-1 + v) * (-1 + 3 * v);

	return Vector3D( dpdv[0], dpdv[1], dpdv[2] );
}

Vector3D BezierPatch::D2PDUU( double u, double v, SoMFVec3d& controlPoints ) const
{
	SbVec3d d2Pduu = 6 * controlPoints[0] * (-1 + u) * pow(-1 + v, 3)
	- 6 * controlPoints[12] * u * pow(-1 + v, 3)
	- 6* controlPoints[4] * (-2 + 3 * u ) * pow(-1 + v, 3)
	+ 6 * controlPoints[8] * (-1 + 3 * u) * pow(-1 + v, 3 )
	+ 3 * controlPoints[9] * (6 - 18 * u) * pow(-1 + v, 2 ) * v
	- 18 * controlPoints[1] * (-1 + u) * pow(-1 + v, 2) * v
	+ 18 * controlPoints[13] * u * pow(-1 + v, 2) * v
	+ 18 * controlPoints[5] * (-2 + 3 * u) * pow(-1 + v, 2) * v
	+ 18 * controlPoints[2] * (-1 + u) * (-1 + v) * v * v
	- 18 * controlPoints[14] * u * (-1 + v) * v * v
	- 18 * controlPoints[6] * (-2 + 3 * u) * (-1 + v) * v * v
	+ 18 * controlPoints[10] * (-1 + 3 * u) * (-1 + v) * v * v
	+ controlPoints[11] * (6 - 18 * u) * v * v * v
	- 6 * controlPoints[3] * (-1 + u) * v * v * v
	+ 6 * controlPoints[15] * u * v * v * v
	+ 6 * controlPoints[7] *(-2 + 3 * u) * v * v * v;


	return Vector3D( d2Pduu[0], d2Pduu[1], d2Pduu[2] );
}

Vector3D BezierPatch::D2PDUV( double u, double v, SoMFVec3d& controlPoints ) const
{
	SbVec3d d2Pduv = 9 * controlPoints[0] * pow(-1 + u, 2) * pow(-1 + v, 2)
	- 9 * controlPoints[12] * u * u * pow(-1 + v, 2 )
	+ 9 * controlPoints[8] * u * (-2 + 3 * u) * pow(-1 + v, 2)
	- 9 * controlPoints[4] * (-1 + u) * (-1 + 3 * u) * pow(-1 + v, 2)
	+ 9 * controlPoints[14] * u * u * (2 - 3 * v) * v
	- 9 * controlPoints[3] * pow(-1 + u, 2) * v * v
	+ 9 * controlPoints[11] * (2 - 3 * u) * u * v * v
	+ 9 * controlPoints[15] * u * u * v * v
	+ 9 * controlPoints[7] * (-1 + u) * (-1 + 3 * u) * v * v
	+ 9 * controlPoints[2] * pow(-1 + u, 2) * v * (-2 + 3 * v)
	+ 9 * controlPoints[10] * u * (-2 + 3 * u) * v * (-2 + 3 * v)
	- 9 * controlPoints[6] * (-1 + u) * (-1 + 3 * u) * v * (-2 + 3 * v)
	- 9 * controlPoints[1] * pow(-1 + u, 2) * (-1 + v) * (-1 + 3 * v)
	+ 9 * controlPoints[13] * u * u * (-1 + v) * (-1 + 3 * v)
	- 9 * controlPoints[9] * u * (-2 + 3 * u) * (-1 + v) * (-1 + 3 * v)
	+ 9 * controlPoints[5] * (-1 + u) * (-1 + 3 * u) * (-1 + v) * (-1 + 3 * v);

	return Vector3D( d2Pduv[0], d2Pduv[1], d2Pduv[2] );
}

Vector3D BezierPatch::D2PDVV( double u, double v, SoMFVec3d& controlPoints ) const
{
	SbVec3d d2Pdvv = 6 * controlPoints[14] * u * u * u * (1 - 3 * v)
	+ 6 * controlPoints[0] * pow(-1 + u, 3) * (-1 + v)
	- 18 * controlPoints[4] * pow(-1 + u, 2) * u *(-1 + v)
	+ 18 * controlPoints[8] * (-1 + u) * u * u * (-1 + v)
	- 6 * controlPoints[12] * u * u * u * (-1 + v)
	- 6 * controlPoints[3] * pow(-1 + u, 3) * v
	+ 18 * controlPoints[7] * pow(-1 + u, 2) * u * v
	- 18 * controlPoints[11] * (-1 + u)* u * u * v
	+ 6 * controlPoints[15] * u * u * u * v
	- 6 * controlPoints[1] * pow(-1 + u, 3) * (-2 + 3 * v)
	+ 18 * controlPoints[5] * pow(-1 + u, 2) * u * (-2 + 3 * v)
	- 18 * controlPoints[9] * (-1 + u) * u * u * (-2 + 3 * v)
	+ 6 * controlPoints[13] * u * u * u * (-2 + 3 * v)
	+ 6 * controlPoints[2] * pow(-1 + u, 3) * (-1 + 3 * v)
	- 18 * controlPoints[6] * pow(-1 + u, 2) * u * (-1 + 3 * v)
	+ 18 * controlPoints[10] * (-1 + u) * u *u * (-1 + 3 * v);


	return Vector3D( d2Pdvv[0], d2Pdvv[1], d2Pdvv[2] );
}

void BezierPatch::HullSplitU( const SoMFVec3d& p, SoMFVec3d* q, SoMFVec3d* r ) const
{
	for( int iv = 0; iv < 4; iv++ )
	{
		const SbVec3d* values = p.getValues( 0 );
		SbVec3d p0 = values[iv] ;
		q->set1Value( iv, p0 );

		SbVec3d p1 = values[4 + iv ] ;
		SbVec3d q1 = ( ( p0 + p1 ) / 2 );
		q->set1Value( 4 + iv, q1 );

		SbVec3d p2 = values[8 + iv] ;
		SbVec3d q2 = ( q1  / 2 ) + ( ( p1 + p2 ) / 4 );
		q->set1Value( 8 + iv, q2 );


		SbVec3d p3 = values[12 + iv ] ;
		r->set1Value( 12 + iv , p3 );

		SbVec3d r2 = ( p2 + p3 ) / 2;
		r->set1Value( 8 + iv , r2 );

		SbVec3d r1 = ( r2 / 2 ) + ( ( p1 + p2 ) / 4 );
		r->set1Value( 4 + iv , r1 );

		SbVec3d q3 = ( q2 + r1 ) / 2 ;
		q->set1Value( 12 + iv, q3 );
		r->set1Value( iv ,q3 );
	}

}

void BezierPatch::HullSplitV( const SoMFVec3d& p, SoMFVec3d* q, SoMFVec3d* r ) const
{
	for( int iv = 0; iv < 4; iv++ )
	{
		const SbVec3d* values = p.getValues( 0 );
		SbVec3d p0 = values[4 * iv] ;
		q->set1Value( 4 * iv, p0 );

		SbVec3d p1 = values[4 * iv + 1] ;
		SbVec3d q1 = ( ( p0 + p1 ) / 2 );
		q->set1Value( 4 * iv + 1, q1 );

		SbVec3d p2 = values[4 * iv + 2] ;
		SbVec3d q2 = ( q1  / 2 ) + ( ( p1 + p2 ) / 4 );
		q->set1Value( 4 * iv + 2, q2 );

		SbVec3d p3 = values[4 * iv + 3] ;
		r->set1Value( 4 * iv + 3, p3 );

		SbVec3d r2 = ( p2 + p3 ) / 2;
		r->set1Value( 4 * iv + 2, r2 );

		SbVec3d r1 = ( r2 / 2 ) + ( ( p1 + p2 ) / 4 );
		r->set1Value( 4 * iv + 1 , r1 );

		SbVec3d q3 = ( q2 + r1 ) / 2 ;
		q->set1Value( 4 * iv + 3, q3 );

		r->set1Value( 4 * iv ,q3 );
	}
}
