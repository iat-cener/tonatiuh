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


#ifndef CMDCHANGENODENAME_H_
#define CMDCHANGENODENAME_H_

#include <QUndoCommand>

class QString;
class SceneModel;
class SoNode;

class CmdChangeNodeName : public QUndoCommand
{
public:
	CmdChangeNodeName( const QModelIndex& index, QString newName, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdChangeNodeName();

     virtual void undo();
     virtual void redo();

private:
     QString m_newName;/*!< New node name*/
     QString m_previousName;/*!< Previous node name*/
     SoNode* m_pNode;	/*!< Changed node*/
     SceneModel* m_pModel; /*!< The scene */

};


#endif /* CMDCHANGENODENAME_H_ */
