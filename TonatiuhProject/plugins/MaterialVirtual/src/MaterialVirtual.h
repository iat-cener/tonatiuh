/*
 * MaterialVirtual.h
 *
 *  Created on: 22/05/2012
 *      Author: amutuberria
 */

#ifndef MATERIALVIRTUAL_H_
#define MATERIALVIRTUAL_H_


#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>

#include "TMaterial.h"
#include "trt.h"

class SoSensor;


class MaterialVirtual : public TMaterial
{
	SO_NODE_HEADER(MaterialVirtual);

public:
	MaterialVirtual( );
	static void initClass();

    QString getIcon();
	bool OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const;

	SoMFColor  m_ambientColor;
	SoMFColor  m_diffuseColor;
	SoMFColor  m_specularColor;
	SoMFColor  m_emissiveColor;
	SoMFFloat m_shininess;
	SoMFFloat m_transparency ;


protected:
   	virtual ~MaterialVirtual();

	static void updateAmbientColor( void* data, SoSensor* );
	static void updateDiffuseColor( void* data, SoSensor* );
	static void updateSpecularColor( void* data, SoSensor* );
	static void updateEmissiveColor( void* data, SoSensor* );
	static void updateShininess( void* data, SoSensor* );
	static void updateTransparency( void* data, SoSensor* );

};

#endif /* MATERIALVIRTUAL_H_ */
