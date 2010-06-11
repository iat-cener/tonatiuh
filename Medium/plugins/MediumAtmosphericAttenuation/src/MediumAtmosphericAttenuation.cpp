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

#include <QIcon>

#include <Inventor/sensors/SoFieldSensor.h>

#include "MediumAtmosphericAttenuation.h"
#include "RandomDeviate.h"
#include "Ray.h"

SO_NODE_SOURCE(MediumAtmosphericAttenuation);


void MediumAtmosphericAttenuation::initClass()
{
	 SO_NODE_INIT_CLASS( MediumAtmosphericAttenuation, TMedium, "Medium" );
}

MediumAtmosphericAttenuation::MediumAtmosphericAttenuation()
{
	SO_NODE_CONSTRUCTOR( MediumAtmosphericAttenuation );


	SO_NODE_ADD_FIELD( atm1, (0.679) );
	SO_NODE_ADD_FIELD( atm2, (11.76) );
	SO_NODE_ADD_FIELD( atm3, (-1.97) );

	SO_NODE_ADD_FIELD( mediumAmbientIntensity, (0.2) );
	SO_NODE_ADD_FIELD( mediumAmbientColor, ( 1.0, 1.0, 1.0 ) );

	SO_NODE_DEFINE_ENUM_VALUE(FogType, NONE);
	SO_NODE_DEFINE_ENUM_VALUE(FogType, HAZE);
	SO_NODE_DEFINE_ENUM_VALUE(FogType, FOG);
	SO_NODE_DEFINE_ENUM_VALUE(FogType, SMOKE);
	SO_NODE_SET_SF_ENUM_TYPE(mediutmFogType, FogType);

	SO_NODE_ADD_FIELD( mediutmFogType, ( NONE) );
	SO_NODE_ADD_FIELD( mediumFogColor, ( 1, 1, 1 ) );
	SO_NODE_ADD_FIELD( mediumFogVisibility, (10.0) );


	SoFieldSensor* atm1Sensor = new SoFieldSensor(updateATM, this);
	atm1Sensor->attach( &atm1 );
	SoFieldSensor* atm2Sensor = new SoFieldSensor(updateATM, this);
	atm2Sensor->attach( &atm2 );
	SoFieldSensor* atm3Sensor = new SoFieldSensor(updateATM, this);
	atm3Sensor->attach( &atm3 );


	SoFieldSensor* mediumAmbientIntensitySensor = new SoFieldSensor( updateAmbientIntensity, this );
	mediumAmbientIntensitySensor->attach( &mediumAmbientIntensity );
	SoFieldSensor* mediumAmbientColorSensor = new SoFieldSensor( updateAmbientColor, this );
	mediumAmbientColorSensor->attach( &mediumAmbientColor );

	SoFieldSensor* mediutmFogTypeSensor = new SoFieldSensor( updateFogType, this );
	mediutmFogTypeSensor->attach( &mediutmFogType );
	SoFieldSensor* mediumFogColorSensor = new SoFieldSensor( updateFogColor, this );
	mediumFogColorSensor->attach( &mediumFogColor );
	SoFieldSensor* mediumFogVisibilitySensor = new SoFieldSensor( updateFogVisibility, this );
	mediumFogVisibilitySensor->attach( &mediumFogVisibility );


}

MediumAtmosphericAttenuation::~MediumAtmosphericAttenuation()
{

}

QIcon MediumAtmosphericAttenuation::GetIcon() const
{
	return QIcon(":icons/MediumAtmosphericAttenuation.png");
}

QString MediumAtmosphericAttenuation::GetName() const
{
	return QString( "Atmospheric Attenuation");

}

bool MediumAtmosphericAttenuation::IsRayAbsorbed( const Ray& incident, RandomDeviate& rand  ) const
{
	double distAim = incident.maxt;
	double r = distAim / 1000;

	double percAttenuation=( atm1.getValue() + atm2.getValue() * r + atm3.getValue() * r * r ) / 100;
	double randomNumber = rand.RandomDouble();
	if ( randomNumber > percAttenuation  ) return true;

	return false;
}

void MediumAtmosphericAttenuation::updateATM(void *data, SoSensor *)
{
	MediumAtmosphericAttenuation* medium = (MediumAtmosphericAttenuation* ) data;
	medium->attenuation.setValue( medium->atm3.getValue(), medium->atm2.getValue(), medium->atm1.getValue() );
}

void MediumAtmosphericAttenuation::updateAmbientIntensity(void *data, SoSensor *)
{
	MediumAtmosphericAttenuation* medium = (MediumAtmosphericAttenuation* ) data;
	medium->ambientIntensity.setValue( medium->mediumAmbientIntensity.getValue() );
}

void MediumAtmosphericAttenuation::updateAmbientColor(void *data, SoSensor *)
{
	MediumAtmosphericAttenuation* medium = (MediumAtmosphericAttenuation* ) data;
	medium->ambientColor.setValue( medium->mediumAmbientColor.getValue() );
}

void MediumAtmosphericAttenuation::updateFogType(void *data, SoSensor *)
{
	MediumAtmosphericAttenuation* medium = (MediumAtmosphericAttenuation* ) data;
	medium->fogType.setValue( medium->mediutmFogType.getValue() );
}

void MediumAtmosphericAttenuation::updateFogColor(void *data, SoSensor *)
{
	MediumAtmosphericAttenuation* medium = (MediumAtmosphericAttenuation* ) data;
	medium->fogColor.setValue( medium->mediumFogColor.getValue() );
}

void MediumAtmosphericAttenuation::updateFogVisibility(void *data, SoSensor *)
{
	MediumAtmosphericAttenuation* medium = (MediumAtmosphericAttenuation* ) data;
	medium->fogVisibility.setValue( medium->mediumFogVisibility.getValue() );
}
