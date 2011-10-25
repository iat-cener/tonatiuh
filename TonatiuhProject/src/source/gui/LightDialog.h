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

#ifndef LIGHTDIALOG_H_
#define LIGHTDIALOG_H_

#include <QDialog>
#include <QMap>


#include "ui_lightdialog.h"

class QComboBox;
class QFrame;
class QGroupBox;
class QTabWidget;
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
	LightDialog(  TLightKit* currentLightKit, QVector< TSunShapeFactory* > sunshapeFactoryList, QWidget* parent = 0 );
	~LightDialog();

	TLightKit* GetTLightKit();

public slots:
	 void accept();
	void SetValue( SoNode* node, QString paramenterName, QString newValue );

protected slots:
	void ChangeSunshape( int index );

private:
	void SunPositionTab();
	void SunshapeTab();
	void SunshapeBox();


    TLightKit* m_currentLightKit;

	int m_currentSunShapeIndex;
    TSunShape* m_newSunShape;

	QMap< QString, TSunShapeFactory* > m_sunshapeList;

};

#endif /*LIGHTDIALOG_H_*/