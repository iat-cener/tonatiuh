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
