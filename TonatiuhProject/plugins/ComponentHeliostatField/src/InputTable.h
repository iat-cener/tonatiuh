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

#ifndef INPUTTABLE_H_
#define INPUTTABLE_H_

#include <QDialog>
#include "ui_inputtablewidget.h"
#include "Point3D.h"


class InputTableWidget : public QDialog, private Ui::InputTableWidget
{
	Q_OBJECT

public:
	InputTableWidget( std::vector< Point3D > helCoords, QDialog * parent = 0 );
	~InputTableWidget();

	std::vector< Point3D > GetCoordsTable( ) const;

private slots:
	void ChangedNumberRowsTableState( int numHeliostat );
	void accept();

private:
	std::vector< Point3D > m_coords;
};



#endif /* INPUTTABLE_H_ */
