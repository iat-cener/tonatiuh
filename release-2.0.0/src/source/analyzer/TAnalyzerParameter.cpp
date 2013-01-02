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

#include "TAnalyzerParameter.h"
#include "TAnalyzerKit.h"
#include <Inventor/sensors/SoFieldSensor.h>

SO_NODE_SOURCE( TAnalyzerParameter );

void TAnalyzerParameter::initClass()
{
	SO_NODE_INIT_CLASS( TAnalyzerParameter, SoNode, "AnalyzerResult" );
}

TAnalyzerParameter::TAnalyzerParameter()
{
	SO_NODE_CONSTRUCTOR( TAnalyzerParameter );
	SO_NODE_ADD_FIELD( split, (1,1,1) );
	SO_NODE_ADD_FIELD( elemSize, (1.0) );

	SO_NODE_DEFINE_ENUM_VALUE( TypeInfoDisplayed, CURRENT_VALUE );
	SO_NODE_DEFINE_ENUM_VALUE( TypeInfoDisplayed, MIN_VALUE );
	SO_NODE_DEFINE_ENUM_VALUE( TypeInfoDisplayed, MAX_VALUE );
	SO_NODE_SET_SF_ENUM_TYPE( infoDisplayed, TypeInfoDisplayed );
	SO_NODE_ADD_FIELD( infoDisplayed, (CURRENT_VALUE) );

	theParent = NULL;
	SoFieldSensor* m_infoDisplayed = new SoFieldSensor( updateInfoDisplayed, this );
	m_infoDisplayed->attach( &infoDisplayed );
/*	SoFieldSensor* m_SizeChange = new SoFieldSensor( updateSize, this );
	m_SizeChange->attach( &split );
	SoFieldSensor* m_SizeChange2 = new SoFieldSensor( updateSize, this );
	m_SizeChange2->attach( &elemSize );*/
}

TAnalyzerParameter::~TAnalyzerParameter()
{
}

void TAnalyzerParameter::updateInfoDisplayed( void* data, SoSensor* )
{
	TAnalyzerParameter* parameter = static_cast< TAnalyzerParameter* >( data );
	if (parameter->theParent)
	{
		((TAnalyzerKit * ) parameter->theParent)->DisplayResults();
	}
}
/*
void TAnalyzerParameter::updateSize( void* data, SoSensor* )
{
	TAnalyzerParameter* parameter = static_cast< TAnalyzerParameter* >( data );
	if (parameter->theParent)
	{
		((TAnalyzerKit * ) parameter->theParent)->UpdateSize();
	}
}*/

void TAnalyzerParameter::setParent( SoNode * parent )
{
	theParent = parent;
}


