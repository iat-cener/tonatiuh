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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "TContainerNode.h"
#include "TNodeType.h"


TNodeType TContainerNode::m_nodeType = TNodeType::CreateEmptyType();


/*!
  Creates a new instance of the class type corresponding object.
*/
void* TContainerNode::CreateInstance( )
{
  return ( new TContainerNode() );
}

/*!
 * Initializes TGroupNode type.
 */
void TContainerNode::Init()
{
	m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Node" ), QString( "ContainerNode" ), &TContainerNode::CreateInstance );
}

/*!
 * Constructor.
 * The parts of the node are empty.
 */
TContainerNode::TContainerNode()
:TNode()
{
	setObjectName(GetType().GetName());

}

/*!
 * Destroys container node.
 * When a container node is removed, the parts of the node are not deleted.
 */
TContainerNode::~TContainerNode()
{
	 m_partsList.clear();
	 m_partsTypeList.clear();
}

/*!
 * Creates a container node objecte as a copy of this node.
 */
TContainerNode* TContainerNode::Copy() const
{
	TContainerNode* containerNode = new TContainerNode();
	if( containerNode == 0 )	return ( 0  );

	QHashIterator<QString, TNodeType > i( containerNode->m_partsTypeList );
	while( i.hasNext() )
	{
	    i.next();
	    QString partName = i.key();
	    TNodeType partType = i.value();
	    TNode* partNode = containerNode->m_partsList[partName];

    	containerNode->AppendPart( partName, partType, partNode->Copy() );
	}

	return ( containerNode );
}

/*!
 * Returns the type of node.
 */
TNodeType TContainerNode::GetType() const
{
	return ( TContainerNode::m_nodeType );
}


/*!
 * Returns true if exists a part with the \a name.
 */
bool TContainerNode::Contains(const QString& name) const
{

	if( m_partsList.contains( name ) ) return ( true );
	return ( false);
}

/*!
 * Returns the node related to the part \a name.
 * If the part does not exit, null value is returned.
 */
TNode* TContainerNode::GetPart( const QString name ) const
{
	if( !m_partsList.contains( name ) ) return ( 0 );

	return ( m_partsList.value( name ) );
}

/*!
 * Returns a list with the part names
 */
QStringList TContainerNode::GetPartNames() const
{
	return ( m_partsList.keys() );
}

/*!
 * Returns the number of parts in the container.
 */
int TContainerNode::NumberOfParts() const
{
	return ( m_partsList.count() );
}

/*!
 * Replaces the node of the part \a name with \a node. If the part does not exit, false is returned.
 * The previous node of the part is not destroyed.
 */
bool TContainerNode::SetPart( const QString name, TNode* node )
{
	if( !m_partsList.contains( name ) ) return ( false );
	if( m_partsList[name] )	m_partsList[name]->RemoveReference();
	m_partsList[name] = 0;

	if( !node ) return ( true );
	if( !node->GetType().IsDerivedFrom( m_partsTypeList.value( name ) ) ) return (false);
	m_partsList[name] = node;
	node->IncreaseReference();

	return ( true );
}


/*!
 * Adds a new part of \a type to the container defined with \a name. The default node for this part is \a defaultNode.
 * If there is a part with the \a name, the part is not included.
 */
void TContainerNode::AppendPart( const QString name, TNodeType type, TNode* defaultNode )
{
	if( m_partsList.contains( name ) ) return;

	m_partsList[name] = defaultNode;
	if( defaultNode ) defaultNode->IncreaseReference();

	m_partsTypeList[name] = type;

}

/*!
 * Removes the part \a name from the node.
 */
void TContainerNode::RevomePart( const QString name )
{
	//Removes from the part list the name of the parte and the type of node.
	if( m_partsTypeList.contains( name ) )
	{
		m_partsTypeList.remove( name );
	}

	//If a node object is defined for the part to remove, the node reference must be removed also.
	if( m_partsList.contains( name ) )
	{
		m_partsList.remove( name );
		m_partsList[name]->RemoveReference();
	}

}
