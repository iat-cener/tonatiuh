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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QLineEdit>
#include <QMessageBox>
#include <iostream>

#include "ContainerEditorMFVec3.h"
#include "gc.h"


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
 *  ContainerEditorMFVec3
 ***************************************************************/
/*!
 * Creates an editor object
 */
ContainerEditorMFVec3::ContainerEditorMFVec3( QWidget* parent )
:FieldEditor( parent )
{

	 setObjectName( QLatin1String( "ContainerEditor" ) );
	 resize(879, 192);

	 QSizePolicy sizePolicy( QSizePolicy::Minimum, QSizePolicy::MinimumExpanding) ;
	 sizePolicy.setHorizontalStretch( 0 );
	 sizePolicy.setVerticalStretch( 0 );
	 setMouseTracking(false);
	 setSizePolicy(sizePolicy);
     setFocusPolicy(Qt::NoFocus);

     m_horizontalLayout = new QHBoxLayout( this );
     m_horizontalLayout->setSpacing(0);
     m_horizontalLayout->setObjectName(QLatin1String("horizontalLayout"));
     m_horizontalLayout->setContentsMargins(0, 0, 0, 0);

	 m_valuesEdit = new CustomPlainTextEdit(this);
	 m_valuesEdit->setObjectName(QLatin1String("valuesEdit"));
	 m_valuesEdit->setMouseTracking(false);
	 m_valuesEdit->setFocusPolicy(Qt::WheelFocus);

	 connect( m_valuesEdit, SIGNAL( editingFinished() ), this, SLOT( CloseEditor( ) ) );
	 m_horizontalLayout->addWidget(m_valuesEdit);

	 m_editButton = new QPushButton(this);
	 m_editButton->setObjectName(QLatin1String("editButton"));
	 m_editButton->setFocusPolicy(Qt::StrongFocus);
	 m_editButton->setText( QLatin1String( "...") );
	 m_horizontalLayout->addWidget(m_editButton);

	connect( m_editButton, SIGNAL( clicked() ), this, SLOT( OpenContainerViewer() ) );

}

/*!
 * Destroys object
 */
ContainerEditorMFVec3::~ContainerEditorMFVec3()
{
	delete m_horizontalLayout;
	//delete m_editButton;
}

/*!
 * Return current value in the editor.
 */
QString ContainerEditorMFVec3::GetData( ) const
{
	return ( m_valuesEdit->text() );
}


/*!
 * Set defaults values in the editor.
 */
void ContainerEditorMFVec3::SetData( QString value )
{
	m_valuesEdit->setText( value );
}


/*!
 * Set headers laber to the editor.
 */
void ContainerEditorMFVec3::SetTitles( QStringList titles )
{
	m_titlesList = titles;
}

/*
 * Closes editor if the elements of the editor are not active.
 */
void ContainerEditorMFVec3::focusOutEvent(QFocusEvent*  event )
{
	QWidget::focusOutEvent( event );
	CloseEditor();
}

/*!
 * Emits an editing finished signal if the edition is finished and the
 */
void ContainerEditorMFVec3::CloseEditor()
{
	if( !hasFocus() && !m_editButton->hasFocus() && !m_valuesEdit->hasFocus()  )
	{
		emit editingFinished( );
	}
}

/*!
 * Opens a table viewer width the values in the container.
 */
void ContainerEditorMFVec3::OpenContainerViewer()
{
	ContainerViewerMFVec3 viewer;
	if( !viewer.SetData( m_valuesEdit->text() ) )
		return;
	if( m_titlesList.count() > 0 ) 	viewer.SetTitles( m_titlesList );
	if( viewer.exec() )
	{
		m_valuesEdit->setText( viewer.GetData( ) ) ;
	}
	m_valuesEdit->setFocus( Qt::ActiveWindowFocusReason );
}




/***************************************************************
 * ContainerViewerMFVec3
 ***************************************************************/

/*!
 * Creates an dialog object
 */
ContainerViewerMFVec3::ContainerViewerMFVec3( QWidget* parent)
 :QDialog( parent )
{
	setupUi( this );
	m_delegate = new DoubleValuesDelegate();
	variableValuesTable->setItemDelegate( new DoubleValuesDelegate()  );

	connect( addButton, SIGNAL( clicked() ), this, SLOT( AddNewRow() ) );
	connect( deleteButton, SIGNAL( clicked() ), this, SLOT( DeleteSelectedRow() ) );
	connect( buttonBox, SIGNAL( accepted() ), this, SLOT( CloseViewer() ) );
	connect( helpButton, SIGNAL( clicked()), this, SLOT(HelpMenu() ) );
}

ContainerViewerMFVec3::~ContainerViewerMFVec3()
{
	delete m_delegate;
}


/*!
 * Return current values in the editor.
 */
QString ContainerViewerMFVec3::GetData( ) const
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
	}

	for ( int c = 0; c < columns -1; c++ )
	{
		QString itemData = variableValuesTable->item( rows-1,c )->text();
		data = data.append( QString( "%1 " ).arg( itemData ) );
	}
	QString itemData = variableValuesTable->item( rows-1,columns -1 )->text();
	data = data.append( QString( "%1]" ).arg( itemData ) );

	return (data);
}

/*!
 * Sets the values in the string into the table.
 */
bool ContainerViewerMFVec3::SetData( QString value )
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


void ContainerViewerMFVec3::SetTitles( QStringList titles )
{
	variableValuesTable->setHorizontalHeaderLabels( titles );
}


/*!
 * Adds an empty row to the end of the table.
 */
void ContainerViewerMFVec3::AddNewRow()
{
	int rows = variableValuesTable->rowCount();
	int columns = variableValuesTable->columnCount();
	variableValuesTable->insertRow( rows );
	for( int i = 0; i < columns; i++ )
	{
		QTableWidgetItem* newItem = new QTableWidgetItem( "0" );
		variableValuesTable->setItem(rows, i, newItem);

	}
}


void ContainerViewerMFVec3::CloseViewer()
{
	if( OkToContinue() )
	{
		QDialog::accept();
	}
}

/*!
 * Delete selected row.
 */
void ContainerViewerMFVec3::DeleteSelectedRow()
{

	int res = QMessageBox::question( this, QLatin1String( "Tonatiuh" ),
			tr ("Are you sure you want to delete current selected row? ") );

	if( res == QMessageBox::Yes  )
	{
		int selectedRow = variableValuesTable->currentRow();
		variableValuesTable->removeRow( selectedRow );
	}
}


/*!
 * Verifies if the table values are well defined.
 */
bool ContainerViewerMFVec3::OkToContinue()
{
	int rows = variableValuesTable->rowCount();
	int columns = variableValuesTable->columnCount();

	double lastAngle = -1;
	for( int r = 0; r < rows; r++ )
	{
		for( int c = 0; c < columns; c++ )
		{
			QString itemData = variableValuesTable->item( r,c )->text();
			if( itemData.isEmpty() )
			{
					QString message = QString( tr( "Defines values are not valid. "
							"Empty value defined at cell [%1,%2]." )).arg( QString::number( r ), QString::number( c ) );
					QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ),
							message );
					return ( false );
			}

			if( c == 0 )
			{
				double angle = itemData.toDouble();

				if( angle < 0.0  || angle > 0.5 * gc::Pi )
				{
					QString message = QString( tr( "Defined values are not valid. Angle values must be defined between [0,0.5*pi]" ) );

					QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
					return ( false );
				}

				if( angle == lastAngle )
				{
					QString message = QString( tr( "Defined values are not valid. Angle values cannot be repeated." ) );
					QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
					return ( false );
				}
				if( angle <= lastAngle )
				{
					QString message = QString( tr( "Defined values are not valid. Angle values must be defined in increasing order." ) );
					QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
					return ( false );

				}
				lastAngle = angle;
			}

			if( c == 1 )
			{
				double reflectivity = itemData.toDouble();

				if( reflectivity < 0.0  || reflectivity > 1 )
				{
					QString message = QString( tr( "Defined values are not valid. Reflectivity values must be defined between [0,1]" ) );

					QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
					return ( false );
				}

			}
			if( c == 2 )
			{
				double transmissivity = itemData.toDouble();

				if( transmissivity < 0.0  || transmissivity > 1 )
				{
					QString message = QString( tr( "Defined values are not valid. Transmissivity values must be between [0,1]" ) );

					QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
					return ( false );
				}

			}
		}
		QString itemDataR = variableValuesTable->item( r,1 )->text();
		QString itemDataT = variableValuesTable->item( r,2 )->text();
		double reflectivity = itemDataR.toDouble();
		double transmissivity =itemDataT.toDouble();
		if( reflectivity + transmissivity < 0.0  || reflectivity + transmissivity > 1 )
						{
							QString message = QString( tr( "Defined values are not valid. The sum between transmissivity and reflectivity values for each angle must be between [0,1]" ) );

							QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
							return ( false );
						}

	}

	return ( true );
}


void ContainerViewerMFVec3::HelpMenu()
{
	QString message = QString( tr("Angles values\n"
			"\t- must be defined between [0,0.5*pi]\n"
			"\t- must be defined in increasing order\n"
			"\t- cannot be repeated\n\n"
			"Reflectivity/Transmissivity values\n"
			"\t- must be defined between [0,1]\n\n"
			"The sum between transmissivity and reflectivity values for each angle must be defined between [0,1]") );
	QMessageBox::information( this,  QLatin1String( "Tonatiuh" ), message );
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

