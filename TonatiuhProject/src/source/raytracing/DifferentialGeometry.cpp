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

#include "DifferentialGeometry.h"
#include "TShape.h"

DifferentialGeometry::DifferentialGeometry()
: u(0.0), v(0.0), pShape(0)
{

}

DifferentialGeometry::DifferentialGeometry( const Point3D& P, const Vector3D& DPDU,
			const Vector3D& DPDV, const Vector3D& DNDU,
			const Vector3D& DNDV, double uu, double vv,
			const TShape *sh )
: point(P), u(uu), v(vv), pShape(sh),
  dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV)
{
	normal = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
}

DifferentialGeometry::~DifferentialGeometry()
{

}
