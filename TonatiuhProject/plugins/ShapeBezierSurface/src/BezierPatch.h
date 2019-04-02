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

#ifndef BEZIERPATCH_H_
#define BEZIERPATCH_H_

#include <vector>

#include "Point3D.h"
#include "Vector3D.h"

#include "BBox.h"
#include "TShape.h"


class BezierPatch //: public SoNurbsSurface
{

public:
	BezierPatch();
    ~BezierPatch();

	BBox GetBBox() const { return ( m_bbox ); } ;
	Point3D GetCentroid() const { return ( m_centoid ); };
	NormalVector GetNormal( double u, double v ) const;
	Point3D GetPoint3D (double u, double v) const;

	void GeneratePrimitives( SoAction *action );
    bool Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, double bezierTol ) const;
    void SetControlPoints( std::vector< Point3D > boundedPoints );


private:
    std::vector< Vector3D > CornerDerivates( std::vector< Point3D > boundedPoints );
	Vector3D DPDU( double u, double v, std::vector< Vector3D >* controlPoint = 0 ) const;
	Vector3D DPDV( double u, double v, std::vector< Vector3D >* controlPoint = 0 ) const;

	Vector3D D2PDUU( double u, double v, std::vector< Vector3D >* controlPoint ) const;
	Vector3D D2PDUV( double u, double v, std::vector< Vector3D >* controlPoint  ) const;
	Vector3D D2PDVV( double u, double v, std::vector< Vector3D >* controlPoint  ) const;

	void SplitIPatch( std::vector< Vector3D > p, std::vector< std::vector< Vector3D > >* iPatches ) const;

	void HullSplitU( std::vector< Vector3D > p, std::vector< Vector3D >* q, std::vector< Vector3D >* r ) const;
	void HullSplitV( std::vector< Vector3D > p, std::vector< Vector3D >* q, std::vector< Vector3D >* r) const;


private:
	BBox m_bbox;
	Point3D m_centoid;
	std::vector< Point3D > m_controlPoints;

	int m_nIterations;
};

#endif /* BEZIERPATCH_H_ */
