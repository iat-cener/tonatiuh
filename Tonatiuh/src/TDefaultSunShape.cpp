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
#include <Inventor/SbBox.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/misc/SoState.h>
#include "TDefaultSunShape.h"

#include <cmath>
#include "tgc.h"
#include "tgf.h"
#include "Vector3D.h"

SO_NODE_SOURCE(TDefaultSunShape);

void TDefaultSunShape::initClass()
{
	SO_NODE_INIT_CLASS(TDefaultSunShape, TSunShape, "TSunShape");
}

TDefaultSunShape::TDefaultSunShape( )
{
	SO_NODE_CONSTRUCTOR(TDefaultSunShape);
	SO_NODE_ADD_FIELD(m_irradiance, (10.0));
	SO_NODE_ADD_FIELD(m_thetaMax, (-5.0));

}

TDefaultSunShape::~TDefaultSunShape()
{
}

//Light Interface
void TDefaultSunShape::generateRayDirection( Vector3D& direction, RandomDeviate& rand ) const
{
    static double TwoPi = 2.0 * tgc::Pi;

    double phi = TwoPi * rand.RandomDouble();
    double sinTheta = sin( m_thetaMax.getValue() )*sqrt( rand.RandomDouble());
    double cosTheta = sqrt( 1.0 - sinTheta*sinTheta );
    double cosPhi = cos( phi );
    double sinPhi = sin( phi );

    direction.x = sinTheta*cosPhi;
    direction.y = cosTheta;
    direction.z = sinTheta*sinPhi;
}

SoNode* TDefaultSunShape::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	TDefaultSunShape* newSunShape = dynamic_cast< TDefaultSunShape* >( SoNode::copy( copyConnections ) );
	
	// Copy the m_thetaMin, m_thetaMax private members explicitly
	newSunShape->m_irradiance = m_irradiance;
	newSunShape->m_thetaMax = m_thetaMax;
	
	return newSunShape;
}
