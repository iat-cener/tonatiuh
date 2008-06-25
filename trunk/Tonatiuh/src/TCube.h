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
 
#ifndef TCUBE_H_
#define TCUBE_H_

#include <QString>
#include <Inventor/SbLinear.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/nodes/SoShape.h>
#include "TShape.h"


class TCube : public TShape
{
	SO_NODE_HEADER(TCube);
	
public:
	TCube( );
	static void initClass();
	QString getIcon(); // {return ":/icons/icons/TShapeKit.png";}
	
	bool Intersect(const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;
	Point3D Sample( double u1, double u2 ) const;
	
	SoSFDouble m_width;
	SoSFDouble m_height;
	SoSFDouble m_depth;

protected:
	virtual void generatePrimitives(SoAction *action);
	virtual void computeBBox( SoAction* action, SbBox3f& box, SbVec3f& center);                             
	virtual ~TCube();
	SbVec3f GetNormal (double u, double v) const;

private:
    // Face normals. These are static because they are computed
    // once and are shared by all instances
    static SbVec3d frontNormal, rearNormal;
    static SbVec3d leftNormal, rightNormal;
    static SbVec3d baseNormal, topNormal;
   
};

#endif /*TCUBE_H_*/
