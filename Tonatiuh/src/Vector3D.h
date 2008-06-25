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
 
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>

struct Point3D;
struct Normal;

struct Vector3D
{
    Vector3D( double dx = 0.0, double dy = 0.0, double dz = 0.0 );
    Vector3D( const Normal& norm );
    explicit Vector3D( Point3D point );
    ~Vector3D( );
    Vector3D& operator=( const Vector3D& vector );  
    Vector3D& operator+=( const Vector3D& vector );
    Vector3D& operator-=( const Vector3D& vector );
    Vector3D& operator*=( double scalar );
    Vector3D& operator/=( double scalar );
    void zero();     
    bool operator==( const Vector3D& vector ) const;    
    bool operator!=( const Vector3D& vector ) const;
    double operator[]( int i ) const;
    double& operator[]( int i );
    double LengthSquared( ) const;
    double Length( ) const;

    double x;
    double y;
    double z;
};

Vector3D operator-( const Vector3D& vector ); 
Vector3D operator+( const Vector3D& lhs, const Vector3D& rhs );
Vector3D operator-( const Vector3D& lhs, const Vector3D& rhs );
Vector3D operator*( const Vector3D& vector, double scalar );
Vector3D operator*( double scalar, const Vector3D& vector );
Vector3D operator/( const Vector3D& vector, double scalar );
std::ostream& operator<<( std::ostream& os, const Vector3D& vector );
double DotProduct( const Vector3D& vA, const Vector3D& vB );
double DotProduct( const Vector3D& vA, const Normal& nB );
double DotProduct( const Normal& nA, const Vector3D& vB );
double AbsDotProduct( const Vector3D& vA, const Vector3D& vB );
double AbsDotProduct( const Vector3D& vA, const Normal& nB );
double AbsDotProduct( const Normal& nA, const Vector3D& vB );
Vector3D CrossProduct( const Vector3D& vA, const Vector3D& vB );
Vector3D CrossProduct( const Vector3D& vA, const Normal& nB );
Vector3D CrossProduct( const Normal& nA, const Vector3D& vB );
Vector3D Normalize( const Vector3D& vA );
bool SameHemisphere( const Vector3D& vA, const Vector3D& vB );

#endif
