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

#include <QMessageBox>
#include "InputTable.h"


InputTableWidget::InputTableWidget( std::vector< Point3D > helCoords, QDialog * parent )
:QDialog( parent ),
 m_coords( helCoords )
{
	setupUi( this );

	nHelSpin->setValue( m_coords.size() );
	tableWidget->setRowCount( m_coords.size() );

	for( unsigned int i = 0; i < m_coords.size(); i++ )
		for( unsigned int j = 0; j < 3; j++ )
			tableWidget->setItem( i, j,new QTableWidgetItem( QString::number( m_coords[i][j] ) ) );


	connect( nHelSpin, SIGNAL( valueChanged( int ) ), this, SLOT( ChangedNumberRowsTableState( int ) ) );
}


InputTableWidget::~InputTableWidget()
{

}


std::vector< Point3D > InputTableWidget::GetCoordsTable( ) const
{
	std::vector< Point3D > coords;
	for( int i = 0; i < tableWidget->rowCount(); i++ )
	{
		Point3D hPoint;
		for( int j = 0; j < tableWidget->columnCount(); j++ )
			hPoint[j] = tableWidget->item(i,j)->text( ).toDouble();

		coords.push_back( hPoint );
	}
	return coords;
}


void InputTableWidget::ChangedNumberRowsTableState( int numHeliostat )
{
	tableWidget->setRowCount( numHeliostat );
}


void InputTableWidget::accept()
{
	for( int i = 0; i < tableWidget->rowCount(); i++ )
	{
		for( int j = 0; j < tableWidget->columnCount(); j++ )
		{
			if( !tableWidget->item(i,j) || tableWidget->item(i,j)->text().isEmpty() )
			{
				QMessageBox::information( this, QLatin1String( "Tonatiuh" ), tr( "Some coordinates are empty" ), 1 );
				return;
			}
			else if ( !tableWidget->item(i,j)->text().toDouble() && tableWidget->item(i,j)->text().toDouble() != 0 )
			{
				QMessageBox::information( this, QLatin1String( "Tonatiuh" ), tr( "Some coordinates are not valid" ), 1 );
				return;
			}
		}
	}

	QDialog::accept();
}

