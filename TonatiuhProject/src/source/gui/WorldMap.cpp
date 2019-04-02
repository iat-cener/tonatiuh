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

#include <iostream>

#include <QColor>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QString>
#include <QToolTip>
#include <QVBoxLayout>

#include "gc.h"

#include "Image.h"
#include "WorldMap.h"

#include <iostream>

WorldMap::WorldMap(QWidget* widget)
	:QWidget(widget), m_rightAscension( 0 ), m_declination( 0 ), m_azimuth( 0 ), m_zenith( 0 )
{
	m_gray = new Image(":/icons/WorldMapGray.BMP");
	m_map.load( ":/icons/mundo.BMP" );
	m_sun.load( ":/icons/sunIcon.png" );
	m_position.load( ":/icons/position.png" );
	m_sun = m_sun.scaledToWidth( 15 );
	m_position = m_position.scaledToWidth( 15 );

}

WorldMap::~WorldMap()
{

}

void WorldMap::mouseMoveEvent( QMouseEvent* e)
{
	int x=e->x();
	int y=e->y();

	if( x < m_map.width() && y < m_map.height() )
	{
		//Change tool tip
		QString country = m_gray->country( x, y );
		if ( !( country == "EMPTY" ) )	QToolTip::showText( e->globalPos(), country );
		else	QToolTip::showText( e->globalPos(), "" );
	}

}

void WorldMap::LocalLatitudeChanged( double latitude )
{
	m_latitude = latitude;
}

void WorldMap::LocalLongitudeChanged( double longitude )
{
	m_longitude = longitude;
}

void WorldMap::SunChanged( cSunCoordinates coordinates )
{
	int width = m_map.width();
	int height = m_map.height();

	m_rightAscension = coordinates.dRightAscension;
	m_declination = coordinates.dDeclination;

	double hourAngle = coordinates.dHourAngle;
	int division = (int ) ( ( hourAngle - m_longitude * gc::Degree )/ gc::TwoPi );
	double remainPart = (hourAngle - m_longitude * gc::Degree ) - ( division * gc::TwoPi );
	m_hourAngle = remainPart;

	m_azimuth = coordinates.dAzimuth;
	m_zenith = coordinates.dZenithAngle;

	double sx = cos( m_declination ) * cos( m_hourAngle );
	double sy = -cos( m_declination) * sin( m_hourAngle );
	double sz = sin( m_declination );

	if ( sz == 0 ) sz = 0.0000000000001;

	m_points.clear();

	for( int x = 0; x < width; ++x )
	{
		double lon = ( ( x * gc::TwoPi ) / width ) - gc::Pi;
		double lat = atan( - ( sx * cos( lon ) + sy * sin( lon )  ) / sz );
		int y = int ( ( height/ 2 ) - ( ( lat * ( height/ 2 ) ) / ( gc::Pi/ 2) ) );
		m_points.append( QPoint( x, y ) );

	}
	update();
}

void WorldMap::paintEvent(QPaintEvent* /*event*/ )
{
 	QPainter painter( this );
 	painter.setPen( QPen( Qt::darkGreen, 2, Qt::SolidLine, Qt::RoundCap ) );
 	//painter.setBrush( QBrush( Qt::darkGreen, Qt::SolidPattern ) );

 	if( m_points.size() > 0 )
 	{


	 	QImage alpha( m_map.width(), m_map.height(), QImage::Format_RGB32 );

	 	QImage copy( m_map.width(), m_map.height(), QImage::Format_RGB32 );
	 	for( int x = 0; x< m_map.width(); ++x )
	 	{
	 		for( int y1 = 0; y1 < m_points[x].y(); ++y1 )
	 		{
	 			QColor color = QColor::fromRgb ( m_map.pixel( x, y1 ) );
	 			QColor pixelColor;
	 			if( m_declination > 0 )	pixelColor = color.lighter ( 110 );
	 			else pixelColor = color.lighter ( 70 );
	 			copy.setPixel( x, y1, pixelColor.rgb() );
	 		}
	 		for( int y2 = m_points[x].y(); y2 < m_map.height(); y2++ )
	 		{
	 			QColor color = QColor::fromRgb ( m_map.pixel( x, y2 ) );
	 			QColor pixelColor;
	 			if( m_declination < 0 )	pixelColor = color.lighter ( 110 );
	 			else	pixelColor = color.lighter ( 70 );
	 			copy.setPixel( x, y2, pixelColor.rgb() );
	 		}

	 	}

	 	painter.drawImage(0, 0, copy );

		QVector< QPoint > points;
		points.append( m_points[0] );
		for( int index = 0; index < m_points.size()-1; ++index )
		{
			points.append( m_points[index] );
			//points.append( m_points[index] );
		}
		points.append( m_points[m_points.size()-1] );
		painter.drawLines( points );

		int x = int ( ( ( -m_hourAngle + gc::Pi) * m_map.width() ) / gc::TwoPi );
		if( x < 0 )	x = m_map.width() + x;
		int y = int ( (m_map.height()/ 2) + ( (-m_declination   * ( m_map.height()/ 2 ) )  / ( gc::Pi / 2 ) ) );
		painter.drawImage( x, y - 10, m_sun );

		x = int ( ( ( m_longitude* gc::Degree + gc::Pi) * m_map.width() ) / gc::TwoPi );
		if( x < 0 )	x = m_map.width() + x;
		y = int ( (m_map.height()/ 2) + ( (-m_latitude* gc::Degree   * ( m_map.height()/ 2 ) )  / ( gc::Pi / 2 ) ) );
		painter.drawImage( x, y-10, m_position );


		//Draw Text
		painter.setPen( QPen( Qt::black, 2, Qt::SolidLine, Qt::RoundCap ) );
		//painter.setFont( QFont( "MS Shell Dlg 2", 8 ) );
		QString azimuth = QString("Azimuth: %1").arg( QString::number( m_azimuth ) );
		painter.drawText ( 20, 280, azimuth );
		QString zenith = QString("Zenith: %1").arg( QString::number( m_zenith ) );
		painter.drawText ( 20, 310, zenith );
		QString ascension = QString("Right Ascension: %1").arg( QString::number( m_rightAscension ) );
		painter.drawText ( 20, 340, ascension );
		QString declination = QString("Declination: %1").arg( QString::number( m_declination ) );
		painter.drawText ( 20, 370, declination );
	 }
	 else	painter.drawImage(0, 0, m_map);


 }
