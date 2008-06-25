/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
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
