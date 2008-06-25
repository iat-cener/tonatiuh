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

#include "CmdParameterModified.h"
#include "Trace.h"

CmdParameterModified::CmdParameterModified( const QStringList& oldValueList, SoBaseKit* coinNode , const QString& coinPart, QUndoCommand* parent )
:QUndoCommand( "ModifyParameter" ,parent ), m_oldValueList( oldValueList ), m_newValueList( 0 ),  m_coinNode( coinNode ), m_nodePart( coinPart )
{
	Trace trace( "CmdParameterModified::CmdParameterModified", false);
	
	SoNode* node = coinNode->getPart( coinPart.toStdString().c_str(), true );
	
	SoFieldList fieldList; 
	int numFields = node->getFields( fieldList );
	
	m_newValueList = new QStringList;
	for(int index = 0; index < numFields; index++)
	{
		SbString indexValue;
		fieldList[index]->get( indexValue );
		m_newValueList->append( QString(indexValue.getString() ) );
	 	
	}

}

CmdParameterModified::~CmdParameterModified()
{
	Trace trace( "CmdParameterModified::~CmdParameterModified", false );
}

void CmdParameterModified::undo()
{
	Trace trace( "CmdParameterModified::undo", false );
	SoNode* partNode = m_coinNode->getPart( m_nodePart.toStdString().c_str(), true );
	
	SoFieldList fieldList; 
	int numFields = partNode->getFields(fieldList);
	
	 for( int index = 0; index < numFields; index ++)
	 {
	 	if(	m_oldValueList[index] != (*m_newValueList)[index] ){
	 		
	 		QString value = m_oldValueList[index];
	 		
	 		fieldList[index]->set( value.toStdString().c_str());
	 		
	 	}
	 } 

}

void CmdParameterModified::redo()
{
	Trace trace( "CmdParameterModified::redo", false );
	
	SoNode* partNode = m_coinNode->getPart( m_nodePart.toStdString().c_str(), true );
	
	SoFieldList fieldList; 
	int numFields = partNode->getFields(fieldList);
	
	 for( int index = 0; index < numFields; index++ )
	 {

	 	if(	m_oldValueList[index] != (*m_newValueList)[index] ){
	 		
	 		QString value = (*m_newValueList)[index];
	 		
	 		fieldList[index]->set( value.toStdString().c_str());
	 	}
	 }

}
