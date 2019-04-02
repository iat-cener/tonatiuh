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


#ifndef TRACKERLINEARFRESNEL_H_
#define TRACKERLINEARFRESNEL_H_

#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/fields/SoSFEnum.h>

#include "TTrackerForAiming.h"
#include "trt.h"

class QString;

class TrackerLinearFresnel : public TTrackerForAiming
{
	SO_NODEENGINE_HEADER( TrackerLinearFresnel );

public:
    static void initClass();
	QString getIcon();

	//Constructor
	TrackerLinearFresnel();
	void Evaluate( Vector3D sunVectorW, Transform parentWT0 );
	void SwitchAimingPointType();

	enum Axis{
		X = 0,
		Y = 1,
		Z = 2,
	};

	trt::TONATIUH_REALVECTOR2 axisOrigin;
	SoSFEnum activeAxis;

protected:
	virtual ~TrackerLinearFresnel();

private:
	virtual void evaluate();

	int m_previousAimingPointType;
	SoFieldSensor* m_infoDisplayed;

};

#endif /* TRACKERLINEARFRESNEL_H_ */
