/*
 * FluxAnalysis.h
 *
 *  Created on: 27/02/2017
 *      Author: iles
 */

#ifndef FLUXANALYSIS_H_
#define FLUXANALYSIS_H_

class TSceneKit;
class SceneModel;
class InstanceNode;
class RandomDeviate;
class TPhotonMap;


class FluxAnalysis
{

public:
	FluxAnalysis( TSceneKit* currentScene, SceneModel& currentSceneModel, InstanceNode* rootSeparatorInstance,
			int sunWidthDivisions, int sunHeightDivisions, RandomDeviate* randomDeviate);
	~FluxAnalysis();
	QString GetSurfaceType( QString nodeURL );
	void RunFluxAnalysis( QString nodeURL, QString surfaceSide, unsigned long nOfRays, bool increasePhotonMap, int heightDivisions, int widthDivisions );
	void UpdatePhotonCounts( int heightDivisions, int widthDivisions );
	void ExportAnalysis( QString directory, QString fileName, bool saveCoords );
	int** photonCountsValue();
	double xminValue();
	double yminValue();
	double xmaxValue();
	double ymaxValue();
	int maximumPhotonsValue();
	int maximumPhotonsXCoordValue();
	int maximumPhotonsYCoordValue();
	int maximumPhotonsErrorValue();
	double wPhotonValue();
	double totalPowerValue();
	void clearPhotonMap();

private:
	bool CheckSurface();
	bool CheckSurfaceSide();
	void UpdatePhotonCounts();
	void FluxAnalysisCylinder( InstanceNode* node );
	void FluxAnalysisFlatDisk( InstanceNode* node );
	void FluxAnalysisFlatRectangle( InstanceNode* node );

	TSceneKit* m_pCurrentScene;
	SceneModel* m_pCurrentSceneModel;
	InstanceNode* m_pRootSeparatorInstance;
	int m_sunWidthDivisions;
	int m_sunHeightDivisions;
	RandomDeviate* m_pRandomDeviate;

	TPhotonMap* m_pPhotonMap;

	QString m_surfaceURL;
	QString m_surfaceSide;
	unsigned long m_tracedRays;
	double m_wPhoton;

	int** m_photonCounts;
	int m_heightDivisions;
	int m_widthDivisions;
	double m_xmin;
	double m_xmax;
	double m_ymin;
	double m_ymax;
	int m_maximumPhotons;
	int m_maximumPhotonsXCoord;
	int m_maximumPhotonsYCoord;
	int m_maximumPhotonsError;
	double m_totalPower;

protected:


};

#endif /* FLUXANALYSIS_H_ */
