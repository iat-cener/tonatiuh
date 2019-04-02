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


#ifndef LINENUMBERWIDGET_H_
#define LINENUMBERWIDGET_H_

#include <QWidget>

class CodeEditArea;

class LineNumberWidget: public QWidget
{
	Q_OBJECT

public:
	LineNumberWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~LineNumberWidget();

	void SetCodeEditor( CodeEditArea* editor );
	QSize sizeHint() const;

	int LineNumberAreaWidth() const;

public slots:
	void UpdateLineNumberArea( const QRect& rect, int dy );

protected:
	void paintEvent( QPaintEvent *event );

private:
	CodeEditArea* m_codeEditArea;

};
#endif /* LINENUMBERWIDGET_H_ */
