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

#include "ProgressUpdater.h"
#include <QString>

using namespace std;

ProgressUpdater::ProgressUpdater( int totalWork, const QString &title, int bar_length, QWidget *creator )
: m_totalBars( bar_length )
{
	m_progressDialog = new QProgressDialog ( title , "Cancel",  0, bar_length, creator );
	m_progressDialog->setModal( true );
	m_progressDialog->setCancelButton( NULL);
	m_progressDialog->setVisible( true );

	m_totalWork = totalWork;
	m_barsPerUnitWork = m_totalBars/m_totalWork;
}

ProgressUpdater::~ProgressUpdater()
{
	delete m_progressDialog;
}

void ProgressUpdater::Update( int work )
{
	m_progressDialog->setValue( work * m_barsPerUnitWork );
}

