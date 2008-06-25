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
 
#include <QtGui>

#include "InstanceNode.h"
#include "SceneModelView.h"
#include "SceneModel.h"
#include "Trace.h"
#include "TSeparatorKit.h"
#include "TShapeKit.h"

#include <Inventor/nodekits/SoBaseKit.h> 
#include <Inventor/nodekits/SoNodeKitListPart.h> 

SceneModelView::SceneModelView(QWidget *parent)
    : QTreeView(parent)
{
    setAcceptDrops(true);
}

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

void SceneModelView::mouseMoveEvent(QMouseEvent *event)
{
	Trace trace( "SceneModelView::mouseMoveEvent", false );
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            startDrag(event);
    }
   // QTreeView::mouseMoveEvent(event);
}

void SceneModelView::dragEnterEvent(QDragEnterEvent *event)
{
	Trace trace( "SceneModelView::dragEnterEvent", false );
	event->setDropAction(Qt::MoveAction);
	event->accept();
}

void SceneModelView::dragMoveEvent(QDragMoveEvent *event)
{
	Trace trace( "SceneModelView::dragMoveEvent", false );
	event->setDropAction(Qt::MoveAction);
	event->accept();
}

void SceneModelView::dropEvent(QDropEvent *event)
{
	Trace trace( "SceneModelView::dropEvent", false );
	
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

void SceneModelView::startDrag(QMouseEvent *event)
{
	Trace trace( "SceneModelView::startDrag", false );
	
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
