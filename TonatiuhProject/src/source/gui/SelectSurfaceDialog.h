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


#ifndef SELECTSURFACEDIALOG_H_
#define SELECTSURFACEDIALOG_H_

#include <QDialog>

#include "ui_selectsurfacedialog.h"

class NodesFilterModel;
//class QItemSelectionModel;
class SceneModel;

class SelectSurfaceDialog: public QDialog, private Ui::SelectSurfaceDialog
{
	Q_OBJECT

public:
	SelectSurfaceDialog( SceneModel& sceneModel, bool enableLight = true, QWidget* parent = 0 );
	~SelectSurfaceDialog();

	void SetShapeTypeFilters( QVector< QString >  shapeTypeFilters );

	QString GetSelectedSurfaceURL() const;

public slots:
	void accept();

private:
	bool m_isLightVisible;
	SceneModel* m_pCurrentSceneModel;
	NodesFilterModel* m_pNodeFilterModel;
};

#endif /* SELECTSURFACEDIALOG_H_ */
