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
 
#ifndef POINT3D_H
#define POINT3D_H

#include <iostream>

struct Vector3D;

struct Point3D
{
    Point3D( double dx = 0.0, double dy = 0.0, double dz = 0.0 );
    ~Point3D( );
    Point3D& operator+=( const Point3D& point );
    Point3D& operator+=( const Vector3D& vector );   
    Point3D& operator-=( const Vector3D& vector );
    Point3D& operator*=( double scalar );
    Point3D& operator/=( double scalar );
    bool operator==( const Point3D& point ) const;    
    bool operator!=( const Point3D& point ) const;
    double operator[]( int i ) const;
    double& operator[]( int i );
    
    double x;
    double y;
    double z;   
};

Point3D operator+( const Point3D& lhs, const Point3D& rhs );                
Point3D operator+( const Point3D& lhs, const Vector3D& rhs );
Vector3D operator-( const Point3D& lhs, const Point3D& rhs );
Point3D operator-( const Point3D& lhs, const Vector3D& rhs );
Point3D operator*( const Point3D& point, double scalar );
Point3D operator*( double scalar, const Point3D& point );
Point3D operator/( const Point3D& point, double scalar );
std::ostream &operator<<( std::ostream& os, const Point3D& point );
double Distance( const Point3D& lhs, const Point3D& rhs );
double DistanceSquared( const Point3D& lhs, const Point3D& rhs );

#endif
