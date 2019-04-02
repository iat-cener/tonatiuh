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

#include "CmdInsertMaterial.h"
#include "SceneModel.h"
#include "TMaterial.h"
#include "TShapeKit.h"

/**
 * Creates a new material insert command that adds a \a material to \a shapekit node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertMaterial::CmdInsertMaterial( TShapeKit* shapeKit, TMaterial* material, SceneModel* model, QUndoCommand * parent )
: QUndoCommand("InsertMaterial", parent), m_shapeKit( shapeKit ),m_material( material ), m_pModel( model ), m_row( -1 )
{
	if( m_shapeKit == 0 ) gf::SevereError( "CmdInsertMaterial called with NULL TShapeKit" );
	if( m_material == 0 ) gf::SevereError( "CmdInsertMaterial called with NULL TMaterial" );
	m_material->ref();

}

/*!
 * Destroys the CmdInsertMaterial object.
 */
CmdInsertMaterial::~CmdInsertMaterial()
{
	m_material->unref();
}

/*!
 * Reverts model state. After undo() is called, the shapekit node will not contain a material type node.
 * \sa redo().
 */
void CmdInsertMaterial::undo()
{
    m_shapeKit->setPart( "material", NULL );
	m_pModel->RemoveCoinNode( m_row, *m_shapeKit );
}

/*!
 * Applies a change to the model. After redo() the model will contain new material node located as shapekit type node child.
 * \sa undo().
 */
void CmdInsertMaterial::redo( )
{
	m_shapeKit->setPart( "material", m_material );
    m_row = m_pModel->InsertCoinNode( *m_material, *m_shapeKit );
}
