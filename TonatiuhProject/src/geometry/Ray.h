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
 
#ifndef RAY_H
#define RAY_H

#include "GeometryLibrary.h"

#include "gc.h"
#include "Point3D.h"
#include "Vector3D.h"

class GEOMETRY_API Ray
{
public:
    Ray() : mint( gc::Epsilon ), maxt( gc::Infinity )
    {
    }

    Ray( const Point3D& orig, const Vector3D& direc, double start = gc::Epsilon, double end = gc::Infinity )
    : origin( orig ), mint( start ), maxt( end )
    {
    	setDirection( direc );
    }

    ~Ray( )
    {
    }

    Point3D operator()( double t ) const
    {
       return origin + m_direction * t;
    }

    const Vector3D& direction() const
    {
    	return m_direction;
    }

    const Vector3D& invDirection() const
    {
    	return m_invDirection;
    }

    void setDirection( const Vector3D& direction )
    {
    	m_direction = direction;
    	m_invDirection.x = 1.0/m_direction.x;
    	m_invDirection.y = 1.0/m_direction.y;
    	m_invDirection.z = 1.0/m_direction.z;
    }

    bool operator==( const Ray& ray ) const
    {
    	if( this == &ray ) return true;
    	return ( ( origin == ray.origin ) && ( m_direction == ray.m_direction ) &&
    		     !( fabs( mint - ray.mint) > gc::Epsilon ) && !( fabs( maxt - ray.maxt) > gc::Epsilon ) );
    }

    Point3D origin;
    mutable double mint;
    mutable double maxt;

private:
    Vector3D m_direction;
    Vector3D m_invDirection;
};


#endif
