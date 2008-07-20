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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez, 
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef SUNDIALOG_H_
#define SUNDIALOG_H_

#include <QDialog>

#include <vector>

class QComboBox;
class QDateEdit;
class QDoubleSpinBox;
class QFrame;
class QLabel;
class QRadioButton;
class QTabWidget;
class QTimeEdit;
class TLightKit;
class TShape;
class TShapeFactory;
class TSunShape;
class TSunShapeFactory;

class SunDialog : public QDialog
{
	Q_OBJECT
	
public:
	SunDialog(  TLightKit* currentLightKit, std::vector< TShapeFactory* > shapeFactoryList, std::vector< TSunShapeFactory* > m_sunshapeFactoryList, QWidget* parent = 0 );
	~SunDialog();
	
	TLightKit* GetTLightKit();
	 
protected slots:
	void changeShape( int index );
	void changeSunshape( int index );
	void openMarble();
	
private:
	
	void DirectionTab();
	void SunshapeTab();
	void SetAngles();

    QTabWidget* m_tabWidget;
    QRadioButton* m_radioAngles;
    QRadioButton* m_radioCalculates;
    QDoubleSpinBox* m_azimuthSpin;
    QDoubleSpinBox* m_zenithSpin;
    QDoubleSpinBox* m_longitudeSpin;
    QDoubleSpinBox* m_latitudeSpin;
    QTimeEdit* m_timeEdit;
    QDateEdit* m_dateEdit;
    QComboBox* m_sunshapeCombox;
    QComboBox* m_shapeCombox;
    QFrame* m_sunshapeParameters;
    QFrame* m_shapeParameters;
    
    
    TLightKit* m_currentLightKit;
	int m_currentSunShapeIndex;
	int m_currentShapeIndex;
	
    double m_zenith;
    double m_azimuth;
    TSunShape* m_sunShape;
    TShape* m_shape;
    std::vector< TShapeFactory* > m_shapeList;
	std::vector< TSunShapeFactory* > m_sunshapeList;

};

#endif /*SUNDIALOG_H_*/
