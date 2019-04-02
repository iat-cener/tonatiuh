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

#include <QString>

#include <Inventor/sensors/SoFieldSensor.h>

#include "gc.h"

#include "DifferentialGeometry.h"
#include "MaterialStandardSpecular.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgf.h"
#include "Transform.h"


SO_NODE_SOURCE(MaterialStandardSpecular);

void MaterialStandardSpecular::initClass()
{
	 SO_NODE_INIT_CLASS( MaterialStandardSpecular, TMaterial, "Material" );
}

MaterialStandardSpecular::MaterialStandardSpecular()
:m_sigmaOpt( 0 )
{
	SO_NODE_CONSTRUCTOR( MaterialStandardSpecular );
	SO_NODE_ADD_FIELD( m_reflectivity, (0.0) );
	SO_NODE_ADD_FIELD( m_sigmaSlope, (2.0) );

	//SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
  	SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
  	SO_NODE_SET_SF_ENUM_TYPE(m_distribution, Distribution);
	SO_NODE_ADD_FIELD( m_distribution, (NORMAL) );

	SO_NODE_ADD_FIELD( m_ambientColor, (0.2f, 0.2f, 0.2f) );
	SO_NODE_ADD_FIELD( m_diffuseColor, (0.8f, 0.8f, 0.8f) );
	SO_NODE_ADD_FIELD( m_specularColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_emissiveColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_shininess, (0.2f) );
	SO_NODE_ADD_FIELD( m_transparency, (0.0f) );

	m_reflectivitySensor = new SoFieldSensor( updateReflectivity, this );
	m_reflectivitySensor->setPriority( 1 );
	m_reflectivitySensor->attach( &m_reflectivity );

	m_ambientColorSensor = new SoFieldSensor( updateAmbientColor, this );
	m_ambientColorSensor->setPriority( 1 );
	m_ambientColorSensor->attach( &m_ambientColor );
	m_diffuseColorSensor = new SoFieldSensor( updateDiffuseColor, this );
	m_diffuseColorSensor->setPriority( 1 );
	m_diffuseColorSensor->attach( &m_diffuseColor );
	m_specularColorSensor = new SoFieldSensor( updateSpecularColor, this );
	m_specularColorSensor->setPriority( 1 );
	m_specularColorSensor->attach( &m_specularColor );
	m_emissiveColorSensor = new SoFieldSensor( updateEmissiveColor, this );
	m_emissiveColorSensor->setPriority( 1 );
	m_emissiveColorSensor->attach( &m_emissiveColor );
	m_shininessSensor = new SoFieldSensor( updateShininess, this );
	m_shininessSensor->setPriority( 1 );
	m_shininessSensor->attach( &m_shininess );
	m_transparencySensor = new SoFieldSensor( updateTransparency, this );
	m_transparencySensor->setPriority( 1 );
	m_transparencySensor->attach( &m_transparency );
}

MaterialStandardSpecular::~MaterialStandardSpecular()
{
	delete m_reflectivitySensor;
	delete m_ambientColorSensor;
	delete m_diffuseColorSensor;
	delete m_specularColorSensor;
	delete m_emissiveColorSensor;
	delete m_shininessSensor;
	delete m_transparencySensor;
}

QString MaterialStandardSpecular::getIcon()
{
	return QLatin1String(":icons/MaterialStandardSpecular.png");
}

void MaterialStandardSpecular::updateReflectivity( void* data, SoSensor* )
{
	MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >( data );
	if( material->m_reflectivity.getValue() < 0.0 ) material->m_reflectivity = 0.0;
   	if( material->m_reflectivity.getValue() > 1.0 ) material->m_reflectivity = 1.0;
}

void MaterialStandardSpecular::updateAmbientColor( void* data, SoSensor* )
{
   	MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >( data );
 	material->ambientColor.setValue( material->m_ambientColor[0] );
}

void MaterialStandardSpecular::updateDiffuseColor( void* data, SoSensor* )
{
   	MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >( data );
 	material->diffuseColor.setValue( material->m_diffuseColor[0] );
}

void MaterialStandardSpecular::updateSpecularColor( void* data, SoSensor* )
{
   	MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >( data );
 	material->specularColor.setValue( material->m_specularColor[0] );
}

void MaterialStandardSpecular::updateEmissiveColor( void* data, SoSensor* )
{
   	MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >( data );
 	material->emissiveColor.setValue( material->m_emissiveColor[0] );
}

void MaterialStandardSpecular::updateShininess( void* data, SoSensor* )
{
   	MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >( data );
 	material->shininess.setValue( material->m_shininess[0] );
}

void MaterialStandardSpecular::updateTransparency( void* data, SoSensor* )
{
   	MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >( data );
 	material->transparency.setValue( material->m_transparency[0] );
}

bool MaterialStandardSpecular::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay ) const
{
	double randomNumber = rand.RandomDouble();
	if ( randomNumber >= m_reflectivity.getValue()  ) return false;//return 0;

	//Compute reflected ray (local coordinates )
	outputRay->origin = dg->point;

	NormalVector normalVector;
	double sigmaSlope = m_sigmaSlope.getValue() / 1000;
	if( sigmaSlope > 0.0 )
	{
		NormalVector errorNormal;
		if ( m_distribution.getValue() == 0 )
		{
			double phi = gc::TwoPi * rand.RandomDouble();
			double theta = sigmaSlope * rand.RandomDouble();

			errorNormal.x = sin( theta ) * sin( phi ) ;
			errorNormal.y = cos( theta );
			errorNormal.z = sin( theta ) * cos( phi );
		 }
		 else if (m_distribution.getValue() == 1 )
		 {
			 errorNormal.x = sigmaSlope * tgf::AlternateBoxMuller( rand );
			 errorNormal.y = 1.0;
			 errorNormal.z = sigmaSlope * tgf::AlternateBoxMuller( rand );

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
