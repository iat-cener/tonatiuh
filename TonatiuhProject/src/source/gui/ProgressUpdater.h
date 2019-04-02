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

#ifndef PROGRESSUPDATER_H_
#define PROGRESSUPDATER_H_


//#include <iostream>
//#include <string>
#include <QProgressDialog>

class ProgressUpdater
{
public:
	ProgressUpdater(int totalWork, const QString &title, int barLength, QWidget* creator);
	~ProgressUpdater();

	void Update( int num = 1 );

private:
	QProgressDialog* m_progressDialog;
    const int m_totalBars;
    double m_totalWork;
    double m_barsPerUnitWork;
};

#endif /* PROGRESSUPDATER_H_ */
