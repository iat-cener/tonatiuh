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

#ifndef ACTIONINSERTTRACKER_H_
#define ACTIONINSERTTRACKER_H_

#include <QAction>

class TTrackerFactory;

//! ActionInsertTracker class is the action to insert a tracker in the scene.
/*!
  ActionInsertTracker is the action added to menu and toolbar for each tracker plugin.
*/

class ActionInsertTracker : public QAction
{
	Q_OBJECT

public:
	ActionInsertTracker( const QString& text, QObject* parent, TTrackerFactory* pTTrackerFactory );
	~ActionInsertTracker();
	
signals:
	void CreateTracker( TTrackerFactory* pTTrackerFactory );
	
public slots:
	void OnActionInsertTrackerTriggered();
	
private:
	TTrackerFactory* m_pTTrackerFactory;
	
};

#endif /*ACTIONINSERTTRACKER_H_*/
