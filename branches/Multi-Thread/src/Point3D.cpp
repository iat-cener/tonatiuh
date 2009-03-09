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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "Point3D.h"
#include "Trace.h"
#include "Vector3D.h"

Point3D::Point3D( double dx, double dy, double dz )
: x(dx), y(dy), z(dz)
{
	Trace trace( "Point3D::Point3D", false );
}

Point3D::~Point3D( )
{
	Trace trace( "Point3D::~Point3D", false );
}

Point3D& Point3D::operator+=( const Point3D& point )
{
	Trace trace( "Point3D::operator+=", false );

    x += point.x;
    y += point.y;
    z += point.z;
    return *this;
}

Point3D& Point3D::operator+=( const Vector3D& vector )
{
	Trace trace( "Point3D::operator+=", false );

    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

Point3D& Point3D::operator-=( const Vector3D& vector )
{
	Trace trace( "Point3D::Point3D", false );

    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

Point3D& Point3D::operator*=( double scalar )
{
	Trace trace( "Point3D::operator*=", false );

    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Point3D& Point3D::operator/=( double scalar )
{
	Trace trace( "Point3D::Point3D", false );

    double inv = 1.0/scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

bool Point3D::operator==( const Point3D& point ) const
{
	Trace trace( "Point3D::operator==", false );

	if( this == &point ) return true;
    else return( ( x == point.x ) && ( y == point.y ) && ( z == point.z ) );
}

bool Point3D::operator!=( const Point3D& point ) const
{
	Trace trace( "Point3D::operator!=", false );

	if( this == &point ) return false;
    else return ( x != point.x ) || ( y != point.y ) || ( z != point.z );
}

double Point3D::operator[]( int i ) const
{
	Trace trace( "Point3D::operator[]", false );

    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

double& Point3D::operator[]( int i )
{
	Trace trace( "Point3D::operator[]", false );

    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

Point3D operator+( const Point3D& lhs, const Point3D& rhs )
{
	Trace trace( "Point3D operator+", false );
    return Point3D( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

Point3D operator+( const Point3D& lhs, const Vector3D& rhs )
{
	Trace trace( "Point3D operator+", false );
    return Point3D( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

Vector3D operator-( const Point3D& lhs, const Point3D& rhs )
{
	Trace trace( "Point3D operator-", false );
    return Vector3D( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

Point3D operator-( const Point3D& lhs, const Vector3D& rhs )
{
	Trace trace( "Point3D operator-", false );
    return Point3D( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

Point3D operator*( const Point3D& point, double scalar )
{
	Trace trace( "Point3D operator*", false );
    return Point3D( point.x*scalar, point.y*scalar, point.z*scalar );
}

Point3D operator*( double scalar, const Point3D& point )
{
	Trace trace( "Point3D operator*", false );
    return Point3D( point.x*scalar, point.y*scalar, point.z*scalar );
}

Point3D operator/( const Point3D& point, double scalar )
{
	Trace trace( "Point3D operator/", false );

    double inv = 1.0/scalar;
    return Point3D( point.x*inv, point.y*inv, point.z*inv );
}

std::ostream &operator<<( std::ostream &os, const Point3D& pA )
{
	Trace trace( "Point3D operator<<", false );

    os << pA.x << ", " << pA.y << ", " << pA.z;
    return os;
}

double Distance( const Point3D& pA, const Point3D& pB )
{
	Trace trace( "Point3D Distance", false );
    return ( pA - pB ).Length();
}

double DistanceSquared( const Point3D& pA, const Point3D& pB )
{
	Trace trace( "Point3D DistanceSquared", false );
    return ( pA - pB ).LengthSquared();
}
