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

#ifndef SHAPESPHERICALRECTANGLE_H_
#define SHAPESPHERICALRECTANGLE_H_

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "TShape.h"
#include "trt.h"

class SoFieldSensor;
class SoSensor;
struct Vector3D;

class ShapeSphericalRectangle : public TShape
{
	SO_NODE_HEADER(ShapeSphericalRectangle);

public:
	enum Side{
		INSIDE = 0,
		OUTSIDE   = 1,
	};

	ShapeSphericalRectangle();
	static void initClass();
	double GetArea() const;
	double GetVolume() const {return 0.0;};
	BBox GetBBox() const;
	QString GetIcon() const;

	bool Intersect(const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;

	Point3D Sample( double u, double v ) const;

	trt::TONATIUH_REAL radius;
	trt::TONATIUH_REAL widthX;
	trt::TONATIUH_REAL widthZ;
	SoSFEnum activeSide;

protected:
	Point3D GetPoint3D ( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;
	bool OutOfRange( double u, double v ) const;
	Vector3D GetDPDU( double u, double v ) const;
	Vector3D GetDPDV( double u, double v ) const;

	static void updateRadius(void *data, SoSensor *);
	static void updateWidthX(void *data, SoSensor *);
	static void updateWidthZ(void *data, SoSensor *);

	void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	void generatePrimitives(SoAction *action);
   	~ShapeSphericalRectangle();

private:
	SoFieldSensor* m_radiusSensor;
	SoFieldSensor* m_widthXSensor;
	SoFieldSensor* m_widthZSensor;
};

#endif /*SHAPESPHERICALRECTANGLE_H_*/
