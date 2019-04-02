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

#ifndef HELIOSTATFIELDWIZARD_H_
#define HELIOSTATFIELDWIZARD_H_

#include <QWizard>
#include "ui_heliostatfieldwizard.h"

class Point3D;
class QWizardPage;

class HeliostatFieldWizard : public QWizard, private Ui::HeliostatFieldWizard
{
	Q_OBJECT

public:
	HeliostatFieldWizard( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
	~HeliostatFieldWizard();

	int GetHelCoordinates() const;
	QString GetCoordinatesFile() const;
	std::vector< Point3D > GetHeliostatCoordsTable() const;

	QString GetComponentFile() const;
	int GetHeliostat() const;
	QString GetHeliostatShape() const;
	double GetHeliostatWidth() const;
	double GetHeliostatHeight() const;
	double GetHeliostatRadius() const;
	double GetHeliostatReflectivity() const;
	double GetHeliostatSigma() const;

	QString GetStrategyFile() const;
	int GetAimingStrategy() const;
	Point3D GetAimingStrategyPoint() const;
	double GetReceiverElevation() const;
	double GetReceiverRadius() const;
	std::vector< Point3D > GetAimingPointsTable() const;

private:
	std::vector< Point3D > m_helCoordsTable;
	std::vector< Point3D > m_aimingPointsTable;

private slots:
	void ActivateHeliostatCoordsTable();
	void on_openCoordFileButton_clicked();
	void on_openHelComponentButton_clicked();
	void on_openStrategyButton_clicked();
	void ChangedFileNameHelState( bool checked );
	void ChangedTableHelState( bool checked );
	void ChangedComponentHelState( bool checked );
	void ChangedNewDesignState( bool checked );
	void ChangedShapeHel( const QString text );
	void ChangedRadiusHelState( bool checked );
	void ChangedFileNameStrategyState( bool checked );
	void ChangedOneDimensionsinalStrategyState( bool checked );
	void ChangedPointStrategyState( bool checked );
	void ChangedTableStrategyState( bool checked );
	void ActivateStrategyTable();
	void accept();
};

#endif /* HELIOSTATFIELDWIZARD_H_ */
