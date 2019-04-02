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

#ifndef BBOX_H_
#define BBOX_H_

#include "Point3D.h"

class Ray;

struct BBox
{
	BBox( );
	explicit BBox( const Point3D& point );
	BBox( const Point3D& point1, const Point3D& point2 );
	bool Overlaps( const BBox& bbox ) const;
	bool Inside( const Point3D& point ) const;
	void Expand( double delta );
	double Volume( ) const;
	int MaximumExtent( ) const;
	void BoundingSphere( Point3D& center, double& radius ) const;
	bool IntersectP( const Ray& ray, double* hitt0 = NULL, double* hitt1 = NULL ) const;

	Point3D pMin;
	Point3D pMax;
};

BBox Union( const BBox& bbox, const Point3D& point );
BBox Union( const BBox& bbox1, const BBox& bbox2 );
std::ostream& operator<<( std::ostream& os, const BBox& bbox );

#endif //BBOX_H_
