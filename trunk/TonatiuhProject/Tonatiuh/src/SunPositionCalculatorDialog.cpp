/***************************************************************************
 Copyright (C) 2008 by the Tonatiuh Software Development Team.

 This file is part of Tonatiuh.

 Tonatiuh program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.


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

 Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola, Gilda Jimenez,
 Juana Amieva, Azael Mancillas, Cesar Cantu.
 ***************************************************************************/

#include "MapDialog.h"
#include "SunPositionCalculatorDialog.h"
#include "tgc.h"
#include "Trace.h"

SunPositionCalculatorDialog::SunPositionCalculatorDialog( QWidget* parent )
: QDialog( parent )
{
	setupUi(this);
	setFixedSize( QSize( 840, 460 ) );

	connect( calendarWidget, SIGNAL( selectionChanged( ) ), this, SLOT( ChangeDate( ) ) );

	connect( uTimeSpin, SIGNAL( timeChanged( QTime ) ), this, SLOT( ChangeSunTime( QTime ) ) );
	connect( cTimeSpin, SIGNAL( timeChanged( QTime ) ), this, SLOT( ChangeSunTime( QTime ) ) );
	connect( zoneSpin, SIGNAL( valueChanged( int ) ), this, SLOT( ChangeSunTimeZone( int ) ) );

	connect( longitudeSpin, SIGNAL( valueChanged(  double ) ), this, SLOT( ChangeLongitude( double ) ) );
	connect( longitudeSpin, SIGNAL( valueChanged(  double ) ), worldMap, SLOT( LocalLongitudeChanged( double ) ) );
	connect( latitudeSpin, SIGNAL( valueChanged( double) ), this, SLOT( ChangeLatitude( double ) ) );
	connect( latitudeSpin, SIGNAL( valueChanged(  double ) ), worldMap, SLOT( LocalLatitudeChanged( double ) ) );

	connect( this, SIGNAL( changeRepresentation( cSunCoordinates ) ), worldMap, SLOT( SunChanged( cSunCoordinates ) ) );
	connect( this, SIGNAL( changeRepresentation( cSunCoordinates ) ), horizontalWidget, SLOT( CoordinatesChanged( cSunCoordinates ) ) );
	connect( this, SIGNAL( changeRepresentation( cSunCoordinates ) ), celestialWidget, SLOT( CoordinatesChanged( cSunCoordinates ) ) );
}

SunPositionCalculatorDialog::~SunPositionCalculatorDialog()
{

}

void SunPositionCalculatorDialog::SetDateTime( QDateTime time )
{
	uTimeSpin->setTime( time.time() );
	cTimeSpin->setTime( time.time() );
	zoneSpin->setValue( 0 );
	calendarWidget-> setSelectedDate( time.date() );
}

void SunPositionCalculatorDialog::ChangePosition( QDateTime time, double longitude, double latitude )
{
	calendarWidget->setSelectedDate( time.date() );
	UT->setChecked( true);
	uTimeSpin->setTime( time.time() );

	longitudeSpin->setValue( longitude );
	latitudeSpin->setValue( latitude );
}

void SunPositionCalculatorDialog::on_updateButton_clicked()
{

	QDateTime* time = GetTime();
	emit changeSunLight( time, longitudeSpin->value(), latitudeSpin->value() );

}

void SunPositionCalculatorDialog::ChangeDate()
{
	CalculateSunPosition();
}

void SunPositionCalculatorDialog::ChangeLongitude( double /*longitude*/ )
{
	CalculateSunPosition();
}

void SunPositionCalculatorDialog::ChangeLatitude( double /*latitude*/ )
{
	CalculateSunPosition();
}

void SunPositionCalculatorDialog::ChangeSunTime( QTime /*time*/ )
{
	CalculateSunPosition();
}

void SunPositionCalculatorDialog::ChangeSunTimeZone( int /*timeZone*/ )
{
	CalculateSunPosition();
}

void SunPositionCalculatorDialog::on_selectButton_clicked()
{
	Trace( "SunPositionCalculatorDialog::on_selectButton_clicked" , true );

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
void SunPositionCalculatorDialog::CalculateSunPosition()
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

QDateTime* SunPositionCalculatorDialog::GetTime()
{
	QTime time = UT->isChecked() ? uTimeSpin->time() : cTimeSpin->time().addSecs( zoneSpin->value() * 3600 );

	QDateTime* dateTime = new QDateTime( calendarWidget->selectedDate(), time );
	return dateTime;
}
