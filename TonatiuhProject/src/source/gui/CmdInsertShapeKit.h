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

#ifndef CMDINSERTSHAPEKIT_H_
#define CMDINSERTSHAPEKIT_H_

#include <QUndoCommand>

class QModelIndex;
class TShapeKit;
class SceneModel;

//!  CmdInsertShapeKit class is the insert command for tshapekit nodes  stored in the command stack.
/*!
  CmdInsertShapeKit represents a single tshapekit insertion action on a scene, insert a new tshapekit node to the scene.
*/

class CmdInsertShapeKit : public QUndoCommand
{
public:
    CmdInsertShapeKit( const QModelIndex& parentIndex, TShapeKit* shapeKit, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdInsertShapeKit();

     virtual void undo();
     virtual void redo();

 private:
   	 SoBaseKit* m_coinParent;
     TShapeKit* m_shapeKit;
     SceneModel* m_pModel;
     int m_row;

};

#endif /*CMDINSERTSHAPEKIT_H_*/
