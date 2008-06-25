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
 
#include <QLineEdit>
#include <QModelIndex>
#include <QObject>

#include <Inventor/SbName.h> 
#include <Inventor/nodekits/SoBaseKit.h>

#include "InstanceNode.h"
#include "NodeNameDelegate.h"
#include "SceneModel.h"

#include "Trace.h"

NodeNameDelegate::NodeNameDelegate( SceneModel* sceneModel,  QObject* parent)
 : QItemDelegate(parent), m_pModel( sceneModel )
{
	Trace trace( "NodeNameDelegate::NodeNameDelegate", false );
} 

NodeNameDelegate::~NodeNameDelegate()
{
	
}

QWidget* NodeNameDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
	Trace trace( "NodeNameDelegate::createEditor", false );
	QLineEdit  *editor = new QLineEdit (parent);
	connect ( editor, SIGNAL (editingFinished()), this, SLOT (commitChangesAndClose()) );
	return editor;

}

void NodeNameDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
	Trace trace( "NodeNameDelegate::setEditorData", false );
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QLineEdit  *textEdit = static_cast<QLineEdit *>(editor);
	
	SoNode* coinNode = m_pModel->NodeFromIndex( index )->GetNode();
	
	QString nodeName;
	if ( coinNode->getName() == SbName() )
 		nodeName = QString( coinNode->getTypeId().getName().getString() );
	else
		nodeName = QString( coinNode->getName().getString() );
		
	textEdit->setText( nodeName );

} 

void NodeNameDelegate::setModelData(QWidget* editor, QAbstractItemModel* /*model*/, const QModelIndex& index ) const
{
	Trace trace( "NodeNameDelegate::setModelData", false );
	QLineEdit  *textEdit = static_cast<QLineEdit *>(editor);
	
	SoNode* coinNode =m_pModel->NodeFromIndex( index )->GetNode();
	
	SbName* coinName = new SbName( textEdit->text().toStdString().c_str() );
	coinNode->setName( *coinName );

} 

void NodeNameDelegate::commitChangesAndClose()
{
	QLineEdit *editor = qobject_cast< QLineEdit* >(sender());
	emit commitData(editor);
	emit closeEditor(editor);
}
