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
