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

#ifndef CMDCUT_H_
#define CMDCUT_H_

#include <QUndoCommand>

class QModelIndex;
class SceneModel;

//!  CmdCut class is the cut command stored in the command stack.
/*!
  CmdCut represents a single cut action on a scene, removes selected node from the scene and stores it in the clipboard.
   \sa CmdCopy, CmdPaste, CmdDelete
*/

class CmdCut : public QUndoCommand
{
public:
    CmdCut( const QModelIndex& selectedIndex, SoNode*& clipboard, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdCut();

     virtual void undo();
     virtual void redo();

private:
	SoNode*& m_pClipboard;
	SoNode* m_previousNode;
	SoNode* m_coinNode;
	SoBaseKit* m_coinParent;
	SceneModel* m_pModel;
	int m_row;
};

#endif /*CMDCUT_H_*/
