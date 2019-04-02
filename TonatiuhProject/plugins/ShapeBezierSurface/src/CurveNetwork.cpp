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
***************************************************************************/
 
#include <algorithm>

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

std::vector< BezierPatch* > CurveNetwork::GetSurface()
{

	int uPatches = m_vCurveList.size() - 1;
	int vPatches = m_uCurveList.size() - 1;

	std::vector< Point3D > uPoints;
	std::vector< Point3D > vPoints;

	for( unsigned int u = 0; u < m_uCurveList.size(); u++ )
	{
		QPair< std::vector<Point3D>, std::vector< double> > uBezier = KnotInsertion( m_uCurveList[u]->GetKnots(), m_uCurveList[u]->GetControlPoints() );
		
		std::vector<Point3D> controlPoints = uBezier.first;
		for( unsigned int p = 0; p < controlPoints.size(); p++ )
			uPoints.push_back( controlPoints[p] );
	}

	for( unsigned int v = 0; v < m_vCurveList.size(); v++ )
	{
		QPair< std::vector<Point3D>, std::vector< double> > vBezier = KnotInsertion( m_vCurveList[v]->GetKnots(), m_vCurveList[v]->GetControlPoints() );
		
		std::vector<Point3D> controlPoints = vBezier.first;
		for( unsigned int p = 0; p < controlPoints.size(); p++ )
			vPoints.push_back( controlPoints[p] );
	}

	std::vector< BezierPatch* > surfacePatches;

	for( int u = 0; u < uPatches; u++ )
	{
		for( int v = 0; v < vPatches; v++ )
		{
			std::vector< Point3D > boundedPoints;
			boundedPoints.push_back(  uPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u ] );
			boundedPoints.push_back( uPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u + 1 ] );
			boundedPoints.push_back( uPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u + 2 ] );
			boundedPoints.push_back( uPoints[ ( v + 1 ) * ( 4 * uPatches ) + 4 * u + 3 ] );
			boundedPoints.push_back( vPoints[ ( u + 1 ) * ( 4 * vPatches ) + 4 * v + 2 ] );
			boundedPoints.push_back( vPoints[ ( u + 1 ) * ( 4 * vPatches ) + 4 * v + 1 ] );
			boundedPoints.push_back( uPoints[ v * ( 4 * uPatches ) + 4 * u + 3 ] );
			boundedPoints.push_back(  uPoints[ v * ( 4 * uPatches ) + 4 * u + 2 ] );
			boundedPoints.push_back(  uPoints[ v * ( 4 * uPatches ) + 4 * u + 1 ] );
			boundedPoints.push_back(  uPoints[ v * ( 4 * uPatches ) + 4 * u ] );
			boundedPoints.push_back(  vPoints[ u * ( 4 * vPatches ) + 4 * v + 1 ] );
			boundedPoints.push_back(  vPoints[ u * ( 4 * vPatches ) + 4 * v + 2 ] );

			BezierPatch* patch = new BezierPatch;
			patch->SetControlPoints( boundedPoints );
			surfacePatches.push_back( patch );
		}
	}
	return surfacePatches;
}

QPair< std::vector<Point3D>, std::vector< double> > CurveNetwork::KnotInsertion( std::vector< double> knots, std::vector< Point3D> control )
{
	bool isBezier = false;

	 while( !isBezier )
	 {
		 int j = knots.size()-1;
		  while( ( j > 0 ) && ( ( knots[j] == knots[ j +1 ] ) || ( std::count (knots.begin(), knots.end(), knots[j]) == m_order ) )  ) j--;

		 if( j == 0 ) isBezier = true;
		 else
		 {
			 control = InsertKnot( j, m_order, knots, control );
			 double knotValue = knots[j];

			 knots.insert( knots.begin()+j, knotValue );
		 }

	 }

	QPair< std::vector<Point3D>, std::vector< double> > curve;
	curve.first = control;
	curve.second = knots;
	return curve;
}

std::vector<Point3D> CurveNetwork::InsertKnot( int j, int k, std::vector< double> knots, std::vector< Point3D> curveControlPoints )
{
	int n = curveControlPoints.size()-1;
	std::vector< Point3D > newControlPoints( n+2 );

	newControlPoints[0] = curveControlPoints[0];
	for( int i = 1; i <= n; i++ )
	{
		double a;
		if( i <= ( j - k + 1 ) ) a = 1;
		else if( i > j ) a = 0;
		else
			a = ( knots[j] - knots[i] ) / ( knots[i+k] - knots[i] );
		newControlPoints[i].x = ( 1 - a ) * curveControlPoints[i-1].x + ( a * curveControlPoints[i].x );
		newControlPoints[i].y = ( 1 - a ) * curveControlPoints[i-1].y + ( a * curveControlPoints[i].y );
		newControlPoints[i].z = ( 1 - a ) * curveControlPoints[i-1].z + ( a * curveControlPoints[i].z );
	}
	newControlPoints[n+1] = curveControlPoints[n];


	return newControlPoints;
}
