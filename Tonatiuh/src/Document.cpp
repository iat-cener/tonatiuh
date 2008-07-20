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

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/VRMLnodes/SoVRMLBackground.h>
#include <Inventor/actions/SoWriteAction.h>

#include <QApplication>
#include <QString>
#include <QMessageBox>
#include <QStatusBar>

#include "Document.h"
#include "TLightKit.h"
#include "TShapeKit.h"
#include <Inventor/SoNodeKitPath.h>
#include "Trace.h"

void selectionFinishCallback( void * userData, SoSelection* selection )
{
	Trace trace( "selectionFinishCallback", false );
    Document* document = static_cast< Document* >( userData  );
    if ( document ) document->selectionFinishCBAux( selection );
}

Document::Document()
: m_root(0), m_scene(0), m_isModified(false)
{
    Trace trace( "Document::Document" );
	m_root = new SoSelection;
	m_root->ref();

    m_root->policy = SoSelection::SINGLE;
    m_root->addFinishCallback( selectionFinishCallback, static_cast< void*>( this ) );

    InitializeScene();
}

Document::~Document()
{
	Trace trace( "Document::~Document" );
	if ( m_root ) m_root->unref();
}

void Document::SetDocumentModified( bool status )
{
    Trace trace( "Document::SetDocumentModified" );
    m_isModified = status;
}

void Document::InitializeScene()
{
    Trace trace( "Document::InitializeScene", false );
	if ( m_scene ) ClearScene();

	m_scene = new SoSceneKit;
	m_root->addChild( m_scene );
    m_scene->setSearchingChildren( true );
}

void Document::New()
{
    Trace trace( "Document::New", false );
	InitializeScene();
	m_isModified = false;
}

void Document::ClearScene()
{
    Trace trace( "Document::ClearScene", false );
    if( m_root )
    {
    	if ( m_scene )
    	{
    	    if ( m_root ) m_root->removeChild( m_scene );
   	        if ( m_scene ) while ( m_scene->getRefCount( ) > 1 ) m_scene->unref();
    	}
    }

    m_scene = 0;
}

bool Document::ReadFile( const QString& fileName )
{
    Trace trace( "Document::ReadFile" );
	if( SoSceneKit* inputScene = GetSceneKitFromFile( fileName ) )
	{
        if ( m_scene ) ClearScene();
	    m_scene = inputScene;
	    m_root->addChild( m_scene );
	    m_scene->setSearchingChildren( true );
	    m_isModified = false;
	    return true;
	}
	return false;
}

bool Document::WriteFile( const QString& fileName )
{
    Trace trace( "Document::WriteFile" );
    SoWriteAction SceneOuput;
    if ( !SceneOuput.getOutput()->openFile( fileName.toLatin1().constData() ) )
	{
        QMessageBox::warning( 0, tr( "Tonatiuh" ),
                              tr( "Cannot open file %1:\n%2. " )
                            .arg( fileName ));
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

bool Document::IsModified( )
{
    Trace trace( "Document::IsModified" );
    return m_isModified;
}

SoSelection* Document::GetRoot() const
{
    Trace trace( "Document::GetRoot" );
	return m_root;
}

SoSceneKit* Document::GetSceneKit() const
{
    Trace trace( "Document::GetSceneKit" );
    return m_scene;
}


SoSceneKit* Document::GetSceneKitFromFile( const QString& fileName )
{
    Trace trace( "Document::GetSceneKitFromFile" );
	SoInput sceneInput;
	if ( !sceneInput.openFile( fileName.toLatin1().constData() ) )
	{
        QMessageBox::warning( 0, tr( "Scene Graph Structure" ),
                              tr( "Cannot open file %1:\n%2." )
                            .arg( fileName ) );
		return 0;
	}

	SoSeparator* graphSeparator = SoDB::readAll( &sceneInput );
	sceneInput.closeFile();

	if ( !graphSeparator )
	{
        QMessageBox::warning( 0, tr( "Scene Graph Structure" ),
                              tr( "Error reading file %1:\n%2." )
                             .arg( fileName ) );
		return 0;
	}

   return static_cast< SoSceneKit* >( graphSeparator->getChild(0) );
}

void Document::selectionFinishCBAux( SoSelection* selection )
{
    Trace trace( "Document::selectionFinishCBAux", false );
	emit selectionFinish( selection );
}
