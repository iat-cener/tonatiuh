/*
 * StatusBarWidget.cpp
 *
 *  Created on: 17-jul-2009
 *      Author: amutuberria
 */

#include "StatusBarWidget.h"
#include "Trace.h"

StatusBarWidget::StatusBarWidget( QWidget* parent )
:QWidget( parent )
{
	Trace trace( "StatusBarWidget::StatusBarWidget", false );
	setupUi( this );

}
StatusBarWidget::~StatusBarWidget()
{
	Trace trace( "StatusBarWidget::~StatusBarWidget", false );
}

void StatusBarWidget::SetText( QString text )
{
	Trace trace( "StatusBarWidget::SetText", false );
	statusLabel->setText( text );
	repaint();
}


void StatusBarWidget::SetProgress( int progress )
{
	Trace trace( "StatusBarWidget::SetProgress", false );
	statusProgressBar->setValue( progress );
}
