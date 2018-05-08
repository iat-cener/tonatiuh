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

#ifndef PHOTON_H_
#define PHOTON_H_

#include <string>

#include "RayTracingLibrary.h"
#include "Point3D.h"
#include "Vector3D.h"


//!  Photon is the structure that stores the information of the rays intersections in the surface.
/*!
  Photon structure provides the information about the ray intersection:
  - intersection point (into global and local to surface coordinates system)
  - the surface url where the intersection has been. Empty if this information is not available or if this photons is not generated due to an intersection with a surface.
  - surface side:
  	  * 0: back (back and front, depends of the surface)
  	  * 1: front
  - the ray has been absorbed or not in the photon intersection point:
    * -1: this information is not available
    * 0: the ray has not been absorbed.
    * 1: the ray has been absorbed.
  - ray direction: incident ray direction. Null vector if this information is not available.
*/

struct RAYTRACING_API  Photon
{
	Photon( );
	Photon( const Photon& photon );
	Photon( Point3D posW,  Point3D posL, int side, double id = 0, std::string intersectedSurfaceURL = std::string( "" ), int absorbedPhoton = 0, Vector3D rayDir = Vector3D() );
	~Photon();

	double id;
	Point3D posWorld; //global
	Point3D posLocal; //local
	std::string intersectedSurfaceURL;
	int side;
	int isAbsorbed;
	Vector3D rayDir;

};

#endif /*PHOTON_H_*/
