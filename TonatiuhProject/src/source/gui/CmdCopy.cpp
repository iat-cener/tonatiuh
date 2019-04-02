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

#include "CmdCopy.h"
#include "InstanceNode.h"
#include "SceneModel.h"

/**
 * Creates a new copy command that represents the copy of the node located with \a index in the \a model to the \a clipborad.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdCopy::CmdCopy( const QModelIndex& index, SoNode*& clipboard, SceneModel* model, QUndoCommand* parent)
: QUndoCommand("Copy", parent), m_pClipboard ( clipboard ), m_pNode( 0 ),m_previousNode ( 0 ), m_pModel( model )
{
	InstanceNode* instanceNode = m_pModel->NodeFromIndex( index );
	m_pNode = instanceNode->GetNode();

	m_previousNode = clipboard ;
}

/*!
 * Destroys the CmdCopy object.
 */
CmdCopy::~CmdCopy()
{
}

/*!
 * Reverts clipboard contain. After undo() is called, the state of the clipboard will be the same as before redo() was called.
 * \sa redo().
 */
void CmdCopy::undo()
{
	m_pClipboard = m_previousNode;
}

/*!
 * Applies a change to the clipboard. After redo() clipboard will contain the node located on the model as index
 * \sa undo().
 */
void CmdCopy::redo( )
{
	m_pClipboard  = m_pNode;
}
