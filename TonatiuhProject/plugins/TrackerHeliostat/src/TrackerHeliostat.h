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

#ifndef TRACKERHELIOSTAT_H_
#define TRACKERHELIOSTAT_H_


#include <Inventor/fields/SoSFEnum.h>

#include "TTrackerForAiming.h"

class QString;

class TrackerHeliostat : public TTrackerForAiming
{
	typedef TTrackerForAiming inherited;
	SO_NODEENGINE_HEADER( TrackerHeliostat );

public:
    static void initClass();
	QString getIcon();

	//Constructor
	TrackerHeliostat();

	void Evaluate( Vector3D sunVectorW, Transform parentWT0 );
	virtual void SwitchAimingPointType();

	enum Rotations{
		YX = 0,
		YZ   = 1,
		XZ   = 2,
		ZX   = 3
	};
	SoSFEnum typeOfRotation;
	trt::TONATIUH_REALVECTOR3 aimingPoint;

protected:
	virtual ~TrackerHeliostat();

private:
	virtual void evaluate();

	int m_previousAimingPointType;
	SoFieldSensor* m_infoDisplayed;



};

#endif /* TRACKERHELIOSTAT_H_ */
