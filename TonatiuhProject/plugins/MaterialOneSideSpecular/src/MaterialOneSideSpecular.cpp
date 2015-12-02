/*
 * MaterialOneSideSpecular.cpp
 *
 *  Created on: 17/11/2015
 *      Author: amutuberria
 */


#include <QString>

#include <Inventor/sensors/SoFieldSensor.h>

#include "gc.h"

#include "DifferentialGeometry.h"
#include "MaterialOneSideSpecular.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgf.h"
#include "Transform.h"


SO_NODE_SOURCE(MaterialOneSideSpecular);

void MaterialOneSideSpecular::initClass()
{
	 SO_NODE_INIT_CLASS( MaterialOneSideSpecular, TMaterial, "Material" );
}

MaterialOneSideSpecular::MaterialOneSideSpecular()
{
	SO_NODE_CONSTRUCTOR( MaterialOneSideSpecular );

	SO_NODE_ADD_FIELD( isFront, (TRUE) );
	SO_NODE_ADD_FIELD( reflectivity, (0.0) );
	SO_NODE_ADD_FIELD( sigmaSlope, (2.0) );

	SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
  	SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
  	SO_NODE_SET_SF_ENUM_TYPE(distribution, Distribution);
	SO_NODE_ADD_FIELD( distribution, (PILLBOX) );

	SO_NODE_ADD_FIELD( m_ambientColor, (0.2f, 0.2f, 0.2f) );
	SO_NODE_ADD_FIELD( m_diffuseColor, (0.8f, 0.8f, 0.8f) );
	SO_NODE_ADD_FIELD( m_specularColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_emissiveColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_shininess, (0.2f) );
	SO_NODE_ADD_FIELD( m_transparency, (0.0f) );

	SoFieldSensor* m_reflectivitySensor = new SoFieldSensor( updateReflectivity, this );
	m_reflectivitySensor->setPriority( 1 );
	m_reflectivitySensor->attach( &reflectivity );

	SoFieldSensor* m_ambientColorSensor = new SoFieldSensor( updateAmbientColor, this );
	m_ambientColorSensor->setPriority( 1 );
	m_ambientColorSensor->attach( &m_ambientColor );
	SoFieldSensor* m_diffuseColorSensor = new SoFieldSensor( updateDiffuseColor, this );
	m_diffuseColorSensor->setPriority( 1 );
	m_diffuseColorSensor->attach( &m_diffuseColor );
	SoFieldSensor* m_specularColorSensor = new SoFieldSensor( updateSpecularColor, this );
	m_specularColorSensor->setPriority( 1 );
	m_specularColorSensor->attach( &m_specularColor );
	SoFieldSensor* m_emissiveColorSensor = new SoFieldSensor( updateEmissiveColor, this );
	m_emissiveColorSensor->setPriority( 1 );
	m_emissiveColorSensor->attach( &m_emissiveColor );
	SoFieldSensor* m_shininessSensor = new SoFieldSensor( updateShininess, this );
	m_shininessSensor->setPriority( 1 );
	m_shininessSensor->attach( &m_shininess );
	SoFieldSensor* m_transparencySensor = new SoFieldSensor( updateTransparency, this );
	m_transparencySensor->setPriority( 1 );
	m_transparencySensor->attach( &m_transparency );
}

MaterialOneSideSpecular::~MaterialOneSideSpecular()
{
}

QString MaterialOneSideSpecular::getIcon()
{
	return QLatin1String(":icons/MaterialOneSideSpecular.png");
}

void MaterialOneSideSpecular::updateReflectivity( void* data, SoSensor* )
{
	MaterialOneSideSpecular* material = static_cast< MaterialOneSideSpecular* >( data );
	if( material->reflectivity.getValue() < 0.0 ) material->reflectivity = 0.0;
   	if( material->reflectivity.getValue() > 1.0 ) material->reflectivity = 1.0;
}

void MaterialOneSideSpecular::updateAmbientColor( void* data, SoSensor* )
{
   	MaterialOneSideSpecular* material = static_cast< MaterialOneSideSpecular* >( data );
 	material->ambientColor.setValue( material->m_ambientColor[0] );
}

void MaterialOneSideSpecular::updateDiffuseColor( void* data, SoSensor* )
{
   	MaterialOneSideSpecular* material = static_cast< MaterialOneSideSpecular* >( data );
 	material->diffuseColor.setValue( material->m_diffuseColor[0] );
}

void MaterialOneSideSpecular::updateSpecularColor( void* data, SoSensor* )
{
   	MaterialOneSideSpecular* material = static_cast< MaterialOneSideSpecular* >( data );
 	material->specularColor.setValue( material->m_specularColor[0] );
}

void MaterialOneSideSpecular::updateEmissiveColor( void* data, SoSensor* )
{
   	MaterialOneSideSpecular* material = static_cast< MaterialOneSideSpecular* >( data );
 	material->emissiveColor.setValue( material->m_emissiveColor[0] );
}

void MaterialOneSideSpecular::updateShininess( void* data, SoSensor* )
{
   	MaterialOneSideSpecular* material = static_cast< MaterialOneSideSpecular* >( data );
 	material->shininess.setValue( material->m_shininess[0] );
}

void MaterialOneSideSpecular::updateTransparency( void* data, SoSensor* )
{
   	MaterialOneSideSpecular* material = static_cast< MaterialOneSideSpecular* >( data );
 	material->transparency.setValue( material->m_transparency[0] );
}

bool MaterialOneSideSpecular::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay ) const
{
	if( dg->shapeFrontSide && !isFront.getValue() )	return ( false );
	if( !dg->shapeFrontSide && isFront.getValue() )	return ( false );


	double randomNumber = rand.RandomDouble();
	if ( randomNumber >= reflectivity.getValue()  ) return false;//return 0;

	//Compute reflected ray (local coordinates )


	NormalVector normalVector;
	double sSlope = sigmaSlope.getValue() / 1000;
	if( sSlope > 0.0 )
	{
		NormalVector errorNormal;
		if ( distribution.getValue() == 0 )
		{
			double phi = gc::TwoPi * rand.RandomDouble();
			double theta = sSlope * rand.RandomDouble();

			errorNormal.x = sin( theta ) * sin( phi ) ;
			errorNormal.y = cos( theta );
			errorNormal.z = sin( theta ) * cos( phi );
		 }
		 else if (distribution.getValue() == 1 )
		 {
			 errorNormal.x = sSlope * tgf::AlternateBoxMuller( rand );
			 errorNormal.y = 1.0;
			 errorNormal.z = sSlope * tgf::AlternateBoxMuller( rand );

		 }
		Vector3D r = dg->normal;
		Vector3D s = Normalize( dg->dpdu );
		Vector3D t = Normalize( dg->dpdv );
		Transform trasform( s.x, s.y, s.z, 0.0,
								r.x, r.y, r.z, 0.0,
								t.x, t.y, t.z, 0.0,
								0.0, 0.0, 0.0, 1.0);

		NormalVector normalDirection = trasform.GetInverse()( errorNormal );
		normalVector = Normalize( normalDirection );
	}
	else
	{
		normalVector = dg->normal;
	}

	double cosTheta = DotProduct( normalVector, incident.direction() );
	outputRay->setDirection( Normalize( incident.direction() - 2.0 * normalVector * cosTheta ) );
	return true;

}

