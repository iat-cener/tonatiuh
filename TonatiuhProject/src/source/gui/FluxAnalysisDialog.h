/*
 * FluxAnalysisDialog.h
 *
 *  Created on: 08/10/2015
 *      Author: amutuberria
 */

#ifndef FLUXANALYSISDIALOG_H_
#define FLUXANALYSISDIALOG_H_

#include <functional>

#include <QDialog>

#include "Point3D.h"
#include "Transform.h"


#include "TPhotonMap.h"

#include "ui_fluxanalysisdialog.h"

class InstanceNode;
class SceneModel;
class RandomDeviate;
class TSceneKit;


class FluxAnalysisDialog: public QDialog, private Ui::FluxAnalysisDialog
{
	Q_OBJECT

public:
	FluxAnalysisDialog( TSceneKit* currentScene, SceneModel& currentSceneModel, InstanceNode* rootSeparatorInstance,
			int widthDivisions, int heightDivisions,
			RandomDeviate* randomDeviate, QWidget* parent = 0 );
	~FluxAnalysisDialog();

protected:
	void resizeEvent(QResizeEvent* event);

private slots:
	void ChangeCurrentSurface();
	void ChangeCurrentSurfaceSide();
	void ExportData();
	void RunFluxAnalysis();
	void SelectExportFile();
	void SelectSurface();
	void UpdateAnalysis( );
	void UpdateSectorPlots();

private:
	void ClearCurrentAnalysis();
	void FluxAnalysis( int activeSideID,  std::function< Point3D( Point3D, std::vector<double>)  > transformFunction, std::vector<double> parametersList, Transform wTO );
	void UpdateSurfaceSides( QString selectedSurfaceURL );


private:
	QString m_currentSurfaceURL;
	TSceneKit* m_pCurrentScene;
	SceneModel* m_pCurrentSceneModel;
	TPhotonMap* m_pPhotonMap;
	RandomDeviate* m_pRandomDeviate;
	InstanceNode* m_pRootSeparatorInstance;
	int m_sunHeightDivisions;
	int m_sunWidthDivisions;
	unsigned long m_tracedRays;
	double m_wPhoton;

	//Current analysis
	double m_xmin;
	double m_xmax;
	double m_ymin;
	double m_ymax;
	int** m_photonCounts;
	int m_heightDivisions;
	int m_widthDivisions;
	double m_maximumFlux;

};


#endif /* FLUXANALYSISDIALOG_H_ */
