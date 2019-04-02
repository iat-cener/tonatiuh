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
#include <QTextBlock>

#include "CodeEditArea.h"

CodeEditArea::CodeEditArea( QWidget* parent )
:QPlainTextEdit( parent )
{
	connect(this, SIGNAL( cursorPositionChanged() ), this, SLOT( UpdateCurrentLine() ) );
	/*connect(this, SIGNAL( blockCountChanged( int ) ), this, SLOT( UpdateCodeEditAreaWidth( int ) ) );
	connect(this, SIGNAL( updateRequest( QRect, int ) ), m_LineNumberWidget, SLOT( UpdateLineNumberArea( QRect, int ) ) );

*/
	//UpdateCodeEditAreaWidth(0);
	UpdateCurrentLine();
}


CodeEditArea::~CodeEditArea()
{

}

int CodeEditArea::BlockTop( const QTextBlock & block ) const
{
	return (int) blockBoundingGeometry(block).translated(contentOffset()).top();
}

int CodeEditArea::BlockHeight( const QTextBlock & block ) const
{
	return (int) blockBoundingRect(block).height();
}

 void CodeEditArea::UpdateCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if( !isReadOnly() )
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::yellow).lighter(160);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}
