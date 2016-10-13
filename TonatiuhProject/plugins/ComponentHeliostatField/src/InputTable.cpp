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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
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

