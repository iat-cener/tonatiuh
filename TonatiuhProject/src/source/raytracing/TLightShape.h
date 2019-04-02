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


#ifndef TLIGHTSHAPE_H_
#define TLIGHTSHAPE_H_

#include <vector>

#include <QPair>

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "TShape.h"
#include "trt.h"

class Transform;

class TLightShape : public SoShape
{
	SO_NODE_HEADER(TLightShape);

public:
	TLightShape( );
	static void initClass();

	double GetValidArea() const;
	std::vector< QPair< int, int > > GetValidAreasCoord() const;
	double GetVolume() const { return 0.0; };

	Point3D Sample( double u, double v, int a, int b ) const;
	void SetLightSourceArea( int h, int w, int** lightArea );

	trt::TONATIUH_REAL xMin;
	trt::TONATIUH_REAL xMax;
	trt::TONATIUH_REAL zMin;
	trt::TONATIUH_REAL zMax;
	trt::TONATIUH_REAL delta;

protected:
	Point3D GetPoint3D ( double u, double v ,int h,int w) const;
	bool OutOfRange( double u, double v ) const;

	void generatePrimitives(SoAction *action);
	void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	~TLightShape();

private:
	int m_heightElements;
	int** m_lightAreaMatrix;
	int m_widthElements;
	std::vector< QPair< int, int > > m_validAreasVector;

};

#endif /* TLIGHTSHAPE_H_ */
