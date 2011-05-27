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


#include <Inventor/SoNodeKitPath.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/VRMLnodes/SoVRMLBackground.h>

#include <QApplication>
#include <QString>
#include <QStatusBar>

#include "Document.h"
#include "TSceneKit.h"

/*!
 * Creates a new document object.
 */
Document::Document()
:
  m_scene(0),
  m_isModified( false )
{
    InitializeScene();
}

/*!
 * Destroys the document object.
 */
Document::~Document()
{
}

/*!
 * Set the state of the document to \a status. If the status is true the document have been modified.
 */
void Document::SetDocumentModified( bool status )
{
    m_isModified = status;
}

/*!
 * Initializes a new empty scene a scene and sets it to the document.
 */
void Document::InitializeScene()
{
    if ( m_scene ) ClearScene();

	m_scene = new TSceneKit;
	m_scene->ref();
    m_scene->setSearchingChildren( true );
}

/*!
 * Initializes the document with a empty scene.
 */
void Document::New()
{
    InitializeScene();
	m_isModified = false;
}

/*!
 * Clears the scene of the document.
 */
void Document::ClearScene()
{
	if ( m_scene )
	{
		if ( m_scene )	while ( m_scene->getRefCount( ) > 1 )	m_scene->unref();
	}

    m_scene = 0;
}

/*!
 * Sets the scene form \a fileName to the document.
 */
bool Document::ReadFile( const QString& fileName )
{
    if( TSceneKit* inputScene = static_cast< TSceneKit* >( GetSceneKitFromFile( fileName ) ) )
	{
        if ( m_scene ) ClearScene();
	    m_scene = inputScene;
	    m_scene->setSearchingChildren( true );
	    m_isModified = false;
	    return true;
	}
	return false;
}

/*!
 * Writes the document scene to a file with the given \a fileName.
 *
 * Returns true if the scene was successfully written; otherwise returns false.
 */
bool Document::WriteFile( const QString& fileName )
{
    SoWriteAction SceneOuput;
    if ( !SceneOuput.getOutput()->openFile( fileName.toLatin1().constData() ) )
	{
		QString message = QString( "Cannot open file %1." ).arg( fileName );
		emit Warning( message );

   		return false;
   	}

    QApplication::setOverrideCursor( Qt::WaitCursor );
   	SceneOuput.getOutput()->setBinary( false );
   	SceneOuput.apply( m_scene );
   	SceneOuput.getOutput()->closeFile();
   	QApplication::restoreOverrideCursor();
   	m_isModified = false;
	return true;
}

/*!
 * Returns whether the scene was modified.
 */
bool Document::IsModified( )
{
    return m_isModified;
}

/*!
 * Returns the document scene.
 */
TSceneKit* Document::GetSceneKit() const
{
    return m_scene;
}

/*!
 * Reads the scene saved on the file with given \a filename and return a pointer to the scene.
 *
 * Returns null on any error.
 */
TSceneKit* Document::GetSceneKitFromFile( const QString& fileName )
{
    SoInput sceneInput;
	if ( !sceneInput.openFile( fileName.toLatin1().constData() ) )
	{
		QString message = QString( "Cannot open file %1." ).arg( fileName );
		emit Warning( message );

		return 0;
	}

	if( !sceneInput.isValidFile() )
	{
		QString message = QString( "Error reading file %1.\n" ).arg( fileName );
		emit Warning( message );

		return 0;
	}

	SoSeparator* graphSeparator = SoDB::readAll( &sceneInput );
	sceneInput.closeFile();

	if ( !graphSeparator )
	{
		QString message = QString( "Error reading file %1.\n" ).arg( fileName );
		emit Warning( message );

		return 0;
	}

   return static_cast< TSceneKit* >( graphSeparator->getChild(0) );
	return 0;
}
