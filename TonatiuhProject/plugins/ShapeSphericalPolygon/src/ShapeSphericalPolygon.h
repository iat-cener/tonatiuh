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
along with this program.  If
	BBox GetBBox() const;not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#ifndef SHAPESPHERICALPOLYGON_H_
#define SHAPESPHERICALPOLYGON_H_

#include <vector>

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>

#include "TShape.h"
#include "trt.h"

class SoFieldSensor;
class SoSensor;

class ShapeSphericalPolygon : public TShape
{
	SO_NODE_HEADER( ShapeSphericalPolygon );

public:
	ShapeSphericalPolygon();
	static void initClass();
    SoNode* copy( SbBool copyConnections ) const;
	double GetArea() const;
	double GetVolume() const {return 0.0;};
	BBox GetBBox() const;
    QString GetIcon() const;

	bool Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const;
	bool IntersectP( const Ray& objectRay ) const;

	Point3D Sample( double u, double v ) const;

	enum Side{
		INSIDE = 0,
		OUTSIDE   = 1,
	};

	trt::TONATIUH_REAL sphereRadius;
	trt::TONATIUH_REAL radius;
	SoSFInt32 polygonSides;
	SoSFEnum activeSide;

protected:
	static void updatePolygonSides(void *data, SoSensor *);

private:
   	~ShapeSphericalPolygon();

	Point3D GetPoint3D( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;
	bool OutOfRange( double u, double v ) const;

	static void RadiusChanged( void* data, SoSensor* );
	static void SidesChanged( void* data, SoSensor* );
	static void SphereRadiusChanged( void* data, SoSensor* );

	void generatePrimitives( SoAction* action );
	void computeBBox( SoAction* action, SbBox3f& box, SbVec3f& center );

	double m_lastValidSidesValue;
	double m_lastValidSphereRadiusValue;

	SoFieldSensor* m_radiusSensor;
	SoFieldSensor* m_sidesSensor;
	SoFieldSensor* m_sphereRadiusSensor;
};

#endif /*SHAPESPHERICALPOLYGON_H_*/
