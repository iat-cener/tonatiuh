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

#ifndef SUNSHAPEPILLBOX_H_
#define SUNSHAPEPILLBOX_H_

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFFloat.h>

#include "TSunShape.h"
#include "trt.h"


class SunshapePillbox : public TSunShape
{
	SO_NODE_HEADER(SunshapePillbox);

public:
    SunshapePillbox( );
    static void initClass();
	SoNode* copy( SbBool copyConnections ) const;

    //Sunshape Interface
    void GenerateRayDirection( Vector3D& direction, RandomDeviate& rand) const;
	double GetIrradiance() const;
    double GetThetaMax() const;

	trt::TONATIUH_REAL irradiance;
	trt::TONATIUH_REAL thetaMax;

protected:
	 ~SunshapePillbox();
};

#endif /*SUNSHAPEPILLBOX_H_*/

