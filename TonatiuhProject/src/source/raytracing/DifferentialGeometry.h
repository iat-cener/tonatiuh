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

#ifndef DIFFERENTIALGEOMETRY_H
#define DIFFERENTIALGEOMETRY_H


#include "NormalVector.h"
#include "Point3D.h"
#include "Vector3D.h"

class TShape;

struct DifferentialGeometry
{
    DifferentialGeometry( );
	DifferentialGeometry( const Point3D& P, const Vector3D& DPDU,
			const Vector3D& DPDV, const Vector3D& DNDU,
			const Vector3D& DNDV, double uu, double vv,
			const TShape* sh );
    ~DifferentialGeometry( );

    Point3D point;
    NormalVector normal;
    double u;
    double v;
    double dudx;
    double dvdx;
    double dudy;
    double dvdy;
    const TShape* pShape;
    Vector3D dpdu;
    Vector3D dpdv;
    Vector3D dndu;
    Vector3D dndv;
    bool shapeFrontSide;
};

#endif
