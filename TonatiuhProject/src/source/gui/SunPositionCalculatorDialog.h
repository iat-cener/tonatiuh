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
 ***************************************************************************/

#ifndef SUNPOSITIONCALCULATORDIALOG_H_
#define SUNPOSITIONCALCULATORDIALOG_H_
#ifndef NO_MARBLE


#include <QDialog>

#include "ui_sunpositioncalculatordialog.h"
#include "sunpos.h"

class QDateTime;
class QTime;

class SunPositionCalculatorDialog : public QDialog, private Ui::SunPositionCalculatorDialog
{
	Q_OBJECT

public:
	SunPositionCalculatorDialog( QWidget* parent = 0 );
	~SunPositionCalculatorDialog();

protected:
    void closeEvent( QCloseEvent* event );

private slots:
	void UpdatePosition( QAbstractButton* button );
	void ChangeDate();
	void ChangeLongitude( double longitude );
	void ChangeLatitude( double latitude );
	void ChangeSunTime( QTime time );
	void ChangeSunTimeZone( int timeZone );

signals:
	void changeRepresentation( cSunCoordinates results );
	void changeSunLight( double azimuth, double zenith );

private:
	void CalculateSunPosition();
	QDateTime GetTime();
	void ReadSettings();
	void WriteSettings();

};
#endif /* NO_MARBLE */

#endif /* SUNPOSITIONCALCULATORDIALOG_H_ */
