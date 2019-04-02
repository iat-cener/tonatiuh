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
