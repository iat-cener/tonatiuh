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
 
#include "GraphicView.h"
#include "Trace.h"
#include "InstanceNode.h"
#include "SceneModel.h"
#include "PathWrapper.h"
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/SbViewportRegion.h> 
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedLineSet.h>

#include <QFont>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QWidget>

#include <QVariant>

GraphicView::GraphicView( QWidget* parent )
: QAbstractItemView( parent ), m_sceneGraphRoot(0)
{
    Trace trace( "GraphicView::GraphicView" );
}

void GraphicView::SetSceneGraph( SoSelection* sceneGraphRoot )
{
    Trace trace( "GraphicView::SetSceneGraph" );
    m_sceneGraphRoot = 	sceneGraphRoot;
    m_myRenderArea = new SoQtExaminerViewer( this );
    m_myRenderArea->setGLRenderAction( new SoBoxHighlightRenderAction() );
    m_myRenderArea->setSceneGraph( m_sceneGraphRoot );
   // m_myRenderArea->setBackgroundColor(SbColor(0,0,0));
    
    ViewCoordinateSystem( true );
}

SbViewportRegion GraphicView::GetViewportRegion() const
{
	return m_myRenderArea->getViewportRegion();
}

SoCamera* GraphicView::GetCamera() const
{
	return m_myRenderArea->getCamera();
}

QModelIndex GraphicView::indexAt( const QPoint& /*point*/ ) const
{
    Trace trace( "GraphicView::indexAt" );
    return QModelIndex();
}

void  GraphicView::scrollTo( const QModelIndex& /*index*/, ScrollHint /*hint*/ )
{
    Trace trace( "GraphicView::scrollTo" );
}

QRect GraphicView::visualRect ( const QModelIndex& /*index*/ ) const
{
    Trace trace( "GraphicView::GraphicView" );
    return QRect();
}

void GraphicView::ViewCoordinateSystem( bool view )
{
	Trace trace( "GraphicView::ViewCoordinateSystem" );
	m_myRenderArea->setFeedbackVisibility( view );
}

void GraphicView::ViewDecoration( bool view )
{
	Trace trace( "GraphicView::ViewDecorations" );	
	m_myRenderArea->setDecoration( view );	
}

void GraphicView::dataChanged( const QModelIndex&  /*topLeft*/, const QModelIndex&  /*bottomRight*/ )
{
    Trace trace( "GraphicView::dataChanged" );	
}

void GraphicView::rowsInserted( const QModelIndex& /*parent*/, int  /*start*/, int  /*end*/ )
{
    Trace trace( "GraphicView::rowsInserted" );
}

void GraphicView::rowsAboutToBeRemoved( const QModelIndex& /*parent*/, int  /*start*/, int  /*end*/ )
{
    Trace trace( "GraphicView::rowsAboutToBeRemoved" );	
}

void GraphicView::setSelection( const QRect&  /*rect*/, QItemSelectionModel::SelectionFlags  /*flags*/ )
{
    Trace trace( "GraphicView::setSelection" );	
}

int GraphicView::horizontalOffset() const
{
    Trace trace( "GraphicView::horizontalOffset" );	
	return 0;
}

int GraphicView::verticalOffset() const
{
    Trace trace( "GraphicView::verticalOffset" );
	return 0;
}

bool GraphicView::isIndexHidden( const QModelIndex& /*index*/ ) const
{
    Trace trace( "GraphicView::isIndexHidden" );	
    return false;
}

QModelIndex GraphicView::moveCursor( CursorAction /*cursorAction*/, Qt::KeyboardModifiers /*modifiers*/ )
{
    Trace trace( "GraphicView::moveCursor" );	
	return QModelIndex();
}

QRegion GraphicView::visualRegionForSelection( const QItemSelection& /*selection*/ ) const
{
    Trace trace( "GraphicView::visualRegionForSelection" );	
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
    	path = static_cast< SoFullPath*>( variant.value<PathWrapper>().GetPath() );
	    m_sceneGraphRoot->select( path );
	   
    }
    m_sceneGraphRoot->touch();
}
