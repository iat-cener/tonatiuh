/*
 * SolarFurnaceAnalysis.h
 *
 *  Created on: 16-jul-2009
 *      Author: amutuberria
 */

#ifndef SOLARFURNACEANALYSIS_H_
#define SOLARFURNACEANALYSIS_H_

#include "PhotonMapAnalysis.h"

class SolarFurnaceAnalysis : public PhotonMapAnalysis
{

public:
	SolarFurnaceAnalysis();
	~SolarFurnaceAnalysis();

	QString ModelName() const;
	void RunSolTrace();
	void RunTonatiuh();

private:
	double m_rMax;

};

#endif /* SOLARFURNACEANALYSIS_H_ */
