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


#ifndef GRIDSETTINGSDIALOG_H_
#define GRIDSETTINGSDIALOG_H_

#include <QDialog>

#include "ui_gridsettingsdialog.h"


class GridSettingsDialog : public QDialog, private Ui::GridSettingsDialog
{
	Q_OBJECT

public:
	GridSettingsDialog( int xDimension = 100, int zDimension = 100, double xSpacing = 1, double zSpacing = 1, QWidget* parent = 0 );
	~GridSettingsDialog();

	int GetXDimension();
	int GetZDimension();

	double GetXSpacing();
	double GetZSpacing();

	bool IsSizeDefined();
};

#endif /* GRIDSETTINGSDIALOG_H_ */
