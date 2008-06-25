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

#include "InstanceNode.h"
#include "CmdCut.h"
#include "Trace.h"
#include "SceneModel.h"


CmdCut::CmdCut( const QModelIndex& selectedIndex, SoNode*& clipboard, SceneModel* model, QUndoCommand* parent )
: QUndoCommand("Cut", parent), m_pClipboard ( clipboard ), m_previousNode ( 0 ),  m_pModel(model)
{
	Trace trace( "CmdCut::CmdCut", false );
	InstanceNode* instanceNode = m_pModel->NodeFromIndex( selectedIndex );
	m_coinNode = instanceNode->GetNode();
	m_coinNode->ref();
	m_coinParent = static_cast< SoBaseKit* > (instanceNode->GetParent()->GetNode() );	
	
	m_previousNode = clipboard ;
	
	
	m_row = instanceNode->GetParent()->children.indexOf( instanceNode );
}

CmdCut::~CmdCut()
{
	Trace trace( "CmdCut::~CmdCut", false );
	m_coinNode->unref();
}

void CmdCut::undo()
{ 
	Trace trace( "CmdCut::undo", false );
	m_pClipboard = m_previousNode;
	m_pModel->Paste( tgc::Shared, *m_coinParent, *m_coinNode, m_row );
}

void CmdCut::redo( )
{
	Trace trace( "CmdCut::redo", false );
	m_pClipboard = m_coinNode;
	m_pModel->Cut( *m_coinParent, m_row );
}
