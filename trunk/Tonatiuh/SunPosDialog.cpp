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

#include <QCalendarWidget>
#include <QDate>
#include <QDateTime>
#include <QMessageBox>

#include "SunPosDialog.h"
#include "MapDialog.h"
#include "sunpos.h"
#include "tgc.h"

SunPosDialog::SunPosDialog( QWidget* parent )
: QDialog( parent )
{
	setupUi(this);
	setFixedSize( QSize( 840, 460 ) );
	
	connect( calendarWidget, SIGNAL( selectionChanged( ) ), this, SLOT( CalculateSunPosition( ) ) );
	
	connect( uTimeSpin, SIGNAL( timeChanged ( QTime ) ), this, SLOT( CalculateSunPosition( ) ) );
	connect( cTimeSpin, SIGNAL( timeChanged ( QTime ) ), this, SLOT( CalculateSunPosition( ) ) );
	connect( zoneSpin, SIGNAL( valueChanged ( int) ), this, SLOT( CalculateSunPosition( ) ) );
	
	connect( longitudeSpin, SIGNAL( valueChanged (  double ) ), this, SLOT( CalculateSunPosition( ) ) );
	connect( longitudeSpin, SIGNAL( valueChanged (  double ) ), worldMap, SLOT( LocalLongitudeChanged( double ) ) );
	connect( latitudeSpin, SIGNAL( valueChanged ( double) ), this, SLOT( CalculateSunPosition( ) ) );
	
	connect( this, SIGNAL( changeRepresentation( cSunCoordinates ) ), worldMap, SLOT( SunChanged( cSunCoordinates ) ) );
	connect( this, SIGNAL( changeRepresentation( cSunCoordinates ) ), horizontalWidget, SLOT( CoordinatesChanged( cSunCoordinates ) ) );
	connect( this, SIGNAL( changeRepresentation( cSunCoordinates ) ), celestialWidget, SLOT( CoordinatesChanged( cSunCoordinates ) ) );
}

SunPosDialog::~SunPosDialog()
{
	
}

void SunPosDialog::SetDateTime( QDateTime time )
{
	uTimeSpin->setTime( time.time() );
	cTimeSpin->setTime( time.time() );
	zoneSpin->setValue( 0 );
	calendarWidget-> setSelectedDate( time.date() );
}
    
void SunPosDialog::ChangePosition( QDateTime time, double longitude, double latitude )
{
	calendarWidget->setSelectedDate( time.date() );
	UT->setChecked( true);
	uTimeSpin->setTime( time.time() );
	
	longitudeSpin->setValue( longitude );
	latitudeSpin->setValue( latitude );
}

void SunPosDialog::on_updateButton_clicked()
{	
	
	//CalculateSunPosition();
	QDateTime* time = GetTime();
	
	emit changeSunLight( time, longitudeSpin->value(), latitudeSpin->value() );

}

void SunPosDialog::CalculateSunPosition()
{

   //Fecha y Hora
	QDateTime* time = GetTime();

	int year = time->date().year();
	int month = time->date().month();
	int day = time->date().day();
	
	double hours = time->time().hour ();
	double minutes = time->time().minute ();
	double seconds = time->time().second ();
	cTime myTime = { year, month, day, hours, minutes, seconds };

	//Localization
	double longitude = longitudeSpin->value();
	double latitude = latitudeSpin->value();
    cLocation myLocation = {longitude , latitude };
    
	//Calculate sun position
	cSunCoordinates results;
	sunpos( myTime, myLocation, &results );
    
    emit changeRepresentation(results);	
}

void SunPosDialog::on_selectButton_clicked()
{
	MapDialog marbleDialog;
	marbleDialog.SetCoordinates( longitudeSpin->value()*( tgc::Pi / 180), -latitudeSpin->value()*( tgc::Pi / 180) );
	if( marbleDialog.exec() )
	{
		double longitude;
		double latitude;
		marbleDialog.GetCoordinates( &longitude, &latitude );
	
    	longitudeSpin->setValue( longitude * ( 180 / tgc::Pi ) );
    	latitudeSpin->setValue( latitude * ( 180 / tgc::Pi ) );
    }
}

QDateTime* SunPosDialog::GetTime()
{
	QDateTime* dateTime = new QDateTime;
	dateTime->setDate( calendarWidget->selectedDate() );
	
	QTime time;
	if(SunPosDialog::UT->isChecked())
		dateTime->setTime( uTimeSpin->time() );
	else
	{
		dateTime->setTime( cTimeSpin->time() );
		dateTime->addSecs( zoneSpin->value() * 3600 );
	}
			

	return dateTime;
}
