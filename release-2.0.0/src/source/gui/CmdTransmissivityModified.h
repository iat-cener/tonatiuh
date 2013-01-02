/*
 * CmdTransmissivityModified.h
 *
 *  Created on: 16/01/2012
 *      Author: amutuberria
 */

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
