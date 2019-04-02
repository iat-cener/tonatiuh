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

#ifndef TSUNSHAPE_H_
#define TSUNSHAPE_H_

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFDouble.h>

#include "Vector3D.h"
#include "RandomDeviate.h"

class TSunShape : public SoNode
{
	  typedef SoNode inherited;

	SO_NODE_ABSTRACT_HEADER(TSunShape);

public:
    static void initClass();

	virtual void GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const = 0;
	virtual double GetIrradiance() const = 0;
    virtual double GetThetaMax() const = 0;

protected:
    TSunShape();
    virtual ~TSunShape();
};

#endif /*TSUNSHAPE_H_*/
