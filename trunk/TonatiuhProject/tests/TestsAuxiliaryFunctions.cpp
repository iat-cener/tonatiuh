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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/
#include <stdlib.h>
#include <time.h>

#include "tgc.h"
#include "TestsAuxiliaryFunctions.h"
#include "BBox.h"
#include "Ray.h"

double taf::randomNumber( double a, double b )
{
	return (b-a)*( (rand() / double(RAND_MAX))  ) + a;
}
//Esta funcion sirve para calcualr el nº aletorio necesario
//para hacer el test de la funcion inverse en la clase Matrix4x4Tests.cpp
double taf::randomBBoxNumber( double a, double b )
{
	return 1+(b-a)*( (rand() / double(RAND_MAX))  ) + a;
}
Point3D taf::randomPoint( double a, double b  )
{
	double x = randomNumber( a, b );
	double y = randomNumber( a, b );
	double z = randomNumber( a, b );
	return Point3D( x, y, z);
}

BBox taf::randomBox( double a, double b )
{
	Point3D point1 = randomPoint( a, b );
	Point3D point2 = randomPoint( a, b );
    return BBox( point1, point2 );
}

Vector3D taf::randomDirection( )
{
	double azimuth = randomNumber( 0, tgc::TwoPi );
	double zenithAngle = randomNumber( 0, tgc::Pi );
	double SinZenithAngle = sin( zenithAngle );
	double x = SinZenithAngle * cos( azimuth );
	double y = SinZenithAngle * sin( azimuth );
	double z = cos( zenithAngle );
	return Normalize( Vector3D( x, y, z) );
}

Ray taf::randomRay( double a, double b )
{
	return Ray( randomPoint(a, b), randomDirection() );
}
