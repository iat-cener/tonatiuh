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

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>

struct Point3D;
struct NormalVector;

struct Vector3D
{
    Vector3D( double dx = 0.0, double dy = 0.0, double dz = 0.0 );
    Vector3D( const NormalVector& norm );
    explicit Vector3D( const Point3D& point );
    ~Vector3D( );
    Vector3D& operator+=( const Vector3D& vector );
    Vector3D operator+( const Vector3D& vector ) const;
    Vector3D& operator-=( const Vector3D& vector );
    Vector3D operator-( const Vector3D& vector ) const;

    Vector3D& operator*=( double scalar );
    Vector3D operator*( double scalar ) const;
    Vector3D& operator/=( double scalar );
    Vector3D operator/( double scalar ) const;

    Vector3D operator-() const;

    bool operator==( const Vector3D& vector ) const;
    bool operator!=( const Vector3D& vector ) const;
    double operator[]( int i ) const;
    double& operator[]( int i );

    void zero();
    double lengthSquared( ) const;
    double length( ) const;

    double x;
    double y;
    double z;
};

Vector3D operator*( double scalar, const Vector3D& vector );
std::ostream& operator<<( std::ostream& os, const Vector3D& vector );
double DotProduct( const Vector3D& vA, const Vector3D& vB );
double DotProduct( const Vector3D& vA, const NormalVector& nB );
double DotProduct( const NormalVector& nA, const Vector3D& vB );
double AbsDotProduct( const Vector3D& vA, const Vector3D& vB );
double AbsDotProduct( const Vector3D& vA, const NormalVector& nB );
double AbsDotProduct( const NormalVector& nA, const Vector3D& vB );
Vector3D CrossProduct( const Vector3D& vA, const Vector3D& vB );
Vector3D CrossProduct( const Vector3D& vA, const NormalVector& nB );
Vector3D CrossProduct( const NormalVector& nA, const Vector3D& vB );
Vector3D Normalize( const Vector3D& vA );
bool SameHemisphere( const Vector3D& vA, const Vector3D& vB );

#endif
