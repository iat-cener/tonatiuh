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

#ifndef RUNOPTIONSDIALOG_H_
#define RUNOPTIONSDIALOG_H_

#include <QDialog>


#include "ui_runoptionsdialog.h"

class SceneModel;
class ExportDialog;

//!  RunOptionsDialog class is the dialog to define the light parameters.
/*!
  RunOptionsDialog sets light parameters. The parameters that user can define are light suunshape, light input aperture shape and light position.
*/
class RunOptionsDialog : public QDialog, private Ui::RunOptionsDialog
{
	Q_OBJECT

public:
	RunOptionsDialog( SceneModel& sceneModel, QWidget* parent = 0 );
	~RunOptionsDialog();


	QString GetExportFileName() const;
    int GeNumberOfPhotonsPerFile() const;
	QString GetSurfaceNode() const;
    QString GetSQLExportFileName() const;
    bool InOneFile();
    bool IsDatFileChecked() const;
    bool IsExportAllPhotonsEnabled() const;
    bool IsSaveCoordinateEnabled() const;
    bool IsSavePowerPerPhotonEnabled() const;
    bool IsSaveSideEnabled() const;
    bool ExportPhotonsInGlobal();
    bool isLaunchActive();

public slots:
	void accept();
	void reject();

private slots:
	void SelectFileSave();
	void ChangeSaveOptions();
	void SQLSelectFile();
	void SelectSurface();

private:
	SceneModel* m_exportSceneModel;
    QString m_surfaceNodeName;
    QString m_surfaceExportFileName;
    bool m_globalCoordinates;
    bool m_launchRays;
};

#endif /*RUNOPTIONSDIALOG_H_*/
