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
 
#ifndef TRACE_H_
#define TRACE_H_

#include <string>

class Trace
{
public:
    Trace( std::string functionName, bool showTrace = true );
    ~Trace( );

private:
    static int level;
    static bool TraceEnabled;
    std::string m_functionName;
    bool m_functionTrace;
};

#endif /*TRACE_H_*/
