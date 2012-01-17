/*
 * CmdTransmissivityModified.cpp
 *
 *  Created on: 16/01/2012
 *      Author: amutuberria
 */

#include "CmdTransmissivityModified.h"
#include "TSceneKit.h"
#include "TTransmissivity.h"

/*!
 * Creates a new transmissivity definition command. The
 */
CmdTransmissivityModified::CmdTransmissivityModified( TTransmissivity* newTransmissivity, TSceneKit* scene, QUndoCommand* parent )
:QUndoCommand( "Transmissivity changed", parent ),
 m_isPreviousTransmissivity( false ),
 m_scene( scene )
{
    if( newTransmissivity == 0 ) tgf::SevereError( "CmdTransmissivityModified called with NULL TTransmissivity " );
    m_pNewTransmissivity = static_cast< TTransmissivity* >( newTransmissivity->copy( true ) );
    m_pNewTransmissivity->ref();

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

	m_scene->setPart( "transmissivity", m_pNewTransmissivity );

}
