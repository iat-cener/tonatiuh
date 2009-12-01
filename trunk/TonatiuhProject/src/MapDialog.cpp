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

#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>

#include <HttpDownloadManager.h>
#include <MarbleControlBox.h>
#include <MarbleDirs.h>
#include <MarbleMap.h>
#include <MarbleModel.h>
#include <MarbleWidget.h>
#include <MapThemeManager.h>

#include "MapDialog.h"
#include "tgc.h"
#include "Trace.h"

#include <iostream>

using namespace Marble;


MapDialog::MapDialog( QWidget *parent )
:QDialog( parent ), m_marbleWidget( 0 ), m_control( 0 ), m_mapThemeManager( 0 ),
m_splitter( 0 ), m_latSpinBox( 0 ), m_latComboBox( 0 ), m_lonSpinBox( 0 ),
m_lonComboBox( 0 ), m_longitude( 0.0 ), m_latitude( 0.0 )
{
	Trace( "MapDialog::MapDialog", false );

	setMouseTracking( true );

	QDir plugisDirectory( qApp->applicationDirPath() );
	plugisDirectory.cd("marble");

	Marble::MarbleDirs::setMarblePluginPath( plugisDirectory.absolutePath() );

	QDir directory( qApp->applicationDirPath() );
	directory.cd( "../../data" );

	Marble::MarbleDirs::setMarbleDataPath( directory.absolutePath() );




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
    connect( m_latSpinBox, SIGNAL( valueChanged ( double ) ), this, SLOT( ChangeGPSPosition() ) );
    m_latComboBox = new QComboBox;
    m_latComboBox->insertItems(0, QStringList()
     << "N"
     << "S" );
    latLayout->addWidget( m_latComboBox );
    connect( m_latComboBox, SIGNAL( currentIndexChanged ( int ) ), this, SLOT( ChangeGPSPosition() ) );

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
    connect( m_lonSpinBox, SIGNAL( valueChanged ( double ) ), this, SLOT( ChangeGPSPosition() ) );
    lonLayout->addWidget( m_lonSpinBox );

    m_lonComboBox = new QComboBox;
    m_lonComboBox->insertItems(0, QStringList()
     << "E"
     << "W" );
    lonLayout->addWidget( m_lonComboBox );
    connect( m_lonComboBox,SIGNAL( currentIndexChanged ( int ) ), this, SLOT( ChangeGPSPosition() ) );

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
    m_marbleWidget->map()->model()->downloadManager()->setDownloadEnabled( false );
    m_control->addMarbleWidget( m_marbleWidget );
	m_splitter->addWidget( m_marbleWidget );

	m_mapThemeManager = new Marble::MapThemeManager;
	m_control->setMapThemeModel(  m_mapThemeManager->mapThemeModel() );
    m_marbleWidget->setProjection( (Marble::Projection) 0 );


	m_splitter->setSizes( QList<int>() << 180 << width()-180 );
	m_splitter->setStretchFactor(m_splitter->indexOf(m_control), 0);
    m_splitter->setStretchFactor(m_splitter->indexOf(m_marbleWidget), 1);

    connect( m_marbleWidget, SIGNAL( mouseClickGeoPosition( qreal, qreal, GeoDataCoordinates::Unit ) ), this, SLOT( UpdateCurrentPosition( qreal, qreal, GeoDataCoordinates::Unit ) ) );
    connect( m_control, SIGNAL( gpsPositionChanged( qreal, qreal ) ), this, SLOT(UpdateCurrentPosition( qreal, qreal ) ) );
}


/*!
 * Returns current coordinates as a decimal number
 */
void MapDialog::GetCoordinates( double* lon, double* lat ) const
{
	*lon = m_longitude;
	*lat = m_latitude;
}

/*!
 * Set control tab visible.
 */
void MapDialog::SetCoordinates(  double lon, double lat )
{
	m_longitude = lon;
	m_latitude = -lat;

	UpdateCurrentPosition( m_longitude, m_latitude, GeoDataCoordinates::Radian );
	m_control->receiveGpsCoordinates( GeoDataCoordinates( m_longitude, m_latitude, 0, Marble::GeoDataCoordinates::Radian ), 0 );
}

/*!
 * Set control tab visible.
 */
bool MapDialog::sideBarShown() const
{
	return m_control->isVisible();
}

/*!
 * Update  current location coordinates.
 */
void MapDialog::UpdateCurrentPosition( qreal lon, qreal lat, GeoDataCoordinates::Unit )
{
	// Gps Position Coordinates are positive for south and east
	if( lon < 0 ) m_lonComboBox->setCurrentIndex( 1 );
	else m_lonComboBox->setCurrentIndex( 0 );
	m_lonSpinBox->setValue( fabs( lon * ( 180 / tgc::Pi ) ) );

	m_latitude = lat;
	if( lat < 0 ) m_latComboBox->setCurrentIndex( 1 );
	else m_latComboBox->setCurrentIndex( 0 );

	m_latSpinBox->setValue( fabs( lat * ( 180 / tgc::Pi ) ) );

	m_longitude = lon;
	m_latitude = lat;
}

/*!
 * Save as a current location the user specified coordinates in the current location tab.
 */
void MapDialog::ChangeGPSPosition()
{
	m_longitude = m_lonSpinBox->value() * tgc::Degree;
	if( m_lonComboBox->currentIndex() == 1 ) m_longitude *= -1;

	m_latitude = m_latSpinBox->value() * tgc::Degree;
	if( m_latComboBox->currentIndex() == 1 ) m_latitude *= -1;
}
