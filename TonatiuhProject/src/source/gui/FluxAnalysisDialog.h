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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/

#ifndef FLUXANALYSISDIALOG_H_
#define FLUXANALYSISDIALOG_H_

#include <QDialog>

#include "TPhotonMap.h"

#include "ui_fluxanalysisdialog.h"

class InstanceNode;
class SceneModel;
class QIntValidator;
class RandomDeviate;
class TSceneKit;
class FluxAnalysis;


class FluxAnalysisDialog: public QDialog, private Ui::FluxAnalysisDialog
{
	Q_OBJECT

public:
	FluxAnalysisDialog( TSceneKit* currentScene, SceneModel& currentSceneModel, InstanceNode* rootSeparatorInstance,
			int sunWidthDivisions, int sunHeightDivisions,
			RandomDeviate* randomDeviate, QWidget* parent = 0 );
	~FluxAnalysisDialog();

protected:
	void resizeEvent( QResizeEvent* event );

private slots:
	void ChangeCurrentSurface();
	void ChangeCurrentSurfaceSide();
	void ExportData();
	void Run();
	void UpdateAnalysis();
	void UpdateSectorPlotSlot();
	void SelectExportFile();
	void SelectSurface();
	void UpdateLabelsUnits();
	void SaveCoordsExport();

private:
	void UpdateStatistics( double totalEnergy, double minimumFlux, double averageFlux, double maximumFlux,
			double maxXCoord, double maxYCoord, double error, double uniformity, double gravityX, double gravityY );
	void UpdateFluxMapPlot( int** photonCounts, double wPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax );
	void CreateSectorPlots( double xmin, double ymin, double xmax, double ymax );
	void UpdateSectorPlots( int** photonCounts, double wPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax, double maximumFlux );
	void ClearCurrentAnalysis();
	void UpdateSurfaceSides( QString selectedSurfaceURL );

	FluxAnalysis* m_fluxAnalysis;
	QString m_currentSurfaceURL;

	SceneModel* m_pCurrentSceneModel;

	QIntValidator* m_pGridWidthVal;
	QIntValidator* m_pGridHeightVal;
	QIntValidator* m_pNOfRays;

	//Labels
	QString m_fluxLabelString;
};


#endif /* FLUXANALYSISDIALOG_H_ */
