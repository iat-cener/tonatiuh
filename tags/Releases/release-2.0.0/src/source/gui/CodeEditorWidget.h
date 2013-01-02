/*
 * CodeEditorWidget.h
 *
 *  Created on: 14/04/2011
 *      Author: amutuberria
 */

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
