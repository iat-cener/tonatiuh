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

#include <QVariant>

#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include "GraphicView.h"
#include "PathWrapper.h"
#include "Trace.h"


GraphicView::GraphicView( QWidget* parent )
:QAbstractItemView( parent ), m_sceneGraphRoot( 0 ), m_myRenderArea( 0 )
{
    Trace trace( "GraphicView::GraphicView", false );
}

GraphicView::~GraphicView()
{
    Trace trace( "GraphicView::~GraphicView", false );
    delete m_myRenderArea;
}

void GraphicView::SetSceneGraph( SoSelection* sceneGraphRoot )
{
    Trace trace( "GraphicView::SetSceneGraph", false );

    m_sceneGraphRoot = 	sceneGraphRoot;
    m_myRenderArea = new SoQtExaminerViewer( this );
    m_myRenderArea->setGLRenderAction( new SoBoxHighlightRenderAction() );
    m_myRenderArea->setSceneGraph( m_sceneGraphRoot );

    ViewCoordinateSystem( true );
}

SbViewportRegion GraphicView::GetViewportRegion() const
{
	Trace trace( "GraphicView::GetViewportRegion", false );
	return m_myRenderArea->getViewportRegion();
}

SoCamera* GraphicView::GetCamera() const
{
	Trace trace( "GraphicView::GetCamera", false );
	return m_myRenderArea->getCamera();
}

QModelIndex GraphicView::indexAt( const QPoint& /*point*/ ) const
{
    Trace trace( "GraphicView::indexAt", false );
    return QModelIndex();
}

void  GraphicView::scrollTo( const QModelIndex& /*index*/, ScrollHint /*hint*/ )
{
    Trace trace( "GraphicView::scrollTo", false );
}

QRect GraphicView::visualRect ( const QModelIndex& /*index*/ ) const
{
    Trace trace( "GraphicView::GraphicView", false );

    return QRect();
}

void GraphicView::ViewCoordinateSystem( bool view )
{
	Trace trace( "GraphicView::ViewCoordinateSystem", false );

	m_myRenderArea->setFeedbackVisibility( view );
}

void GraphicView::ViewDecoration( bool view )
{
	Trace trace( "GraphicView::ViewDecorations", false );
	m_myRenderArea->setDecoration( view );
}

void GraphicView::dataChanged( const QModelIndex&  /*topLeft*/, const QModelIndex&  /*bottomRight*/ )
{
    Trace trace( "GraphicView::dataChanged", false );
}

void GraphicView::rowsInserted( const QModelIndex& /*parent*/, int  /*start*/, int  /*end*/ )
{
    Trace trace( "GraphicView::rowsInserted", false );
}

void GraphicView::rowsAboutToBeRemoved( const QModelIndex& /*parent*/, int  /*start*/, int  /*end*/ )
{
    Trace trace( "GraphicView::rowsAboutToBeRemoved", false );
}

void GraphicView::setSelection( const QRect&  /*rect*/, QItemSelectionModel::SelectionFlags  /*flags*/ )
{
    Trace trace( "GraphicView::setSelection", false );
}

int GraphicView::horizontalOffset() const
{
    Trace trace( "GraphicView::horizontalOffset", false );
	return 0;
}

int GraphicView::verticalOffset() const
{
    Trace trace( "GraphicView::verticalOffset", false );
	return 0;
}

bool GraphicView::isIndexHidden( const QModelIndex& /*index*/ ) const
{
    Trace trace( "GraphicView::isIndexHidden", false );
    return false;
}

QModelIndex GraphicView::moveCursor( CursorAction /*cursorAction*/, Qt::KeyboardModifiers /*modifiers*/ )
{
    Trace trace( "GraphicView::moveCursor",false );
	return QModelIndex();
}

QRegion GraphicView::visualRegionForSelection( const QItemSelection& /*selection*/ ) const
{
    Trace trace( "GraphicView::visualRegionForSelection", false );
	return QRegion();
}

void GraphicView::currentChanged( const QModelIndex & current, const QModelIndex& /*previous*/ )
{
	Trace trace( "GraphicView::currentChanged", false );

    m_sceneGraphRoot->deselectAll();
    SoFullPath* path;
    QVariant variant = current.data(Qt::UserRole);
    if ( variant.canConvert<PathWrapper>() )
    {
    	path = static_cast< SoFullPath*>( variant.value< PathWrapper >().GetPath() );
	    m_sceneGraphRoot->select( path );

    }
    m_sceneGraphRoot->touch();
}
