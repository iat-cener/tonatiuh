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


#ifndef GRAPHICROOTTRACKER_H_
#define GRAPHICROOTTRACKER_H_


#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/fields/SoSFVec3f.h>

#include "TTracker.h"
#include "trt.h"

class QString;

class GraphicRootTracker : public TTracker
{
	SO_NODEENGINE_HEADER( GraphicRootTracker );

public:
    static void initClass();
	QString getIcon();

	//Constructor
	GraphicRootTracker();

	void Disconnect();
	void SetAzimuthAngle( trt::TONATIUH_REAL* azimuthField );
	void SetZenithAngle( trt::TONATIUH_REAL* zenithField );

protected:
	virtual ~GraphicRootTracker();

private:
	virtual void evaluate();

};

#endif /* GRAPHICROOTTRACKER_H_ */
