 /****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain additional
 ** rights.  These rights are described in the Nokia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Nokia at qt-info@nokia.com.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>


class LineNumberWidget;
class QPaintEvent;
class QResizeEvent;

/*!
 * \class CodeEditor.
 * \brief CodeEditor class is the dialog to define the photon map export mode.
 *
 * \section Keys Using key bindings
 *
 * The list of key bindings for editing are:
 * 	\li \b Backspace: Deletes the character to the left of the cursor.
 * 	\li \b Delete: Deletes the character to the right of the cursor.
 *  \li \b Ctrl+C: Copy the selected text to the clipboard.
 *  \li \b Ctrl+Insert: Copy the selected text to the clipboard.
 *  \li \b Ctrl+K: Deletes to the end of the line.
 *  \li \b Ctrl+V: Pastes the clipboard text into text edit.
 *  \li \b Shift+Insert: Pastes the clipboard text into text edit.
 *  \li \b Ctrl+X: Deletes the selected text and copies it to the clipboard.
 *  \li \b Shift+Delete: Deletes the selected text and copies it to the clipboard.
 *  \li \b Ctrl+Z: Undoes the last operation.
 *  \li \b Ctrl+Y: Redoes the last operation.
 *  \li \b LeftArrow: Moves the cursor one character to the left.
 *  \li \b Ctrl+LeftArrow: Moves the cursor one word to the left.
 *  \li \b RightArrow: Moves the cursor one character to the right.
 *  \li \b Ctrl+RightArrow: Moves the cursor one word to the right.
 *  \li \b UpArrow: Moves the cursor one line up.
 *  \li \b Ctrl+UpArrow: Moves the cursor one word up.
 *  \li \b DownArrow: Moves the cursor one line down.
 *  \li \b Ctrl+Down Arrow: Moves the cursor one word down.
 *  \li \b PageUp: Moves the cursor one page up.
 *  \li \b PageDown: Moves the cursor one page down.
 *  \li \b Home: Moves the cursor to the beginning of the line.
 *  \li \b Ctrl+Home: Moves the cursor to the beginning of the text.
 *  \li \b End: Moves the cursor to the end of the line.
 *  \li \b Ctrl+End: Moves the cursor to the end of the text.
 *  \li \b Alt+Wheel: Scrolls the page horizontally (the Wheel is the mouse wheel).
 *  \li \b Ctrl+Wheel: Zooms the text.
 *
*/

class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditor( QWidget *parent = 0 );
	~CodeEditor();

	int BlockTop( const QTextBlock & block ) const;
	int BlockHeight( const QTextBlock & block ) const;

public slots:
	void UpdateCodeEditorWidth( int width );

protected:
	void resizeEvent(QResizeEvent* event );

private slots:
	void UpdateCurrentLine();

private:
	LineNumberWidget* m_LineNumberWidget;
};

#endif /*CODEEDITOR_H*/
