/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
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
