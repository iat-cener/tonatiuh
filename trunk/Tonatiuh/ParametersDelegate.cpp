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
 
#include <QComboBox>
#include <QLineEdit>
#include <QModelIndex>
#include <QString>

#include <Inventor/fields/SoSFEnum.h> 

#include "ParametersItem.h"
#include "ParametersDelegate.h"
#include "ParametersModel.h"
#include "Trace.h"

ParametersDelegate::ParametersDelegate(  QObject* parent)
 : QItemDelegate(parent)
{
	
} 

ParametersDelegate::~ParametersDelegate()
{
	
}

QWidget* ParametersDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& /*option*/, const QModelIndex &index) const
{
	Trace trace( "ParametersDelegate::createEditor", false );
	
	const ParametersModel* model = static_cast< const ParametersModel* >( index.model() );
	SoField* field = model->ModelItem( index )->GetField();
	
	if( field->getTypeId().isDerivedFrom( SoSFEnum::getClassTypeId() ) )
	{
		QComboBox* editor = new QComboBox( parent );
		SoSFEnum* enumField = static_cast< SoSFEnum* >( field );
		for( int i = 0; i < enumField->getNumEnums() ; i++ )
		{
			SbName enumName;
			enumField->getEnum( i , enumName );
			editor->addItem( enumName.getString() );
		}
		return editor;
	}
	else
	{
		QLineEdit* editor = new QLineEdit(parent);
		return editor;
	}
}

void ParametersDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
	const ParametersModel* model = static_cast< const ParametersModel* >( index.model() );
	SoField* field = model->ModelItem( index )->GetField();
	
	if( field->getTypeId().isDerivedFrom( SoSFEnum::getClassTypeId() ) )
	{
;
		SoSFEnum* enumField = static_cast< SoSFEnum* >( field );
		int selectedIndex = enumField->getValue();
		
		QComboBox* combo = static_cast<QComboBox *>(editor);
		combo->setCurrentIndex( selectedIndex );

	}
	else
	{
		QString value = index.model()->data(index, Qt::DisplayRole).toString();
			
		QLineEdit  *textEdit = static_cast<QLineEdit *>(editor);
		textEdit->setText(value);
	}

    	
}
