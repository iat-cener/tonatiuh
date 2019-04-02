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

#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>

#include "gf.h"

#include "CmdLightKitModified.h"
#include "SceneModel.h"
#include "TLightKit.h"

/**
 * Creates a new lightKit modification command that represents a new light defined as \a newLightKit to the \a scene.
 *
 * If the model has not previous light a light node is added to \a sceneModel.
 */

CmdLightKitModified::CmdLightKitModified( TLightKit* newLightKit, SoSceneKit* scene, SceneModel& sceneModel, QUndoCommand* parent )
: QUndoCommand("Modify LightKit", parent),
  m_previousLightKit( false ),
  m_previousAzimuth( 0 ),
  m_previousDisbleNodes( "" ),
  m_previousZenith( 0 ),
  m_pPreviousSunShape( 0 ),
  m_pNewLightKit( 0 ),
  m_scene( scene ),
  m_pModel( &sceneModel )
{
    if( newLightKit == 0 ) gf::SevereError( "CmdLightKitModified called with NULL TLightKit*" );
    m_pNewLightKit = static_cast< TLightKit* >( newLightKit->copy( true ) );
    m_pNewLightKit->ref();

    if( m_scene->getPart("lightList[0]", false ) )
    {
    	m_previousLightKit = true;
    	TLightKit* lightKit = dynamic_cast< TLightKit* >( m_scene->getPart("lightList[0]", false ) );
    	if( lightKit )
    	{
			m_previousAzimuth = lightKit->azimuth.getValue();
			m_previousZenith = lightKit->zenith.getValue();
			m_previousDisbleNodes = QString( lightKit->disabledNodes.getValue().getString() );

			m_pPreviousSunShape = dynamic_cast< TSunShape* >( lightKit->getPart( "tsunshape", false )->copy( true ) );
			if( m_pPreviousSunShape ) m_pPreviousSunShape->ref();
    	}
    }
}

/*!
 * Destroys the CmdLightKitModified object.
 */
CmdLightKitModified::~CmdLightKitModified()
{
	m_pNewLightKit->unref();
	if( m_previousLightKit )
	{
		m_pPreviousSunShape->unref();
	}
}

/*!
 * Reverts to the previous light. After undo() is called, the state of the scene will be the same as before redo() was called.
 *  * \sa redo().
 */
void CmdLightKitModified::undo()
{
	if( m_previousLightKit )
    {
    	TLightKit* lightKit = static_cast< TLightKit* > ( m_scene->getPart("lightList[0]", false) );
    	lightKit->setPart("tsunshape", m_pPreviousSunShape );
   		lightKit->ChangePosition( m_previousAzimuth, m_previousZenith );
   		lightKit->disabledNodes.setValue( m_previousDisbleNodes.toStdString().c_str() );
    }
    else	m_pModel->RemoveLightNode( *m_pNewLightKit );
}

/*!
 * Applies a change to the light. After redo() scene will contain the light with the new definition.
 * \sa undo().
 */

void CmdLightKitModified::redo( )
{
	if( !m_previousLightKit )
   	{
   		 m_pModel->InsertLightNode( *m_pNewLightKit );
   	}
   	else
   	{
   		TLightKit* lightKit = static_cast< TLightKit* > ( m_scene->getPart("lightList[0]", false) );

   		TSunShape* sunhape =static_cast< TSunShape* > ( m_pNewLightKit->getPart("tsunshape", false) );
   		lightKit->setPart("tsunshape", sunhape );

   		lightKit->ChangePosition( m_pNewLightKit->azimuth.getValue(), m_pNewLightKit->zenith.getValue() );
   		lightKit->disabledNodes.setValue( m_pNewLightKit->disabledNodes.getValue() );
   	}

}
