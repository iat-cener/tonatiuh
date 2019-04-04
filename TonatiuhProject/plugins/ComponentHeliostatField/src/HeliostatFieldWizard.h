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
