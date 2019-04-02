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

#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>

#include "CodeEditorWidget.h"
#include "LineNumberWidget.h"

LineNumberWidget::LineNumberWidget( QWidget* parent, Qt::WindowFlags f )
:QWidget( parent, f ),
 m_codeEditArea( 0 )
{

}

LineNumberWidget::~LineNumberWidget()
{

}

void LineNumberWidget::SetCodeEditor( CodeEditArea* editor )
{
	m_codeEditArea = editor;
}

QSize LineNumberWidget::sizeHint() const
{
	return QSize( LineNumberAreaWidth(), 0 );
}

int LineNumberWidget::LineNumberAreaWidth() const
{
	int digits = 1;
	int max = qMax(1, m_codeEditArea->blockCount() );
	while (max >= 10)
	{
		max /= 10;
		++digits;
	}

	int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

void LineNumberWidget::UpdateLineNumberArea( const QRect& rect, int dy )
{
	if( dy )	scroll( 0, dy );
	else	update( 0, rect.y(), width(), rect.height() );

	QWidget* viewport = m_codeEditArea->viewport();
	viewport->setContentsMargins( LineNumberAreaWidth(), 0, 0, 0 );

}

void  LineNumberWidget::paintEvent( QPaintEvent *event )
{
	QPainter painter( this );
	painter.fillRect(event->rect(), Qt::lightGray);

	QTextBlock block = m_codeEditArea->document()->begin();
	int blockNumber = block.blockNumber();

	int top = m_codeEditArea->BlockTop( block );
	int bottom = top + m_codeEditArea->BlockHeight(block);

	while( block.isValid() && top <= ( event->rect().bottom() ) )
	{
		if( block.isVisible() && ( bottom >= event->rect().top() ) )
		{
			QString number = QString::number( blockNumber + 1 );
			painter.setPen(Qt::black);
			painter.drawText( 0, top,width(), fontMetrics().height(), Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + m_codeEditArea->BlockHeight(block);
		++blockNumber;
	}
}
