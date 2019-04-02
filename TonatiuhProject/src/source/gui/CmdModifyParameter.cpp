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

//!  CmdModifyParameter class is the parameter modification command stored in the command stack.
/*!
  CmdModifyParameter represents a modification of the scene element parameter. The modification could be for any element of the scene concentrator parameters.
  This class saves the new value of the parameter and old one to restore the previous state.
   \sa CmdLightPositionModified
*/

#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/nodes/SoNode.h>

#include "gf.h"

#include "CmdModifyParameter.h"
#include "InstanceNode.h"
#include "SceneModel.h"

/**
 * CmdModifyParameter creates a new command that represents a scene node parameter value modification. This object saves the \a parameterName parameter value of \a nodeIndex node and the new one
 * defined as \a parameterValue to apply and restore them.
 *
 * The \a parent is command object parent.
 */
CmdModifyParameter::CmdModifyParameter(  SoNode* node, QString parameterName, QString parameterValue, SceneModel* model, QUndoCommand* parent )
:QUndoCommand( parent ),
 m_coinNode(  node ),
 m_newValue ( parameterValue ),
 m_oldValue( "" ),
 m_parameterName( parameterName ),
 m_pModel( model )
{
	if( !m_coinNode )		gf::SevereError( "CmdModifyParameter called with invalid node." );

	SoField* parameterField = m_coinNode->getField( SbName( m_parameterName.toStdString().c_str() ) );
	if( !parameterField )	gf::SevereError( "CmdModifyParameter called with invalid parameterName." );

	SbString fieldValue;
	parameterField->get( fieldValue );
	m_oldValue = QString( fieldValue.getString() );


}

/*!
 * Destroys CmdModifyParameter object.
 */
CmdModifyParameter::~CmdModifyParameter()
{

}

/*!
 * Applies a change to the scene. After undo() the node parameter value will be changed to \a m_oldValue defined in the constructor.
 * \sa undo().
 */
void CmdModifyParameter::undo()
{
	SoField* parameterField = m_coinNode->getField( m_parameterName.toStdString().c_str() );
	if( parameterField )	parameterField->set( m_oldValue.toStdString().c_str() );

}

/*!
 * Applies a change to the scene. After redo() the node parameter value will be changed to \a m_newValue defined in the constructor.
 * \sa undo().
 */
void CmdModifyParameter::redo()
{

	SoField* parameterField = m_coinNode->getField( m_parameterName.toStdString().c_str() );
	if( parameterField )
	{
		parameterField->set( m_newValue.toStdString().c_str() );
	}

}

