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

#include <cmath>
#include <float.h>

#include "NormalVector.h"
#include "Point3D.h"
#include "Trace.h"
#include "Vector3D.h"

Vector3D::Vector3D( double dx, double dy, double dz )
: x(dx), y(dy), z(dz)
{
	Trace trace( "Vector3D::Vector3D", false );
}

Vector3D::Vector3D( const NormalVector& norm )
: x(norm.x), y(norm.y), z(norm.z)
{
	Trace trace( "Vector3D::Vector3D", false );
}

Vector3D::Vector3D( Point3D point )
: x(point.x), y(point.y), z(point.z)
{
	Trace trace( "Vector3D::Vector3D", false );
}

Vector3D::~Vector3D( )
{
	Trace trace( "Vector3D::Vector3D", false );
}

Vector3D& Vector3D::operator=( const Vector3D& vector )
{
	Trace trace( "Vector3D::operator=", false );

    if( this != &vector )
    {
        x = vector.x;
        y = vector.y;
        z = vector.z;
    }
    return *this;
}

Vector3D& Vector3D::operator+=( const Vector3D& vector )
{
	Trace trace( "Vector3D::operator+=", false );

    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

Vector3D& Vector3D::operator-=( const Vector3D& vector )
{
	Trace trace( "Vector3D::operator-=", false );

    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

Vector3D& Vector3D::operator*=( double scalar )
{
	Trace trace( "Vector3D::operator*=", false );
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3D& Vector3D::operator/=( double scalar )
{
	Trace trace( "Vector3D::operator/=", false );

    double inv = 1.0/scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

void Vector3D::zero()
{
	Trace trace( "Vector3D::zero", false );

    x = 0.0;
    y = 0.0;
    z = 0.0;
}

bool Vector3D::operator==( const Vector3D& vector ) const
{
	Trace trace( "Vector3D::operator==", false );

	if( this == &vector )
    	return true;
    else
    return(
    	( fabs(x - vector.x) < DBL_EPSILON ) &&
    	( fabs(y - vector.y) < DBL_EPSILON ) &&
    	( fabs(z - vector.z) < DBL_EPSILON ) );
}

bool Vector3D::operator!=( const Vector3D& vector ) const
{
	Trace trace( "Vector3D::operator!=", false );

	if( this == &vector ) return false;
    else return ( x != vector.x ) || ( y != vector.y ) || ( z != vector.z );
}

double Vector3D::operator[]( int i ) const
{
	Trace trace( "Vector3D::operator[]", false );

    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

double& Vector3D::operator[]( int i )
{
	Trace trace( "Vector3D::operator[]", false );

    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

double Vector3D::LengthSquared( ) const
{
	Trace trace( "Vector3D::LengthSquared", false );
    return( x*x + y*y + z*z );
}

double Vector3D::Length( ) const
{
	Trace trace( "Vector3D::Length", false );
    return std::sqrt( LengthSquared( ) );
}

Vector3D operator-( const Vector3D& vector )
{
	Trace trace( "Vector3D operator+", false );
    return Vector3D( -vector.x, -vector.y, -vector.z );
}

Vector3D operator+( const Vector3D& lhs, const Vector3D& rhs )
{
	Trace trace( "Vector3D operator+", false );
    return Vector3D( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

Vector3D operator-( const Vector3D& lhs, const Vector3D& rhs )
{
	Trace trace( "Vector3D operator-", false );
    return Vector3D( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

Vector3D operator*( const Vector3D& vector, double scalar )
{
	Trace trace( "Vector3D operator*", false );
    return Vector3D( vector.x*scalar, vector.y*scalar, vector.z*scalar );
}

Vector3D operator*( double scalar, const Vector3D& vector )
{
	Trace trace( "Vector3D operator*", false );
    return Vector3D( vector.x*scalar, vector.y*scalar, vector.z*scalar );
}

Vector3D operator/( const Vector3D& vector, double scalar )
{
	Trace trace( "Vector3D operator/", false );

    double inv = 1.0/scalar;
    return Vector3D( vector.x*inv, vector.y*inv, vector.z*inv );
}

std::ostream& operator<<( std::ostream& os, const Vector3D& vector )
{
	Trace trace( "Vector3D operator<<", false );

    os << vector.x << ", " << vector.y << ", " << vector.z;
    return os;
}

double DotProduct( const Vector3D& vA, const Vector3D& vB )
{
	Trace trace( "Vector3D DotProduct", false );
    return vA.x*vB.x + vA.y*vB.y + vA.z*vB.z;
}

double DotProduct( const Vector3D& vA, const NormalVector& nB )
{
	Trace trace( "Vector3D DotProduct", false );
    return vA.x*nB.x + vA.y*nB.y + vA.z*nB.z;
}

double DotProduct( const NormalVector& nA, const Vector3D& vB )
{
	Trace trace( "Vector3D DotProduct", false );
    return nA.x*vB.x + nA.y*vB.y + nA.z*vB.z;
}

double AbsDotProduct( const Vector3D& vA, const Vector3D& vB )
{
	Trace trace( "Vector3D AbsDotProduct", false );
    return fabs( DotProduct( vA, vB ) );
}

double AbsDotProduct( const Vector3D& vA, const NormalVector& nB )
{
	Trace trace( "Vector3D AbsDotProduct", false );
    return fabs( DotProduct( vA, nB ) );
}

double AbsDotProduct( const NormalVector& nA, const Vector3D& vB )
{
	Trace trace( "Vector3D AbsDotProduct", false );
    return fabs( DotProduct( nA, vB ) );
}

Vector3D CrossProduct( const Vector3D& vA, const Vector3D& vB )
{
	Trace trace( "Vector3D CrossProduct", false );

    return Vector3D( ( vA.y*vB.z ) - ( vA.z*vB.y ),
                     ( vA.z*vB.x ) - ( vA.x*vB.z ),
                     ( vA.x*vB.y ) - ( vA.y*vB.x ) );
}

Vector3D CrossProduct( const Vector3D& vA, const NormalVector& nB )
{
	Trace trace( "Vector3D CrossProduct", false );

    return Vector3D( ( vA.y*nB.z ) - ( vA.z*nB.y ),
                     ( vA.z*nB.x ) - ( vA.x*nB.z ),
                     ( vA.x*nB.y ) - ( vA.y*nB.x ) );
}

Vector3D CrossProduct( const NormalVector& nA, const Vector3D& vB )
{
	Trace trace( "Vector3D CrossProduct", false );

    return Vector3D( ( nA.y*vB.z ) - ( nA.z*vB.y ),
                     ( nA.z*vB.x ) - ( nA.x*vB.z ),
                     ( nA.x*vB.y ) - ( nA.y*vB.x ) );
}

Vector3D Normalize( const Vector3D& vA )
{
	Trace trace( "Vector3D Normalize", false );
    return vA / vA.Length();
}

bool SameHemisphere( const Vector3D& vA, const Vector3D& vB )
{
	Trace trace( "Vector3D SameHemisphere", false );
	return ( vA.z * vB.z > 0.0 );
}
