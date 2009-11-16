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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "ProgressUpdater.h"
#include <QString>
#include "Trace.h"

using namespace std;

ProgressUpdater::ProgressUpdater( int totalWork, const QString &title, int bar_length, QWidget *creator )
: m_totalBars( bar_length )
{
	Trace trace( "ProgressUpdater::ProgressUpdater", false );
	m_progressDialog = new QProgressDialog ( title , "Cancel",  0, bar_length, creator );
	m_progressDialog->setModal( true );
	m_progressDialog->setCancelButton( NULL);
	m_progressDialog->setVisible( true );

	m_barsPrinted = 0;
	m_frequency = (float)totalWork / (float)m_totalBars;
	m_count = m_frequency;
	m_timer = new Timer();
	m_timer->Start();
}

ProgressUpdater::~ProgressUpdater()
{
	Trace trace( "ProgressUpdater::~ProgressUpdater", false );
	delete m_timer;
	delete m_progressDialog;
}

void ProgressUpdater::Update(int num) const
{
	Trace trace( "ProgressUpdater::Update", false );
    m_count -= num;
    bool updatedAny = false;
    while (m_count <= 0)
    {
			m_count += m_frequency;
			if (m_barsPrinted++ < m_totalBars)
				m_progressDialog->setValue(m_barsPrinted);
			updatedAny = true;
	}
/*

	if (updatedAny)
	{
			// Update elapsed time and estimated time to completion
			float percentDone = (float)m_barsPrinted / (float)m_totalBars;
			float seconds = (float) m_timer->Time();
			// float estRemaining = seconds / percentDone - seconds;
			// cout << "(" << seconds << "s|" << estRemaining <<"s)" << endl;
	}
*/
}


void ProgressUpdater::Done() const
{
	Trace trace( "ProgressUpdater::Done", false );
	// cout << "Total Time: " << m_timer->Time();
}

