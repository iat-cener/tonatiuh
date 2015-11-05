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
***************************************************************************/

#include <QLineEdit>
#include <QMessageBox>

#include "ContainerEditor.h"

#include <iostream>

/***************************************************************
 *  CustomPlainTextEdit
 ***************************************************************/

/*!
 * Creates an editor object
 */
CustomPlainTextEdit::CustomPlainTextEdit( QWidget* parent )
:QLineEdit( parent )
{
}

/*!
 * Destroys object
 */
CustomPlainTextEdit::~CustomPlainTextEdit()
{

}

/*!
 * Emits edition finished signal.
 */
void CustomPlainTextEdit::focusOutEvent(QFocusEvent*  event )
{
	emit editingFinished( );
	QLineEdit::focusOutEvent( event );


}

/***************************************************************
 *  ContainerEditor
 ***************************************************************/
/*!
 * Creates an editor object
 */
ContainerEditor::ContainerEditor( QWidget* parent )
:QWidget( parent )
{
	//setupUi(this );

	 setObjectName( QLatin1String( "ContainerEditor" ) );
	 resize(879, 192);

	 QSizePolicy sizePolicy( QSizePolicy::Minimum, QSizePolicy::MinimumExpanding) ;
	 sizePolicy.setHorizontalStretch( 0 );
	 sizePolicy.setVerticalStretch( 0 );
	// QSizePolicy currentSizePolicy = sizePolicy();
	 //sizePolicy.setHeightForWidth(sizePolicy().hasHeightForWidth());
	 setMouseTracking(false);
	 setSizePolicy(sizePolicy);
     setFocusPolicy(Qt::NoFocus);

	 horizontalLayout = new QHBoxLayout( this );
	 horizontalLayout->setSpacing(0);
	 horizontalLayout->setObjectName(QLatin1String("horizontalLayout"));
	 horizontalLayout->setContentsMargins(0, 0, 0, 0);

	 valuesEdit = new CustomPlainTextEdit(this);
	 valuesEdit->setObjectName(QLatin1String("valuesEdit"));
	 valuesEdit->setMouseTracking(false);
	 valuesEdit->setFocusPolicy(Qt::WheelFocus);

	 connect( valuesEdit, SIGNAL( editingFinished() ), this, SLOT( CloseEditor( ) ) );
	 horizontalLayout->addWidget(valuesEdit);

	 editButton = new QPushButton(this);
	 editButton->setObjectName(QLatin1String("editButton"));
	 editButton->setFocusPolicy(Qt::StrongFocus);
	 editButton->setText( QLatin1String( "...") );
	 horizontalLayout->addWidget(editButton);

	connect( editButton, SIGNAL( clicked() ), this, SLOT( OpenContainerViewer() ) );

}

/*!
 * Destroys object
 */
ContainerEditor::~ContainerEditor()
{

}

/*!
 * Return current value in the editor.
 */
QString ContainerEditor::GetData( ) const
{
	//return ( valuesEdit->toPlainText() );
	return ( valuesEdit->text() );
}


/*!
 * Set defaults values in the editor.
 */
void ContainerEditor::SetData( QString value )
{
	//valuesEdit->setPlainText( value );
	valuesEdit->setText( value );
}

/*
 * Closes editor if the elements of the editor are not active.
 */
void ContainerEditor::focusOutEvent(QFocusEvent*  event )
{
	QWidget::focusOutEvent( event );
	CloseEditor();
}

/*!
 * Emits an editing finished signal if the edition is finished and the
 */
void ContainerEditor::CloseEditor()
{
	if( !hasFocus() && !editButton->hasFocus() && !valuesEdit->hasFocus()  )
	{
		emit editingFinished( );
	}
}

/*!
 * Opens a table viewer width the values in the container.
 */
void ContainerEditor::OpenContainerViewer()
{
	ContainerViewer viewer;
	//if( !viewer.SetData( valuesEdit->toPlainText() ) )
	if( !viewer.SetData( valuesEdit->text() ) )
		return;
	if( viewer.exec() )
	{
		//valuesEdit->setPlainText( viewer.GetData( ) ) ;
		valuesEdit->setText( viewer.GetData( ) ) ;
	}
	valuesEdit->setFocus( Qt::ActiveWindowFocusReason );
}



/***************************************************************
 * ContainerViewer
 ***************************************************************/

/*!
 * Creates an dialog object
 */
ContainerViewer::ContainerViewer( QWidget* parent)
 :QDialog( parent )
{
	setupUi( this );
	m_delegate = new DoubleValuesDelegate();
	variableValuesTable->setItemDelegate( new DoubleValuesDelegate()  );

	connect( addButton, SIGNAL( clicked() ), this, SLOT( AddNewRow() ) );
	connect( deleteButton, SIGNAL( clicked() ), this, SLOT( DeleteSelectedRow() ) );
}

ContainerViewer::~ContainerViewer()
{
	delete m_delegate;
}


/*!
 * Return current values in the editor.
 */
QString ContainerViewer::GetData( ) const
{
	int rows = variableValuesTable->rowCount();
	int columns = variableValuesTable->columnCount();

	QString data = QLatin1String ( "[" );
	for( int r = 0; r < rows-1; r++ )
	{
		for ( int c = 0; c < columns -1; c++ )
		{
			QString itemData = variableValuesTable->item( r,c )->text();
			data = data.append( QString( "%1 " ).arg( itemData ) );
		}
		QString itemData = variableValuesTable->item( r,columns -1 )->text();
		data = data.append( QString( "%1,\n" ).arg( itemData ) );
		std::cout<<"data: "<<data.toStdString()<<std::endl;
	}

	for ( int c = 0; c < columns -1; c++ )
	{
		QString itemData = variableValuesTable->item( rows-1,c )->text();
		data = data.append( QString( "%1 " ).arg( itemData ) );
	}
	QString itemData = variableValuesTable->item( rows-1,columns -1 )->text();
	data = data.append( QString( "%1]" ).arg( itemData ) );

	std::cout<<"data: "<<data.toStdString()<<std::endl;
	return (data);
}

/*!
 * Sets the values in the string into the table.
 */
bool ContainerViewer::SetData( QString value )
{
	QStringList values = value.split( QRegExp( "[\\[\\]]" ), QString::SkipEmptyParts );
	if( values.count() < 1 || values.count() > 1 )	return false;

	QStringList elements =values[0].split( QRegExp( "," ), QString::SkipEmptyParts );
	int rows = elements.count();
	int columns  = -1;
	for( int i = 0; i < rows; i++ )
	{

		QStringList elementValues =elements[i].split( QRegExp("\\s+"), QString::SkipEmptyParts );
		if( i ==  0 ) columns = elementValues.count();
		else if( i > 0 && columns != elementValues.count() )	return false;

	}

	variableValuesTable->setRowCount( rows );
	variableValuesTable->setColumnCount( columns );
	for( int i = 0; i < rows; i++ )
	{
		QStringList elementValues =elements[i].split( QRegExp("\\s+"), QString::SkipEmptyParts );
		for( int j = 0; j < columns; j++ )
		{

		    QTableWidgetItem* newItem = new QTableWidgetItem( elementValues[j] );
		    variableValuesTable->setItem(i, j, newItem);
		}

	}
	return true;
}


/*!
 * Adds an empty row to the end of the table.
 */
void ContainerViewer::AddNewRow()
{
	int rows = variableValuesTable->rowCount();
	variableValuesTable->insertRow( rows );
}

/*!
 * Delete selected row.
 */
void ContainerViewer::DeleteSelectedRow()
{

	int res = QMessageBox::question( this, QLatin1String( "Tonatiuh" ),
			tr ("Are you sure you want to delete current selected row? ") );

	if( res == QMessageBox::Yes  )
	{
		int selectedRow = variableValuesTable->currentRow();
		variableValuesTable->removeRow( selectedRow );
	}
}


/**************************************************************
 * DoubleValuesDelegate
 * ************************************************************/

/*!
 * Creates delegate object.
 */
DoubleValuesDelegate::DoubleValuesDelegate(QObject* parent)
:QItemDelegate( parent )
{
}

/*!
 * Destroys delegate object.
 */
DoubleValuesDelegate::~DoubleValuesDelegate()
{
}

/*!
 * Creates an editor for the item identified by \a index.
 */
QWidget* DoubleValuesDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/ ) const
{
	QLineEdit* editor = new QLineEdit(parent);

	QDoubleValidator* validator = new QDoubleValidator();
	validator->setNotation( QDoubleValidator::StandardNotation );
	validator->setParent( editor );
	editor->setValidator( validator );
	return editor;
}

/*!
 * Sets the item value to the editor.
 */
void DoubleValuesDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	double value = index.model()->data(index,Qt::EditRole).toDouble();
	QLineEdit* lineEditor = static_cast<QLineEdit*>(editor);
	lineEditor->setText( QString().setNum( value ) );
}


/*!
 * Sets the editor value to the item.
 */
void DoubleValuesDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
	QLineEdit* lineEditor = static_cast<QLineEdit*>( editor );
	QString value = lineEditor->text();
	model->setData( index, value );
}

/*!Updates editor geometry */
void DoubleValuesDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
{
	editor->setGeometry(option.rect);
}

