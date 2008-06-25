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
#include <cfloat>
#include "Normal.h"
#include "Vector3D.h"

Normal::Normal( double dx, double dy, double dz )
: x(dx), y(dy), z(dz)
{
}

Normal::Normal( const Vector3D& vector )
: x(vector.x), y(vector.y), z(vector.z)
{
}
	
Normal::~Normal( )
{
}
 
Normal& Normal::operator+=( const Normal& nRhs )
{
    x += nRhs.x;
    y += nRhs.y;
    z += nRhs.z;
    return *this;
}

Normal& Normal::operator-=( const Normal& nRhs )
{
    x -= nRhs.x;
    y -= nRhs.y;
    z -= nRhs.z;
    return *this;    
}

Normal& Normal::operator*=( double a )
{
    x *= a;
    y *= a;
    z *= a;
    return *this;
}

Normal& Normal::operator/=( double a )
{
    double inv = 1.0/a;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

bool Normal::operator==( const Normal& norm ) const
{
    if( this == &norm ) 
    	return true;
    else 
    return( 
    	( fabs(x - norm.x) < DBL_EPSILON ) && 
    	( fabs(y - norm.y) < DBL_EPSILON ) && 
    	( fabs(z - norm.z) < DBL_EPSILON ) );
}

double Normal::operator[]( int i ) const
{
    if( i == 0 ) return x;
    else if( i == 1 ) return y;
    return z;
}

double& Normal::operator[]( int i )
{
    if( i == 0 ) return x;
    else if( i == 1 ) return y;
    return z;
}
   
double Normal::LengthSquared( ) const
{
    return( x*x + y*y + z*z );
}

double Normal::Length( ) const
{
    return sqrt( LengthSquared( ) );
}

Normal operator-( const Normal& normal )
{
    return Normal( -normal.x, -normal.y, -normal.z );
}

Normal operator+( const Normal& lhs, const Normal& rhs )
{
    return Normal( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

Normal operator-( const Normal& lhs, const Normal& rhs )
{
    return Normal( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

Normal operator*( const Normal& normal, double scalar )
{
    return Normal( normal.x*scalar, normal.y*scalar, normal.z*scalar );
}

Normal operator*( double scalar, const Normal& normal )
{
    return Normal( normal.x*scalar, normal.y*scalar, normal.z*scalar );
}

Normal operator/( const Normal& normal, double scalar )
{
    double inv = 1.0/scalar;
    return Normal( normal.x*inv, normal.y*inv, normal.z*inv );
}

std::ostream& operator<<( std::ostream& os, const Normal& normal )
{
    os << normal.x << ", " << normal.y << ", " << normal.z;
    return os;
}

double DotProduct( const Normal& nA, const Normal& nB )
{
    return nA.x*nB.x + nA.y*nB.y + nA.z*nB.z;
}

double AbsDotProduct( const Normal& nA, const Normal& nB )
{
    return fabs( DotProduct( nA, nB ) );
}

Normal Normalize(const Normal& n)
{
    return n / n.Length();
}

