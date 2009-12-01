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

#include <iostream>

#include "CmdInsertTracker.h"
#include "InstanceNode.h"
#include "SceneModel.h"
#include "Trace.h"
#include "tgf.h"
#include "TLightKit.h"
#include "TTracker.h"

CmdInsertTracker::CmdInsertTracker( TTracker* tracker,  const QModelIndex& parentIndex, SoSceneKit* scene, SceneModel* model, QUndoCommand* parent )
: QUndoCommand("Insert Tracker", parent), m_tracker ( tracker ), m_coinParent( 0 ), m_scene( scene ), m_pModel( model ), m_row( 0 )
{
	if( !m_tracker ) tgf::SevereError( "CmdInsertTracker Null tracker." );
	m_tracker->ref();

	//if( !m_scene->getPart("lightList[0]", false) )	 tgf::SevereError( "CmdInsertTracker Null lightKit." );

	if( !parentIndex.isValid() ) tgf::SevereError( "CmdInsertTracker called with invalid ModelIndex." );
	InstanceNode* instanceParent = m_pModel->NodeFromIndex( parentIndex );
	if( !instanceParent->GetNode() ) tgf::SevereError( "CmdInsertTracker called with NULL parent node." );
	m_coinParent = static_cast< SoBaseKit* > ( instanceParent->GetNode() );
}

CmdInsertTracker::~CmdInsertTracker()
{
	m_tracker->unref();
}

void CmdInsertTracker::undo()
{
	m_tracker->Disconnect();
	m_pModel->RemoveCoinNode( m_row, *m_coinParent );
}

/**
 *Inserts the tracker in the scene model.
 */
void CmdInsertTracker::redo()
{
	TLightKit* lightKit = static_cast< TLightKit* >( m_scene->getPart("lightList[0]", false) );
	if( lightKit )
	{
		m_tracker->SetAzimuthAngle( &lightKit->azimuth );
		m_tracker->SetZenithAngle( &lightKit->zenith );
	}
	m_row = m_pModel->InsertCoinNode( *m_tracker, *m_coinParent );

}
