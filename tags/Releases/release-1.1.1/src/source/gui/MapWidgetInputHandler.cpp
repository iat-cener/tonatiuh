/*
 * MapWidgetInputHandler.cpp
 *
 *  Created on: 25/10/2010
 *      Author: amutuberria
 */

#include <QEvent>
#include "MapWidgetInputHandler.h"

MapWidgetInputHandler::MapWidgetInputHandler()
:MarbleWidgetDefaultInputHandler()
{

}

MapWidgetInputHandler::~MapWidgetInputHandler()
{

}


bool MapWidgetInputHandler::eventFilter( QObject* object, QEvent* event )
{

	bool eventOut = MarbleWidgetDefaultInputHandler::eventFilter( object, event );

	if( event->type() != QEvent::MouseMove )	emit WidgetModified();

	return eventOut;
}
