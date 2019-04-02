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

#ifndef EXPORTDIALOG_H_
#define EXPORTDIALOG_H_

#include <QDialog>
#include <QItemSelectionModel>

#include "SceneModel.h"
#include "ui_exportdialog.h"

//!  ExportDialog class is the dialog to define the photon map export mode.
/*!
  ExportDialog sets the photons to export, the coordinates system and the file to save selected information.
*/
class ExportDialog : public QDialog, private Ui::ExportDialog
{
	Q_OBJECT

public:
	ExportDialog( SceneModel& sceneModel, QString previousSurfaceUrl = 0, bool previusInGlobal = true, QString previousFile = 0, QWidget* parent = 0 );
	ExportDialog( SceneModel& sceneModel, QWidget* parent );
	~ExportDialog();

	bool ExportAllPhotonMap() const;
	bool ExportPhotonsInGlobal() const;
	QString GetExportFileName() const;
	QString GetSelectedSurface() const;


public slots:
	void accept();

private slots:
	void SetExportAllPhotons( bool allPhotos );
	void SetExportSurfacePhotons( bool surfacePhotos );
	void SelectFile();


private:
	SceneModel* m_exportSceneModel;
	QItemSelectionModel*  m_exportSelectionModel;
};

#endif /* EXPORTDIALOG_H_ */
