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

#ifndef CMDLIGHTKITMODIFIED_H_
#define CMDLIGHTKITMODIFIED_H_

#include <QUndoCommand>

class SceneModel;
class SoSceneKit;
class TLightKit;
class TShape;
class TSunShape;

//!  CmdLightKitModified class is the light definition command stored in the command stack.
/*!
  CmdLightKitModified represents a modification of the scene light. The modification could be a new light definition,
  a modificaiton of the light position on the scene or change on the sun shape parameters value.
   \sa CmdLightPositionModified
*/

class CmdLightKitModified : public QUndoCommand
{
public:
    CmdLightKitModified( TLightKit* newLightKit, SoSceneKit* scene, SceneModel& sceneModel, QUndoCommand* parent = 0 );
    ~CmdLightKitModified();

     virtual void undo();
     virtual void redo();

private:
     bool m_previousLightKit;
     double m_previousAzimuth;
     QString m_previousDisbleNodes;
     double m_previousZenith;
     TSunShape* m_pPreviousSunShape;
     TLightKit* m_pNewLightKit;
     SoSceneKit* m_scene;
     SceneModel* m_pModel;
};

#endif /*CMDLIGHTKITMODIFIED_H_*/
