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

#ifndef CODEEDITAREA_H_
#define CODEEDITAREA_H_

#include <QPlainTextEdit>

class CodeEditArea : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditArea( QWidget* parent = 0 );
	~CodeEditArea();

	int BlockTop( const QTextBlock & block ) const;
	int BlockHeight( const QTextBlock & block ) const;

//public slots:
	//void UpdateCodeEditAreaWidth( int width );

//protected:
	//void resizeEvent(QResizeEvent* event );

private slots:
	void UpdateCurrentLine();

//private:
	//LineNumberWidget* m_LineNumberWidget;
};

#endif /* CODEEDITAREA_H_ */
