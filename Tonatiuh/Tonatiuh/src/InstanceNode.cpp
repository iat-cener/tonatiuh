/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments: 

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco, 
then Chair of the Department of Engineering of the University of Texas at 
Brownsville. From May 2004 to July 2008, it was supported by the Department 
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under 
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06. 
During 2007, NREL also contributed to the validation of Tonatiuh under the 
framework of the Memorandum of Understanding signed with the Spanish 
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117). 
Since June 2006, the development of Tonatiuh is being led by the CENER, under the 
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez, 
Juana Amieva, Azael Mancillas, Cesar Cantu.
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

