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
 
#ifndef BBOX_H_
#define BBOX_H_ 

#include <iostream>
#include "Point3D.h"

struct Ray;

/***********************************************************************//**
 * Axis-aligned Bounding Box class used by all shapes
***************************************************************************/
struct BBox
{
	/***********************************************************************//**
 	* Default constructor creates an empty box.
	***************************************************************************/
	BBox( );
	
	/***********************************************************************//**
 	* Creates a bounding box with no volume at the given point. 
	***************************************************************************/
	explicit BBox( const Point3D& point );
	
	/***********************************************************************//**
 	* Creates a bounding box with minimum and maximum extents given by the coordinates
 	* of point1 and point2.
	***************************************************************************/
	BBox( const Point3D& point1, const Point3D& point2 );
	
	/***********************************************************************//**
 	* Returns true if 2 this box intersects with argument, false otherwise. 
	***************************************************************************/
	bool Overlaps( const BBox& bbox ) const;
	
	/***********************************************************************//**
 	* Returns true if point is within the extents of the bounding box, false otherwise. 
	***************************************************************************/
	bool Inside( const Point3D& point ) const;
	
	/***********************************************************************//**
 	* Expands the extent of the bounding box by delta in all directions. 
	***************************************************************************/
	void Expand( double delta );
	
	/***********************************************************************//**
 	* Returns the volume of space contained within the box.
	***************************************************************************/
	double Volume( ) const;
	
	/***********************************************************************//**
 	* Returns the bounding box's axis of greatest length
 	* 0 for x-axis
 	* 1 for y-axis
 	* 2 for z-axis
	***************************************************************************/
	int MaximumExtent( ) const;
	
	/***********************************************************************//**
 	* Returns the center and radius of the smallest sphere that fully contains
 	* the bounding box.
	***************************************************************************/
	void BoundingSphere( Point3D *center, float *radius ) const;
	
	/***********************************************************************//**
 	* Calculates the intersection of a given ray with the bounding box.
 	* Returns false if the ray and box do not intersect.
 	* If an intersection does occur, the intersection points are returned as pointers. 
	***************************************************************************/
	bool IntersectP( const Ray& ray, double* hitt0 = NULL, double* hitt1 = NULL ) const;
	
	Point3D pMin;
	Point3D pMax;
};

std::ostream& operator<<( std::ostream& os, const BBox& bbox );
BBox Union( const BBox& bbox, const Point3D& point );
BBox Union( const BBox& bbox1, const BBox& bbox2 );

#endif //BBOX_H_
