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
 
#include <Inventor/SbString.h>
#include <Inventor/SbName.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SoFieldList.h> 
#include <Inventor/fields/SoFieldData.h> 
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <QtGui>
#include <QStringList>
#include <QTreeWidgetItem>

#include "FieldContainerWidget.h"
#include "MainWindow.h"
#include "ParametersDelegate.h"
#include "ParametersItem.h"
#include "ParametersModel.h"
#include "ParametersView.h"
#include "Trace.h"

FieldContainerWidget::FieldContainerWidget( SoFieldContainer* fieldContainer, QString containerName, QWidget* parent )
: m_pFieldContainer( fieldContainer ), m_pModel ( 0 ), m_containerName( containerName )
{
	
    m_ptreeView = new QTreeView;
    m_ptreeView->setAlternatingRowColors( true ); 
    connect (m_ptreeView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT( EditorOpened( const QModelIndex& ) ) );
    

	ParametersDelegate* delegate = new ParametersDelegate( );         
    m_ptreeView->setItemDelegate( delegate );
    connect (delegate, SIGNAL( closeEditor( QWidget* ) ), this, SLOT( EditorClosed( QWidget* ) ) );
    
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget( m_ptreeView );
    setLayout( mainLayout );
    
    m_pModel = new ParametersModel();
    m_pModel->SetEditable( true );
    m_pModel->setHorizontalHeaderLabels( QStringList() << tr("Parameter") << tr("Value") );
	m_ptreeView->setModel(m_pModel);
	
    if( m_pFieldContainer ) ReadFields( );
}

void FieldContainerWidget::ReadFields( )
{
    SoFieldList fieldList;  
    int totalFields = m_pFieldContainer->getFields( fieldList );
    
    SoField* pField = 0; 
    SbName fieldName;
    SbString fieldValue = "null";
    
	for( int index = 0; index < totalFields; index++ )
	{
		pField = fieldList.get( index );
		if( pField )
		{	
    		pField->get( fieldValue );
    	    if( m_pFieldContainer->getFieldName( pField, fieldName ) )
    	    {
				m_pModel->setItem( index, false, new ParametersItem ( QString(fieldName.getString()), false, pField ));
				ParametersItem* valueItem = new ParametersItem ( QString(fieldValue.getString()), true, pField );
				m_pModel->setItem( index, true, valueItem );
    	    }
		}
	}
}

void FieldContainerWidget::SetEditable( bool editable )
{
	Trace trace( "FieldContainerWidget::SetEditable", false );
	
	m_pModel->SetEditable( editable );
}

void FieldContainerWidget::EditorOpened( const QModelIndex& index )
{
	Trace trace( "FieldContainerWidget::EditorOpened", false );
	m_lastEditingIndex = index;
}

void FieldContainerWidget::EditorClosed(  QWidget* editor )
{
	Trace trace( "FieldContainerWidget::EditorClosed", false );
	
	QString newValue;
	SoField* field = m_pModel->ModelItem( m_lastEditingIndex )->GetField();
	if( field->getTypeId().isDerivedFrom( SoSFEnum::getClassTypeId() ) )
	{
		QComboBox* combo = static_cast<QComboBox *>(editor);
		newValue = combo->currentText( );

	}
	else
	{
		QLineEdit  *textEdit = dynamic_cast<QLineEdit *>(editor);
		newValue = textEdit->text();
	}
	
	SoFieldList fieldList; 
	int numFields = m_pFieldContainer->getFields (fieldList);
	
	QStringList* oldValuesList = new QStringList;
	
	for(int num = 0; num < numFields; num++ )
	{
		SbString indexValue;
		fieldList[num]->get( indexValue );
		oldValuesList->append( QString(indexValue.getString() ) );
	}
	 
	m_pModel->setData(m_lastEditingIndex, newValue, Qt::UserRole);
	
	emit valueModificated( *oldValuesList, m_containerName );
}

void FieldContainerWidget::Reset()
{
	m_ptreeView->reset();
}
