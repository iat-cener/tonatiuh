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
 
#include "CmdInsertMaterial.h"
#include "SceneModel.h"
#include "TMaterial.h"
#include "TShapeKit.h"
#include "tgf.h"
#include "Trace.h"

CmdInsertMaterial::CmdInsertMaterial( TShapeKit* shapeKit, TMaterial* material, SceneModel* model, QUndoCommand * parent )
: QUndoCommand("InsertMaterial", parent), m_shapeKit(shapeKit), m_previousMaterial(0),m_material(material), m_pModel( model )
{
	Trace trace( "CmdInsertMaterial::CmdInsertMaterial", false );
	
	if( m_shapeKit == 0 ) tgf::SevereError( "CmdInsertMaterial called with NULL TShapeKit" );
	if( m_material == 0 ) tgf::SevereError( "CmdInsertMaterial called with NULL TMaterial" );
	m_material->ref();	
    m_previousMaterial = dynamic_cast< TMaterial* >( m_shapeKit->getPart( "material", false ) );
}

CmdInsertMaterial::~CmdInsertMaterial()
{
	Trace trace( "CmdInsertMaterial::~CmdInsertMaterial" );
    m_material->unref();
}

void CmdInsertMaterial::undo()
{
	Trace trace( "CmdInsertMaterial::undo", false );

    m_shapeKit->setPart( "material", NULL );
	m_pModel->RemoveCoinNode( m_row, *m_shapeKit );
}

void CmdInsertMaterial::redo( )
{
	Trace trace( "CmdInsertMaterial::redo", false );
    m_shapeKit->setPart( "material", m_material );
    m_row = m_pModel->InsertCoinNode( *m_material, *m_shapeKit );
}
