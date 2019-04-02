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

#include <QString>

#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "ParametersItem.h"

void updateItem(void *data, SoSensor* )
{
	ParametersItem* parametersItem = (ParametersItem *) data;
	parametersItem->setData(parametersItem->data(Qt::DisplayRole), Qt::DisplayRole );
}

ParametersItem::ParametersItem( QString text,  bool editable, SoField* field  )
:QStandardItem( text ),
 m_pField( field ),
 m_pFieldSensor( 0 ),
 m_text( text )
{
 	setEditable (editable );
 	m_pFieldSensor = new SoFieldSensor(updateItem, this);
 	m_pFieldSensor->attach( m_pField );

}

ParametersItem::~ParametersItem()
{
	delete m_pFieldSensor;
}

QVariant ParametersItem::data ( int role ) const
{
	if( role == Qt::DisplayRole )
	{
		if ( column() == 0 ) return m_text;

		SbString fieldValue = "null";
		m_pField->get( fieldValue );
		return QString( fieldValue.getString() );
	}
	else
		return QStandardItem::data( role );
}

void ParametersItem::setData ( const QVariant& value, int role )
{
	if( role == Qt::EditRole ){
		if ( column() == 1 )
			m_text = value.toString();
	}
	else if( role == Qt::UserRole )
	{
		if( m_pField ) m_pField->set( value.toString().toStdString().c_str() );
	}
	else
		return QStandardItem::setData( value, role );
}

QString ParametersItem::text () const
{
	return m_text;
}
