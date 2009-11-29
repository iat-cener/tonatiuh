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

Developers: Manuel Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "Point3D.h"
#include "Vector3D.h"

Point3D::Point3D( double dx, double dy, double dz )
: x(dx), y(dy), z(dz)
{
}

Point3D::Point3D( const Vector3D& vector )
: x(vector.x), y(vector.y), z(vector.z)
{
}

Point3D::~Point3D( )
{
}

Point3D& Point3D::operator+=( const Vector3D& vector )
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

Point3D Point3D::operator+( const Vector3D& vector ) const
{
	return Point3D( x + vector.x, y + vector.y, z + vector.z );
}

Point3D& Point3D::operator-=( const Vector3D& vector )
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

Point3D Point3D::operator-( const Vector3D& vector ) const
{
	return Point3D( x - vector.x, y - vector.y, z - vector.z );
}

Point3D& Point3D::operator*=( double scalar )
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Point3D Point3D::operator*( double scalar ) const
{
	return Point3D( scalar * x, scalar * y, scalar * z );
}

Point3D& Point3D::operator/=( double scalar )
{
    double inv = 1.0/scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

Point3D Point3D::operator/( double scalar ) const
{
    double inv = 1.0/scalar;
    return Point3D( inv * x, inv * y, inv * z );
}

Vector3D Point3D::operator-( const Point3D& point ) const
{
	return Vector3D( x - point.x, y - point.y, z - point.z );
}


bool Point3D::operator==( const Point3D& point ) const
{
	if( this == &point ) return true;
    else return( ( x == point.x ) && ( y == point.y ) && ( z == point.z ) );
}

bool Point3D::operator!=( const Point3D& point ) const
{
	if( this == &point ) return false;
    else return ( x != point.x ) || ( y != point.y ) || ( z != point.z );
}

double Point3D::operator[]( int i ) const
{
    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

double& Point3D::operator[]( int i )
{
    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

std::ostream &operator<<( std::ostream &os, const Point3D& pA )
{
    os << pA.x << ", " << pA.y << ", " << pA.z;
    return os;
}

double distance( const Point3D& pointA, const Point3D& pointB )
{
    return ( pointA - pointB ).Length();
}

double distanceSquared( const Point3D& pointA, const Point3D& pointB )
{
    return ( pointA - pointB ).LengthSquared();
}

