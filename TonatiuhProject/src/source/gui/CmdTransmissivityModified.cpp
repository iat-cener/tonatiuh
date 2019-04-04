/***************************************************************************
 Copyright (C) 2008 by the Tonatiuh Software Development Team.

 This file is part of Tonatiuh.

 Tonatiuh program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.


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

 Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola, Gilda Jimenez,
 Juana Amieva, Azael Mancillas, Cesar Cantu.
 ***************************************************************************/

#include "gf.h"

#include "CmdTransmissivityModified.h"
#include "TSceneKit.h"
#include "TTransmissivity.h"

/*!
 * Creates a new transmissivity definition command. The
 */
CmdTransmissivityModified::CmdTransmissivityModified( TTransmissivity* newTransmissivity, TSceneKit* scene, QUndoCommand* parent )
:QUndoCommand( "Transmissivity changed", parent ),
 m_isPreviousTransmissivity( false ),
 m_pNewTransmissivity( 0 ),
 m_scene( scene )
{
    if( newTransmissivity )
    {
    	m_pNewTransmissivity = static_cast< TTransmissivity* >( newTransmissivity->copy( true ) );
    	m_pNewTransmissivity->ref();
    }

    if( m_scene->getPart("transmissivity", false ) )
    {
    	m_isPreviousTransmissivity = true;
    	m_pPreviousTransmissivity = dynamic_cast< TTransmissivity* >( m_scene->getPart("transmissivity", false ) );
    	if( m_pPreviousTransmissivity )	m_pPreviousTransmissivity->ref();

    }
}

/*!
 * Destroys the CmdLightPositionModified object.
 */
CmdTransmissivityModified::~CmdTransmissivityModified()
{
	//if( m_pNewTransmissivity && m_pNewTransmissivity->getRefCount() > 0 ) 	m_pNewTransmissivity->unref();
	//if( m_pPreviousTransmissivity && m_pPreviousTransmissivity->getRefCount() > 0 ) m_pPreviousTransmissivity->unref();
}

/*!
 * Reverts to the previous transmissivity definition. After undo() is called, the scene transmissivityn will be the same as before redo() was called.
 *
 * \sa redo().
 */
void CmdTransmissivityModified::undo()
{
	if( m_isPreviousTransmissivity )	m_scene->setPart( "transmissivity", m_pPreviousTransmissivity );
	else	m_scene->setPart( "transmissivity", 0 );
}

/*!
 * Applies a change to the scene. After redo() the transmissivity will be new transmissivity defined by the constructor parameters.
 * \sa undo().
 */
void CmdTransmissivityModified::redo()
{

	if( m_pNewTransmissivity )	m_scene->setPart( "transmissivity", m_pNewTransmissivity );
	else	m_scene->setPart( "transmissivity", 0 );

}
