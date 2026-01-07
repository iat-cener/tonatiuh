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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Iñigo Pagola, Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoField.h>

#include "FieldContainerWidget.h"
#include "ParametersItem.h"
#include "ParameterValueException.h"
#include "UserSField.h"
#include "UserMField.h"
#include "TMaterial.h"
#include "TShape.h"

/*!
 * Creates an empty widget.
 */
FieldContainerWidget::FieldContainerWidget( QWidget* parent )
:QTreeView( parent ),
 m_containerName( QString( "" ) ),
 m_currentIndex(),
 m_pDelegate( 0 ),
 m_pFieldContainer( 0 ),
 m_pModel ( 0 )
{
	setAlternatingRowColors( true );

	m_pDelegate = new ParametersDelegate;
	setItemDelegate( m_pDelegate );

	m_pModel = new ParametersModel();
	m_pModel->SetEditable( true );
	setModel(m_pModel);

}

/**
 * Creates a new FieldContainerWidget for the parameters in the \a fieldContainer with parent \a parent.
 *
 * The container name is \a containerName.
 */
FieldContainerWidget::FieldContainerWidget( SoNode* fieldContainer, QString containerName, QWidget* parent )
:QTreeView( parent ),
 m_containerName( containerName ),
 m_currentIndex(),
 m_pDelegate( 0 ),
 m_pFieldContainer( fieldContainer ),
 m_pModel ( 0 )
{
	setAlternatingRowColors( true );

	m_pDelegate = new ParametersDelegate;
	setItemDelegate( m_pDelegate );

	m_pModel = new ParametersModel();
	m_pModel->SetEditable( true );
	m_pModel->setHorizontalHeaderLabels( QStringList() << tr("Parameter") << tr("Value") );
	setModel(m_pModel);

	if( m_pFieldContainer ) ReadFields( );
	resizeColumnToContents ( 1 );
}

/**
 * Destroys the FieldContainerWidget object.
 */
FieldContainerWidget::~FieldContainerWidget()
{
 	delete m_pDelegate;
 	delete m_pModel;

}

/*!
 * Sets \a fieldContainer as widget container and \a containerName as its containerName name.
 */
void FieldContainerWidget::SetContainer( SoNode* fieldContainer, QString containerName )
{
	m_pModel->clear();

	m_pFieldContainer = fieldContainer;
	m_containerName = containerName;

	if( m_pFieldContainer ) ReadFields( );
	resizeColumnToContents ( 1 );
}

/**
 * Sets if the parameters values can be modified.
 */
void FieldContainerWidget::SetEditable( bool editable )
{
	m_pModel->SetEditable( editable );
}

/*!
 * Sets \a current as the view current element index.
 */
void FieldContainerWidget::currentChanged( const QModelIndex& current, const QModelIndex& /*previous*/ )
{
	m_currentIndex = current;
}

void FieldContainerWidget::closeEditor( QWidget* editor, QAbstractItemDelegate::EndEditHint hint )
{
	QTreeView::closeEditor( editor, hint );
	
	QString newValue;
	SoField* field = m_pModel->ModelItem( m_currentIndex )->GetField();
	if( field->getTypeId().isDerivedFrom( SoSFEnum::getClassTypeId() ) )
	{
		QComboBox* combo = qobject_cast<QComboBox *>(editor);
		newValue = combo->currentText( );
	}

	else if( field->getTypeId().isDerivedFrom( UserSField::getClassTypeId() ) )
	{
		FieldEditor* fieldEdit = static_cast< FieldEditor *>(editor);
		newValue = fieldEdit->GetData( );

	}

	else if( field->getTypeId().isDerivedFrom( UserMField::getClassTypeId() ) )
	{
		FieldEditor* fieldEdit = static_cast< FieldEditor *>(editor);
		newValue = fieldEdit->GetData( );

	}
	else
	{
		QLineEdit* textEdit = qobject_cast<QLineEdit *>(editor);
		newValue = textEdit->text();
	}
	SbName fieldName;
	m_pFieldContainer->getFieldName( field, fieldName );
	QString parameterName( fieldName.getString() );

	
	try{
		if( m_pFieldContainer->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
		{
			TShape* shape = static_cast< TShape* >( m_pFieldContainer );
			shape->ValidateParamaterValue( parameterName.toStdString(), newValue.toStdString() );
		}
		if( m_pFieldContainer->getTypeId().isDerivedFrom( TMaterial::getClassTypeId() ) )
		{
			TMaterial* material = static_cast< TMaterial* >( m_pFieldContainer );
			material->ValidateParamaterValue( parameterName.toStdString(), newValue.toStdString() );
		}
	}
	catch(ParameterValueException &ex ){
		QMessageBox::warning( 0, QString( "Tonatiuh" ), ex.what() );
		newValue = "";
	}

	if( !newValue.isEmpty() )
	{
		emit valueModificated( m_pFieldContainer, parameterName, newValue );

	} 	

}

/**
 * Reads container parameters and for each parameters adds its name and value to de widget.
 */
void FieldContainerWidget::ReadFields( )
{
	m_pModel->clear();
	m_pModel->setHorizontalHeaderLabels( QStringList() << tr("Parameter") << tr("Value") );

	SoFieldList fieldList;
	int totalFields = m_pFieldContainer->getFields( fieldList );

	SoField* pField = 0;
	SbName fieldName;
	SbString fieldValue = "null";

	for( int index = 0; index < totalFields; ++index )
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
