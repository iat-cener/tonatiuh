/*
 * LS3Analyzer.h
 *
 *  Created on: 16-jul-2009
 *      Author: amutuberria
 */

#ifndef LS3ANALYZER_H_
#define LS3ANALYZER_H_

#include "PhotonMapAnalysis.h"
#include "Point3D.h"

class LS3Analysis : public PhotonMapAnalysis
{

public:
	LS3Analysis();
	~LS3Analysis();

	QString ModelName() const;
	void RunSolTrace() const;
	void RunTonatiuh() const;

private:
	double m_receiverRadius;
	double m_receiverLength;
};

#endif /* LS3ANALYZER_H_ */
