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

#include "ActionInsertShape.h"
#include "TShapeFactory.h"

/**
 * Constructor.
 */
ActionInsertShape::ActionInsertShape ( const QString& text, QObject* parent, TShapeFactory* pTShapeFactory )
: QAction(text,parent), m_pTShapeFactory(pTShapeFactory)
{
}

/**
 * Destructor.
 */
ActionInsertShape::~ActionInsertShape()
{
}

/**
 * Emits a create shape signal.
 */
void ActionInsertShape::OnActionInsertShapeTriggered()
{
	emit CreateShape( m_pTShapeFactory );
}
