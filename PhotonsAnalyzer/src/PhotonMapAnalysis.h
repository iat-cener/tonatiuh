/*
 * PhotonMapAnalysis.h
 *
 *  Created on: 15-jul-2009
 *      Author: amutuberria
 */

#ifndef PHOTONMAPANALYSIS_H_
#define PHOTONMAPANALYSIS_H_

#include <QObject>
#include <QString>

class PhotonMapAnalysis : public QObject
{
    Q_OBJECT

public:
	PhotonMapAnalysis();
	virtual ~PhotonMapAnalysis();

	virtual QString ModelName() const = 0;
	virtual void RunSolTrace() = 0;
	virtual void RunTonatiuh() = 0;
	void SetAnalysisData(  QString dataDirectory, QString saveDirectory, int matrixWidth, int matrixHeight );

signals:
	void AnalysisProgressChanged( int progress );

protected:
	QString* m_dataDirectory;
	QString* m_saveDirectory;
	int m_matrixWidth;
	int m_matrixHeight;
};


#endif /* PHOTONMAPANALYSIS_H_ */
