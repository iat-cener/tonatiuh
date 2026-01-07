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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Iñigo Pagola, Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/
#include <Inventor/sensors/SoFieldSensor.h>

#include "MaterialVirtual.h"

SO_NODE_SOURCE(MaterialVirtual);

void MaterialVirtual::initClass()
{
	 SO_NODE_INIT_CLASS( MaterialVirtual, TMaterial, "Material" );
}

MaterialVirtual::MaterialVirtual()
:m_ambientColorSensor( 0 ),
m_diffuseColorSensor( 0 ),
m_specularColorSensor( 0 ),
m_emissiveColorSensor( 0 ),
m_shininessSensor( 0 ),
m_transparencySensor( 0 )
{
	SO_NODE_CONSTRUCTOR( MaterialVirtual );

	SO_NODE_ADD_FIELD( m_ambientColor, (0.2f, 0.2f, 0.2f) );
	SO_NODE_ADD_FIELD( m_diffuseColor, (0.8f, 0.8f, 0.8f) );
	SO_NODE_ADD_FIELD( m_specularColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_emissiveColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_shininess, (0.2f) );
	SO_NODE_ADD_FIELD( m_transparency, (0.0f) );

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

MaterialVirtual::~MaterialVirtual()
{
	delete m_ambientColorSensor;
	delete m_diffuseColorSensor;
	delete m_specularColorSensor;
	delete m_emissiveColorSensor;
	delete m_shininessSensor;
	delete m_transparencySensor;
}

std::string MaterialVirtual::GetIcon()
{
	return ( ":icons/MaterialVirtual.png" );
}

void MaterialVirtual::updateAmbientColor( void* data, SoSensor* )
{
   	MaterialVirtual* material = static_cast< MaterialVirtual* >( data );
 	material->ambientColor.setValue( material->m_ambientColor[0] );
}

void MaterialVirtual::updateDiffuseColor( void* data, SoSensor* )
{
   	MaterialVirtual* material = static_cast< MaterialVirtual* >( data );
 	material->diffuseColor.setValue( material->m_diffuseColor[0] );
}

void MaterialVirtual::updateSpecularColor( void* data, SoSensor* )
{
   	MaterialVirtual* material = static_cast< MaterialVirtual* >( data );
 	material->specularColor.setValue( material->m_specularColor[0] );
}

void MaterialVirtual::updateEmissiveColor( void* data, SoSensor* )
{
   	MaterialVirtual* material = static_cast< MaterialVirtual* >( data );
 	material->emissiveColor.setValue( material->m_emissiveColor[0] );
}

void MaterialVirtual::updateShininess( void* data, SoSensor* )
{
   	MaterialVirtual* material = static_cast< MaterialVirtual* >( data );
 	material->shininess.setValue( material->m_shininess[0] );
}

void MaterialVirtual::updateTransparency( void* data, SoSensor* )
{
   	MaterialVirtual* material = static_cast< MaterialVirtual* >( data );
 	material->transparency.setValue( material->m_transparency[0] );
}

bool MaterialVirtual::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& /*rand*/, Ray* outputRay ) const
{
	outputRay->origin = dg->point;
	outputRay->setDirection( incident.direction() );

	return true;
}
