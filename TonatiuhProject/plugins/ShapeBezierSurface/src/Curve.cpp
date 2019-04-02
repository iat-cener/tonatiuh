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

#include <math.h>

#include "Curve.h"
#include "Point3D.h"
#include "Vector3D.h"

Curve::Curve( std::vector< Point3D> curvePoints, std::vector< double> knots )
:m_controlPointsList( curvePoints.size()+2 ), m_knotList( knots )
{
	Interpolate_BSpline( curvePoints );
}

Curve::~Curve()
{

}

std::vector< Point3D > Curve::GetControlPoints() const
{
	return ( m_controlPointsList );
}

std::vector< double > Curve::GetKnots() const
{
	return m_knotList;
}

void Curve::Interpolate_BSpline( std::vector< Point3D> curvePoints )
{

	int nData = curvePoints.size();
	float t1, t2, t3, t4, t5;

	double* alpha = new double[nData];
	double* beta = new double[nData];
	double* gamma = new double[nData];

	alpha[0] = 0.0;
	alpha[nData-1]= 0.0;

	beta[0] = 1.0;
	beta[nData-1]= 1.0;

	gamma[0] = 0.0;
	gamma[nData-1]= 0.0;

	for( int i = 1; i < nData-1; i++ )
	{
		t1 = m_knotList[i+1];
		t2 = m_knotList[i+2];
		t3 = m_knotList[i+3];
		t4 = m_knotList[i+4];
		t5 = m_knotList[i+5];

		alpha[i] = ( t4 - t3 ) * ( t4 - t3 ) / ( t4 - t1 );
		beta[i] = ( t3 - t1 ) * ( t4 - t3 ) / ( t4 - t1 ) + ( t5 - t3 ) * ( t3 - t2 ) / ( t5 - t2 );
		gamma[i] = ( t3 - t2 ) * ( t3 - t2 ) / ( t5 - t2 );
		alpha[i] /= ( t4 - t2 );
		beta[i] /= ( t4 - t2 );
		gamma[i] /= ( t4 - t2 );
	}

	std::vector<Point3D> x = TriDiag( alpha, beta, gamma, curvePoints );

	//m_controlPointsList
	m_controlPointsList.clear();
	m_controlPointsList.push_back( x[0] );
	for( unsigned int j = 0; j < x.size() ; j++)
		m_controlPointsList.push_back( x[j] );

	m_controlPointsList.push_back( x[x.size()-1] );


	delete alpha;
	delete beta;
	delete gamma;
}

std::vector<Point3D> Curve::TriDiag( double* a, double* b, double* c, std::vector< Point3D> curvePoints  )
{
	int n = curvePoints.size();

	double beta;


	Point3D* x = new Point3D[n+1];
	double* gamma = new double[n];
	Point3D* delta = new Point3D[n];

	//forward elimination sweep
	gamma[0] = c[0] / b[0];
	delta[0] = curvePoints[0] / b[0];

	for( int j = 1; j< n; j++ )
	{
		beta = b[j] - a[j] * gamma[j-1];
		gamma[j] = c[j] / beta;

		//delta[j] =( curvePoints[j] + ( -1 * a[j] * delta[j-1] ) ) / beta;
		delta[j].x =( curvePoints[j].x + ( -1 * a[j] * delta[j-1].x ) ) / beta;
		delta[j].y =( curvePoints[j].y + ( -1 * a[j] * delta[j-1].y ) ) / beta;
		delta[j].z =( curvePoints[j].z + ( -1 * a[j] * delta[j-1].z ) ) / beta;
	}

	//backward substitution sweep
	x[n-1] = delta[n-1];
	for( int j = n-2; j >=0; j--)
	{
		//Vector3D aux = delta[j] - gamma[j] * x[j+1];
		//x[j] = delta[j] + ( -1 * gamma[j] * x[j+1] );

		x[j].x = delta[j].x + ( -1 * gamma[j] * x[j+1].x );
		x[j].y = delta[j].y + ( -1 * gamma[j] * x[j+1].y );
		x[j].z = delta[j].z + ( -1 * gamma[j] * x[j+1].z );
	}

	std::vector<Point3D> control( n );
	for( int j = 0; j < n ; j++)
	{
		Point3D point = x[j];
		control[j] = point;
	}

	return ( control );
}
