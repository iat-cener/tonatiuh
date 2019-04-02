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

#ifndef CMDCOPY_H_
#define CMDCOPY_H_

#include <QUndoCommand>

class QModelIndex;
class SoBaseKit;
class SceneModel;

//!  CmdCopy class is the copy command stored in the command stack.
/*!
  CmdCopy represents a single copy action on a scene, copy on a clipboard a node from the scene.
   \sa CmdCut, CmdPaste, CmdDelete
*/

class CmdCopy : public QUndoCommand
{
public:
    CmdCopy( const QModelIndex& index, SoNode*& clipboard, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdCopy();

     virtual void undo();
     virtual void redo();

private:

	SoNode*& m_pClipboard; /*!< The Clipboard */
	SoNode* m_pNode;	/*!< Copied node to the clipboard*/
	SoNode* m_previousNode; /*!< The node saved on the clipboard befor apply copy*/
	SceneModel* m_pModel; /*!< The scene */

};

#endif /*CMDCOPY_H_*/
