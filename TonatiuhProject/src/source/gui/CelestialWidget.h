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

#ifndef CELESTIALWIDGET_H
#define CELESTIALWIDGET_H

#include <QWidget>

#include "sunpos.h"

class QLabel;
class SoSeparator;


class CelestialWidget : public QWidget
{
	Q_OBJECT
public:
	CelestialWidget( QWidget* parent = 0 );
	~CelestialWidget();

	double getSphereRadio() {return sphereRadio;}
	double getDeclination() {return m_declination;}
	double getRightAscension() {return m_rightAscension;}

public slots:
	void CoordinatesChanged( cSunCoordinates coordinates );

private:
	QLabel* m_rightValue;
	QLabel* m_declinationValue;

	SoSeparator* CelestialEquator() const;
	SoSeparator* Declination();
	SoSeparator* Ecliptic() const;
	SoSeparator* Ejes() const;
	SoSeparator* Points() const;
	SoSeparator* RightAscension();
	SoSeparator* Sphere() const;
	SoSeparator* Star();

	SoSeparator* m_rootNode;

	const double sphereRadio;

	double m_declination;
	double m_rightAscension;
};

#endif /*CELESTIALWIDGET_H*/
