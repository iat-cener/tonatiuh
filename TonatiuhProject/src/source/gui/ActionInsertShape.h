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

#ifndef ACTIONINSERTSHAPE_H_
#define ACTIONINSERTSHAPE_H_

#include <QAction>

class TShapeFactory;

//!  ActionInsertShape class is the action to insert a shape in the scene.
/*!
  ActionInsertShape is the action added to menu and toolbar for each shape plugin.
*/

class ActionInsertShape : public QAction
{
	Q_OBJECT

public:
	ActionInsertShape( const QString& text, QObject* parent, TShapeFactory* pTShapeFactory );
	~ActionInsertShape();
	
signals:
	void CreateShape( TShapeFactory* pTShapeFactory );
	
public slots:
	void OnActionInsertShapeTriggered();
	
private:
	TShapeFactory* m_pTShapeFactory;
	
};

#endif /*ACTIONINSERTSHAPE_H_*/

