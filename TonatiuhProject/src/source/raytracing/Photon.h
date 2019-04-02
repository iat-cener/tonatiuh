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

#ifndef PHOTON_H_
#define PHOTON_H_

#include "InstanceNode.h"
#include "Point3D.h"

struct Photon
{
	Photon( );
	Photon( const Photon& photon );
	Photon( Point3D pos, int side, double id = 0, InstanceNode* intersectedSurface = 0, int absorbedPhoton = 0 );
	~Photon();

	double id;
	Point3D pos;
	int side;
	InstanceNode* intersectedSurface;
	int isAbsorbed;
};

#endif /*PHOTON_H_*/
