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
 
#ifndef PATHWRAPPER_H_
#define PATHWRAPPER_H_

#include <QMetaType>

class SoPath;

class PathWrapper
{
public:
    PathWrapper( SoPath* path = 0 );
    PathWrapper( const PathWrapper& rhs );
    ~PathWrapper();
    SoPath* GetPath() const;
    
private:
    SoPath* m_path;
};

Q_DECLARE_METATYPE(PathWrapper)

#endif /*PATHWRAPPER_H_*/
