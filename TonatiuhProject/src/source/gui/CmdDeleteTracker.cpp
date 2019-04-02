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

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "gf.h"

#include "CmdDeleteTracker.h"
#include "InstanceNode.h"
#include "SceneModel.h"
#include "TLightKit.h"
#include "TSeparatorKit.h"
#include "TTracker.h"

/*!
 * Contructor.
 */
CmdDeleteTracker::CmdDeleteTracker( const QModelIndex& selectedIndex, SoSceneKit* scene, SceneModel& model, QUndoCommand* parent )
: QUndoCommand("Delete", parent), m_tracker( 0 ),  m_coinParent( 0 ),  m_scene( scene ), m_pModel( &model ), m_row( 0 )
{
	//if( !m_scene->getPart("lightList[0]", false) )	 gf::SevereError( "CmdDeleteTracker Null lightKit." );

	if( !selectedIndex.isValid() ) gf::SevereError( "CmdDeleteTracker called with invalid ModelIndex." );
	InstanceNode* instanceSelection = m_pModel->NodeFromIndex( selectedIndex );

	if( !instanceSelection->GetNode() ) gf::SevereError( "CmdDeleteTracker called with NULL selection node." );
	m_coinParent = static_cast< TSeparatorKit* > ( instanceSelection->GetParent()->GetNode() );
	if( !m_coinParent ) gf::SevereError( "CmdDeleteTracker called with invalid tracker parent." );

	m_tracker = dynamic_cast<TTracker* > ( m_coinParent->getPart( "tracker", false) );
	if( !m_tracker ) gf::SevereError( "CmdDeleteTracker Null tracker." );
	m_tracker->ref();

	m_row = instanceSelection->GetParent()->children.indexOf( instanceSelection );

}

/*!
 * Destructor.
 */
CmdDeleteTracker::~CmdDeleteTracker()
{
	m_tracker->unref();
}

/*!
 * Restores the tracker to the state before deletion.
 * \sa redo
 */
void CmdDeleteTracker::undo()
{
	SoTransform* parentTransform = static_cast< SoTransform* > ( m_coinParent->getPart("transform", true ) );
	if( !parentTransform ) gf::SevereError( "CmdInsertTracker Null node transform." );

	TLightKit* lightKit = static_cast< TLightKit* >( m_scene->getPart("lightList[0]", false) );
	if( lightKit )
	{
		m_tracker->SetAzimuthAngle( &lightKit->azimuth );
		m_tracker->SetZenithAngle( &lightKit->zenith );
	}

	m_tracker->ConnectParentTranform(parentTransform);
	m_pModel->Paste( tgc::Shared, *m_coinParent, *m_tracker, m_row );
}

/**
 * Deletes a tracker from the scene.
 * \sa undo
 */
void CmdDeleteTracker::redo( )
{
	SoTransform* transform = static_cast< SoTransform* > ( m_coinParent->getPart("transform", true ) );
	transform->translation.disconnect();
	transform->rotation.disconnect();
	transform->scaleFactor.disconnect();
	transform->scaleOrientation.disconnect();
	transform->center.disconnect();
	//m_tracker->Disconnect();

	m_pModel->Cut( *m_coinParent, m_row );

}
