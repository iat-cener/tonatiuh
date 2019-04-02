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

#include "gf.h"

#include "CmdInsertShape.h"
#include "SceneModel.h"
#include "TShape.h"
#include "TShapeKit.h"

/**
 * Creates a new shape insert command that adds a \a shape to \a shapekit node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertShape::CmdInsertShape( TShapeKit* shapeKit, TShape* shape, SceneModel* model, QUndoCommand * parent )
: QUndoCommand("InsertShape", parent), m_shapeKit(shapeKit), m_shape(shape), m_pModel( model ), m_row( -1 )
{
	if( m_shapeKit == 0 ) gf::SevereError( "CmdInsertShape called with NULL TShapeKit*" );
	if( m_shape == 0 ) gf::SevereError( "CmdInsertShape called with NULL TShape*" );
	m_shape->ref();
}

/*!
 * Destroys the CmdInsertShape object.
 */
CmdInsertShape::~CmdInsertShape()
{
	m_shape->unref();
}

/*!
 * Reverts model state. After undo() is called, the \a shapekit node will not contain a shape type node.
 * \sa redo().
 */
void CmdInsertShape::undo()
{
	m_shapeKit->setPart( "shape", NULL );
	m_pModel->RemoveCoinNode( m_row, *m_shapeKit );
}

/*!
 * Applies a change to the model. After redo() the model will contain new shape node located as \a shapekit type node child.
 * \sa undo().
 */
void CmdInsertShape::redo( )
{
	m_shapeKit->setPart( "shape", m_shape );
    m_row = m_pModel->InsertCoinNode( *m_shape, *m_shapeKit );
}
