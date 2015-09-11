/*
 * MaterialAngleDependent.h
 *
 *  Created on: 30/03/2015
 *      Author: amutuberria
 */

#ifndef MATERIALANGLEDEPENDENT_H_
#define MATERIALANGLEDEPENDENT_H_


#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "TMaterial.h"
#include "trt.h"

class SoSensor;

class MaterialAngleDependent : public TMaterial
{
	SO_NODE_HEADER(MaterialAngleDependent);

public:
	enum Distribution {
		PILLBOX = 0,
		NORMAL = 1,
	   };
	MaterialAngleDependent( );
	static void initClass();

    QString getIcon();
    bool OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const;

	//int	getFields(SoFieldList & fields) const;

    SoSFBool reflectivityFront;
    trt::TONATIUH_CONTAINERREALVECTOR2 reflectivityFrontValues;
    SoSFBool reflectivityBack;
    trt::TONATIUH_CONTAINERREALVECTOR2 reflectivityBackValues;

	trt::TONATIUH_REAL sigmaSlope;
	SoSFEnum distribution;
	SoSFColor ambient_Color;
	SoSFColor diffuse_Color;
	SoSFColor specular_Color;
	SoSFColor emissive_Color;
	SoSFFloat shininessValue;
	SoSFFloat transparencyValue;




protected:
   	virtual ~MaterialAngleDependent();

	double OutputPropertyValue( std::vector< double > anglesList,  std::vector< double > valuesList, double angle ) const;

   	static void updateReflectivityFront( void* data, SoSensor* );
   	static void updateReflectivityBack( void* data, SoSensor* );

	static void updateAmbientColor( void* data, SoSensor* );
	static void updateDiffuseColor( void* data, SoSensor* );
	static void updateSpecularColor( void* data, SoSensor* );
	static void updateEmissiveColor( void* data, SoSensor* );
	static void updateShininess( void* data, SoSensor* );
	static void updateTransparency( void* data, SoSensor* );


private:
	SoFieldSensor* m_reflectivityFrontValuesSensor;
	SoFieldSensor* m_reflectivityBackValuesSensor;

	SoFieldSensor* m_ambientColorSensor;
	SoFieldSensor* m_diffuseColorSensor;
	SoFieldSensor* m_specularColorSensor;
	SoFieldSensor* m_emissiveColorSensor;
	SoFieldSensor* m_shininessSensor;
	SoFieldSensor* m_transparencySensor;

	std::vector< double > m_frontReflectivityIncidenceAngle;
	std::vector< double > m_frontReflectivityValue;
	std::vector< double > m_backReflectivityIncidenceAngle;
	std::vector< double > m_backReflectivityValue;
};



#endif /* MATERIALANGLEDEPENDENT_H_ */
