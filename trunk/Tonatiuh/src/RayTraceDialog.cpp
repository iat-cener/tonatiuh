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
 
#include "RayTraceDialog.h"

#include <QFileDialog>
#include <QDir>
RayTraceDialog::RayTraceDialog( QWidget * parent, Qt::WindowFlags f )
:QDialog ( parent, f )
{
	setupUi( this );
	connect( this, SIGNAL( accepted() ), this, SLOT( saveChanges() ) );
}

RayTraceDialog::RayTraceDialog( int numRays, double fraction, bool drawPhotons, bool increasePhotonMap, QString resultsFile, QWidget * parent, Qt::WindowFlags f )
:QDialog ( parent, f ), m_numRays( numRays ), m_fraction( fraction ), m_drawPhotons( drawPhotons ), m_increasePhotonMap( increasePhotonMap ), m_resultsFile( resultsFile )
{
	setupUi( this );
	raysSpinBox->setValue( m_numRays );
	drawSpinBox->setValue( m_fraction );
	photonsCheckBox->setChecked( m_drawPhotons );
	
	if ( m_increasePhotonMap )
		increaseMapRadioButton->setChecked( true );
	else
		newMapRadioButton->setChecked( true );
		
	resultsFileLineEdit->setText( m_resultsFile );
	connect( this, SIGNAL( accepted() ), this, SLOT( saveChanges() ) );
}

RayTraceDialog::~RayTraceDialog()
{
	
}

void RayTraceDialog::on_applyPushButton_clicked( bool checked )
{

	m_numRays = raysSpinBox->value();
	
	m_fraction = drawSpinBox->value();
	m_drawPhotons = photonsCheckBox->isChecked();
	
	if( newMapRadioButton->isChecked() ) 
		m_increasePhotonMap = false;
	else
		m_increasePhotonMap = true;
	m_resultsFile = resultsFileLineEdit->text();
	
	
	
}

void RayTraceDialog::on_fileDialogButton_clicked( bool checked )
{
	QString oldFile = resultsFileLineEdit->text();
	QString fileName = QFileDialog::getSaveFileName( this,
	                       tr( "Save results document" ), "results",
	                       tr( "*.dat" ) );
	
	if (fileName.isNull() ) resultsFileLineEdit->setText( oldFile );
	else resultsFileLineEdit->setText( fileName );
	
	
}

void RayTraceDialog::saveChanges()
{
	m_numRays = raysSpinBox->value();
	
	m_fraction = drawSpinBox->value();
	m_drawPhotons = photonsCheckBox->isChecked();
	
	if( newMapRadioButton->isChecked() ) 
		m_increasePhotonMap = false;
	else
		m_increasePhotonMap = true;
		
	m_resultsFile = resultsFileLineEdit->text();

	
}