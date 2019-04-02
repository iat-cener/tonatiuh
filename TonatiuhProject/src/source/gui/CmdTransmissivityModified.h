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

#ifndef CMDTRANSMISSIVITYMODIFIED_H_
#define CMDTRANSMISSIVITYMODIFIED_H_

//!  CmdTransmissivityModified class is the transmissivity definition command stored in the command stack.
/*!
  CmdTransmissivityModified represents a modification of the scene transmissivity. The modification could be a new transmissivity definition,
  or a modificaiton of the transmissivity parameters values.
*/

#include <QUndoCommand>

class TSceneKit;
class TTransmissivity;

class CmdTransmissivityModified : public QUndoCommand
{

public:
	CmdTransmissivityModified( TTransmissivity* newTransmissivity, TSceneKit* scene,  QUndoCommand* parent = 0 );
    ~CmdTransmissivityModified();

     virtual void undo();
     virtual void redo();

private:
     bool m_isPreviousTransmissivity;
     TTransmissivity* m_pPreviousTransmissivity;
     TTransmissivity* m_pNewTransmissivity;
     TSceneKit* m_scene;
};


#endif /* CMDTRANSMISSIVITYMODIFIED_H_ */
