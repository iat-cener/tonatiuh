/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This FilesModelfile is part of Tonatiuh.

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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>
#include <stdio.h>

#include <QDateTime>
#include <QScriptEngine>

#include "FilesModel.h"
#include "ScriptEditorDialog.h"
#include "ScriptRayTracer.h"
#include "tonatiuh_script.h"

/**
 * Creates a dialog to edit scripts and run them. The list \a listRandomDeviateFactory is
 * the random generator types that can be defined in the scripts to run Tonatiuh. The dialog explorer shows the directories and scripts files from \a dirName path.
 */
ScriptEditorDialog::ScriptEditorDialog( QVector< RandomDeviateFactory* > listRandomDeviateFactory, QWidget* parent )
:QDialog( parent ),
 m_currentScritFileName( "" ),
 m_fileModel( 0 ),
 m_interpreter( 0 )
{
	setupUi( this );
	setWindowFlags( windowFlags() | Qt::WindowMinMaxButtonsHint );

	QSize windowSize = size();
	QList<int> sizes;
    sizes<<windowSize.height() * 0.8 <<windowSize.height() * 0.2;
    dialogVSplitter->setSizes ( sizes );

	QStringList fileNamesFilter;
	fileNamesFilter<<"*.tnhs";

	m_fileModel = new FilesModel;
	m_fileModel->setRootPath( "" );
	m_fileModel->setNameFilters( fileNamesFilter );
    fileTree->setModel( m_fileModel );


  	//Init QtScript environment
	m_interpreter = new QScriptEngine;
	m_interpreter->importExtension( "qt.core" );

	QScriptValue tonatiuh = m_interpreter->newQObject( parent );
	m_interpreter->globalObject().setProperty( "tonatiuh", tonatiuh );

	QScriptValue logConsoleObject = m_interpreter->newQObject( logWidget );
	m_interpreter->globalObject().setProperty( "console", logConsoleObject );

	QObject* rayTracer = new ScriptRayTracer( listRandomDeviateFactory );
	QScriptValue rayTracerValue = m_interpreter->newQObject( rayTracer );
	m_interpreter->globalObject().setProperty( "rayTracer", rayTracerValue );

	QScriptValue printFunction = m_interpreter->newFunction( ScriptEditorDialog::PrintMessage );
	m_interpreter->globalObject().setProperty("print", printFunction );

	//m_interpreter->globalObject().setProperty( "print", m_interpreter->newFunction( ScriptEditorDialog::WriteMessage ) );



	 connect( fileTree, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( OpenScriptFile( const QModelIndex& ) ) );
	 connect( codeEditorWidget, SIGNAL( FileOpened( QString ) ), this, SLOT( SetCurrentFile( QString ) ) );
	 connect( codeEditorWidget, SIGNAL( FileSaved( QString ) ), this, SLOT( SetCurrentFile( QString ) ) );
	 connect( codeEditorWidget, SIGNAL( RunScript( ) ), this, SLOT( RunScript() ) );
	 connect( runButton, SIGNAL( clicked( bool ) ), this, SLOT( RunScript() ) );
	 connect( closeButton, SIGNAL( clicked( bool ) ), this, SLOT( Close( bool ) ) );
	 connect( parent, SIGNAL( Abort( QString ) ), this, SLOT( AbortEvaluation( QString ) ) );



}

/**
 * Destroys the file dialog.
 */
ScriptEditorDialog::~ScriptEditorDialog()
{
	delete m_fileModel;
	delete m_interpreter;
}

void ScriptEditorDialog::AbortEvaluation( QString error )
{
	QScriptContext* context = m_interpreter->currentContext();
	context->throwError(  error  );

}

/**
 * Reimplemented from QDialog::closeEvent().
 */
void ScriptEditorDialog::closeEvent( QCloseEvent* event )
{
    if ( codeEditorWidget->OkToContinue() )	event->accept();
    else event->ignore();
}

/*!
 * Closes dialog.
 */
/*!
 * Closes the dialog.
 */
void ScriptEditorDialog::Close( bool /* checked */  )
{
   close();
}


/*!
 * Opens the file selected into the file explorer.
 *
 * Before open the selected script, it verifies if the current script can be closed.
 */
void ScriptEditorDialog::OpenScriptFile( const QModelIndex& index )
{
	QString fileName = m_fileModel->data( index, QFileSystemModel::FilePathRole ).toString();
	QFileInfo fileInfo( fileName );
	if( !fileInfo.isDir() )
	{
		codeEditorWidget->OpenScriptFile( fileName );
	}

}

/**
 * Executes the code editor script.
 */
void  ScriptEditorDialog::RunScript()
{

    QDateTime start=QDateTime::currentDateTime();
	QString logmessage = QString( "[%1]\t Start running script.\n").arg( start.toString() );
	WriteMessage( logmessage );

	int initialized = tonatiuh_script::init( m_interpreter );
	if( !initialized )
	{
		QString logmessage = QString( "[%1]\t Script Execution Error.\n").arg( QDateTime::currentDateTime().toString() );
		WriteMessage( logmessage );
		std::cerr<<logmessage.toStdString()<<std::endl;

		return;
	}
	QScriptValue rayTracerValue = m_interpreter->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	QFileInfo currentFile( m_currentScritFileName );
	rayTracer->SetDir( currentFile.absoluteDir().absolutePath()  );

	QTextDocument* document = codeEditorWidget->Document();

	QString program = document->toPlainText();

	QScriptSyntaxCheckResult checkResult = m_interpreter->checkSyntax( program );
	if( checkResult.state() != QScriptSyntaxCheckResult::Valid )
	{
		QString logmessage = QString( "[%1]\t Script Execution Error.\n"
				"\t Line: %2. %3\n" ).arg( QDateTime::currentDateTime().toString(), QString::number( checkResult.errorLineNumber() ), checkResult.errorMessage () );
		WriteMessage( logmessage );
		std::cerr<<logmessage.toStdString()<<std::endl;

		return;
	}

	QScriptValue result = m_interpreter->evaluate(document->toPlainText());
	if( result.isError () )
	{
		QString logmessage = QString( "[%1]\t Script Execution Error. %2\n" ).arg( QDateTime::currentDateTime().toString(), result.toString() );
		WriteMessage( logmessage );
		//std::cerr<<logmessage.toStdString()<<std::endl;
	}
	else
	{
		/*
		 *Display a dialog
		 *
		QString st;
		QString area( QString("The valid sun area is %2" ).arg(st.setNum(rayTracer->GetArea())));
		QString drawRays( QString("Number of rays traced per interaction: %1").arg(st.setNum(int(rayTracer->GetNumrays()))));
		QString logmessage = QString( "[%1]\t The script execution is successfully finished in %2 seconds.\n \t\t %3 \n \t\t %4 \n" ).arg( QDateTime::currentDateTime().toString(),st.setNum(start.secsTo( QDateTime::currentDateTime() )),area,drawRays );
		WriteMessage( logmessage );
		*
		*/
		QString logmessage = QString( "[%1]\t The script execution is successfully finished.\n" ).arg( QDateTime::currentDateTime().toString());
		WriteMessage( logmessage );
	}

}

/*!
 * Sets the editor current script file to \a fileName.
 *
 * Changes the editor dialog title to shows the current file base name.
 */
void ScriptEditorDialog::SetCurrentFile( QString fileName )
{
	m_currentScritFileName = fileName;

	QString shownName = "Untitled";
	if ( !m_currentScritFileName.isEmpty() )
	{
		shownName = QFileInfo( fileName ).fileName();
	}

	setWindowTitle( tr( "%1[*] - %2" ).arg( shownName ).arg( tr( "Tonatiuh" ) ) );

	QString time = QDateTime::currentDateTime().toString();
	QString logmessage = QString( "[%1]\t Current script file: '%2'").arg( QDateTime::currentDateTime().toString(), m_currentScritFileName  );

	WriteMessage( logmessage );
}

/*!
 * Write the \a message at log window.
 */
void ScriptEditorDialog::WriteMessage( QString message )
{
	logWidget->appendPlainText( message );
}


QScriptValue ScriptEditorDialog::PrintMessage( QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue consoleObject = engine->globalObject().property( "console" );
	QPlainTextEdit* console = ( QPlainTextEdit* ) consoleObject.toQObject();


	if( context->argumentCount() != 1 )	return 0;
	if( !context->argument( 0 ).isString() )	return 0;

	QString message = context->argument(0).toString();
	console->insertPlainText( message );

	return 1;
}


