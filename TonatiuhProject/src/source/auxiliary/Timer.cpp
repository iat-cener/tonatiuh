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
#include <string>
#include "Timer.h"

// Timer Method Definitions
Timer::Timer()
{
	#if defined( WIN32 )
        // Windows Timer Initialization
        QueryPerformanceFrequency( &m_performance_frequency );
        m_oneoverfrequency = 1.0/((double)m_performance_frequency.QuadPart);
	#endif

        m_time0 = m_elapsed = 0;
        m_running = 0;
}

double Timer::GetTime()
{

#if defined( WIN32 )
        // Windows GetTime
        QueryPerformanceCounter( &m_performance_counter );
        return (double) m_performance_counter.QuadPart * m_oneoverfrequency;
#else
        // UNIX GetTime
        gettimeofday( &m_timeofday, NULL );
        return m_timeofday.tv_sec + m_timeofday.tv_usec / 1000000.0;
#endif
}

Timer::~Timer()
{

}

void Timer::Start()
{
	m_running = 1;
    m_time0 = GetTime();
}

void Timer::Stop()
{
	m_running = 0;
	m_elapsed += GetTime() - m_time0;
}

void Timer::Reset()
{
	m_running = 0;
    m_elapsed = 0;
}

double Timer::Time()
{
	if (m_running)
    {
    	Stop();
        Start();
    }
    return m_elapsed;
}
