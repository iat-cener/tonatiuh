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

#include "TNodesList.h"
#include "TParameterList.h"

TNodeType TNodesList::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Initializes TNodesList type.
 */
void TNodesList::Init()
{
	m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Node" ), "NodesList" );
}

/*!
 * TGroupNode : public TNode
 */
TNodesList::TNodesList()
:TNode()
{
	SetName( GetType().GetName() );
}

/*!
 * Destructor.
 * The nodes in the list are not destroyed.
 */
TNodesList::~TNodesList()
{
	for( unsigned int c = 0; c < m_children.size(); c++ )
		m_children[c]->RemoveReference();

	m_children.clear();
	m_validNodeTypesList.clear();

}

/*!
 * If the list does not conatins the \a type , the method adds it to the type list.
 */
void TNodesList::AddValidNodeType( TNodeType type )
{
	for( unsigned int i = 0; i < m_validNodeTypesList.size(); i++)
	{
		if (m_validNodeTypesList[i] == type )	return;
	}

	m_validNodeTypesList.push_back( type );
}

/*!
 * Creates a container node objecte as a copy of this node.
 */
TNodesList* TNodesList::Copy() const
{
	TNodesList* nodeList = new TNodesList();
	if( nodeList == 0 )	return ( 0  );

	std::vector< TNode* > m_children;
	std::vector< TNodeType > m_validNodeTypesList;


	for( unsigned int i = 0; i < m_validNodeTypesList.size(); i++ )
		nodeList->AddValidNodeType( m_validNodeTypesList[i] );

	for( unsigned int i = 0; i < m_children.size(); i++ )
		nodeList->InsertItem( m_children[i]->Copy() );

	//There are not parameters in TNodesLists
	//
	return ( nodeList );
}

/*!
 * Returns icon file name
 */
std::string TNodesList::GetIcon() const
{
	return ( ":/icons/tnodelist.png" );
}


/*!
 * Returns the type of node.
 */
TNodeType TNodesList::GetType() const
{
	return ( TNodesList::m_nodeType );
}

/*!
 *	Returns the child number \a index.
 */
TNode* TNodesList::Item( int index ) const
{
	return ( m_children[index] );
}

/*!
 * Returns the index of the \a child in the children list.
 * If \a child is not this node child, returns -1.
 */
int TNodesList::IndexOf(const TNode* child ) const
{
	for( unsigned int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == child)
			return ( i );
	}
	return ( -1 );
}

/*!
 * Inserts a \a node as a child in the position \a index.
 */
bool TNodesList::InsertItem( TNode* node, int index  )
{

	bool validType = false;
	for( unsigned int i = 0; i < m_validNodeTypesList.size(); i++ )
	{

		if( node->GetType().IsDerivedFrom( m_validNodeTypesList[i] ) )
		{
			validType = true;
			break;
		}
	}

	if( !validType ) return (false);

	if( ( index < 0 )  || ( index > int ( m_children.size() ) ) )
		m_children.push_back( node );
	else
		m_children.insert( m_children.begin() + index, node );
	node->IncreaseReference();
	return (true);

}

/*!
 * Returns the number of children of this node.
 */
int TNodesList::Count() const
{
	return ( m_children.size() );
}

/*!
 * Removes the item in the list identified by  \a index.
 */
bool TNodesList::RemoveItem( int index )
{
	if( index >= int( m_children.size() ) )	return ( false );

	TNode* nodeToRevome = m_children[index];
	m_children.erase( m_children.begin() + index );
	nodeToRevome->RemoveReference();

	return ( true );

}
