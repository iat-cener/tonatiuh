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

#include "ParametersModel.h"
#include "ParametersItem.h"

ParametersModel::ParametersModel(QObject * parent)
:QStandardItemModel (parent )
{

}
ParametersModel::~ParametersModel()
{

}

QVariant ParametersModel::data ( const QModelIndex & index, int role ) const
{
	ParametersItem* parameter =static_cast<ParametersItem* > ( itemFromIndex( index ) );
	return parameter->data( role );
}
bool ParametersModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
	ParametersItem* parameter = static_cast<ParametersItem* > ( itemFromIndex( index ) );
	if( parameter )
	{
	    parameter->setData ( value, role );
	    emit itemChanged( parameter );
	    return true;
	}
	return false;
}

Qt::ItemFlags ParametersModel::flags(const QModelIndex &index) const
{
	if( m_editable )
		return QStandardItemModel::flags(index);
	else
		return ( QStandardItemModel::flags(index) & ~Qt::ItemIsEditable );
}

ParametersItem* ParametersModel::ModelItem( const QModelIndex & index ) const
{
	ParametersItem* item =static_cast<ParametersItem* > ( itemFromIndex( index ) );
	return item;
}

void ParametersModel::SetEditable( bool editable )
{
	m_editable = editable;
}
