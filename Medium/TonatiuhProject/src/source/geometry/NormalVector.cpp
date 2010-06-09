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

#include <cmath>
#include <float.h>

#include "NormalVector.h"
#include "Vector3D.h"

NormalVector::NormalVector( double dx, double dy, double dz )
: x(dx), y(dy), z(dz)
{
}

NormalVector::NormalVector( const Vector3D& vector )
: x(vector.x), y(vector.y), z(vector.z)
{
}

NormalVector::~NormalVector( )
{
}

NormalVector& NormalVector::operator+=( const NormalVector& nV )
{
	x += nV.x;
    y += nV.y;
    z += nV.z;
    return *this;
}

NormalVector& NormalVector::operator-=( const NormalVector& nV )
{
    x -= nV.x;
    y -= nV.y;
    z -= nV.z;
    return *this;
}

NormalVector& NormalVector::operator*=( double scalar )
{
	x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

NormalVector NormalVector::operator*( double scalar ) const
{
	return NormalVector( x * scalar, y * scalar, z * scalar );
}

NormalVector& NormalVector::operator/=( double scalar )
{
	double inv = 1.0/scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

NormalVector NormalVector::operator/( double scalar ) const
{
	double inv = 1.0/scalar;
    return NormalVector( x * inv, y * inv, z * inv );
}

NormalVector NormalVector::operator-() const
{
	return NormalVector( -x, -y, -z );
}

bool NormalVector::operator==( const NormalVector& nV ) const
{
	if( this == &nV ) return true;
    else return( ( fabs(x - nV.x) < DBL_EPSILON ) &&
				 ( fabs(y - nV.y) < DBL_EPSILON ) &&
				 ( fabs(z - nV.z) < DBL_EPSILON ) );
}

bool NormalVector::operator!=( const NormalVector& nV ) const
{
	return !( *this == nV );
}

double NormalVector::operator[]( int i ) const
{
	if( i == 0 ) return x;
    else if( i == 1 ) return y;
    return z;
}

double& NormalVector::operator[]( int i )
{
	if( i == 0 ) return x;
    else if( i == 1 ) return y;
    return z;
}

double NormalVector::lengthSquared( ) const
{
	return( x*x + y*y + z*z );
}

double NormalVector::length( ) const
{
	return sqrt( x*x + y*y + z*z );
}

NormalVector operator+( NormalVector lhs, const NormalVector& rhs )
{
	// Note that lhs is taken by value
	return lhs += rhs;
}

NormalVector operator-( NormalVector lhs, const NormalVector& rhs )
{
	// Note that lhs is taken by value
	return lhs -= rhs;
}

NormalVector operator*( double scalar, const NormalVector& nV )
{
	return NormalVector( scalar * nV.x, scalar * nV.y, scalar * nV.z );
}

std::ostream& operator<<( std::ostream& os, const NormalVector& nV )
{
	os << nV.x << ", " << nV.y << ", " << nV.z;
    return os;
}

double DotProduct( const NormalVector& nA, const NormalVector& nB )
{
	return nA.x*nB.x + nA.y*nB.y + nA.z*nB.z;
}

double AbsDotProduct( const NormalVector& nA, const NormalVector& nB )
{
	return fabs( DotProduct( nA, nB ) );
}

NormalVector Normalize( const NormalVector& nV )
{
	return nV / nV.length();
}
