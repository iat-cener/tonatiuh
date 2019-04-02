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

#include <Inventor/nodekits/SoBaseKit.h>

#include "CmdDelete.h"
#include "InstanceNode.h"
#include "SceneModel.h"

/**
 * Creates a new delete command that represents the \a selectedIndex node in the \a model deletion.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */

CmdDelete::CmdDelete( const QModelIndex& selectedIndex, SceneModel& model, QUndoCommand* parent )
: QUndoCommand("Delete", parent),  m_coinNode( 0 ), m_coinParent( 0 ), m_pModel(&model), m_row( -1 )
{
	InstanceNode* instanceNode = m_pModel->NodeFromIndex( selectedIndex );
	m_coinNode = instanceNode->GetNode();
	m_coinNode->ref();
	m_coinParent = static_cast< SoBaseKit* > ( instanceNode->GetParent()->GetNode() );
	m_row = instanceNode->GetParent()->children.indexOf( instanceNode );
}

/*!
 * Destroys the CmdDelete object.
 */
CmdDelete::~CmdDelete()
{
	m_coinNode->unref();
}

/*!
 * Reverts deletion. After undo() is called, the state of the model will be the same as before redo() was called.
 * \sa redo().
 */
void CmdDelete::undo()
{
	m_pModel->Paste( tgc::Shared, *m_coinParent, *m_coinNode, m_row );
}

/*!
 * Applies a change to the scene. After redo() the scene will not contain the node located on the model as selectedIndex.
 * \sa undo().
 */
void CmdDelete::redo( )
{
	m_pModel->Cut( *m_coinParent, m_row );
}
