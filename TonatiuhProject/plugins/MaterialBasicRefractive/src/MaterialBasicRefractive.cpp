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

#include <QMessageBox>
#include <QString>

#include <Inventor/sensors/SoFieldSensor.h>

#include "gc.h"

#include "DifferentialGeometry.h"
#include "MaterialBasicRefractive.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgf.h"
#include "Transform.h"


SO_NODE_SOURCE( MaterialBasicRefractive );

void MaterialBasicRefractive::initClass()
{
	 SO_NODE_INIT_CLASS( MaterialBasicRefractive, TMaterial, "Material" );
}

MaterialBasicRefractive::MaterialBasicRefractive()
:m_sigmaOpt( 0 )
{
	SO_NODE_CONSTRUCTOR( MaterialBasicRefractive );
	SO_NODE_ADD_FIELD( reflectivityFront, (0.0) );
	SO_NODE_ADD_FIELD( reflectivityBack, (0.0) );
	SO_NODE_ADD_FIELD( transmissivityFront, (0.0) );
	SO_NODE_ADD_FIELD( transmissivityBack, (0.0) );
	SO_NODE_ADD_FIELD( nFront, (0.0) );
	SO_NODE_ADD_FIELD( nBack, (0.0) );
	SO_NODE_ADD_FIELD( sigmaSlope, (2.0) );
	//SO_NODE_ADD_FIELD( m_sigmaSpecularity, (0.5) );

	//SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
  	SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
  	SO_NODE_SET_SF_ENUM_TYPE( distribution, Distribution);
	SO_NODE_ADD_FIELD( distribution, (NORMAL) );

	SO_NODE_ADD_FIELD( m_ambientColor, (0.2f, 0.2f, 0.2f) );
	SO_NODE_ADD_FIELD( m_diffuseColor, (0.8f, 0.8f, 0.8f) );
	SO_NODE_ADD_FIELD( m_specularColor, (0.0f, 0.0f, 0.0f) );
	SO_NODE_ADD_FIELD( m_emissiveColor, (0.0f, 0.0f, 0.0f) );
	SO_NODE_ADD_FIELD( m_shininess, (0.2f) );
	SO_NODE_ADD_FIELD( m_transparency, (0.0) );

	SoFieldSensor* reflectivityFrontSensor = new SoFieldSensor( updateReflectivityFront, this );
	reflectivityFrontSensor->setPriority( 1 );
	reflectivityFrontSensor->attach( &reflectivityFront );
	SoFieldSensor* transmissivityFrontSensor = new SoFieldSensor( updateTransmissivityFront, this );
	transmissivityFrontSensor->setPriority( 1 );
	transmissivityFrontSensor->attach( &transmissivityFront );

	SoFieldSensor* reflectivityBackSensor = new SoFieldSensor( updateReflectivityBack, this );
	reflectivityBackSensor->setPriority( 1 );
	reflectivityBackSensor->attach( &reflectivityBack );
	SoFieldSensor* transmissivityBackSensor = new SoFieldSensor( updateTransmissivityBack, this );
	transmissivityBackSensor->setPriority( 1 );
	transmissivityBackSensor->attach( &transmissivityBack );

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

MaterialBasicRefractive::~MaterialBasicRefractive()
{
}

QString MaterialBasicRefractive::getIcon()
{
	return QString(":icons/MaterialBasicRefractive.png");
}

void MaterialBasicRefractive::updateReflectivityFront( void* data, SoSensor* )
{
	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
	if( material->reflectivityFront.getValue() < 0.0 ) material->reflectivityFront = 0.0;
   	if( material->reflectivityFront.getValue() > 1.0 ) material->reflectivityFront = 1.0;
   	if( ( material->reflectivityFront.getValue() + material->transmissivityFront.getValue() ) > 1.0 )
   	{
   		material->reflectivityFront = 0.0;
   		QMessageBox::warning( 0, QString("Tonatiuh Action"), QString( " Is not a valid value for ReflectivityFront") );
   	}

}

void MaterialBasicRefractive::updateTransmissivityFront( void* data, SoSensor* )
{

	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
	if( material->transmissivityFront.getValue() < 0.0 ) material->transmissivityFront = 0.0;
   	if( material->transmissivityFront.getValue() > 1.0 ) material->transmissivityFront = 1.0;
   	if( ( material->reflectivityFront.getValue() + material->transmissivityFront.getValue() ) > 1.0 )
   	{
   		material->transmissivityFront = 0.0;
   		QMessageBox::warning( 0, QString("Tonatiuh Action"), QString( " Is not a valid value for TransmissivityFront") );
   	}
}

void MaterialBasicRefractive::updateReflectivityBack( void* data, SoSensor* )
{

	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
	if( material->reflectivityBack.getValue() < 0.0 ) material->reflectivityBack = 0.0;
   	if( material->reflectivityBack.getValue() > 1.0 ) material->reflectivityBack = 1.0;
   	if( ( material->reflectivityBack.getValue() + material->transmissivityBack.getValue() ) > 1.0 )
   	{
   			material->reflectivityBack = 0.0;
			QMessageBox::warning( 0, QString("Tonatiuh Action"), QString( " Is not a valid value for ReflectivityBack") );
   	}
}

void MaterialBasicRefractive::updateTransmissivityBack( void* data, SoSensor* )
{
	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
	if( material->transmissivityBack.getValue() < 0.0 ) material->transmissivityBack = 0.0;
   	if( material->transmissivityBack.getValue() > 1.0 ) material->transmissivityBack = 1.0;
   	if( ( material->reflectivityBack.getValue() + material->transmissivityBack.getValue() ) > 1.0 )
   	{
   		material->transmissivityBack = 0.0;
   		QMessageBox::warning( 0, QString("Tonatiuh Action"), QString( " Is not a valid value for TransmissivityBack") );
   	}

}

void MaterialBasicRefractive::updateAmbientColor( void* data, SoSensor* )
{
   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->ambientColor.setValue( material->m_ambientColor[0] );
}

void MaterialBasicRefractive::updateDiffuseColor( void* data, SoSensor* )
{
   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->diffuseColor.setValue( material->m_diffuseColor[0] );
}

void MaterialBasicRefractive::updateSpecularColor( void* data, SoSensor* )
{
	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->specularColor.setValue( material->m_specularColor[0] );
}

void MaterialBasicRefractive::updateEmissiveColor( void* data, SoSensor* )
{
   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->emissiveColor.setValue( material->m_emissiveColor[0] );
}

void MaterialBasicRefractive::updateShininess( void* data, SoSensor* )
{
   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->shininess.setValue( material->m_shininess[0] );
}

void MaterialBasicRefractive::updateTransparency( void* data, SoSensor* )
{
   	MaterialBasicRefractive* material = static_cast< MaterialBasicRefractive* >( data );
 	material->transparency.setValue( material->m_transparency[0] );
}

bool MaterialBasicRefractive::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const
{

	double randomNumber = rand.RandomDouble();
	if( dg->shapeFrontSide )
	{
		if ( randomNumber < reflectivityFront.getValue()  )
		{
			*outputRay = ReflectedRay( incident, dg, rand );
			return true;
		}
		else if ( randomNumber < ( reflectivityFront.getValue() + transmissivityFront.getValue() ) )
		{
			*outputRay = RefractedtRay( incident, dg, rand );
			return true;
		}
		else return false;
	}
	else
	{
		if ( randomNumber < reflectivityBack.getValue()  )
		{
			*outputRay = ReflectedRay( incident, dg, rand );
			return true;
		}
		else if ( randomNumber < ( reflectivityBack.getValue() + transmissivityBack.getValue() ) )
		{
			*outputRay = RefractedtRay( incident, dg, rand );
			return true;
		}
		else return false;

	}
}

Ray MaterialBasicRefractive::ReflectedRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand ) const
{
	NormalVector dgNormal;
	if( dg->shapeFrontSide )	dgNormal = dg->normal;
	else	dgNormal = - dg->normal;


	//Compute reflected ray (local coordinates )
	Ray reflected;
	reflected.origin = dg->point;

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
		Vector3D r = dgNormal;
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
		normalVector = dgNormal;
	}

	double cosTheta = DotProduct( normalVector, incident.direction() );
	reflected.setDirection( Normalize( incident.direction() - 2.0 * normalVector * cosTheta ) );
	return reflected;

}

Ray MaterialBasicRefractive::RefractedtRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& /* rand */  ) const
{

	NormalVector s;
	double n1;
	double n2;

	if( dg->shapeFrontSide )
	{
		s = dg->normal;
		n1 = nFront.getValue();
		n2 = nBack.getValue();
	}
	else
	{
		s = - dg->normal;
		n1 = nBack.getValue();
		n2 = nFront.getValue();
	}

	//Compute refracted ray (local coordinates )
	Ray refracted;
	refracted.origin = dg->point;


	double cosTheta = DotProduct( -incident.direction(), s );
/*
	double disc = ( cosTheta * cosTheta ) * ( ( n2 / n1 ) * ( n2 / n1 ) ) - 1;
	if( n1 > n2 )
	{
		if( disc > 0 )refracted->setDirection( ( n1 / n2 ) * ( incident.direction() + ( cosTheta - sqrt( disc ) )* s ) );
		else
			refracted->setDirection( Normalize( incident.direction() + 2.0 * cosTheta * s ) );
	}
	else
		refracted->setDirection( ( n1 / n2 ) * ( incident.direction() + ( cosTheta - sqrt( disc ) )* s ) );
	*/

	double sin2Theta = ( n1/ n2 ) * ( n1/ n2 )  * ( 1 - cosTheta  * cosTheta );
	if( n1 > n2 )
	{
		if( sin2Theta < 1.0 ) refracted.setDirection( Normalize( ( n1/ n2 ) * incident.direction() + ( ( n1/ n2 ) * cosTheta -  sqrt(1 - sin2Theta ) ) * s ) );
		else //Total Internal Reflection
			refracted.setDirection( Normalize( incident.direction() + 2.0 * cosTheta * s ) );
	}
	else
		refracted.setDirection( Normalize( ( n1/ n2 ) * incident.direction() + ( ( n1/ n2 ) * cosTheta -  sqrt(1 - sin2Theta ) ) * s ) );

	return refracted;
}
