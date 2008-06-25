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
 
#include "CmdInsertShape.h"
#include "SceneModel.h"
#include "TShape.h"
#include "TShapeKit.h"
#include "tgf.h"
#include "Trace.h"

CmdInsertShape::CmdInsertShape( TShapeKit* shapeKit, TShape* shape, SceneModel* model, QUndoCommand * parent )
: QUndoCommand("InsertShape", parent), m_shapeKit(shapeKit), m_previousShape(0),m_shape(shape), m_pModel( model )
{
	Trace trace( "CmdInsertShape::CmdInsertShape" );
	if( m_shapeKit == 0 ) tgf::SevereError( "CmdInsertShape called with NULL TShapeKit*" );
	if( m_shape == 0 ) tgf::SevereError( "CmdInsertShape called with NULL TShape*" );
	m_shape->ref();	
    m_previousShape = dynamic_cast< TShape* >( m_shapeKit->getPart( "shape", false ) );
}

CmdInsertShape::~CmdInsertShape()
{
	Trace trace( "CmdInsertShape::~CmdInsertShape" );
    m_shape->unref();
}

void CmdInsertShape::undo()
{
	Trace trace( "CmdInsertShape::undo", false );

    m_shapeKit->setPart( "shape", NULL );
	m_pModel->RemoveCoinNode( m_row, *m_shapeKit );
}

void CmdInsertShape::redo( )
{
	Trace trace( "CmdInsertShape::redo", false );
    m_shapeKit->setPart( "shape", m_shape );
    m_row = m_pModel->InsertCoinNode( *m_shape, *m_shapeKit );
}
