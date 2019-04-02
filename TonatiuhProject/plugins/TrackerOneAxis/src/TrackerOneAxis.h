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
 
#ifndef TRACKERONEAXIS_H_
#define TRACKERONEAXIS_H_

#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/fields/SoSFVec3f.h>

#include "TTracker.h"

class QString; 

class TrackerOneAxis : public TTracker
{
	SO_NODEENGINE_HEADER( TrackerOneAxis );
	
public:
    static void initClass();
	QString getIcon();
	
	//Constructor
	TrackerOneAxis();

	void Evaluate( Vector3D sunVectorW, Transform parentWT0 );

protected:	
	virtual ~TrackerOneAxis();

private:
  virtual void evaluate();
    
};

#endif /*TRACKERONEAXIS_H_*/
