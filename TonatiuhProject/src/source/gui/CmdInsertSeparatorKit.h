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

#ifndef CMDINSERTSEPARATORKIT_H_
#define CMDINSERTSEPARATORKIT_H_

#include <QUndoCommand>

class InstanceNode;
class QModelIndex;
class TSeparatorKit;
class SceneModel;

//! CmdInsertSeparatorKit class is the insert command for TSeparatorKit nodes stored in the command stack.
/*!
  CmdInsertSeparatorKit represents a single TSeparatorKit insertion action on a scene, insert a new TSeparatorKit node to the scene.
*/

class CmdInsertSeparatorKit : public QUndoCommand
{
public:
    CmdInsertSeparatorKit( TSeparatorKit* separatorKit,  const QModelIndex& parentIndex, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdInsertSeparatorKit();

     virtual void undo();
     virtual void redo();

private:
     TSeparatorKit* m_separatorKit;
     SoBaseKit* m_coinParent;
     SceneModel* m_pModel;
     int m_row;

};

#endif /*CMDINSERTSEPARATORKIT_H_*/
