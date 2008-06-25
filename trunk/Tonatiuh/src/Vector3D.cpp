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
 
#include <cmath>
#include <float.h>

#include "Vector3D.h"
#include "Point3D.h"
#include "Normal.h"

Vector3D::Vector3D( double dx, double dy, double dz )
: x(dx), y(dy), z(dz)
{
}

Vector3D::Vector3D( const Normal& norm )
: x(norm.x), y(norm.y), z(norm.z)
{
}

Vector3D::Vector3D( Point3D point )
: x(point.x), y(point.y), z(point.z)
{
}

Vector3D::~Vector3D( )
{
}

Vector3D& Vector3D::operator=( const Vector3D& vector )
{
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

Vector3D& Vector3D::operator/=( double scalar )
{
    double inv = 1.0/scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

void Vector3D::zero()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

bool Vector3D::operator==( const Vector3D& vector ) const
{
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
	if( this == &vector ) return false;
    else return ( x != vector.x ) || ( y != vector.y ) || ( z != vector.z );
}

double Vector3D::operator[]( int i ) const
{
    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

double& Vector3D::operator[]( int i )
{
    if( i == 0 ) return x;
    else return (i == 1) ? y : z;
}

double Vector3D::LengthSquared( ) const
{
    return( x*x + y*y + z*z );
}

double Vector3D::Length( ) const
{
    return std::sqrt( LengthSquared( ) );
}

Vector3D operator-( const Vector3D& vector )
{
    return Vector3D( -vector.x, -vector.y, -vector.z );
}

Vector3D operator+( const Vector3D& lhs, const Vector3D& rhs )
{
    return Vector3D( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

Vector3D operator-( const Vector3D& lhs, const Vector3D& rhs )
{
    return Vector3D( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

Vector3D operator*( const Vector3D& vector, double scalar )
{
    return Vector3D( vector.x*scalar, vector.y*scalar, vector.z*scalar );
}

Vector3D operator*( double scalar, const Vector3D& vector )
{
    return Vector3D( vector.x*scalar, vector.y*scalar, vector.z*scalar );
}

Vector3D operator/( const Vector3D& vector, double scalar )
{
    double inv = 1.0/scalar;
    return Vector3D( vector.x*inv, vector.y*inv, vector.z*inv );
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

double DotProduct( const Vector3D& vA, const Normal& nB )
{
    return vA.x*nB.x + vA.y*nB.y + vA.z*nB.z;
}

double DotProduct( const Normal& nA, const Vector3D& vB )
{
    return nA.x*vB.x + nA.y*vB.y + nA.z*vB.z;
}

double AbsDotProduct( const Vector3D& vA, const Vector3D& vB )
{
    return fabs( DotProduct( vA, vB ) );
}

double AbsDotProduct( const Vector3D& vA, const Normal& nB )
{
    return fabs( DotProduct( vA, nB ) );
}

double AbsDotProduct( const Normal& nA, const Vector3D& vB )
{
    return fabs( DotProduct( nA, vB ) );
}

Vector3D CrossProduct( const Vector3D& vA, const Vector3D& vB )
{
    return Vector3D( ( vA.y*vB.z ) - ( vA.z*vB.y ),
                     ( vA.z*vB.x ) - ( vA.x*vB.z ),
                     ( vA.x*vB.y ) - ( vA.y*vB.x ) );
}

Vector3D CrossProduct( const Vector3D& vA, const Normal& nB )
{
    return Vector3D( ( vA.y*nB.z ) - ( vA.z*nB.y ),
                     ( vA.z*nB.x ) - ( vA.x*nB.z ),
                     ( vA.x*nB.y ) - ( vA.y*nB.x ) );
}

Vector3D CrossProduct( const Normal& nA, const Vector3D& vB )
{
    return Vector3D( ( nA.y*vB.z ) - ( nA.z*vB.y ),
                     ( nA.z*vB.x ) - ( nA.x*vB.z ),
                     ( nA.x*vB.y ) - ( nA.y*vB.x ) );
}

Vector3D Normalize( const Vector3D& vA )
{
    return vA / vA.Length();
}

bool SameHemisphere( const Vector3D& vA, const Vector3D& vB ) 
{
	return ( vA.z * vB.z > 0.0 );
}
