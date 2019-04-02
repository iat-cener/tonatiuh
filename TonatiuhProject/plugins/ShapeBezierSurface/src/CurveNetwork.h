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
