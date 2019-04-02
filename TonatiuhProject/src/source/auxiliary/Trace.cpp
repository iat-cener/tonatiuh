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
 
#include "Trace.h"
#include <iostream>

int Trace::level = -1;
bool Trace::TraceEnabled = true;

Trace::Trace( std::string functionName, bool showTrace )
: m_functionName( functionName ), m_functionTrace( showTrace )
{
	if(!TraceEnabled || !m_functionTrace)
		return;

	level++;
	for( int i = 0; i < level; ++i ) std::cout << "      ";
	std::cout << "( " << level << " )" << "<ENTERING " << m_functionName << ">" << std::endl;
}

Trace::~Trace( )
{
	if(!TraceEnabled || !m_functionTrace)
		return;

	for( int i = 0; i < level; ++i ) std::cout << "      ";
    std::cout << "( " << level << " )" << "<EXITING  " << m_functionName << ">" << std::endl;
    level--;
}
