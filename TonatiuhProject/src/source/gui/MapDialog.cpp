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


#include <AbstractFloatItem.h>
#include <GeoSceneDocument.h>
#include <GeoSceneHead.h>
#include <HttpDownloadManager.h>
#include <LegendWidget.h>
#include <MapViewWidget.h>
#include <MarbleDirs.h>
#include <MarbleMap.h>
#include <MarbleModel.h>
#include <NavigationWidget.h>

#include "MapDialog.h"
#include "tgc.h"

#include <iostream>

#include <QPainter>
#include <QTextDocument>

using namespace Marble;


MapDialog::MapDialog( QWidget *parent )
:QDialog( parent )/*, m_marbleWidget( 0 ), m_control( 0 ), m_mapThemeManager( 0 ),
m_splitter( 0 ), m_latSpinBox( 0 ), m_latComboBox( 0 ), m_lonSpinBox( 0 ),
m_lonComboBox( 0 ), m_longitude( 0.0 ), m_latitude( 0.0 )*/
{

	QDir directory( qApp->applicationDirPath() );
	directory.cd( ".." );
	directory.cd( ".." );
	directory.cd( "data" );


	Marble::MarbleDirs::setMarbleDataPath( directory.absolutePath() );

	setupUi(this);
	setMouseTracking( true );
	QList<int> sizes;
	sizes<<100<<400;
	mainSplitter->setSizes ( sizes );

	connect( latitudeSpin, SIGNAL( valueChanged( double ) ), this, SLOT( SetHomeLatitude() ) );
	connect( latitudeCombo, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( SetHomeLatitude() ) );
	connect( longitudeSpin, SIGNAL( valueChanged( double ) ), this, SLOT( SetHomeLongitude() ) );
	connect( longitudeCombo, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( SetHomeLongitude() ) );


	m_mapThemeManager = new Marble::MapThemeManager;
	marbleWidget->setProjection( (Marble::Projection) 0 );

	marbleWidget->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ) );
	marbleWidget->map()->model()->downloadManager()->setDownloadEnabled( false );
	marbleWidget->setMapThemeId( DefaultMapThemeId() );

	MarbleModel* marbleModel = marbleWidget->model();

	//connect( marbleModel, SIGNAL( modelChanged() ), this, SLOT( ModelChanged() ) );

	navigationWidget->setMarbleWidget( marbleWidget );


	MapViewWidget* mapViewWidget = new MapViewWidget( this );
	controlTab->addTab( mapViewWidget, mapViewWidget->windowTitle() );
	mapViewWidget->setMarbleWidget( marbleWidget );

	LegendWidget* legendWidget = new LegendWidget( this );
	controlTab->addTab( legendWidget, legendWidget->windowTitle() );
	legendWidget->setMarbleWidget( marbleWidget );

	marbleWidget->setShowOverviewMap( true );
	marbleWidget->setShowScaleBar( true );

}

MapDialog::~MapDialog( )
{
	delete m_mapThemeManager;
}

void MapDialog::mouseReleaseEvent(QMouseEvent *event)
{
	double lon;
	double lat;
	int zoom;

	marbleWidget->home( lon, lat, zoom );
	SetHomePosition( -lon, lat );
}
/*
 * Sets home coordinates latitude value to the values of the GUI.
 */
void MapDialog::SetHomeLatitude()
{
	double latitude = latitudeSpin->value();
	if( latitudeCombo->currentText() == "S" )	latitude = -latitude;

	UpdateHomePosition( m_longitude, latitude );
}

/*
* Sets home coordinates longitude value to the values of the GUI.
 */
void MapDialog::SetHomeLongitude()
{
	double longitude = longitudeSpin->value();
	if( longitudeCombo->currentText() == "W" )	longitude = -longitude;

	UpdateHomePosition( longitude, m_latitude );
}

/*!
 * Update home location to \a lon and \a lat coordinates.
 * The \a lon and lat variables are in degree. lon is positive to east and lat is positive for north.
 */
void MapDialog::SetHomePosition( double lon, double lat )
{
	if( lon < 0 ) longitudeCombo->setCurrentIndex( 1 );
	else longitudeCombo->setCurrentIndex( 0 );
	longitudeSpin->setValue( fabs( lon ) );

	if( lat < 0 ) latitudeCombo->setCurrentIndex( 1 );
	else latitudeCombo->setCurrentIndex( 0 );
	latitudeSpin->setValue( fabs( lat ) );
}

/*!
 * Update home location to \a lon and \a lat coordinates.
 * The \a lon and lat variables are in degree. lon is positive to east and lat is positive for north.
 */
void MapDialog::UpdateHomePosition( double lon, double lat)
{
	m_longitude = lon;
	m_latitude = lat;

	marbleWidget->setHome( m_longitude , m_latitude, marbleWidget->zoom() );
	marbleWidget->goHome();
}

/*
 * Defines the default maps for marble.
 */
QString MapDialog::DefaultMapThemeId() const
{
    QStringList fallBackThemes;
    fallBackThemes << "earth/srtm/srtm.dgml";
    fallBackThemes << "earth/bluemarble/bluemarble.dgml";
    fallBackThemes << "earth/openstreetmap/openstreetmap.dgml";

    QStringList installedThemes;
    QList<GeoSceneDocument const*> themes = m_mapThemeManager->mapThemes();
    for( int i = 0; i < themes.count(); ++ i)
        installedThemes << themes[i]->head()->mapThemeId();

    for( int i = 0; i < fallBackThemes.count(); ++ i )
    	if( installedThemes.contains( fallBackThemes[i] ) )	return fallBackThemes[i];

    if( installedThemes.size() > 0 )	return installedThemes.first();

    return QString();
}
