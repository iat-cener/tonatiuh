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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QPainter>
#include <QTextBlock>

#include "CodeEditor.h"
#include "LineNumberWidget.h"

LineNumberWidget::LineNumberWidget( CodeEditor* parent, Qt::WindowFlags f )
:QWidget( parent, f ), m_codeEditor( parent )
{

}

LineNumberWidget::~LineNumberWidget()
{

}

void LineNumberWidget::SetCodeEditor( CodeEditor* editor )
{
	m_codeEditor = editor;
}

QSize LineNumberWidget::sizeHint() const
{
	return QSize( LineNumberAreaWidth(), 0 );
}

int LineNumberWidget::LineNumberAreaWidth() const
{
	int digits = 1;
	int max = qMax(1, m_codeEditor->blockCount());
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

	m_codeEditor->UpdateCodeEditorWidth( 0 );
}

void  LineNumberWidget::paintEvent( QPaintEvent *event )
{
	QPainter painter( this );
	painter.fillRect(event->rect(), Qt::lightGray);

	QTextBlock block = m_codeEditor->document()->begin();
	int blockNumber = block.blockNumber();
	int top = m_codeEditor->BlockTop(block);
	int bottom = top + m_codeEditor->BlockHeight(block);

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
		bottom = top + m_codeEditor->BlockHeight(block);
		++blockNumber;
	}
}
