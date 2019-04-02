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

#ifndef HORIZONTALWIDGET_H_
#define HORIZONTALWIDGET_H_

#include <QWidget>

#include "sunpos.h"

class QLabel;
class SoSeparator;

class HorizontalWidget : public QWidget
{
	Q_OBJECT
public:
	HorizontalWidget( QWidget* parent = 0 );
	~HorizontalWidget();

public slots:
	void CoordinatesChanged( cSunCoordinates coordinates );

private:
	QLabel* m_azimuthValue;
	QLabel* m_zenithValue;

	SoSeparator* AzimuthLine();
	SoSeparator* Ejes() const;
	SoSeparator* Horizon() const;
	SoSeparator* Sphere() const;
	SoSeparator* Star();
	SoSeparator* Text();
	SoSeparator* ZenithLine();

	SoSeparator* m_rootNode;

	const double sphereRadio;

	double m_azimuth;
	double m_zenith;


};

#endif /*HORIZONTALWIDGET_H_*/
