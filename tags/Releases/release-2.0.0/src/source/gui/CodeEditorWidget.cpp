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

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>
#include <QTextBlock>
#include <QTextStream>

#include "CodeEditorWidget.h"
#include "LineNumberWidget.h"

/*!
 * Creates a new code editor widget.
 */
CodeEditorWidget::CodeEditorWidget( QWidget *parent, Qt::WindowFlags f )
:QWidget( parent, f ),
 m_currentScritFileName( QString( "" ) )
{

	setupUi( this );

	SetupToolbar();

	lineNumberWidget->SetCodeEditor( codeEditor );

	connect( codeEditor, SIGNAL( updateRequest( QRect, int ) ), lineNumberWidget, SLOT( UpdateLineNumberArea( QRect, int ) ) );

}

/*!
 * Destroys editor.
 */
CodeEditorWidget::~CodeEditorWidget()
{

}


QTextDocument* CodeEditorWidget::Document() const
{
	return codeEditor->document();
}

/*!
 * Returns \a true if the application is ready to start with other script. Otherwise,
 * returns \a false.
 */
bool CodeEditorWidget::OkToContinue()
{

	QTextDocument* document = codeEditor->document();

	if ( document->isModified () )
	{
		int answer = QMessageBox::warning( this, tr( "Tonatiuh" ),
		                 tr( "The document has been modified.\n"
		                     "Do you want to save your changes?"),
		                 QMessageBox::Yes | QMessageBox::Default,
		                 QMessageBox::No,
		                 QMessageBox::Cancel | QMessageBox::Escape );

		if ( answer == QMessageBox::Yes ) return SaveScript();
		else if ( answer == QMessageBox::Cancel ) return false;
	}
	return true;
}

/*!
 * Opens the script saved at \a fileName.
 */
void CodeEditorWidget::OpenScriptFile( QString fileName )
{
	if( !OkToContinue() ) return;

	StartDocument( fileName );

}

/*!
 * Creates a new empty script.
 *
 * Before creates the new scripts, it verifies if the current script can be closed.
 */
void CodeEditorWidget::NewScriptFile()
{
	if( !OkToContinue() ) return;

	QTextDocument* document = codeEditor->document();
	document->clear();

	m_currentScritFileName = "";
	emit FileOpened( QString( "" ) );

	document->setModified( false );

}

/*!
 * Opens a script file into the script editor and sets this file as current file.
 *
 * A dialog is created to select an existing script file.
 *
 * Before open the selected script, it verifies if the current script can be closed.
 */
void CodeEditorWidget::OpenScriptFile()
{
	if( !OkToContinue() ) return;

	QSettings settings( "NREL UTB CENER", "Tonatiuh" );
	QString dirName = settings.value( "codeeditorwidget.openDirectory", "." ).toString();


	QString fileName = QFileDialog::getOpenFileName( this,
	                               tr( "Open File" ),  dirName,
	                               tr( "Tonatiuh script file (*.tnhs)" ) );

	if ( fileName.isEmpty() )	return;

	QFileInfo file( fileName );
	 settings.setValue( "codeeditorwidget.openDirectory", file.absolutePath() );

	StartDocument( fileName );

}

/**
 * Executes the code editor script.
 */
void CodeEditorWidget::Run()
{
	 emit RunScript();
}

/*!
 * Returns \a true if script is correctly saved. Otherwise, returns \a false. A file dialog is created to select a file.
 *
 * \sa SaveScriptFile, SaveFile.
 */
bool CodeEditorWidget::SaveAsScriptFile()
{
	QSettings settings( "NREL UTB CENER", "Tonatiuh" );

	QString dirName = settings.value( "codeeditorwidget.saveDirectory", "." ).toString();


	QString fileName = QFileDialog::getSaveFileName( this,
		                       tr( "Save File" ), dirName,
		                       tr( "Tonatiuh script file (*.tnhs)" ) );
		if( fileName.isEmpty() ) return false;

	QFileInfo file( fileName );
	settings.setValue( "codeeditorwidget.saveDirectory", file.absolutePath() );


	return SaveScriptFile( fileName );

}

/*!
 * Saves the editor code in the file defined as current. If a current file is not defined, it calls to \a SaveAsScriptFile to define a current file.
 *
 * \sa SaveScriptFile, SaveAsScriptFile.
 */
bool CodeEditorWidget::SaveScript()
{
	if ( m_currentScritFileName.isEmpty() ) return SaveAsScriptFile();
	else return SaveScriptFile( m_currentScritFileName );
}

/*!
 * Returns \a true if the scripts is correctly saved into the the given \a fileName. Otherwise,
 * returns \a false.
 *
 * \sa SaveAsScriptFile, SaveScript.
 */
bool CodeEditorWidget::SaveScriptFile( const QString& fileName )
{
	QTextDocument* document = codeEditor->document();

	QFile scriptFile( fileName );
	if( !scriptFile.open( QIODevice::WriteOnly) )
	{
		QMessageBox::warning( this, tr( "Tonatiuh warning" ),
									  tr( "Cannot open file %1." )
									.arg( fileName ) );
		return false;
	}

	QTextStream out( &scriptFile );
	out<<document->toPlainText();

	scriptFile.close();
	document->setModified( false );


	m_currentScritFileName = fileName;
	emit FileSaved( fileName );
	return true;
}

/*!
 * Updates code editor width.
 */
void CodeEditorWidget::UpdateCodeEditorWidth( int /* width*/ )
{
	QWidget* viewport = codeEditor->viewport();
	viewport->setContentsMargins( lineNumberWidget->LineNumberAreaWidth(), 0, 0, 0 );
}

/*!
 * Creates connector for toolbar buttons.
 */
void CodeEditorWidget::SetupToolbar()
{

	connect( newButton, SIGNAL( clicked( bool) ), this, SLOT( NewScriptFile() ) );
	connect( openButton, SIGNAL( clicked( bool ) ), this, SLOT( OpenScriptFile() ) );
	connect( saveButton, SIGNAL( clicked( bool ) ), this, SLOT( SaveScript() ) );
	connect( saveAsButton, SIGNAL( clicked( bool ) ), this, SLOT( SaveAsScriptFile() ) );

	connect( undoButton, SIGNAL( clicked( bool ) ), codeEditor, SLOT( undo() ) );
	connect( redoButton, SIGNAL( clicked( bool ) ), codeEditor, SLOT( redo() ) );

	connect( cutButton, SIGNAL( clicked( bool ) ), codeEditor, SLOT( cut() ) );
	connect( copyButton, SIGNAL( clicked( bool ) ), codeEditor, SLOT( copy() ) );
	connect( pasteButton, SIGNAL( clicked( bool ) ), codeEditor, SLOT( paste() ) );

	connect( runButton, SIGNAL( clicked( bool) ), this, SLOT( Run() ) );
}

/*!
 * Sets the code editor's contents to \a fileName contents and sets the currents file to \a fileName.
 */
void CodeEditorWidget::StartDocument( QString fileName )
{
	QFile scriptFile( fileName );
	if( !scriptFile.open( QIODevice::ReadOnly) )
	{
		QMessageBox::warning( this, tr( "Tonatiuh warning" ),
									  tr( "Cannot open file %1." )
									.arg( fileName ) );
		return;
	}

	QTextStream in( &scriptFile );

	QTextDocument* document = codeEditor->document();
	document->clear();
	document->setPlainText( in.readAll() );
	scriptFile.close();

	m_currentScritFileName = fileName;
	emit FileOpened( fileName );
	document->setModified( false );
}
