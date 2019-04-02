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

#ifndef CMDINSERTTRACKER_H_
#define CMDINSERTTRACKER_H_

#include <QUndoCommand>
#include <QModelIndex>

class InstanceNode;
class TTracker;
class SceneModel;
class SoSceneKit;

//!  CmdInsertTracker class is the insert command for trackers stored in the command stack.
/*!
  CmdInsertTracker represents a single shape insertion action on a scene, insert a new trackers node to the scene.
*/

class CmdInsertTracker : public QUndoCommand
{
public:
    CmdInsertTracker( TTracker* tracker, const QModelIndex& parentIndex, SoSceneKit* scene, SceneModel* model, QUndoCommand* parent = 0 );
	~CmdInsertTracker();

	virtual void undo();
	virtual void redo();

private:

	TTracker* m_tracker;
	SoBaseKit* m_coinParent;;
	SoSceneKit* m_scene;
	SceneModel* m_pModel;
	int m_row;
     
};

#endif /*CMDINSERTTRACKER_H_*/
