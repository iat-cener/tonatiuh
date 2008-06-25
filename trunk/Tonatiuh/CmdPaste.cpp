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
 
#include <Inventor/nodekits/SoBaseKit.h>
#include "CmdPaste.h"
#include "SceneModel.h"
#include "InstanceNode.h"
#include "Trace.h"
#include "tgf.h"

CmdPaste::CmdPaste( tgc::PasteType type, const QModelIndex& parentModelIndex,  SoNode*& coinClipboard, SceneModel& sceneModel, QUndoCommand* parent )
: QUndoCommand("Paste", parent), m_pasteType( type ), m_coinParent( 0 ), m_coinChild( coinClipboard ), m_sceneModel( &sceneModel )
{
	Trace trace( "CmdPaste::CmdPaste", false);
	
	//if( !m_coinChild ) tgf::SevereError( "CmdPaste Null m_coinChild." );
	if( !parentModelIndex.isValid() ) tgf::SevereError( "CmdPaste called with invalid ModelIndex." );
	
	InstanceNode* instanceParent = m_sceneModel->NodeFromIndex( parentModelIndex );
	m_coinParent = static_cast< SoBaseKit* > ( instanceParent-> GetNode() ); 
	if( !m_coinParent ) tgf::SevereError( "CmdPaste Null m_coinParent." );
	
	m_row = instanceParent->children.size();
}

CmdPaste::~CmdPaste()
{
	Trace trace( "CmdPaste::~CmdPaste", false );	
}

void CmdPaste::undo()
{
	Trace trace( "CmdPaste::undo", false );
	m_sceneModel->Cut( *m_coinParent, m_row );
}

void CmdPaste::redo( )
{
	Trace trace( "CmdPaste::redo", false );
	
	m_sceneModel->Paste( m_pasteType, *m_coinParent, *m_coinChild, m_row );
}
