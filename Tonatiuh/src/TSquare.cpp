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

#include "TSquare.h"
#include "DifferentialGeometry.h"
#include "tgf.h"


SO_NODE_SOURCE(TSquare);

void TSquare::initClass()
{
	SO_NODE_INIT_CLASS(TSquare, TShape, "TShape");
}

TSquare::TSquare( )
{
	SO_NODE_CONSTRUCTOR(TSquare);
	SO_NODE_ADD_FIELD(m_sideLength, (10.0));
}

TSquare::~TSquare()
{
	
}

void TSquare::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;

    // Access the state from the action.
    SoState  *state = action->getState();
   
    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );
                         
    // If we need to generate texture coordinates with a
    // function, we'll need an SoGLTextureCoordinateElement.
    // Otherwise, we'll set up the coordinates directly.
    const SoTextureCoordinateElement* tce = 0;
    SbVec4f texCoord;  
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);
    else 
    {
        texCoord[2] = 0.0;
        texCoord[3] = 1.0;
    }

    // We'll use this macro to make the code easier. It uses the
    // "point" variable to store the primitive vertex's point.
    SbVec3f  point;
    

	const int rows = 50; // Number of points per row
    const int columns = 50; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid
    
    float vertex[totalPoints][6];    
   
    int h = 0;
    double ui = 0;
	double vj = 0;
	
	double u_step = 1.0 /(double)(rows-1); // Size of the step in parameter u to go from 0 to 1 in the number of rows
	double v_step = 1.0 /(double)(columns-1); // Size of the step in parameter v to go from 0 to 1 in the number of columns
	
    for (int i = 0; i < rows; i++)
    {
    	for ( int j = 0 ; j < columns ; j++ )
    	{
    		vertex[h][0] = GetPoint3D(ui, vj).x;
    		vertex[h][1] = GetPoint3D(ui, vj).y;
    		vertex[h][2] = GetPoint3D(ui, vj).z;
    		vertex[h][3] = GetNormal(ui, vj)[0];
    		vertex[h][4] = GetNormal(ui, vj)[1];
    		vertex[h][5] = GetNormal(ui, vj)[2];
    		pv.setPoint(vertex[h][0], vertex[h][1], vertex[h][2] );
    		h++; //Increase h to the next point. 
    		vj += v_step; // Increase parameter vj to go from initial 0 to 1 at the end of the row (keep the same ui for the whole row)
    	}
    	vj = 0; // Re-initialize parameter vj to 0 in order to start from 0 in the next row
		ui += u_step; // Increase parameter ui to go from initial 0 to 1 in the last row.
    }

	const int totalIndices  = (rows-1)*(columns-1)*4;
    int32_t* indices = new int32_t[totalIndices]; 
    int k = 0;
    for(int irow = 0; irow < (rows-1); irow++)
           for(int icolumn = 0; icolumn < (columns-1); icolumn++)
           {
           	indices[k] = irow*columns + icolumn;
        	indices[k+1] = indices[k] + 1;        
        	indices[k+3] = indices[k] + columns;        
        	indices[k+2] = indices[k+3] + 1;
    
        	k+=4; //Set k to the first point of the next face.
           }
           
    float finalvertex[totalIndices][6];
    for(int ivert = 0; ivert<totalIndices;ivert++)
    {
    	finalvertex[ivert][0] = vertex[indices[ivert]][0];
    	finalvertex[ivert][1] = vertex[indices[ivert]][1];
    	finalvertex[ivert][2] = vertex[indices[ivert]][2];  
		finalvertex[ivert][3] = vertex[indices[ivert]][3];
		finalvertex[ivert][4] = vertex[indices[ivert]][4];
		finalvertex[ivert][5] = vertex[indices[ivert]][5];
    }
    
#define GEN_VERTEX(pv, x, y, z, s, t, normal)   \
     point.setValue( x,                         \
                     y,                         \
                     z);                        \
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

    
    float u = 1;
    float v = 1;  
	
	beginShape(action, QUADS );
    for( int i = 0; i < totalIndices; i++ )
    {
    	//SbVec3f normal=GetNormal(finalvertex[i][0],finalvertex[i][1], finalvertex[i][2] );
    	SbVec3f normal(finalvertex[i][3],finalvertex[i][4], finalvertex[i][5] );
    	GEN_VERTEX(pv,  finalvertex[i][0], finalvertex[i][1],  finalvertex[i][2], u,  v, normal);
    }         		
    endShape();
}

bool TSquare::Intersect(const Ray& /*objectRay*/, double* /*tHit*/, DifferentialGeometry* /*dg*/) const 
{
	return true;
}

bool TSquare::IntersectP( const Ray& objectRay ) const 
{	
	return Intersect( objectRay, 0, 0 );
}

Point3D TSquare::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

void TSquare::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
	Point3D min = Point3D(-m_sideLength.getValue()/2,0.0, -m_sideLength.getValue()/2);
	Point3D max = Point3D(m_sideLength.getValue()/2,0.0,m_sideLength.getValue()/2);
	box.setBounds(SbVec3f( min.x, min.y, min.z ), SbVec3f( max.x, max.y, max.z ));
}

Point3D TSquare::GetPoint3D (double u, double v) const
{
	if ((u < 0.0 && u > 1) && (v < 0.0 && v > 1))
	{
		
		tgf::SevereError("Function TSquare::GetPoint3D called with invalid parameters" );
	}
	
	double x = (u * m_sideLength.getValue()) - (m_sideLength.getValue()/2);
	double z = v * m_sideLength.getValue() - (m_sideLength.getValue()/2);

	return Point3D(x,0,z);
}

SbVec3f TSquare::GetNormal (double /*u*/, double /*v*/) const
{
	return SbVec3f( 0, 1, 0);
}
