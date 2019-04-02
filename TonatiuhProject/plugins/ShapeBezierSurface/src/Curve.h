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
