/*
 * MapWidgetInputHandler.h
 *
 *  Created on: 25/10/2010
 *      Author: amutuberria
 */

#ifndef MAPWIDGETINPUTHANDLER_H_
#define MAPWIDGETINPUTHANDLER_H_

#include "MarbleWidgetInputHandler.h"

using namespace Marble;

class MapWidgetInputHandler : public MarbleWidgetDefaultInputHandler
{
    Q_OBJECT

public:
	MapWidgetInputHandler(MarbleWidget *marbleWidget);
	~MapWidgetInputHandler();

    virtual bool eventFilter( QObject* object, QEvent* event );

signals:
	void WidgetModified();

};
#endif /* MAPWIDGETINPUTHANDLER_H_ */
