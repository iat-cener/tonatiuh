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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>
#include <QVariant>

#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include "GraphicRoot.h"
#include "GraphicView.h"
#include "PathWrapper.h"

/**
 * Creates a new GraphicView with a \a parerent for the model data 3D representation.
 *
 * Use setModel() to set the model.
 */
GraphicView::GraphicView( QWidget* parent )
:QAbstractItemView( parent ),
 m_sceneGraphRoot( 0 ),
 m_myRenderArea( 0 )
{

}

/*!
* Destroys graphic view object.
*/
GraphicView::~GraphicView()
{
    delete m_myRenderArea;
}

/*!
* Sets view scene to \a sceneGraphRoot.
*/
void GraphicView::SetSceneGraph( GraphicRoot* sceneGraphRoot )
{

    m_sceneGraphRoot = 	sceneGraphRoot;
    m_myRenderArea = new SoQtExaminerViewer( this );
    m_myRenderArea->setGLRenderAction( new SoBoxHighlightRenderAction() );
    m_myRenderArea->setTransparencyType( SoGLRenderAction::SORTED_OBJECT_BLEND );
    m_myRenderArea->setSceneGraph( m_sceneGraphRoot->GetNode() );

    ViewCoordinateSystem( true );
}

/*!
* Returns the render area region.
*/
SbViewportRegion GraphicView::GetViewportRegion() const
{
	return m_myRenderArea->getViewportRegion();
}

/*!
* Returns the render area camera.
*/

SoCamera* GraphicView::GetCamera() const
{
	return m_myRenderArea->getCamera();
}

/*!
* This function has no effect.
*/
QModelIndex GraphicView::indexAt( const QPoint& /*point*/ ) const
{
    return QModelIndex();
}

/*!
* This function has no effect.
*/
void  GraphicView::scrollTo( const QModelIndex& /*index*/, ScrollHint /*hint*/ )
{
}

/*!
* This function an empty rect.
*/
QRect GraphicView::visualRect ( const QModelIndex& /*index*/ ) const
{
    return QRect();
}

/*!
* This function has no effect.
*/
void GraphicView::ViewCoordinateSystem( bool view )
{
	m_myRenderArea->setFeedbackVisibility( view );
}

void GraphicView::ViewDecoration( bool view )
{
	m_myRenderArea->setDecoration( view );
}

/*!
* This function has no effect.
*/
int GraphicView::horizontalOffset() const
{
	return 0;
}

/*!
* This function has no effect.
*/
bool GraphicView::isIndexHidden( const QModelIndex& /*index*/ ) const
{
    return false;
}

/*!
* This function has no effect.
*/
QModelIndex GraphicView::moveCursor( CursorAction /*cursorAction*/, Qt::KeyboardModifiers /*modifiers*/ )
{
	return QModelIndex();
}

/*!
* Item selection is not supported in this view, is implemented in coin. As a result, this function has no effect
*/
void GraphicView::setSelection( const QRect&  /*rect*/, QItemSelectionModel::SelectionFlags  /*flags*/ )
{

}

/*!
* Returns zero offset of the view, in this view there is no scroll area.

*/
int GraphicView::verticalOffset() const
{
	return 0;
}

/*!
* Returns the region from the viewport of the items in the given \a selection.
* This method is required becouse the class inherits QAbstractItemView. In this class, returns an empty region.
*/
QRegion GraphicView::visualRegionForSelection( const QItemSelection& /*selection*/ ) const
{
	return QRegion();
}

/*!
* When an item becomes the current item, preivious selections are removed and the node with the index \a current is selected into coin.
*/
void GraphicView::currentChanged( const QModelIndex & current, const QModelIndex& /*previous*/ )
{
	if( m_sceneGraphRoot )
	{
		m_sceneGraphRoot->DeselectAll();

		SoFullPath* path;
		QVariant variant = current.data(Qt::UserRole);

		if ( variant.canConvert<PathWrapper>() )
		{
			path = static_cast< SoFullPath*>( variant.value< PathWrapper >().GetPath() );
			m_sceneGraphRoot->Select( path );
		}
	}
}
