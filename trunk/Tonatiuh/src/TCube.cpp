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
 
#include <Inventor/SbBox.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
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
#include "TCube.h"
#include "DifferentialGeometry.h"
#include "tgf.h"

#include "Trace.h"

using namespace std;

SO_NODE_SOURCE(TCube);

// Normals to the six sides of the cube
   SbVec3d TCube::frontNormal, TCube::rearNormal;
   SbVec3d TCube::leftNormal, TCube::rightNormal;
   SbVec3d TCube::baseNormal, TCube::topNormal;

void TCube::initClass()
{
	Trace trace("TCube::initClass", false );
	SO_NODE_INIT_CLASS(TCube, TShape, "TShape");
}

TCube::TCube()
{
	Trace trace("TCube::TCube", false );
	
	SO_NODE_CONSTRUCTOR(TCube);
	SO_NODE_ADD_FIELD(m_width, (2.0));
	SO_NODE_ADD_FIELD(m_height, (2.0));
	SO_NODE_ADD_FIELD(m_depth, (2.0));
	
	// If this is the first time the constructor is called, set
	// up the static normals.
	if ( SO_NODE_IS_FIRST_INSTANCE() )
	{
		frontNormal.setValue( 0.0, 0.0, 1.0 );
		rearNormal.setValue( 0.0, 0.0, -1.0 );
		leftNormal.setValue( 1.0, 0.0, 0.0 );
		rightNormal.setValue( -1.0, 0.0, 0.0 );
		baseNormal.setValue( 0.0, -1.0, 0.0 );
		topNormal.setValue( 0.0, 1.0, 0.0 ); 
	}
}

TCube::~TCube()
{

}


QString TCube::getIcon() 
{
	Trace trace("TCube::getIcon", false );
	return ":/icons/tcube.png";
}

void TCube::generatePrimitives(SoAction *action)
{
    // The TCube will generate 12 triangles: 2 for each side
    // This variable is used to store each vertex.
    SoPrimitiveVertex pv;

    // Access the state from the action.
    SoState  *state = action->getState();

    // We need the size to adjust the coordinates.
    double halfWidth = m_width.getValue()/2.0;
    double halfHeight = m_height.getValue()/2.0;
    double halfDepth = m_depth.getValue()/2.0;

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

   // If we need to generate texture coordinates with a
   // function, we'll need an SoGLTextureCoordinateElement.
   // Otherwise, we'll set up the coordinates directly.
   const SoTextureCoordinateElement* tce = 0;
   SbVec4f texCoord;
   if ( useTexFunc )
     tce = SoTextureCoordinateElement::getInstance( state );
   else {
     texCoord[2] = 0.0;
     texCoord[3] = 1.0;
   }

   // We'll use this macro to make the code easier. It uses the
   // "point" variable to store the primitive vertex's point.
   SbVec3f  point;
  
#define GEN_VERTEX(pv, x, y, z, s, t, normal)   \
     point.setValue(halfWidth  * x,             \
                halfHeight * y,                 \
                halfDepth  * z);                \
     if (useTexFunc)                            \
       texCoord = tce->get(point, normal);      \
     else {                                     \
       texCoord[0] = s;                         \
       texCoord[1] = t;                         \
     }                                          \
     pv.setPoint(point);                        \
     pv.setNormal(normal);                      \
     pv.setTextureCoords(texCoord);             \
     shapeVertex(&pv)
    
     // We will generate two triangles for the base, as a
     // triangle strip.
     beginShape(action, TRIANGLE_STRIP);

     // Base: left front, left rear, right front, right rear
     GEN_VERTEX(pv,  1.0, -1.0,  1.0, 0.0,  1.0, baseNormal);
     GEN_VERTEX(pv,  1.0, -1.0, -1.0, 0.0,  0.0, baseNormal);
     GEN_VERTEX(pv, -1.0, -1.0,  1.0, 1.0,  1.0, baseNormal);
     GEN_VERTEX(pv, -1.0, -1.0, -1.0, 1.0,  0.0, baseNormal);

     endShape();
     
     // We will generate two triangles for the top, as a
     // triangle strip.
     beginShape(action, TRIANGLE_STRIP);

     // Top: left front, left rear, right front, right rear
     GEN_VERTEX(pv,  1.0, 1.0,  1.0, 0.0,  1.0, baseNormal);
     GEN_VERTEX(pv,  1.0, 1.0, -1.0, 0.0,  0.0, baseNormal);
     GEN_VERTEX(pv, -1.0, 1.0,  1.0, 1.0,  1.0, baseNormal);
     GEN_VERTEX(pv, -1.0, 1.0, -1.0, 1.0,  0.0, baseNormal);

     endShape();
}    


void TCube::computeBBox(SoAction*, SbBox3f& box, SbVec3f& center)
{

    // These points define the min and max extents of the box.
    SbVec3f min, max;

     // Compute the half-width, half-height, and half-depth of
     // the pyramid. We'll use this info to set the min and max
     // points.
    double halfWidth = m_width.getValue()/2.0;
    double halfHeight = m_height.getValue()/2.0;
    double halfDepth = m_depth.getValue()/2.0;

    min.setValue(-halfWidth, -halfHeight, -halfDepth);
    max.setValue(halfWidth, halfHeight, halfDepth);


    // Set the box to bound the two extreme points.
    box.setBounds(min, max);

   // This defines the "natural center" of the TCube. We could
   // define it to be the center of the base, if we want, but
   // let's just make it the center of the bounding box.
   center.setValue(0.0, 0.0, 0.0);
}

bool TCube::Intersect(const Ray& /*objectRay*/, double* /*tHit*/, DifferentialGeometry* /*dg*/) const 
{
    //Yet to be implemented
	return false;
}

bool TCube::IntersectP( const Ray& /*objectRay*/ ) const 
{	
	//Yet to be implemented
	return false;
}


Point3D TCube::Sample( double /*u1*/, double /*u2*/ ) const
{
	//Yet to be implemented
	return Point3D( 0, 0, 0 );	
}

SbVec3f TCube::GetNormal (double /*u*/, double /*v*/ ) const
{
	return SbVec3f( 0, 1, 0 );
}
