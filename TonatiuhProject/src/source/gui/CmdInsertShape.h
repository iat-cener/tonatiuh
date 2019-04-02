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

#ifndef CMDINSERTSHAPE_H_
#define CMDINSERTSHAPE_H_

#include <QUndoCommand>

class SceneModel;
class TShapeKit;
class TShape;

//!  CmdInsertShape class is the insert command for shapes stored in the command stack.
/*!
  CmdInsertShape represents a single shape insertion action on a scene, insert a new shape node to the scene.
*/

class CmdInsertShape : public QUndoCommand
{
public:
    CmdInsertShape( TShapeKit* shapeKit, TShape* shape, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdInsertShape();

     virtual void undo();
     virtual void redo();

 private:
     TShapeKit* m_shapeKit;
     TShape* m_shape;
     SceneModel* m_pModel;
     int m_row;
};

#endif
