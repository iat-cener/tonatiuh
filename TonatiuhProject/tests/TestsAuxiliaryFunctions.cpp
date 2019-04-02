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


#include <stdlib.h>
#include <time.h>

#include "BBox.h"
#include "Ray.h"

#include "TestsAuxiliaryFunctions.h"

double taf::randomNumber( double a, double b )
{
	return (b-a)*( (rand() / double(RAND_MAX))  ) + a;
}
//Esta funcion sirve para calcualr el n� aletorio necesario
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
	double azimuth = randomNumber( 0, gc::TwoPi );
	double zenithAngle = randomNumber( 0, gc::Pi );
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
