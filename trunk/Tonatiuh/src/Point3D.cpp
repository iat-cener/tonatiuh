/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
 ***************************************************************************/
 
#include "Point3D.h"
#include "Vector3D.h"

Point3D::Point3D( double dx, double dy, double dz )
: x(dx), y(dy), z(dz)
{
}

Point3D::~Point3D( )
{
}

Point3D& Point3D::operator+=( const Point3D& point )
{
    x += point.x;
    y += point.y;
    z += point.z;
    return *this;
}
 
Point3D& Point3D::operator+=( const Vector3D& vector )
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

Point3D& Point3D::operator-=( const Vector3D& vector )
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

Point3D& Point3D::operator*=( double scalar )
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Point3D& Point3D::operator/=( double scalar )
{
    double inv = 1.0/scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;    
}

bool Point3D::operator==( const Point3D& point ) const
{
	if( this == &point ) return true;
    else return( ( x == point.x ) && ( y == point.y ) && ( z == point.z ) );
} 
  
bool Point3D::operator!=( const Point3D& point ) const
{
	if( this == &point ) return false;
    else return ( x != point.x ) || ( y != point.y ) || ( z != point.z );
}
    
double Point3D::operator[]( int i ) const
{
    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

double& Point3D::operator[]( int i )
{
    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

Point3D operator+( const Point3D& lhs, const Point3D& rhs )
{
    return Point3D( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

Point3D operator+( const Point3D& lhs, const Vector3D& rhs )
{
    return Point3D( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

Vector3D operator-( const Point3D& lhs, const Point3D& rhs )
{
    return Vector3D( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

Point3D operator-( const Point3D& lhs, const Vector3D& rhs )
{
    return Point3D( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

Point3D operator*( const Point3D& point, double scalar )
{
    return Point3D( point.x*scalar, point.y*scalar, point.z*scalar );
}

Point3D operator*( double scalar, const Point3D& point )
{
    return Point3D( point.x*scalar, point.y*scalar, point.z*scalar );
}

Point3D operator/( const Point3D& point, double scalar )
{
    double inv = 1.0/scalar;
    return Point3D( point.x*inv, point.y*inv, point.z*inv );
}

std::ostream &operator<<( std::ostream &os, const Point3D& pA )
{
    os << pA.x << ", " << pA.y << ", " << pA.z;
    return os;
}

double Distance( const Point3D& pA, const Point3D& pB )
{
    return ( pA - pB ).Length();
}

double DistanceSquared( const Point3D& pA, const Point3D& pB )
{
    return ( pA - pB ).LengthSquared();
}
