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


#ifndef ShapeHyperboloid_H_
#define ShapeHyperboloid_H_

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "TShape.h"
#include "trt.h"

struct Vector3D;

class ShapeHyperboloid : public TShape
{
	SO_NODE_HEADER(ShapeHyperboloid);

public:

	enum Side{
		INSIDE = 0,
		OUTSIDE   = 1,
	};

	ShapeHyperboloid( );
	static void initClass();
	double GetArea() const;
	double GetVolume() const {return 0.0;};
	BBox GetBBox() const;
	QString GetIcon() const;

	bool Intersect( const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;

	Point3D Sample( double u, double v ) const;

	trt::TONATIUH_REAL focusLegth;
	trt::TONATIUH_REAL distanceTwoFocus;
	trt::TONATIUH_REAL reflectorMaxDiameter;
	SoSFEnum activeSide;



protected:
	bool OutOfRange( double u, double v ) const;
	Point3D GetPoint3D ( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;

	void generatePrimitives(SoAction *action);
	void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	virtual ~ShapeHyperboloid();

private:
	Vector3D Dpdu( double u, double v ) const;
	Vector3D Dpdv( double u, double v ) const;
};

#endif /* ShapeHyperboloid_H_ */
