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

#include "gc.h"
#include "Point3D.h"
#include "Vector3D.h"
#include <iostream>

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
	return Point3D( x * scalar, y * scalar, z * scalar );
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
    else return ( !( fabs(x - point.x) > gc::Epsilon ) &&
				  !( fabs(y - point.y) > gc::Epsilon ) &&
				  !( fabs(z - point.z) > gc::Epsilon ) );
}

bool Point3D::operator!=( const Point3D& point ) const
{
	if( this == &point ) return false;
    else return ( ( fabs(x - point.x) > gc::Epsilon ) ||
				  ( fabs(y - point.y) > gc::Epsilon ) ||
				  ( fabs(z - point.z) > gc::Epsilon ) );
}

double Point3D::operator[]( int i ) const
{
    if( i == 0 ) return x;
    if( i == 1 ) return y;
    return z;
}

double& Point3D::operator[]( int i )
{
    if( i == 0 ) return x;
    if( i == 1 ) return y;
    return z;
}

Point3D operator*( double scalar, const Point3D& point )
{
	return point * scalar;
}

std::ostream &operator<<( std::ostream &os, const Point3D& pA )
{
    os << pA.x << ", " << pA.y << ", " << pA.z;
    return os;
}

double Distance( const Point3D& pointA, const Point3D& pointB )
{
	return (pointA - pointB).length();
}

double DistanceSquared( const Point3D& pointA, const Point3D& pointB )
{
    return ( pointA - pointB ).lengthSquared();
}

