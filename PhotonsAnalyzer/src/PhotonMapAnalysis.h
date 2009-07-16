/*
 * PhotonMapAnalysis.h
 *
 *  Created on: 15-jul-2009
 *      Author: amutuberria
 */

#ifndef PHOTONMAPANALYSIS_H_
#define PHOTONMAPANALYSIS_H_

#include <QString>

class PhotonMapAnalysis
{

public:
	PhotonMapAnalysis();
	virtual ~PhotonMapAnalysis();

	virtual QString ModelName() const = 0;
	virtual void RunSolTrace() const = 0;
	virtual void RunTonatiuh() const = 0;
	void SetAnalysisData(  QString dataDirectory, QString saveDirectory, int matrixWidth, int matrixHeight );

protected:
	QString* m_dataDirectory;
	QString* m_saveDirectory;
	int m_matrixWidth;
	int m_matrixHeight;
};


#endif /* PHOTONMAPANALYSIS_H_ */
