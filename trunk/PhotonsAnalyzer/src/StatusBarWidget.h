/*
 * StatusBarWidget.h
 *
 *  Created on: 17-jul-2009
 *      Author: amutuberria
 */

#ifndef STATUSBARWIDGET_H_
#define STATUSBARWIDGET_H_

#include <QWidget>

#include "ui_statusbarwidget.h"

class StatusBarWidget : public QWidget, private Ui::StatusBarWidget
{
	Q_OBJECT

public:
	StatusBarWidget( QWidget* parent = 0 );
	~StatusBarWidget();

	void SetText( QString text );

public slots:
	void SetProgress( int progress );

};
#endif /* STATUSBARWIDGET_H_ */
