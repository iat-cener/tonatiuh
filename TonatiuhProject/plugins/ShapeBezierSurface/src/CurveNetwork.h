/*
 * CurveNetwork.h
 *
 *  Created on: 26-feb-2009
 *      Author: amutuberria
 */

#ifndef CURVENETWORK_H_
#define CURVENETWORK_H_

#include <vector>

#include <QVarLengthArray>

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

	std::vector< BezierPatch* > GetSurface();
	QPair< std::vector<Point3D>, std::vector< double> >  KnotInsertion( std::vector< double> knots, std::vector< Point3D> curveControlPoints );

private:
	std::vector<Point3D> InsertKnot( int j, int k, std::vector< double> knots, std::vector< Point3D> curve );

	int m_order;
	std::vector< Curve* >  m_uCurveList;
	std::vector< Curve* >  m_vCurveList;
};
#endif /* CURVENETWORK_H_ */
