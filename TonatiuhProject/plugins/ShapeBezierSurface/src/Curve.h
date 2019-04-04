/*
 * Curve.h
 *
 *  Created on: 02-mar-2009
 *      Author: amutuberria
 */

#ifndef CURVE_H_
#define CURVE_H_

#include <vector>

#include "Point3D.h"
#include "Vector3D.h"

class Curve
{
public:
	Curve( std::vector< Point3D> curvePoints, std::vector< double> knots );
	~Curve();

	std::vector< Point3D > GetControlPoints() const;
	std::vector< double > GetKnots() const;

private:
	void Interpolate_BSpline( std::vector< Point3D> curvePoints );
	std::vector<Point3D> TriDiag( double* a, double* b, double* c, std::vector< Point3D> curvePoints );

	std::vector< Point3D > m_controlPointsList;
	std::vector< double > m_knotList;

};

#endif /* CURVE_H_ */
