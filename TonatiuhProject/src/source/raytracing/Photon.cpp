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

#include "Photon.h"

Photon::Photon( )
:id( -1 ), pos( Point3D()), side(-1 ), intersectedSurface(0 ), isAbsorbed( -1 )
{

}

Photon::Photon( const Photon& photon )
:id( photon.id ), pos( photon.pos ), side( photon.side ), intersectedSurface( photon.intersectedSurface ), isAbsorbed( photon.isAbsorbed )
{

}

Photon::Photon( Point3D pos, int side, double id, InstanceNode* intersectedSurface, int absorbedPhoton  )
:id(id), pos(pos), side( side ), intersectedSurface( intersectedSurface ), isAbsorbed( absorbedPhoton)
{

}

Photon::~Photon()
{
}
