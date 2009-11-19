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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QString>

#include <Inventor/sensors/SoFieldSensor.h>

#include "DifferentialGeometry.h"
#include "MaterialBasicRefractive.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgc.h"
#include "tgf.h"
#include "Trace.h"
#include "Transform.h"


SO_NODE_SOURCE( MaterialBasicRefractive );

void MaterialBasicRefractive::initClass()
{
	Trace trace( "MaterialBasicRefractive::MaterialBasicRefractive", false );
	 SO_NODE_INIT_CLASS( MaterialBasicRefractive, TMaterial, "Material" );
}

MaterialBasicRefractive::MaterialBasicRefractive()
{
	Trace trace( "MaterialBasicRefractive::MaterialBasicRefractive", false );

	SO_NODE_CONSTRUCTOR( MaterialBasicRefractive );
	SO_NODE_ADD_FIELD( reflectivity, (0.0) );
	SO_NODE_ADD_FIELD( transmissivity, (0.0) );
	SO_NODE_ADD_FIELD(n1, (0.0) );
	SO_NODE_ADD_FIELD(n2, (0.0) );
	SO_NODE_ADD_FIELD( sigmaSlope, (2.0) );
	//SO_NODE_ADD_FIELD( m_sigmaSpecularity, (0.5) );
	SO_NODE_ADD_FIELD( ambientColor, (0.2, 0.2, 0.2) );
	SO_NODE_ADD_FIELD( diffuseColor, (0.8, 0.8, 0.8) );
	SO_NODE_ADD_FIELD( specularColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( emissiveColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( shininess, (0.2) );
	SO_NODE_ADD_FIELD( transparency, (0.0) );

	SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
  	SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
  	SO_NODE_SET_SF_ENUM_TYPE( distribution, Distribution);
	SO_NODE_ADD_FIELD( distribution, (PILLBOX) );

	SoFieldSensor* m_reflectivitySensor = new SoFieldSensor( updateReflectivity, this );
	m_reflectivitySensor->attach( &reflectivity );
	SoFieldSensor* m_transmissivitySensor = new SoFieldSensor( updateTransmissivity, this );
	m_transmissivitySensor->attach( &transmissivity );

	SoFieldSensor* m_ambientColorSensor = new SoFieldSensor( updateAmbientColor, this );
	m_ambientColorSensor->attach( &ambientColor );
	SoFieldSensor* m_diffuseColorSensor = new SoFieldSensor( updateDiffuseColor, this );
	m_diffuseColorSensor->attach( &diffuseColor );
	SoFieldSensor* m_specularColorSensor = new SoFieldSensor( updateSpecularColor, this );
	m_specularColorSensor->attach( &specularColor );
	SoFieldSensor* m_emissiveColorSensor = new SoFieldSensor( updateEmissiveColor, this );
	m_emissiveColorSensor->attach( &emissiveColor );
	SoFieldSensor* m_shininessSensor = new SoFieldSensor( updateShininess, this );
	m_shininessSensor->attach( &shininess );
	SoFieldSensor* m_transparencySensor = new SoFieldSensor( updateTransparency, this );
	m_transparencySensor->attach( &transparency );
}

MaterialBasicRefractive::~MaterialBasicRefractive()
{
	Trace trace( "MaterialBasicRefractive::~MaterialBasicRefractive", false );
}

QString MaterialBasicRefractive::getIcon()
{
	Trace trace( "MaterialBasicRefractive::getIcon", false );
	return QString(":icons/MaterialBasicRefractive.png");
}

void MaterialBasicRefractive::updateReflectivity( void* data, SoSensor* )
{
	Trace trace( "MaterialBasicRefractive::updateReflectivity", false );

	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
	if( material->reflectivity.getValue() < 0.0 ) material->reflectivity = 0.0;
   	if( material->reflectivity.getValue() > 1.0 ) material->reflectivity = 1.0;
}

void MaterialBasicRefractive::updateTransmissivity( void* data, SoSensor* )
{
	Trace trace( "MaterialBasicRefractive::updateTransmissivity", false );

	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
	if( material->transmissivity.getValue() < 0.0 ) material->transmissivity = 0.0;
   	if( material->transmissivity.getValue() > 1.0 ) material->transmissivity = 1.0;
}

void MaterialBasicRefractive::updateAmbientColor( void* data, SoSensor* )
{
	Trace trace( "MaterialBasicRefractive::updateAmbientColor", false );

   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->ambientColor.setValue( material->ambientColor[0] );
}

void MaterialBasicRefractive::updateDiffuseColor( void* data, SoSensor* )
{
	Trace trace( "MaterialBasicRefractive::updateDiffuseColor", false );

   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->diffuseColor.setValue( material->diffuseColor[0] );
}

void MaterialBasicRefractive::updateSpecularColor( void* data, SoSensor* )
{
	Trace trace( "MaterialBasicRefractive::updateSpecularColor", false );

   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->specularColor.setValue( material->specularColor[0] );
}

void MaterialBasicRefractive::updateEmissiveColor( void* data, SoSensor* )
{
	Trace trace( "MaterialBasicRefractive::updateEmissiveColor", false );

   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->emissiveColor.setValue( material->emissiveColor[0] );
}

void MaterialBasicRefractive::updateShininess( void* data, SoSensor* )
{
	Trace trace( "MaterialBasicRefractive::updateShininess", false );

   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->shininess.setValue( material->shininess[0] );
}

void MaterialBasicRefractive::updateTransparency( void* data, SoSensor* )
{
	Trace trace( "MaterialBasicRefractive::updateTransparency", false );

   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->transparency.setValue( material->transparency[0] );
}
Ray* MaterialBasicRefractive::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand  ) const
{
	Trace trace( "MaterialBasicRefractive::OutputRay", false );
	double randomNumber = rand.RandomDouble();
	if ( randomNumber < reflectivity.getValue()  ) return ReflectedRay( incident, dg, rand );
	else if ( randomNumber < ( reflectivity.getValue() + transmissivity.getValue() ) ) return RefractedtRay( incident, dg, rand );
	else return 0;
}

Ray* MaterialBasicRefractive::ReflectedRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand ) const
{
	Trace trace( "MaterialBasicRefractive::ReflectedRay", false );

	//Compute reflected ray (local coordinates )
	Ray* reflected = new Ray();
	reflected->origin = dg->point;

	NormalVector normalVector;
	double sSlope = sigmaSlope.getValue() / 1000;
	if( sSlope > 0.0 )
	{
		NormalVector errorNormal;
		if ( distribution.getValue() == 0 )
		{
			double phi = tgc::TwoPi * rand.RandomDouble();
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

	double cosTheta = DotProduct( normalVector, incident.direction );
	reflected->direction = Normalize( incident.direction - 2.0 * normalVector * cosTheta );
	return reflected;

}

Ray* MaterialBasicRefractive::RefractedtRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand  ) const
{
	Trace trace( "MaterialBasicRefractive::RefractedtRay", false );

	//Compute refracted ray (local coordinates )
	Ray* refracted = new Ray();
	refracted->origin = dg->point;


	NormalVector s = -dg->normal;
	double disc = ( DotProduct( incident.direction, s ) * DotProduct( incident.direction, s ) )
					+ ( ( n2.getValue() / n1.getValue() ) * ( n2.getValue() / n1.getValue() ) )
					- 1;

	double cosTheta = DotProduct( incident.direction, s );
	if( disc > 0 )
	{
		refracted->direction = ( n1.getValue() / n2.getValue() ) * ( incident.direction - ( cosTheta - sqrt( disc ) )* s );
	}
	else
	{
		refracted->direction = Normalize( incident.direction - 2.0 * cosTheta * s );
	}

	return refracted;
}
