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
***************************************************************************/

/*!
 * \class CmdChangeNodeName
 * \brief CmdChangeNodeName is the command that represents a node name change.
 *
 *CmdChangeNodeName represents a change in a node from the scene. The node can be any one.
 */

#include <QModelIndex>
#include <QString>

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>

#include "gf.h"

#include "CmdChangeNodeName.h"
#include "InstanceNode.h"
#include "SceneModel.h"

/*!
 * Creates a new CmdChangeNodeName command.
 */
CmdChangeNodeName::CmdChangeNodeName( const QModelIndex& index, QString newName, SceneModel* model, QUndoCommand* parent )
: QUndoCommand("Change node name", parent), m_newName ( newName ), m_previousName( "" ), m_pNode( 0 ), m_pModel( model )
{
	if( !index.isValid() ) gf::SevereError( "CmdChangeNodeName called with invalid ModelIndex." );
	InstanceNode* selectedNodeInstance = m_pModel->NodeFromIndex( index );
	if( !selectedNodeInstance ) gf::SevereError( "CmdChangeNodeName called with invalid node." );
	m_pNode = selectedNodeInstance->GetNode();

	m_previousName = QString( m_pNode->getName().getString() );

}

/*!
 * Destroys command object.
 */
CmdChangeNodeName::~CmdChangeNodeName()
{
}

/*!
 * Changes the node name to the name that the node takes before the command is applied.
 */
void CmdChangeNodeName::undo()
{
	m_pModel->SetNodeName( m_pNode, m_previousName );
}

/*!
 * Changes the node name to the name defined in the command constructor.
 */
void CmdChangeNodeName::redo()
{
	m_pModel->SetNodeName( m_pNode, m_newName );
}
