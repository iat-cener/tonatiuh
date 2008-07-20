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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez, 
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "MaterialGeneric.h"
#include "tgc.h"
#include "tgf.h" 
#include "Trace.h"

#include <QTime>

SO_NODE_SOURCE(MaterialGeneric);

void MaterialGeneric::initClass()
{
	Trace trace( "MaterialGeneric::MaterialGeneric", false );
	
	 SO_NODE_INIT_CLASS( MaterialGeneric, TMaterial, "Material" );
}

MaterialGeneric::MaterialGeneric()
{
	Trace trace( "MaterialGeneric::MaterialGeneric", false );
	
	SO_NODE_CONSTRUCTOR( MaterialGeneric );	
	SO_NODE_ADD_FIELD( m_reflectivity, (0.0) );
	SO_NODE_ADD_FIELD( m_sigmaSlope, (2.0) );
	SO_NODE_ADD_FIELD( m_sigmaSpecularity, (0.5) );
	SO_NODE_ADD_FIELD( m_ambientColor, (0.2, 0.2, 0.2) );
	SO_NODE_ADD_FIELD( m_diffuseColor, (0.8, 0.8, 0.8) );
	SO_NODE_ADD_FIELD( m_specularColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_emissiveColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( m_shininess, (0.2) );
	SO_NODE_ADD_FIELD( m_transparency, (0.0) );
	
	SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
  	SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
  	SO_NODE_SET_SF_ENUM_TYPE(m_distribution, Distribution); 
	SO_NODE_ADD_FIELD( m_distribution, (PILLBOX) );
	
	m_sigmaOpt = tgf::computeSigmaOptica( m_sigmaSlope.getValue(), m_sigmaSpecularity.getValue() );

	SoFieldSensor* m_reflectivitySensor = new SoFieldSensor( updateReflectivity, this );
	m_reflectivitySensor->attach( &m_reflectivity );	
				
	m_sigmaSlopeSensor = new SoFieldSensor( updateSigmaOpt, this );
	m_sigmaSlopeSensor->attach( &m_sigmaSlope );	
	
	m_sigmaSpecularitySensor = new SoFieldSensor( updateSigmaOpt, this );
	m_sigmaSpecularitySensor->attach( &m_sigmaSpecularity );	
	
	SoFieldSensor* m_ambientColorSensor = new SoFieldSensor( updateAmbientColor, this );
	m_ambientColorSensor->attach( &m_ambientColor );
	SoFieldSensor* m_diffuseColorSensor = new SoFieldSensor( updateDiffuseColor, this );
	m_diffuseColorSensor->attach( &m_diffuseColor );	
	SoFieldSensor* m_specularColorSensor = new SoFieldSensor( updateSpecularColor, this );
	m_specularColorSensor->attach( &m_specularColor );	
	SoFieldSensor* m_emissiveColorSensor = new SoFieldSensor( updateEmissiveColor, this );
	m_emissiveColorSensor->attach( &m_emissiveColor );	
	SoFieldSensor* m_shininessSensor = new SoFieldSensor( updateShininess, this );
	m_shininessSensor->attach( &m_shininess );	
	SoFieldSensor* m_transparencySensor = new SoFieldSensor( updateTransparency, this );
	m_transparencySensor->attach( &m_transparency );		
}

MaterialGeneric::~MaterialGeneric()
{	
	Trace trace( "MaterialGeneric::~MaterialGeneric", false );
}

QString MaterialGeneric::getIcon()
{
	Trace trace( "MaterialGeneric::getIcon", false );
	
	return QString(":icons/MaterialGeneric.png");
}
 
void MaterialGeneric::updateSigmaOpt( void* data, SoSensor* )
{
	Trace trace( "MaterialGeneric::updateSigmaOpt", false );
	MaterialGeneric* material = static_cast< MaterialGeneric* >( data );
   	material->m_sigmaOpt = tgf::computeSigmaOptica( material->m_sigmaSlope.getValue(), material->m_sigmaSpecularity.getValue() );
}

void MaterialGeneric::updateReflectivity( void* data, SoSensor* )
{
	Trace trace( "MaterialGeneric::updateReflectivity", false );
	
	MaterialGeneric* material = static_cast< MaterialGeneric* >( data );
	if( material->m_reflectivity.getValue() < 0.0 ) material->m_reflectivity = 0.0;
   	if( material->m_reflectivity.getValue() > 1.0 ) material->m_reflectivity = 1.0;
}

void MaterialGeneric::updateAmbientColor( void* data, SoSensor* )
{
	Trace trace( "MaterialGeneric::updateAmbientColor", false );
	
   	MaterialGeneric* material = static_cast< MaterialGeneric* >( data );
 	material->ambientColor.setValue( material->m_ambientColor[0] );
}

void MaterialGeneric::updateDiffuseColor( void* data, SoSensor* )
{
	Trace trace( "MaterialGeneric::updateDiffuseColor", false );
	
   	MaterialGeneric* material = static_cast< MaterialGeneric* >( data );
 	material->diffuseColor.setValue( material->m_diffuseColor[0] );
}

void MaterialGeneric::updateSpecularColor( void* data, SoSensor* )
{
	Trace trace( "MaterialGeneric::updateSpecularColor", false );
	
   	MaterialGeneric* material = static_cast< MaterialGeneric* >( data );
 	material->specularColor.setValue( material->m_specularColor[0] );
}

void MaterialGeneric::updateEmissiveColor( void* data, SoSensor* )
{
	Trace trace( "MaterialGeneric::updateEmissiveColor", false );
	
   	MaterialGeneric* material = static_cast< MaterialGeneric* >( data );
 	material->emissiveColor.setValue( material->m_emissiveColor[0] );
}

void MaterialGeneric::updateShininess( void* data, SoSensor* )
{
	Trace trace( "MaterialGeneric::updateShininess", false );
	
   	MaterialGeneric* material = static_cast< MaterialGeneric* >( data );
 	material->shininess.setValue( material->m_shininess[0] );
}

void MaterialGeneric::updateTransparency( void* data, SoSensor* )
{
	Trace trace( "MaterialGeneric::updateTransparency", false );
	
   	MaterialGeneric* material = static_cast< MaterialGeneric* >( data );
 	material->transparency.setValue( material->m_transparency[0] );
}
	
Ray* MaterialGeneric::GetReflectedRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand ) const
{
	Trace trace( "MaterialGeneric::GetReflectedRay", false );
	
	double randomNumber = rand.RandomDouble();	
	if ( randomNumber >= m_reflectivity.getValue()  ) return 0;
	
	//Compute reflected ray (local coordinates )
	Ray* reflected = new Ray();
	reflected->origin = dg->point;
	double cosTheta = DotProduct( dg->normal, incident.direction );	
	reflected->direction = incident.direction - 2.0 * dg->normal * cosTheta; 
	
	double alfa = 0.0;
	double ro = 0.0;
 
	if ( m_distribution.getValue() == 0 )
	{
		alfa = tgc::TwoPi * rand.RandomDouble();
		ro = m_sigmaOpt * rand.RandomDouble(); 
	 }
	 else if (m_distribution.getValue() == 1 )
	 {
		alfa = tgc::Pi * rand.RandomDouble();
		ro = m_sigmaOpt * tgf::AlternateBoxMuller( rand );
	 }
	 
	//Compute unit vector s
	Vector3D s = CrossProduct( dg->normal, reflected->direction );
	Vector3D t;
	if( s.Length() != 0 )
	{
		s = Normalize( s );
		
	 	//Compute unit vector t
	 	t = Normalize( CrossProduct( s, dg->normal ) ); 
	}
	else
	{
		//Si el rayo reflejado y la normal son colineales dpdu y dpdv son 
		//perpendiculares al rayo reflejado
		s = Normalize( dg->dpdu );
		t = Normalize( dg->dpdv );	
			
	}

	 reflected->direction = Normalize( reflected->direction );
	 
	 return reflected; 		
}
