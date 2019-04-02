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

#ifndef CMDPASTE_H_
#define CMDPASTE_H_

#include <QUndoCommand>

#include "tgc.h"

class InstanceNode;
class QModelIndex;
class SceneModel;
class SoBaseKit;

//!  CmdPaste class is the paste command stored in the command stack.
/*!
  CmdPaste represents a single paste action on a scene, paste the node on the clipboard to selected node in the scene.
	If the tgc::Shared parameter defined to the command, the node at the clipboard is shared between the previous parents and the new parent. tgc::Copy to create a new node before the paste.
   \sa CmdCopy, CmdCut, CmdDelete
*/

class CmdPaste : public QUndoCommand
{
public:
    CmdPaste( tgc::PasteType type, const QModelIndex& parentModelIndex,  SoNode*& coinClipboard, SceneModel& sceneModel, QUndoCommand* parent = 0 );
    ~CmdPaste();

     virtual void undo();
     virtual void redo();

private:
    tgc::PasteType m_pasteType;
	InstanceNode* m_parentInstance;
	SoNode* m_coinChild;
	SceneModel* m_sceneModel;
	QString m_oldNodeName;
	int m_row;
};

#endif /*CMDPASTE_H_*/
