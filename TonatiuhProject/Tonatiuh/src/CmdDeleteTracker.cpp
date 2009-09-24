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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "CmdDeleteTracker.h"
#include "InstanceNode.h"
#include "SceneModel.h"
#include "tgf.h"
#include "TLightKit.h"
#include "Trace.h"
#include "TTracker.h"


CmdDeleteTracker::CmdDeleteTracker( const QModelIndex& selectedIndex, SoSceneKit* scene, SceneModel& model, QUndoCommand* parent )
: QUndoCommand("Delete", parent), m_tracker( 0 ),  m_coiNode( 0 ),  m_scene( scene ), m_pModel( &model )
{
	Trace trace( "CmdDeleteTracker::CmdDeleteTracker", false );



	if( !m_scene->getPart("lightList[0]", false) )	 tgf::SevereError( "CmdDeleteTracker Null lightKit." );

	if( !selectedIndex.isValid() ) tgf::SevereError( "CmdDeleteTracker called with invalid ModelIndex." );
	InstanceNode* instanceSelection = m_pModel->NodeFromIndex( selectedIndex );
	if( !instanceSelection->GetNode() ) tgf::SevereError( "CmdDeleteTracker called with NULL selection node." );
	m_coiNode = static_cast< SoBaseKit* > ( instanceSelection->GetNode() );

	SoTransform* transformNode = dynamic_cast< SoTransform* > ( m_coiNode->getPart( "transform", false ) );
	if( !transformNode  )	 tgf::SevereError( "CmdDeleteTracker Null node transform." );

	if( !transformNode->rotation.isConnected() ) tgf::SevereError( "CmdDeleteTracker Null tracker." );

	SoEngineOutput* outputEngine;
	transformNode->rotation.getConnectedEngine( outputEngine );
	if( !outputEngine  )	tgf::SevereError( "CmdDeleteTracker Null tracker." );

	m_tracker = dynamic_cast<TTracker* > ( outputEngine->getContainer() );
	if( !m_tracker ) tgf::SevereError( "CmdDeleteTracker Null tracker." );
	m_tracker->ref();

}

CmdDeleteTracker::~CmdDeleteTracker()
{
	Trace trace( "CmdDeleteTracker::~CmdDeleteTracker", false );

	m_tracker->unref();
}

void CmdDeleteTracker::undo()
{
	Trace trace( "CmdDeleteTracker::undo", false );

	TLightKit* lightKit = static_cast< TLightKit* >( m_scene->getPart("lightList[0]", false) );
	SoTransform* lightTransform = static_cast< SoTransform* > ( lightKit->getPart("transform", true ) );
	if( !lightTransform ) tgf::SevereError( "CmdInsertTracker Null lightTransform." );
	SoTransform* parentTransform = static_cast< SoTransform* > ( m_coiNode->getPart("transform", true ) );
	if( !parentTransform ) tgf::SevereError( "CmdInsertTracker Null node transform." );

	m_tracker->inputRotation.connectFrom( &lightTransform->rotation );
	parentTransform->rotation.connectFrom( &m_tracker->outputRotation );
}

void CmdDeleteTracker::redo( )
{
	Trace trace( "CmdDeleteTracker::redo", false );

	SoTransform* transform = static_cast< SoTransform* > ( m_coiNode->getPart("transform", true ) );
	transform->rotation.disconnect();
	m_tracker->inputRotation.disconnect();

}
