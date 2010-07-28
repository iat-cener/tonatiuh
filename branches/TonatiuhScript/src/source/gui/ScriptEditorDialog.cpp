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

#include <iostream>

#include <QAction>
#include <QFileDialog>
#include <QIcon>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QScriptEngine>
#include <QScriptSyntaxCheckResult>
#include <QScriptValueList>
#include <QString>
#include <QToolBar>
#include <QTextStream>

#include "ScriptEditorDialog.h"
#include "ScriptRayTracer.h"
#include "tonatiuh_script.h"

ScriptEditorDialog::ScriptEditorDialog(  QVector< TPhotonMapFactory* > listTPhotonMapFactory, QVector< RandomDeviateFactory* > listRandomDeviateFactory, QString dirName, QWidget* parent )
:QDialog( parent ),
 m_dirLineEdit( 0 ),
 m_currentScritFileName( 0 ),
 m_interpreter( 0 )
{
	setupUi( this );
	setWindowFlags( windowFlags() | Qt::WindowMinMaxButtonsHint );

	QSplitter* pSplitter = findChild<QSplitter *>( "mainSplitter" );

	QList<int> sizes;
	sizes<<mainWidget->width()*0.05<<mainWidget->width()*0.95;
	pSplitter->setSizes ( sizes );

	AddCodeEditorWidgetToolbar();
	AddFilesExplorerWidgetToolbar( dirName );

	connect( filesList, SIGNAL( itemDoubleClicked ( QListWidgetItem*  ) ), this, SLOT( OpenScriptFile( QListWidgetItem*  ) ) );

	//Init QtScript environment
	m_interpreter = new QScriptEngine;

	QObject* rayTracer = new ScriptRayTracer( listTPhotonMapFactory, listRandomDeviateFactory );
	QScriptValue rayTracerValue = m_interpreter->newQObject( rayTracer );
	m_interpreter->globalObject().setProperty( "rayTracer", rayTracerValue );


}

ScriptEditorDialog::~ScriptEditorDialog()
{
	delete m_interpreter;
}

QString ScriptEditorDialog::GetCurrentDirectory()
{
	return m_dirLineEdit->text();
}

void ScriptEditorDialog::closeEvent( QCloseEvent* event )
{
    if ( OkToContinue() )
    {
    	event->accept();
    }
    else event->ignore();
}

void ScriptEditorDialog::AddCodeEditorWidgetToolbar()
{
	QToolBar* codeToolBar = new QToolBar;
	codeToolBar->setIconSize(QSize(22, 22));
	codeToolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	codeWidgetLayout->insertWidget( 0, codeToolBar );

	QAction* actionNew = new QAction( QIcon( ":/icons/new.png" ), tr("New" ), this );
	actionNew->setObjectName( QString::fromUtf8("actionNew") );
	codeToolBar->addAction( actionNew );
	connect(actionNew, SIGNAL( triggered() ), this, SLOT( NewScriptFile() ) );

	QAction* actionOpen = new QAction( QIcon( ":/icons/open.png" ), tr("Open" ), this );
	actionOpen->setObjectName( QString::fromUtf8("actionOpen") );
	codeToolBar->addAction( actionOpen );
	connect(actionOpen, SIGNAL( triggered() ), this, SLOT( OpenScriptFile() ) );

	QAction* actionSave = new QAction( QIcon( ":/icons/save.png" ), tr("Save" ), this );
	actionSave->setObjectName( QString::fromUtf8("actionSave") );
	codeToolBar->addAction( actionSave );
	connect(actionSave, SIGNAL( triggered() ), this, SLOT( SaveScriptFile() ) );

	QAction* actionSaveAs = new QAction( QIcon( ":/icons/saveas.png" ), tr("SaveAs" ), this );
	actionSaveAs->setObjectName( QString::fromUtf8("actionSaveAs") );
	codeToolBar->addAction( actionSaveAs );
	connect(actionSaveAs, SIGNAL( triggered() ), this, SLOT( SaveAsScriptFile() ) );

	codeToolBar->addSeparator();

	QAction* actionUndo = new QAction( QIcon( ":/icons/undo.png" ), tr("Undo" ), this );
	actionUndo->setObjectName( QString::fromUtf8("actionUndo") );
	codeToolBar->addAction( actionUndo );
	connect(actionUndo, SIGNAL( triggered() ), codeEditor, SLOT( undo() ) );

	QAction* actionRedo= new QAction( QIcon( ":/icons/redo.png" ), tr("Undo" ), this );
	actionRedo->setObjectName( QString::fromUtf8("actionRedo") );
	codeToolBar->addAction( actionRedo );
	connect(actionRedo, SIGNAL( triggered() ), codeEditor, SLOT( redo() ) );

	codeToolBar->addSeparator();

	QAction* actionCut = new QAction( QIcon( ":/icons/cut.png" ), tr("Cut" ), this );
	actionCut->setObjectName( QString::fromUtf8("actionCut") );
	codeToolBar->addAction( actionCut );
	connect(actionCut, SIGNAL( triggered() ), codeEditor, SLOT( cut() ) );

	QAction* actionCopy = new QAction( QIcon( ":/icons/copy.png" ), tr("Copy" ), this );
	actionCopy->setObjectName( QString::fromUtf8("actionCopy") );
	codeToolBar->addAction( actionCopy );
	connect(actionCopy, SIGNAL( triggered() ), codeEditor, SLOT( copy() ) );

	QAction* actionPaste= new QAction( QIcon( ":/icons/paste.png" ), tr("Paste" ), this );
	actionPaste->setObjectName( QString::fromUtf8("actionPaste") );
	codeToolBar->addAction( actionPaste );
	connect(actionPaste, SIGNAL( triggered() ), codeEditor, SLOT( paste() ) );

	codeToolBar->addSeparator();

	QAction* actionRun= new QAction( QIcon( ":/icons/run.png" ), tr("Undo" ), this );
	actionRun->setObjectName( QString::fromUtf8("actionRun") );
	codeToolBar->addAction( actionRun );
	connect(actionRun, SIGNAL( triggered() ), this, SLOT( RunScript() ) );

}

void ScriptEditorDialog::AddFilesExplorerWidgetToolbar( QString dirName )
{
	QToolBar* filesToolBar = new QToolBar;
	filesToolBar->setIconSize(QSize(22, 22));
	filesToolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	filesWidgetLayout->insertWidget( 0, filesToolBar );

	QAction* actionUp = new QAction( QIcon( ":/icons/up.png" ), tr("Up One Level" ), this );
	actionUp->setObjectName( QString::fromUtf8("actionUp") );
	filesToolBar->addAction( actionUp );
	connect(actionUp, SIGNAL( triggered() ), this, SLOT( CdUpDir() ) );

	QAction* actionRefresh = new QAction( QIcon( ":/icons/refresh.png" ), tr("Refresh" ), this );
	actionRefresh->setObjectName( QString::fromUtf8("actionRefresh") );
	filesToolBar->addAction( actionRefresh );
	connect(actionRefresh, SIGNAL( triggered() ), this, SLOT( RefreshDirList() ) );

	m_dirLineEdit = new QLineEdit;

	if( !dirName.isEmpty() )
	{
		QDir dir( dirName );
		if( dir.exists() )
		{
			m_dirLineEdit->setText( dir.absolutePath() );
			RefreshDirList();
		}
	}
	filesToolBar->addWidget( m_dirLineEdit );

	QAction* actionOpenDirectory = new QAction( QIcon( ":/icons/open.png" ), tr("Open Directory" ), this );
	actionOpenDirectory->setObjectName( QString::fromUtf8("actionOpenDirectory") );
	filesToolBar->addAction( actionOpenDirectory );

	connect(actionOpenDirectory, SIGNAL( triggered() ), this, SLOT( OpenDirectory() ) );

}

bool ScriptEditorDialog::OkToContinue()
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

		if ( answer == QMessageBox::Yes ) return SaveScriptFile();
		else if ( answer == QMessageBox::Cancel ) return false;
	}
	return true;
}

bool ScriptEditorDialog::SaveFile( const QString& fileName )
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
	SetCurrentFile( fileName );
	document->setModified( false );

	return true;
}

void ScriptEditorDialog::SetCurrentFile( const QString& fileName )
{
	m_currentScritFileName = fileName;

	QString shownName = "Untitled";
	if ( !m_currentScritFileName.isEmpty() )
	{
		shownName = QFileInfo( fileName ).fileName();
	}

	setWindowTitle( tr( "%1[*] - %2" ).arg( shownName ).arg( tr( "Tonatiuh" ) ) );
}

void ScriptEditorDialog::StartDocument( QString fileName )
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

	SetCurrentFile( fileName );
	document->setModified( false );
}

void ScriptEditorDialog::OpenDirectory()
{
	QString oldDirName = m_dirLineEdit->text();

	QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			oldDirName,
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	QDir dir( dirName );
	if( dir.exists() )
	{
		m_dirLineEdit->setText( dir.absolutePath() );
		RefreshDirList();
	}
}

void ScriptEditorDialog::OpenScriptFile( QListWidgetItem* item )
{
	if( !OkToContinue() ) return;

	QString dirName = m_dirLineEdit->text();
	QString fileName = item->text();
	QFileInfo fileInfo( dirName, fileName );

	StartDocument( fileInfo.absoluteFilePath() );
}


void ScriptEditorDialog::RefreshDirList()
{

	QString dirName = m_dirLineEdit->text();

	QDir dir( dirName );
	dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
	dir.setSorting( QDir::DirsFirst  | QDir::Name );

	QFileInfoList fileList = dir.entryInfoList( );

	QIcon folderIcon;
	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
	QIcon fileIcon;
	fileIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

	filesList->clear();

	for( int file = 0; file < fileList.size(); ++file )
	{
		if( !fileList[file].isHidden() )
		{
			if( fileList[file].isDir() )	filesList->addItem( new QListWidgetItem( folderIcon, fileList[file].fileName () ) );
			else if( fileList[file].completeSuffix() == "tnhs" )	filesList->addItem( new QListWidgetItem( fileIcon, fileList[file].fileName () ) );
		}
	}
}


void ScriptEditorDialog::CdUpDir()
{
	QString dirName = m_dirLineEdit->text();
	QDir dir( dirName );

	if( dir.cdUp() )
	{
		m_dirLineEdit->setText( dir.absolutePath() );
		RefreshDirList();
	}

}


void ScriptEditorDialog::NewScriptFile()
{
	if( !OkToContinue() ) return;

	QTextDocument* document = codeEditor->document();
	document->clear();

	SetCurrentFile( "" );
	document->setModified( false );

}

void ScriptEditorDialog::OpenScriptFile()
{
	if( !OkToContinue() ) return;

	QString fileName = QFileDialog::getOpenFileName( this,
	                               tr( "Open File" ),  m_dirLineEdit->text(),
	                               tr( "Tonatiuh script file (*.tnhs)" ) );

	if ( fileName.isEmpty() )	return;

	StartDocument( fileName );

}


bool ScriptEditorDialog::SaveScriptFile()
{
	if ( m_currentScritFileName.isEmpty() ) return SaveAsScriptFile();
	else return SaveFile( m_currentScritFileName );
}

bool ScriptEditorDialog::SaveAsScriptFile()
{
	QString fileName = QFileDialog::getSaveFileName( this,
	                       tr( "Save File" ), m_dirLineEdit->text(),
	                       tr( "Tonatiuh script file (*.tnhs)" ) );
	if( fileName.isEmpty() ) return false;

	return SaveFile( fileName );

}

void  ScriptEditorDialog::RunScript()
{

	int initialized = tonatiuh_script::init( m_interpreter );
	if( !initialized )
	{
		QMessageBox::warning( this, tr( "Tonatiuh" ), tr( "Script Execution Error." ) );
		return;
	}

	QTextDocument* document = codeEditor->document();

	QString program = document->toPlainText();
	QScriptSyntaxCheckResult checkResult = m_interpreter->checkSyntax( program );
	if( checkResult.state() != QScriptSyntaxCheckResult::Valid )
	{
		QMessageBox::warning( this, tr( "Tonatiuh" ),
				QString( "Script Execution Error.\n"
						"Line: %1. %2" ).arg( QString::number( checkResult.errorLineNumber() ), checkResult.errorMessage () ) );
		return;
	}

	QScriptValue result = m_interpreter->evaluate(document->toPlainText());
	if( result.isError () )
	{

		QScriptValue lineNumber = result.property( "lineNumber");
		QMessageBox::warning( this, tr( "Tonatiuh" ),
			QString( "Script Execution Error.\nLine%1. %2" ).arg( QString::number( lineNumber.toNumber() ), result.toString() ) );
	}
	else
		QMessageBox::information ( this, tr( "Tonatiuh" ), tr( "The script execution is successfully finished" ) );
}
