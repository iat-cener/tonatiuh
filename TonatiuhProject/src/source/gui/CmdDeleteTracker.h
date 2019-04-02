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

#ifndef CMDDELETETRACKER_H_
#define CMDDELETETRACKER_H_

#include <QUndoCommand>

class QModelIndex;
class SoSceneKit;
class SceneModel;
class TSeparatorKit;
class TTracker;

//!  CmdDeleteTracker class is the delete command for trackers nodes stored in the command stack.
/*!
  CmdDeleteTracker represents a single tracker node delete action on a scene. Deletes a node from the scene.
   \sa CmdDelete, CmdCopy, CmdCut, CmdPaste
*/

class CmdDeleteTracker : public QUndoCommand
{
public:
    CmdDeleteTracker( const QModelIndex& selectedIndex, SoSceneKit* scene, SceneModel& model, QUndoCommand* parent = 0 );
    ~CmdDeleteTracker();

     virtual void undo();
     virtual void redo();

private:
	TTracker* m_tracker;
	TSeparatorKit* m_coinParent;
    SoSceneKit* m_scene;
	SceneModel* m_pModel;
    int m_row;
};

#endif /*CMDDELETETRACKER_H_*/
