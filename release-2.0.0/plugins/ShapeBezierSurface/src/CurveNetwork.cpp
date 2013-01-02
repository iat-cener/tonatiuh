/*
 * CurveNetwork.cpp
 *
 *  Created on: 26-feb-2009
 *      Author: amutuberria
 */

#include <QPair>
#include <QVarLengthArray>


#include "BezierPatch.h"
#include "CurveNetwork.h"
#include "Vector3D.h"

CurveNetwork::CurveNetwork( int order )
: m_order( order )
{

}

CurveNetwork::~CurveNetwork()
{

}

void CurveNetwork::AddUCurve( Curve* curve )
{
	m_uCurveList.push_back( curve );
}

void CurveNetwork::AddVCurve( Curve* curve )
{
	m_vCurveList.push_back( curve );
}

QVector< BezierPatch* > CurveNetwork::GetSurface()
{

	int uPatches = m_vCurveList.size() - 1;
	int vPatches = m_uCurveList.size() - 1;

	QVector< Point3D > uPoints;
	QVector< Point3D > vPoints;

	for( int u = 0; u < m_uCurveList.size(); u++ )
	{
		QPair< QVector<Point3D>, QVector< double> > uBezier = KnotInsertion( m_uCurveList[u]->GetKnots(), m_uCurveList[u]->GetControlPoints() );
		QVector<Point3D> controlPoints = uBezier.first;
		uPoints<<uBezier.first;
	}

	for( int v = 0; v < m_vCurveList.size(); v++ )
	{
		QPair< QVector<Point3D>, QVector< double> > vBezier = KnotInsertion( m_vCurveList[v]->GetKnots(), m_vCurveList[v]->GetControlPoints() );
		QVector<Point3D> controlPoints = vBezier.first;
		vPoints<<vBezier.first;
	}

	QVector< BezierPatch* > surfacePatches;

	for( int u = 0; u < uPatches; u++ )
	{
		for( int v = 0; v < vPatches; v++ )
		{
			QVector< Point3D > boundedPoints;
			boundedPoints<< uPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u ];
			boundedPoints<< uPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u + 1 ];
			boundedPoints<< uPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u + 2 ];
			boundedPoints<< uPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u + 3 ];
			boundedPoints<< vPoints[ ( u + 1 ) * ( 4 * vPatches ) + 4 * v + 2 ];
			boundedPoints<< vPoints[ ( u + 1 ) * ( 4 * vPatches ) + 4 * v + 1 ];
			boundedPoints<< uPoints[ v * ( 4 * uPatches ) + 4 * u + 3 ];
			boundedPoints<< uPoints[ v * ( 4 * uPatches ) + 4 * u + 2 ];
			boundedPoints<< uPoints[ v * ( 4 * uPatches ) + 4 * u + 1 ];
			boundedPoints<< uPoints[ v * ( 4 * uPatches ) + 4 * u ];
			boundedPoints<< vPoints[ u * ( 4 * vPatches ) + 4 * v + 1 ];
			boundedPoints<< vPoints[ u * ( 4 * vPatches ) + 4 * v + 2 ];
			/*boundedPoints<< uPoints[ u * ( 4 * vPatches ) + 4 * v ];
			boundedPoints<< uPoints[ u * ( 4 * vPatches ) + 4 * v + 1];
			boundedPoints<< uPoints[ u * ( 4 * vPatches ) + 4 * v + 2 ];
			boundedPoints<< uPoints[ u * ( 4 * vPatches ) + 4 * v + 3 ];
			boundedPoints<< vPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u + 1 ];
			boundedPoints<< vPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u + 2 ];
			boundedPoints<< uPoints[ ( u + 1 )  * ( 4 * vPatches ) + 4 * v + 3];
			boundedPoints<< uPoints[ ( u + 1 ) * ( 4 * vPatches ) + 4 * v + 2];
			boundedPoints<< uPoints[ ( u + 1 ) * ( 4 * vPatches ) + 4 * v + 1 ];
			boundedPoints<< uPoints[ ( u + 1 ) * ( 4 * vPatches ) + 4 * v ];
			boundedPoints<< vPoints[ v * ( 4 * uPatches ) + 4 * u + 2 ];
			boundedPoints<< vPoints[ v * ( 4 * uPatches ) + 4 * u + 1 ];*/

			BezierPatch* patch = new BezierPatch;
			patch->SetControlPoints( boundedPoints );
			surfacePatches<<patch;
		}
	}
	return surfacePatches;
}

QPair< QVector<Point3D>, QVector< double> > CurveNetwork::KnotInsertion( QVector< double> knots, QVector< Point3D> control )
{
	bool isBezier = false;

	 while( !isBezier )
	 {
		 int j = knots.size()-1;
		 while( ( j > 0 ) && ( ( knots[j] == knots[ j +1 ] ) || ( knots.count(  knots[j] ) == m_order ) )  )j--;

		 if( j == 0 ) isBezier = true;
		 else
		 {
			 control = InsertKnot( j, m_order, knots, control );
			 double knotValue = knots[j];
			 knots.insert( j, knotValue );
		 }

	 }

	QPair< QVector<Point3D>, QVector< double> > curve;
	curve.first = control;
	curve.second = knots;
	return curve;
}

QVector<Point3D> CurveNetwork::InsertKnot( int j, int k, QVector< double> knots, QVector< Point3D> curveControlPoints )
{
	int n = curveControlPoints.size()-1;
	QVector< Point3D > newControlPoints( n+2 );

	newControlPoints[0] = curveControlPoints[0];
	for( int i = 1; i <= n; i++ )
	{
		double a;
		if( i <= ( j - k + 1 ) ) a = 1;
		else if( i > j ) a = 0;
		else
			a = ( knots[j] - knots[i] ) / ( knots[i+k] - knots[i] );
		//newControlPoints[i] = ( 1 - a ) * curveControlPoints[i-1] + ( a * curveControlPoints[i] );
		newControlPoints[i].x = ( 1 - a ) * curveControlPoints[i-1].x + ( a * curveControlPoints[i].x );
		newControlPoints[i].y = ( 1 - a ) * curveControlPoints[i-1].y + ( a * curveControlPoints[i].y );
		newControlPoints[i].z = ( 1 - a ) * curveControlPoints[i-1].z + ( a * curveControlPoints[i].z );
	}
	newControlPoints[n+1] = curveControlPoints[n];


	return newControlPoints;
}
