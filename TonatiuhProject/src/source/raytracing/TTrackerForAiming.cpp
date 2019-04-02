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

#include "TTrackerForAiming.h"
#include "TSceneKit.h"
#include "Transform.h"
#include "trf.h"


SO_NODEENGINE_ABSTRACT_SOURCE( TTrackerForAiming );

void TTrackerForAiming::initClass()
{


	SO_NODEENGINE_INIT_ABSTRACT_CLASS( TTrackerForAiming, TTracker, "TTracker" );
}

TTrackerForAiming::TTrackerForAiming()
{

}


TTrackerForAiming::~TTrackerForAiming()
{

}

void TTrackerForAiming::SetAimingPointRelativity(bool relative)
{
	if ((typeOfAimingPoint.getValue() == 1) != relative)
	{		
		if (relative)
		{
			typeOfAimingPoint.setValue(1);
		}
		else
		{
			typeOfAimingPoint.setValue(0);
		}
	}
}


void TTrackerForAiming::updateTypeOfAimingPoint( void* data, SoSensor* )
{
	TTrackerForAiming* tracker = (TTrackerForAiming *) data;
	tracker->SwitchAimingPointType();
}
