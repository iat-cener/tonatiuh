/*
 * Curve.h
 *
 *  Created on: 02-mar-2009
 *      Author: amutuberria
 */

#ifndef CURVE_H_
#define CURVE_H_

#include <QVector>
#include "Point3D.h"
#include "Vector3D.h"

class Curve
{
public:
	Curve( QVector< Point3D> curvePoints, QVector< double> knots );
	~Curve();

	QVector< Point3D > GetControlPoints() const;
	QVector< double > GetKnots() const;

private:
	void Interpolate_BSpline( QVector< Point3D> curvePoints );
	QVector<Point3D> TriDiag( double* a, double* b, double* c, QVector< Point3D> curvePoints );

	QVector< Point3D > m_controlPointsList;
	QVector< double > m_knotList;

};

#endif /* CURVE_H_ */
