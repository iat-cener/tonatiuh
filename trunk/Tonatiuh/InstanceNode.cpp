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
#include "Trace.h"

#include <iostream>

InstanceNode::InstanceNode( SoNode* node )
: m_coinNode( node ), m_parent( 0 ) 
{
	Trace trace( "InstanceNode::InstanceNode" );
}

InstanceNode::InstanceNode( const InstanceNode* node )
{
	

	Trace trace( "InstanceNode::InstanceNode" );
	m_coinNode = node->m_coinNode;
	node->m_coinNode->ref();
	m_parent = node->m_parent;
	
	
	for( int index = 0; index < node->children.count(); index++ ) 
    {
    	InstanceNode* child = new InstanceNode(*(node->children[index]));
    	
    	children.append(child);
    }

	
}

InstanceNode::~InstanceNode()
{
	Trace trace( "InstanceNode::~InstanceNode" );
	qDeleteAll( children );
}

void InstanceNode::SetParent( InstanceNode* parent )
{
	m_parent = parent;
}

void InstanceNode::SetNode( SoNode* node )
{
	m_coinNode = node;
}

SoNode* InstanceNode::GetNode() const
{
	Trace trace( "InstanceNode::GetNode" );
	return m_coinNode;
}

InstanceNode* InstanceNode::GetParent() const
{
	Trace trace( "InstanceNode::GetParent" );
	return m_parent;
}

void InstanceNode::Print( int level ) const
{
	Trace trace( "InstanceNode::Print" );
	for( int i = 0; i < level; i++ ) std::cout << " ";
    std::cout << m_coinNode->getTypeId().getName().getString() << " has " << children.size() << " children "<<std::endl;
    for( int index = 0; index < children.count(); index++ ) 
    {
    	children[index]->Print( level++ );
    }
}

void InstanceNode::AddChild( InstanceNode* child )
{
    children.append( child );
    child->SetParent( this );
}

void InstanceNode::InsertChild( int row, InstanceNode* instanceChild)
{
	if( row > children.size() ) row = children.size();
 	children.insert( row, instanceChild);
 	instanceChild->SetParent(this);
}

QDataStream & operator<< ( QDataStream & s, const InstanceNode & node )
{
	s << node.m_coinNode;
	return s;
}

QDataStream & operator>> ( QDataStream & s, const InstanceNode & node )
{
	s >> node;
	return s;
}

bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode)
{
	return ( (thisNode.GetNode() == otherNode.GetNode()) && 
			(thisNode.GetParent()->GetNode()==otherNode.GetParent()->GetNode()));
}

