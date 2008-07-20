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

#include <QColor>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QString>
#include <QToolTip>
#include <QVBoxLayout>

#include "Image.h"
#include "tgc.h"
#include "WorldMap.h"

#include <iostream>

WorldMap::WorldMap(QWidget* widget)
	:QWidget(widget), m_rightAscension( 0 ), m_declination( 0 ), m_azimuth( 0 ), m_zenith( 0 )
	//m_hourAngle( 0 ), m_rightAscension( 0 ), m_declination( 0 ), m_azimuth( 0 ), m_zenith( 0 )
{
	m_gray = new Image(":/icons/WorldMapGray.BMP");
	m_map.load( ":/icons/mundo.BMP" );
	m_sun.load( ":/icons/sunIcon.png" );
	m_sun = m_sun.scaledToWidth( 15 );
	
}

WorldMap::~WorldMap()
{
	
}

void WorldMap::mouseMoveEvent( QMouseEvent* e)
{
	int x=e->x();
	int y=e->y();

	//Change tool tip	
	QString country = m_gray->country( x, y );
	if ( !( country == "EMPTY" ) )	QToolTip::showText( e->globalPos(), country );
	else	QToolTip::showText( e->globalPos(), "" );
	
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
	int division = (int ) ( ( hourAngle - m_longitude * tgc::Degree )/ tgc::TwoPi );
	double remainPart = (hourAngle - m_longitude * tgc::Degree ) - ( division * tgc::TwoPi );
	m_hourAngle = remainPart;
	
	m_azimuth = coordinates.dAzimuth;
	m_zenith = coordinates.dZenithAngle;
	
	double sx = cos( m_declination ) * cos( m_hourAngle );
	double sy = -cos( m_declination) * sin( m_hourAngle );
	double sz = sin( m_declination );

	if ( sz == 0 ) sz = 0.0000000000001;
	
	m_points.clear();
	
	for( int x = 0; x < width; x++ )
	{
		double lon = ( ( x * tgc::TwoPi ) / width ) - tgc::Pi;
		double lat = atan( - ( sx * cos( lon ) + sy * sin( lon )  ) / sz );
		//double y = ( lat + tgc::Pi/ 2) * height / tgc::Pi;
		double y = ( height/ 2 ) - ( ( lat * ( height/ 2 ) ) / ( tgc::Pi/ 2) ); 
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
	 	for( int x = 0; x< m_map.width(); x++ )
	 	{
	 		for( int y1 = 0; y1 < m_points[x].y(); y1++ )
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
		for( int index = 0; index < m_points.size()-1; index++ )
		{
			points.append( m_points[index] );
			points.append( m_points[index] );
		}
		points.append( m_points[m_points.size()-1] );
		painter.drawLines( points );
		
		double x = 	( ( -m_hourAngle + tgc::Pi) * m_map.width() ) / tgc::TwoPi;
		if( x < 0 )	x = m_map.width() + x;
		double y = 	(m_map.height()/ 2) + ( (-m_declination   * ( m_map.height()/ 2 ) )  / ( tgc::Pi / 2 ) );
		painter.drawImage( x, y, m_sun );
		
		//Draw Text
		painter.setPen( QPen( Qt::black, 2, Qt::SolidLine, Qt::RoundCap ) );
		painter.setFont( QFont( "MS Shell Dlg 2", 8 ) );
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