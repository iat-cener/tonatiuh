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

#ifndef SHAPECYLINDER_H_
#define SHAPECYLINDER_H_

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>

#include "TShape.h"
#include "trt.h"

class ShapeCylinder : public TShape
{
	SO_NODE_HEADER(ShapeCylinder);

public:
	ShapeCylinder( );
	static void initClass();
	double GetArea() const;
	double GetVolume() const;
	BBox GetBBox() const;
	QString GetIcon() const;

	bool Intersect( const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;

	Point3D Sample( double u, double v ) const;


	enum Side{
		INSIDE = 0,
		OUTSIDE   = 1,
	};

	trt::TONATIUH_REAL radius;
	trt::TONATIUH_REAL length;
	trt::TONATIUH_REAL phiMax;
	SoSFEnum activeSide;

protected:
	bool OutOfRange( double u, double v ) const;
	Point3D GetPoint3D ( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;

	void generatePrimitives(SoAction *action);
	void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	virtual ~ShapeCylinder();
};

#endif /*SHAPECYLINDER_H_*/
