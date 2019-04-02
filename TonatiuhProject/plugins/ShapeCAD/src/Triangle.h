/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
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

#ifndef TRIANGLE_H_
#define TRIANGLE_H_


#include "BBox.h"
#include "NormalVector.h"
#include "Point3D.h"
#include "Vector3D.h"

class DifferentialGeometry;
class Ray;

class Triangle
{

public:
	Triangle( Point3D v1, Point3D v2, Point3D v3, NormalVector normal );


	BBox GetBBox() const { return ( m_bbox ); } ;
	Point3D GetCentroid() const { return ( m_centoid ); };
	NormalVector GetNormal() const { return ( m_normal ); } ;
	Point3D GetVertex1() const { return ( m_v1 ); } ;
	Point3D GetVertex2() const { return ( m_v2 ); } ;
	Point3D GetVertex3() const { return ( m_v3 ); } ;

	bool Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const;

private:

	BBox m_bbox;
	Point3D m_centoid;
	NormalVector m_normal;
	Point3D m_v1;
	Point3D m_v2;
	Point3D m_v3;


	Vector3D m_vE1;
	Vector3D m_vE2;
	Vector3D m_vW1;

	double m_tol;
};


#endif /* TRIANGLE_H_ */
