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

#ifndef MaterialStandardRoughSpecular_H_
#define MaterialStandardRoughSpecular_H_

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>

#include "TMaterial.h"
#include "trt.h"

class SoSensor;
class Vector3D;


class MaterialStandardRoughSpecular : public TMaterial
{
	SO_NODE_HEADER(MaterialStandardRoughSpecular);

public:
	enum Distribution {
		//PILLBOX = 0,
		NORMAL = 1,
	   };
	MaterialStandardRoughSpecular( );
	static void initClass();

    QString getIcon();
	bool OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const;

	trt::TONATIUH_REAL reflectivity;
	trt::TONATIUH_REAL sigmaSlope;
	trt::TONATIUH_REAL sigmaSpecularity;
	SoSFEnum distribution;

	SoMFColor mAmbientColor;
	SoMFColor mDiffuseColor;
	SoMFColor mSpecularColor;
	SoMFColor mEmissiveColor;
	SoMFFloat mShininess;
	SoMFFloat mTransparency;


protected:
   	virtual ~MaterialStandardRoughSpecular();

   	Vector3D ComputeErrorVector( double simgaError, RandomDeviate& rand ) const;

	static void updateReflectivity( void* data, SoSensor* );
	static void updateAmbientColor( void* data, SoSensor* );
	static void updateDiffuseColor( void* data, SoSensor* );
	static void updateSpecularColor( void* data, SoSensor* );
	static void updateEmissiveColor( void* data, SoSensor* );
	static void updateShininess( void* data, SoSensor* );
	static void updateTransparency( void* data, SoSensor* );

};

#endif /*MaterialStandardRoughSpecular_H_*/
