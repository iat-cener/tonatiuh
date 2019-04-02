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

#ifndef MATERIALANGLEDEPENDENTREFRACTIVE_H_
#define MATERIALANGLEDEPENDENTREFRACTIVE_H_

#include <vector>

#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "TMaterial.h"
#include "MFVec3.h"
#include "trt.h"

class SoSensor;

class MaterialAngleDependentRefractive : public TMaterial
{
	SO_NODE_HEADER(MaterialAngleDependent);

public:
	enum Distribution {
		//PILLBOX = 0,
		NORMAL = 1,
	   };
	MaterialAngleDependentRefractive( );
	static void initClass();

    QString getIcon();
    bool OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const;

	//int	getFields(SoFieldList & fields) const;

    //SoSFBool reflectivityFront;
    trt::TONATIUH_REAL nFront;
    MFVec3 frontOpticValues;
    //SoSFBool reflectivityBack;
    trt::TONATIUH_REAL nBack;
    MFVec3 backOpticValues;

	trt::TONATIUH_REAL sigmaSlope;
	SoSFEnum distribution;
	SoSFColor ambient_Color;
	SoSFColor diffuse_Color;
	SoSFColor specular_Color;
	SoSFColor emissive_Color;
	SoSFFloat shininessValue;
	SoSFFloat transparencyValue;




protected:
   	virtual ~MaterialAngleDependentRefractive();

	std::vector < double > OutputPropertyValue( std::vector < double > incidenceAnglesList, std::vector < double > reflectivityValuesList, std::vector < double > transmissivityValuesList, double incidenceAngle ) const;

   	static void updateOpticFront( void* data, SoSensor* );
   	static void updateOpticBack( void* data, SoSensor* );

	static void updateAmbientColor( void* data, SoSensor* );
	static void updateDiffuseColor( void* data, SoSensor* );
	static void updateSpecularColor( void* data, SoSensor* );
	static void updateEmissiveColor( void* data, SoSensor* );
	static void updateShininess( void* data, SoSensor* );
	static void updateTransparency( void* data, SoSensor* );

	Ray ReflectedRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand  ) const;
	Ray RefractedtRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand  ) const;


private:
	SoFieldSensor* m_frontOpticValuesSensor;
	SoFieldSensor* m_backOpticValuesSensor;

	SoFieldSensor* m_ambientColorSensor;
	SoFieldSensor* m_diffuseColorSensor;
	SoFieldSensor* m_specularColorSensor;
	SoFieldSensor* m_emissiveColorSensor;
	SoFieldSensor* m_shininessSensor;
	SoFieldSensor* m_transparencySensor;

	std::vector< double > m_frontReflectivityIncidenceAngle;
	std::vector< double > m_frontReflectivityValue;
	std::vector< double > m_frontTransmissivityValue;
	std::vector< double > m_backReflectivityIncidenceAngle;
	std::vector< double > m_backReflectivityValue;
	std::vector< double > m_backTransmissivityValue;
};



#endif /* MATERIALANGLEDEPENDENTREFRACTIVE_H_ */
