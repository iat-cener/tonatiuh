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
 
#ifndef RAY_H
#define RAY_H

#include "gc.h"
#include "Point3D.h"
#include "Vector3D.h"

class Ray
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
