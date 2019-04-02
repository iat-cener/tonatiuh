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
 
#ifndef POINT3D_H
#define POINT3D_H

#include <iostream>
struct Vector3D;

struct Point3D
{
    Point3D( double dx = 0.0, double dy = 0.0, double dz = 0.0 );
    explicit Point3D ( const Vector3D& vector );
    ~Point3D( );

    Point3D& operator+=( const Vector3D& vector );
    Point3D operator+( const Vector3D& vector ) const;
    Point3D& operator-=( const Vector3D& vector );
    Point3D operator-( const Vector3D& vector ) const;
    Vector3D operator-( const Point3D& point ) const;

    Point3D& operator*=( double scalar );
    Point3D operator*( double scalar ) const;
    Point3D& operator/=( double scalar );
    Point3D operator/( double scalar ) const;

    bool operator==( const Point3D& point ) const;    
    bool operator!=( const Point3D& point ) const;
    double operator[]( int i ) const;
    double& operator[]( int i );

    double x;
    double y;
    double z;   
};

Point3D operator*( double scalar, const Point3D& point );
std::ostream &operator<<( std::ostream& os, const Point3D& point );
double Distance( const Point3D& pointA, const Point3D& pointB );
double DistanceSquared( const Point3D& pointA, const Point3D& pointB );

#endif
