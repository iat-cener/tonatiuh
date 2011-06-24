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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <Inventor/SbVec3f.h>
#include <cmath>
#include <float.h>
#include <iostream>

#include "NormalVector.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Vector3D.h"

Vector3D::Vector3D( double dx, double dy, double dz )
: x(dx), y(dy), z(dz)
{
}

Vector3D::Vector3D( const NormalVector& norm )
: x(norm.x), y(norm.y), z(norm.z)
{
}

Vector3D::Vector3D( const Point3D& point )
: x(point.x), y(point.y), z(point.z)
{
}
Vector3D::Vector3D( const SbVec3f& vec )
: x(vec[0]), y(vec[1]), z(vec[2])
{
}



Vector3D::~Vector3D( )
{
}

SbVec3f Vector3D::ToSbVec3f()
{
	return SbVec3f(float(x),float(y),float(z));
}

Vector3D& Vector3D::operator+=( const Vector3D& vector )
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

Vector3D& Vector3D::operator-=( const Vector3D& vector )
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

Vector3D& Vector3D::operator*=( double scalar )
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3D Vector3D::operator*( double scalar ) const
{
    return Vector3D( x * scalar, y * scalar, z * scalar );
}

Vector3D& Vector3D::operator/=( double scalar )
{
    double inv = 1.0/scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

Vector3D Vector3D::operator/( double scalar ) const
{
    double inv = 1.0/scalar;
    return Vector3D( x * inv, y * inv, z * inv );
}

Vector3D Vector3D::operator-() const
{
    return Vector3D( -x, -y, -z );
}

bool Vector3D::operator==( const Vector3D& vector ) const
{
	if( this == &vector ) return true;
    else return( ( fabs(x - vector.x) < DBL_EPSILON ) &&
				 ( fabs(y - vector.y) < DBL_EPSILON ) &&
				 ( fabs(z - vector.z) < DBL_EPSILON ) );
}

bool Vector3D::operator!=( const Vector3D& vector ) const
{
	return !( *this == vector );
}

double Vector3D::operator[]( int i ) const
{
    if( i == 0 ) return x;
    if( i == 1 ) return y;
    return z;
}

double& Vector3D::operator[]( int i )
{
    if( i == 0 ) return x;
    if( i == 1 ) return y;
    return z;
}

void Vector3D::zero()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

double Vector3D::lengthSquared( ) const
{
    return( x*x + y*y + z*z );
}

double Vector3D::length( ) const
{
	return std::sqrt( x*x + y*y + z*z );
}

Vector3D operator+( Vector3D lhs, const Vector3D& rhs )
{
	//lhs take by value to let the compile to make the copy
	return lhs += rhs;
}

Vector3D operator-( Vector3D lhs, const Vector3D& rhs )
{
	//lhs take by value to let the compile to make the copy
	return lhs -= rhs;
}

Vector3D operator*( double scalar, const Vector3D& vector )
{
    return Vector3D( scalar * vector.x, scalar * vector.y, scalar * vector.z );
}

std::ostream& operator<<( std::ostream& os, const Vector3D& vector )
{
    os << vector.x << ", " << vector.y << ", " << vector.z;
    return os;
}

double DotProduct( const Vector3D& vA, const Vector3D& vB )
{
    return vA.x*vB.x + vA.y*vB.y + vA.z*vB.z;
}

double DotProduct( const Vector3D& vA, const NormalVector& nB )
{
    return vA.x*nB.x + vA.y*nB.y + vA.z*nB.z;
}

double DotProduct( const NormalVector& nA, const Vector3D& vB )
{
    return nA.x*vB.x + nA.y*vB.y + nA.z*vB.z;
}

double AbsDotProduct( const Vector3D& vA, const Vector3D& vB )
{
    return fabs( DotProduct( vA, vB ) );
}

double AbsDotProduct( const Vector3D& vA, const NormalVector& nB )
{
    return fabs( DotProduct( vA, nB ) );
}

double AbsDotProduct( const NormalVector& nA, const Vector3D& vB )
{
    return fabs( DotProduct( nA, vB ) );
}

Vector3D CrossProduct( const Vector3D& vA, const Vector3D& vB )
{
    return Vector3D( ( vA.y*vB.z ) - ( vA.z*vB.y ),
                     ( vA.z*vB.x ) - ( vA.x*vB.z ),
                     ( vA.x*vB.y ) - ( vA.y*vB.x ) );
}

Vector3D CrossProduct( const Vector3D& vA, const NormalVector& nB )
{
    return Vector3D( ( vA.y*nB.z ) - ( vA.z*nB.y ),
                     ( vA.z*nB.x ) - ( vA.x*nB.z ),
                     ( vA.x*nB.y ) - ( vA.y*nB.x ) );
}

Vector3D CrossProduct( const NormalVector& nA, const Vector3D& vB )
{
    return Vector3D( ( nA.y*vB.z ) - ( nA.z*vB.y ),
                     ( nA.z*vB.x ) - ( nA.x*vB.z ),
                     ( nA.x*vB.y ) - ( nA.y*vB.x ) );
}

Vector3D Normalize( const Vector3D& vA )
{
	if (vA.length() > 0.0f) return vA / vA.length();
	return vA;
}

bool SameHemisphere( const Vector3D& vA, const Vector3D& vB )
{
	return ( vA.z * vB.z > 0.0 );
}
