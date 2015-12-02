/*
 * MaterialOneSideSpecular.h
 *
 *  Created on: 17/11/2015
 *      Author: amutuberria
 */

#ifndef MATERIALONESIDESPECULAR_H_
#define MATERIALONESIDESPECULAR_H_


#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>

#include "TMaterial.h"
#include "trt.h"

class SoSensor;


class MaterialOneSideSpecular : public TMaterial
{
	SO_NODE_HEADER(MaterialOneSideSpecular);

public:
	enum Distribution {
		PILLBOX = 0,
		NORMAL = 1,
	   };


	MaterialOneSideSpecular( );
	static void initClass();

    QString getIcon();
	bool OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const;


    SoSFBool isFront;
	trt::TONATIUH_REAL reflectivity;
	trt::TONATIUH_REAL sigmaSlope;
	SoSFEnum distribution;

	SoMFColor  m_ambientColor;
	SoMFColor  m_diffuseColor;
	SoMFColor  m_specularColor;
	SoMFColor  m_emissiveColor;
	SoMFFloat m_shininess;
	SoMFFloat m_transparency ;


protected:
   	virtual ~MaterialOneSideSpecular();

   	double m_sigmaOpt;

	static void updateReflectivity( void* data, SoSensor* );
	static void updateAmbientColor( void* data, SoSensor* );
	static void updateDiffuseColor( void* data, SoSensor* );
	static void updateSpecularColor( void* data, SoSensor* );
	static void updateEmissiveColor( void* data, SoSensor* );
	static void updateShininess( void* data, SoSensor* );
	static void updateTransparency( void* data, SoSensor* );

};




#endif /* MATERIALONESIDESPECULAR_H_ */
