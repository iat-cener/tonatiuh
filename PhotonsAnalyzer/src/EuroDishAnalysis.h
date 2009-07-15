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
	void Run() const;

private:
	double m_sunInputArea;
	double m_irradiance;

};

#endif /* EURODISHALYSIS_H_ */
