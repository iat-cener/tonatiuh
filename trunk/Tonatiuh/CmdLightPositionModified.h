/***************************************************************************
 *   Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             *
 *                         Victor Martin, Javier Garcia-Barberena,         *
 *			   			   Inaki Perez, Inigo Pagola					   *                      			   *
 *                                   					  			 	   *
 *   mblanco@cener.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	   *
 *									   									   *
 ***************************************************************************/

#ifndef CMDLIGHTPOSITIONMODIFIED_H_
#define CMDLIGHTPOSITIONMODIFIED_H_

#include <QDateTime>
#include <QUndoCommand>

class TLightKit;


class CmdLightPositionModified : public QUndoCommand
{

public: 
	CmdLightPositionModified( TLightKit* light, QDateTime time, double longitude, double latitude, QUndoCommand* parent = 0 );
	~CmdLightPositionModified();
	
	virtual void undo();
    virtual void redo();
    
private:
	TLightKit* lightKit;
	
	QDateTime oldTime;
	QDateTime newTime;
	
	double oldLongitude;
	double newLongitude;
	
	double oldLatitude;
	double newLatitude;
};

#endif /*CMDLIGHTPOSITIONMODIFIED_H_*/
