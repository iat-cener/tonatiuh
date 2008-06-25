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
 
#include "CmdLightKitModified.h"

#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>

#include "TGateEngine.h"
#include "tgf.h"
#include "Trace.h"


CmdLightKitModified::CmdLightKitModified( TLightKit* newLightKit, SoSceneKit* scene, SceneModel& sceneModel, QUndoCommand* parent )
: QUndoCommand("Modify LightKit", parent), m_pPreviousLightKit( 0 ), m_pNewLightKit( newLightKit ), m_scene( scene ), m_pModel( &sceneModel )
{
    Trace trace( "CmdLightKitModified::CmdLightKitModified", false );
	m_pNewLightKit->ref();
	
	TLightKit* lightKit = static_cast< TLightKit* > ( m_scene->getPart("lightList[0]", false) );
	if( lightKit )	m_pPreviousLightKit = dynamic_cast< TLightKit* >( lightKit->copy( true ) );

}

CmdLightKitModified::~CmdLightKitModified()
{
	Trace trace( "CmdLightKitModified::~CmdLightKitModified", false );
	m_pNewLightKit->unref();
}

void CmdLightKitModified::undo()
{
	Trace trace( "CmdLightKitModified::undo", false );
  
    if( m_pPreviousLightKit )
    {
    	TLightKit* lightKit = static_cast< TLightKit* > ( m_scene->getPart("lightList[0]", false) );
   		
   		TShape* shape =static_cast< TShape* > ( m_pPreviousLightKit->getPart("icon", false) );
   		lightKit->setPart("icon", shape );
   		
   		TSunShape* sunhape =static_cast< TSunShape* > ( m_pPreviousLightKit->getPart("tsunshape", false) );
   		lightKit->setPart("tsunshape", sunhape );
    }
    else
    {
    	m_pModel->RemoveLightNode( *m_pNewLightKit );
    }
}

void CmdLightKitModified::redo( )
{
	Trace trace( "CmdLightKitModified::redo", false );
	
   if( !m_pPreviousLightKit )
   	{
   		 m_pModel->InsertLightNode( *m_pNewLightKit );
   	}
   	else
   	{	
   		TLightKit* lightKit = static_cast< TLightKit* > ( m_scene->getPart("lightList[0]", false) );
   		
   		TShape* shape =static_cast< TShape* > ( m_pNewLightKit->getPart("icon", false) );
   		lightKit->setPart("icon", shape );
   		
   		TSunShape* sunhape =static_cast< TSunShape* > ( m_pNewLightKit->getPart("tsunshape", false) );
   		lightKit->setPart("tsunshape", sunhape );
   		
   	}

}
