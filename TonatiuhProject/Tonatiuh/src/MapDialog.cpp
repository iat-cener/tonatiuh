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

#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLayout>
#include <QObject>
#include <QSpacerItem>
#include <QSplitter>
#include <QStringListModel>

#include <MarbleDirs.h>
#include "MarbleWidget.h"

#include "MapDialog.h"
#include "tgc.h"
#include "Trace.h"


#include <iostream>

MapDialog::MapDialog( QWidget *parent )
   : QDialog(parent)
{
	QDir directory( qApp->applicationDirPath() );
	directory.cd( "data" );

	MarbleDirs::setMarbleDataPath( directory.absolutePath() );

    setWindowTitle(tr("Marble - Desktop Globe"));

    resize( 680, 500 );

	QVBoxLayout* vlayout = new QVBoxLayout( );
	vlayout->setMargin( 0 );
	setLayout( vlayout );

	m_splitter = new QSplitter( this );
	vlayout->addWidget( m_splitter );

	//ControlWidget
	QWidget* controlWidget = new QWidget;
	m_splitter->addWidget( controlWidget );

	QVBoxLayout* tabLayout = new QVBoxLayout( );
	controlWidget->setLayout(tabLayout );

	QTabWidget* tabWidget = new QTabWidget( this );
	tabLayout->addWidget( tabWidget );

	//Position Tab
    QWidget* currentLocationWidget = new QWidget;
    tabWidget->addTab( currentLocationWidget, "Current Location" );

    QVBoxLayout* locationLayout = new QVBoxLayout;
    currentLocationWidget->setLayout( locationLayout );

    QLabel* latLabel = new QLabel( "Latitude" );
    locationLayout->addWidget( latLabel );

    QHBoxLayout* latLayout = new QHBoxLayout;
    locationLayout->addLayout( latLayout );

    m_latSpinBox = new QDoubleSpinBox;
    latLayout->addWidget( m_latSpinBox );
    m_latSpinBox->setMinimumSize(QSize(82, 0));
    m_latSpinBox->setDecimals(4);
    m_latSpinBox->setMaximum(90);

    m_latComboBox = new QComboBox;
    m_latComboBox->insertItems(0, QStringList()
     << "N"
     << "S" );
    latLayout->addWidget( m_latComboBox );

    QSpacerItem* spacerItem = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    latLayout->addItem( spacerItem );

    QLabel*  lonLabel = new QLabel( "Longitude" );
    locationLayout->addWidget( lonLabel );

    QHBoxLayout* lonLayout = new QHBoxLayout;
    locationLayout->addLayout( lonLayout );

    m_lonSpinBox = new QDoubleSpinBox;
    m_lonSpinBox->setMinimumSize(QSize(82, 0));
    m_lonSpinBox->setDecimals(4);
    m_lonSpinBox->setMaximum(180);
    lonLayout->addWidget( m_lonSpinBox );

    m_lonComboBox = new QComboBox;
    m_lonComboBox->insertItems(0, QStringList()
     << "E"
     << "W" );
    lonLayout->addWidget( m_lonComboBox );

    QSpacerItem* spacerItem2 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    lonLayout->addItem( spacerItem2 );

	QSpacerItem* verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    locationLayout->addItem( verticalSpacer );

	//Control Tab
    m_control = new MarbleControlBox( this );
    m_control->setLegendTabShown( false );
    tabWidget->addTab( m_control, "Control" );

    //Dialog buttons
    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect( buttons, SIGNAL( accepted() ), this, SLOT( accept() ) );
	connect( buttons, SIGNAL( rejected() ), this, SLOT( reject() ) );
    tabLayout->addWidget( buttons );

    //Marble Widget
    m_marbleWidget = new MarbleWidget( this );
    m_marbleWidget->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ) );
    m_marbleWidget->setMinimumZoom( m_control->minimumZoom() );
	m_control->addMarbleWidget( m_marbleWidget );
    m_splitter->addWidget( m_marbleWidget );

    m_splitter->setSizes( QList<int>() << 180 << width()-180 );
    m_splitter->setStretchFactor(m_splitter->indexOf(m_control), 0);
    m_splitter->setStretchFactor(m_splitter->indexOf(m_marbleWidget), 1);

    connect( m_marbleWidget, SIGNAL( mouseClickGeoPosition( double, double, GeoPoint::Unit ) ), this, SLOT( changeGpsPosition( double, double, GeoPoint::Unit ) ) );
}

void MapDialog::GetCoordinates( double* lon, double* lat ) const
{
	*lon = m_longitude;
	*lat = m_latitude;
}

void MapDialog::SetCoordinates(  double lon, double lat )
{
	m_longitude = lon;
	m_latitude = lat;

	changeGpsPosition( m_longitude, -m_latitude, GeoPoint::Radian );
	m_control->receiveGpsCoordinates( m_longitude, -m_latitude, GeoPoint::Radian );
}

void MapDialog::zoomIn()
{
    m_marbleWidget->zoomIn();
}

void MapDialog::zoomOut()
{
    m_marbleWidget->zoomOut();
}

void MapDialog::moveLeft()
{
    m_marbleWidget->moveLeft();
}

void MapDialog::moveRight()
{
    m_marbleWidget->moveRight();
}

void MapDialog::moveUp()
{
    m_marbleWidget->moveUp();
}

void MapDialog::moveDown()
{
    m_marbleWidget->moveDown();
}

void MapDialog::changeGpsPosition( double lon, double lat, GeoPoint::Unit )
{
	Trace trace( "MapDialog::changeGpsPosition", false );

	// Gps Position Coordinates are positive for south and east

	m_longitude = lon;
	if( m_longitude < 0 ) m_lonComboBox->setCurrentIndex( 1 );
	else m_lonComboBox->setCurrentIndex( 0 );
	m_lonSpinBox->setValue( abs( m_longitude * ( 180 / tgc::Pi ) ) );

	m_latitude = -lat;
	if( m_latitude < 0 ) m_latComboBox->setCurrentIndex( 1 );
	else m_latComboBox->setCurrentIndex( 0 );

	m_latSpinBox->setValue( abs( m_latitude * ( 180 / tgc::Pi ) ) );
}
