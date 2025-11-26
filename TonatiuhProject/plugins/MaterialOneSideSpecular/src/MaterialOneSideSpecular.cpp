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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/
#include <Inventor/sensors/SoFieldSensor.h>

#include "gc.h"

#include "DifferentialGeometry.h"
#include "MaterialOneSideSpecular.h"
#include "ParameterValueException.h"
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

  	SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
  	SO_NODE_SET_SF_ENUM_TYPE(distribution, Distribution);
	SO_NODE_ADD_FIELD( distribution, (NORMAL) );

	SO_NODE_ADD_FIELD( m_ambientColor, (0.2f, 0.2f, 0.2f) );
	SO_NODE_ADD_FIELD( m_diffuseColor, (0.8f, 0.8f, 0.8f) );
	SO_NODE_ADD_FIELD( m_specularColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_emissiveColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_shininess, (0.2f) );
	SO_NODE_ADD_FIELD( m_transparency, (0.0f) );

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

std::string MaterialOneSideSpecular::GetIcon()
{
	return ( ":icons/MaterialOneSideSpecular.png" );
}

/*!
 * @brief Validates material parameter value.
 *
 * Checks whether a given parameter value is acceptable for this material.
 * If the value is invalid, a `ParameterValueException` is thrown.
 *
 * @param name The name of the parameter to validate.
 * @param value The value of the parameter to validate.
 * @return true if the parameter value is valid.
 * @throws ParameterValueException if the parameter value is invalid.
 */
bool MaterialOneSideSpecular::ValidateParamaterValue( std::string name, std::string value ) const
{
    if( ( name == "reflectivity" )  && ( ( std::stod( value ) < 0 ) || ( std::stod( value ) > 1 ) ) )
		throw ParameterValueException( "reflectivity", " The value of the 'reflectivity' parameter must be in the range [0,1]" );
   
    if( name == "sigmaSlope" && std::stod( value ) < 0 ) 
		throw ParameterValueException( "sigmaSlope", " The 'sigmaSlope' must be a positive number" );

	return true;
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
	outputRay->origin = dg->point;

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

