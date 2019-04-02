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
***************************************************************************/

#include <QApplication>
#include <QDrag>
#include <QLineEdit>
#include <QMimeData>
#include <QMouseEvent>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>

#include "InstanceNode.h"
#include "SceneModelView.h"
#include "SceneModel.h"
#include "TSeparatorKit.h"
#include "TShapeKit.h"

/**
 * Creates a new view for a model. This
 */
SceneModelView::SceneModelView( QWidget *parent )
: QTreeView(parent),
  m_currentIndex( ),
  m_iteimsDelegate( 0 )
{

	m_iteimsDelegate = new NodeNameDelegate( );
	setItemDelegate( m_iteimsDelegate );

    //setAcceptDrops(true);

	connect( this, SIGNAL( collapsed( const QModelIndex& ) ),
			this, SLOT ( resizeViewToContents ( const QModelIndex& ) ) );
	connect( this, SIGNAL( expanded ( const QModelIndex& ) ),
			this, SLOT ( resizeViewToContents ( const QModelIndex& ) ) );

}

/*!
 * Destoryes view object.
 */
SceneModelView::~SceneModelView( )
{
	delete m_iteimsDelegate;
}

/**
 * Not yet docummented
 */
void SceneModelView::mousePressEvent(QMouseEvent *event)
{
	
    if (event->button() == Qt::LeftButton){
        startPos = event->pos();
    }
    else if( event->button() == Qt::RightButton)
    {
    	emit showMenu( indexAt( event->pos() ) );	
    }
    QTreeView::mousePressEvent(event);
}

/**
 * Not yet docummented
 */
void SceneModelView::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if( distance >= QApplication::startDragDistance() )
            startDrag(event);
    }
   // QTreeView::mouseMoveEvent(event);
}

/**
 * Not yet docummented
 */
void SceneModelView::dragEnterEvent(QDragEnterEvent *event)
{
	event->setDropAction(Qt::MoveAction);
	event->accept();
}

/**
 * Not yet docummented
 */
void SceneModelView::dragMoveEvent(QDragMoveEvent *event)
{
	event->setDropAction(Qt::MoveAction);
	event->accept();
}

/**
 * Not yet docummented
 */
void SceneModelView::dropEvent(QDropEvent *event)
{
	QModelIndex newParent= indexAt(event->pos());
    if ( newParent.isValid() )
    {
    	SceneModel* modelScene = static_cast< SceneModel* >(model());
		QString type = modelScene->NodeFromIndex( newParent )->GetNode()->getTypeId().getName().getString();
		
		SoNode* parentNode = modelScene->NodeFromIndex( newParent )->GetNode();
		if (! parentNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) return;
	
		QByteArray data = event->mimeData()->data("text/objetID");
		QDataStream stream(&data, QIODevice::ReadOnly);
		QPoint position;
		stream >> position;
			
		QModelIndex nodeIndex = indexAt( position );
		
		if ( modelScene->parent( nodeIndex ) == newParent ) return;
		
		SoNode* childNode = modelScene->NodeFromIndex( nodeIndex )->GetNode();
		
			
		if ( ( parentNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) ) && 
			 !( childNode->getTypeId().isDerivedFrom( SoSeparatorKit::getClassTypeId() ) ) )  return;
			 
		if ( ( parentNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) && 
			 ( childNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) )  return;
			 
		if ( ( parentNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) &&
			 !( childNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) ) 
		{
			
			TShapeKit* shapeKit = static_cast< TShapeKit* > ( parentNode );
			if( shapeKit->getPart("shape", false) ) return;
		}
			
		QModelIndex ancestor = newParent;
		while(ancestor.parent().isValid())
		{
			if(ancestor.parent() == nodeIndex)
				return;
			else
				ancestor = ancestor.parent();	
		}
			
		if ( nodeIndex != newParent )
		{
			if (event->keyboardModifiers () == Qt::ControlModifier)
     			emit dragAndDropCopy(newParent, nodeIndex);
     			
     		else
     			emit dragAndDrop(newParent, nodeIndex);
     		
			event->acceptProposedAction();
			event->accept();
    	}
    }

}


/*!
 * Sets \a current as the view current element index.
 */
void SceneModelView::currentChanged( const QModelIndex& current, const QModelIndex& previous )
{
	m_currentIndex = current;
	QTreeView::currentChanged( current, previous );
}

void SceneModelView::closeEditor( QWidget* editor, QAbstractItemDelegate::EndEditHint hint )
{

	QLineEdit* textEdit = qobject_cast<QLineEdit *>(editor);
	QString	newValue = textEdit->text();

	 emit nodeNameModificated( m_currentIndex, newValue );
	 QTreeView::closeEditor( editor, hint );
}


/**
 * Resizes the view to the size of its contents.
 */
void SceneModelView::resizeViewToContents( const QModelIndex& index )
{
	resizeColumnToContents( index.column() );
}

/**
 * Not yet docummented
 */
void SceneModelView::startDrag(QMouseEvent *event)
{
	QPoint position = event->pos();
	QModelIndex index = indexAt(position);
	
	if( index.parent() == rootIndex() ) return;
	
	QByteArray itemPos;
	QDataStream stream (&itemPos, QIODevice::WriteOnly);
	stream << position;
		
	QMimeData* mimeData = new QMimeData;
	mimeData->setData("text/objetID", itemPos);
	  
	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->start(Qt::MoveAction);

}
