/*
 * CurveNetwork.h
 *
 *  Created on: 26-feb-2009
 *      Author: amutuberria
 */

#ifndef CURVENETWORK_H_
#define CURVENETWORK_H_

#include <QVarLengthArray>
#include <QVector>

#include "Curve.h"
#include "Point3D.h"

class BezierPatch;


class CurveNetwork
{
public:
	CurveNetwork( int order = 4 );
	~CurveNetwork();

	void AddUCurve( Curve* curve );
	void AddVCurve( Curve* curve );

	QVector< BezierPatch* > GetSurface();
	QPair< QVector<Point3D>, QVector< double> >  KnotInsertion( QVector< double> knots, QVector< Point3D> curveControlPoints );
	//QVector<Point3D>  Interpolate_BSpline( QVector< double > knots );


private:
	//QVector<Point3D> TriDiag( double* a, double* b, double* c );
	//QPair< QVector<Point3D>, QVector< double> >  KnotInsertion( QVector< double> knots, QVector< Point3D> curveControlPoints );
	QVector<Point3D> InsertKnot( int j, int k, QVector< double> knots, QVector< Point3D> curve );

	int m_order;
	QVector< Curve* >  m_uCurveList;
	QVector< Curve* >  m_vCurveList;
};
#endif /* CURVENETWORK_H_ */
