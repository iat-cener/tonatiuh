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

#ifndef LIGHTDIALOG_H_
#define LIGHTDIALOG_H_

#include <QDialog>
#include <QMap>


#include "ui_lightdialog.h"

class QComboBox;
class QFrame;
class QGroupBox;
class QItemSelectionModel;
class QTabWidget;
class SceneModel;
class TLightKit;
class TShape;
class TShapeFactory;
class TSunShape;
class TSunShapeFactory;

//!  LightDialog class is the dialog to define the light parameters.
/*!
  LightDialog sets light parameters. The parameters that user can define are light suunshape, light input aperture shape and light position.
*/
class LightDialog : public QDialog, private Ui::LightDialog
{
	Q_OBJECT

public:
	LightDialog( SceneModel& sceneModel, TLightKit* currentLightKit, QVector< TSunShapeFactory* > sunshapeFactoryList, QWidget* parent = 0 );
	~LightDialog();

	TLightKit* GetTLightKit();

public slots:
	 void accept();
	void SetValue( SoNode* node, QString paramenterName, QString newValue );

protected slots:
	void ChangeSunshape( int index );
	void AddNodeToDisabledNodeList();
	void RemoveNodeFromDisabledNodeList();

private:
	//void GenerateNodeTree( QModelIndex parentIndex, QTreeWidgetItem* parentIntem );
	void SetupSunSizeTab();
	void SetupTriggers();
	void SunPositionTab();
	void SunshapeTab();
	void SunshapeBox();


    TLightKit* m_currentLightKit;
    SceneModel* m_currentSceneModel;
	int m_currentSunShapeIndex;
    TSunShape* m_newSunShape;
    QItemSelectionModel* m_sceneSelectionModel;

	QMap< QString, TSunShapeFactory* > m_sunshapeList;

};

#endif /*LIGHTDIALOG_H_*/
