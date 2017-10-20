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

#include "TNode.h"
#include "TNodeType.h"
#include "TParameterList.h"

TNodeType TNode::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Initializes node class.
 */
void TNode::Init()
{
	TNode::m_nodeType = TNodeType::CreateType( TNodeType::CreateEmptyType(), "Node" );
}

/*!
 * Creates a TNode object as a child of \a parent node.
 * The initial reference count will be set to zero.
 */
TNode::TNode()
: //QObject(),
  m_id( 0 ),
  m_name( GetType().GetName().c_str() ),
  m_referenceCount( 0 )
{

	static int nodeIndex = 0;
	m_id = nodeIndex;
	nodeIndex++;

	//setObjectName(GetType().GetName().c_str() );
	m_parametersList = new TParameterList;
}

/*!
 * Destroys node object.
 */
TNode::~TNode()
{

}

/*!
 * Creates a copy of the node.
 */
TNode* TNode::Copy() const
{
	TNode* node = new TNode();

	TParameterList* parametersList = node->m_parametersList;

	std::vector< std::string > parameterNames = m_parametersList->GetParametersNames();
	for( unsigned int p = 0; p < parameterNames.size(); p++ )
	{
		QVariant parameterValue = m_parametersList->GetValue( parameterNames[p] );
		bool parameterVisibility = m_parametersList->GetVisibility( parameterNames[p] );
		parametersList->Append(parameterNames[p], parameterValue, parameterVisibility );
	}


	return ( node );
}

/*!
 * Returns node identifier.
 */
int TNode::GetID() const
{
	return ( m_id );
}

/*!
 * Returns node object name.
 */
std::string TNode::GetName() const
{
	return ( m_name );
}

/*!
 * Returns icon file name
 */
std::string TNode::GetIcon() const
{
	return ( ":/icons/tnode.png" );
}

/*!
 * Returns the value of the parameter \a name.
 */
QVariant TNode::GetParameterValue( std::string name ) const
{

	if(!m_parametersList )	return( QVariant() );
	if( !m_parametersList->Contains( name ) || !m_parametersList->GetVisibility( name ) )
		return( QVariant() );
	return (m_parametersList->GetValue( name ) );

}

/*!
 * Returns the number of references.
 */
int TNode::GetReferences() const
{
	return ( m_referenceCount );
}

/*!
 * Returns node type.
 */
TNodeType TNode::GetType() const
{
	return ( TNode::m_nodeType );
}

/*!
 * Returns the list of the parameters accesible exteranlly to the node.
 */
std::vector<std::string> TNode::GetVisibleParametersName() const
{
	std::vector< std::string > allParametersNames = m_parametersList->GetParametersNames();

	//Remove the no visible parameters starting from the end to avoid the changes in the indexes
	int nParameters = allParametersNames.size();
	for( int i = nParameters -1; i >=0; i-- )
	{
		if( !m_parametersList->GetVisibility( allParametersNames[i] ) )
			allParametersNames.erase( allParametersNames.begin() + i );
	}
	return ( allParametersNames );

}

/*!
 * Increases in one the number of references to the node.
 */
void TNode::IncreaseReference()
{
	m_referenceCount++;
}

void TNode::RemoveReference()
{
	m_referenceCount--;
	if( m_referenceCount < 1 )
		delete this;
}

/*!
 * Sets \a name to the node object.
 */
void TNode::SetName( std::string name )
{
	m_name = name;
}

/*!
 * Sets to the parameter \a name the \a value.
 */
bool TNode::SetParameterValue( const std::string& name, const QVariant& value )
{

	if( !m_parametersList )	return ( false );
	if( !m_parametersList->Contains( name ) || !m_parametersList->GetVisibility( name ) )
		return ( false );
	if( !m_parametersList->SetValue( name, value ) ) 	return ( false );

	return (true);
}
