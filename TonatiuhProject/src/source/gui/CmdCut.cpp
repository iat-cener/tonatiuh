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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <Inventor/nodekits/SoBaseKit.h>

#include "InstanceNode.h"
#include "CmdCut.h"
#include "SceneModel.h"

/**
 * Creates a new cut command that represents the cut the node located with \a index from the \a model.
 * The node is stored at the \a clipborad.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdCut::CmdCut( const QModelIndex& selectedIndex, SoNode*& clipboard, SceneModel* model, QUndoCommand* parent )
: QUndoCommand("Cut", parent), m_pClipboard ( clipboard ), m_previousNode ( 0 ), m_coinNode( 0 ), m_coinParent( 0 ), m_pModel( model ), m_row ( -1 )
{
	InstanceNode* instanceNode = m_pModel->NodeFromIndex( selectedIndex );
	m_coinNode = instanceNode->GetNode();
	m_coinNode->ref();
	m_coinParent = static_cast< SoBaseKit* > (instanceNode->GetParent()->GetNode() );

	m_previousNode = clipboard ;

	m_row = instanceNode->GetParent()->children.indexOf( instanceNode );
}

/*!
 * Destroys the CmdCut object.
 */
CmdCut::~CmdCut()
{
	m_coinNode->unref();
}

/*!
 * Reverts clipboard and scene contain. After undo() is called, the state of the scene and clipboard will be the same as before redo() was called.
 * \sa redo().
 */
void CmdCut::undo()
{
	m_pClipboard = m_previousNode;
	m_pModel->Paste( tgc::Shared, *m_coinParent, *m_coinNode, m_row );
}

/*!
 * Applies a change to the scene and clipboard. After redo() clipboard will contain the node located on the model as index
 * and the node will remove from the scene.
 * \sa undo().
 */
void CmdCut::redo( )
{
	m_pClipboard = m_coinNode;
	m_pModel->Cut( *m_coinParent, m_row );
}
