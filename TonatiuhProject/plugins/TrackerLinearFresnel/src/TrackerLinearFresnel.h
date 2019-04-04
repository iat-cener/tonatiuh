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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
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
