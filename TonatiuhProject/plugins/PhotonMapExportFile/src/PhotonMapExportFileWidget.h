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


#ifndef PHOTONMAPEXPORTFILEWIDGET_H_
#define PHOTONMAPEXPORTFILEWIDGET_H_

#include <QWidget>

#include "PhotonMapExportParametersWidget.h"

#include "ui_photonmapexportfilewidget.h"

class PhotonMapExportFileWidget : public PhotonMapExportParametersWidget, private Ui::PhotonMapExportFileWidget
{
	Q_OBJECT

public:
	PhotonMapExportFileWidget( QWidget* parent = 0 );
	~PhotonMapExportFileWidget();

    QStringList GetParameterNames() const;
    QString GetParameterValue( QString parameter ) const;

private slots:
	void SelectSaveDirectory();

private:
    void SetupTriggers();
};

#endif /* PHOTONMAPEXPORTFILEWIDGET_H_ */



