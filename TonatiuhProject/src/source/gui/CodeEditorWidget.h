/***************************************************************************
 Copyright (C) 2008 by the Tonatiuh Software Development Team.

 This file is part of Tonatiuh.

 Tonatiuh program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.


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

 Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola, Gilda Jimenez,
 Juana Amieva, Azael Mancillas, Cesar Cantu.
 ***************************************************************************/

#ifndef CODEEDITORWIDGET_H_
#define CODEEDITORWIDGET_H_


#include <QWidget>

#include "ui_codeeditorwidget.h"

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

class CodeEditorWidget : public QWidget, private Ui::CodeEditorWidget
{
	Q_OBJECT

public:
	CodeEditorWidget( QWidget *parent = 0, Qt::WindowFlags f = 0);
	~CodeEditorWidget();

	QTextDocument* Document() const;
	bool OkToContinue();
	void OpenScriptFile( QString fileName );

signals:
	void FileOpened( QString fileName );
	void FileSaved( QString fileName );
	void RunScript();
	void Warging( QString message );

public slots:
	void NewScriptFile();
	void OpenScriptFile();
	void UpdateCodeEditorWidth( int width );

private slots:
	void Run();
	bool SaveAsScriptFile();
	bool SaveScript();

private:
	bool SaveScriptFile( const QString& fileName );
	void SetupToolbar();
	void StartDocument( QString fileName );

	QString m_currentScritFileName;
};


#endif /* CODEEDITORWIDGET_H_ */
