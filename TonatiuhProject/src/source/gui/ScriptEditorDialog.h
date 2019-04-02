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


#ifndef SCRIPTEDITORDIALOG_H_
#define SCRIPTEDITORDIALOG_H_

#include <QDialog>
#include <QScriptValue>

#include "ui_scripteditordialog.h"

class FilesModel;
class QItemSelectionModel;
class QLineEdit;
class QScriptContext;
class QScriptEngine;
class TPhotonMapFactory;
class RandomDeviateFactory;

//!  ScriptEditorDialog class is the dialog to edit and run scripts with Tonatiuh.
/*!
  ScriptEditorDialog allow to the user open, edit, run and save scripts to automate the ray tracing.
*/

class ScriptEditorDialog : public QDialog, private Ui::ScriptEditorDialog
{
	Q_OBJECT

public:
	ScriptEditorDialog( QVector< RandomDeviateFactory* > listRandomDeviateFactory, QWidget* parent = 0 );
	~ScriptEditorDialog();

	void ExecuteScript( QString tonatiuhScriptFile );


	static QScriptValue ImportExtension(QScriptContext *context, QScriptEngine *engine);
	static QScriptValue PrintMessage( QScriptContext* context, QScriptEngine* engine );

public slots:
	void AbortEvaluation( QString error );

protected:
    void closeEvent( QCloseEvent* event );

private slots:
	void Close( bool checked  );
	void OpenScriptFile( const QModelIndex& index );
	void RunScript();
	void SetCurrentFile( QString fileName );
	void WriteMessage(  QString message );

private:
	QString m_currentScritFileName;
	FilesModel* m_fileModel;
	QScriptEngine* m_interpreter;
};

#endif /* SCRIPTEDITORDIALOG_H_ */
