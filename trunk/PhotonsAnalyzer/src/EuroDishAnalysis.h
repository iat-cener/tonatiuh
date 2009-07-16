/*
 * EurotroughAnalysis.h
 *
 *  Created on: 15-jul-2009
 *      Author: amutuberria
 */

#ifndef EURODISHALYSIS_H_
#define EURODISHALYSIS_H_

#include "PhotonMapAnalysis.h"

class EuroDishAnalysis : public PhotonMapAnalysis
{

public:
	EuroDishAnalysis();
	~EuroDishAnalysis();

	QString ModelName() const;
	void RunSolTrace() const;
	void RunTonatiuh() const;

private:
	double m_rMax;
};

#endif /* EURODISHALYSIS_H_ */
