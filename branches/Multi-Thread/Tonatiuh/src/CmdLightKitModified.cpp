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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>

#include "CmdLightKitModified.h"
#include "SceneModel.h"
#include "tgf.h"
#include "TLightKit.h"
#include "Trace.h"


CmdLightKitModified::CmdLightKitModified( TLightKit* newLightKit, SoSceneKit* scene, SceneModel& sceneModel, QUndoCommand* parent )
: QUndoCommand("Modify LightKit", parent), m_pPreviousLightKit( 0 ), m_pNewLightKit( newLightKit ), m_scene( scene ), m_pModel( &sceneModel )
{
    Trace trace( "CmdLightKitModified::CmdLightKitModified", false );
	m_pNewLightKit->ref();

	TLightKit* lightKit = dynamic_cast< TLightKit* > ( m_scene->getPart("lightList[0]", false) );
	if( lightKit )	m_pPreviousLightKit = dynamic_cast< TLightKit* >( lightKit->copy( true ) );

}

CmdLightKitModified::~CmdLightKitModified()
{
	Trace trace( "CmdLightKitModified::~CmdLightKitModified", false );
	m_pNewLightKit->unref();
}

void CmdLightKitModified::undo()
{
	Trace trace( "CmdLightKitModified::undo", false );

    if( m_pPreviousLightKit )
    {
    	TLightKit* lightKit = dynamic_cast< TLightKit* > ( m_scene->getPart("lightList[0]", false) );

   		TShape* shape = dynamic_cast< TShape* > ( m_pPreviousLightKit->getPart("icon", false) );
   		lightKit->setPart("icon", shape );

   		TSunShape* sunhape = dynamic_cast< TSunShape* > ( m_pPreviousLightKit->getPart("tsunshape", false) );
   		lightKit->setPart("tsunshape", sunhape );
    }
    else
    {
    	m_pModel->RemoveLightNode( *m_pNewLightKit );
    }
}

void CmdLightKitModified::redo( )
{
	Trace trace( "CmdLightKitModified::redo", false );

   if( !m_pPreviousLightKit )
   	{
   		 m_pModel->InsertLightNode( *m_pNewLightKit );
   	}
   	else
   	{
   		TLightKit* lightKit = dynamic_cast< TLightKit* > ( m_scene->getPart("lightList[0]", false) );

   		TShape* shape =dynamic_cast< TShape* > ( m_pNewLightKit->getPart("icon", false) );
   		lightKit->setPart("icon", shape );

   		TSunShape* sunhape =dynamic_cast< TSunShape* > ( m_pNewLightKit->getPart("tsunshape", false) );
   		lightKit->setPart("tsunshape", sunhape );

   	}

}
