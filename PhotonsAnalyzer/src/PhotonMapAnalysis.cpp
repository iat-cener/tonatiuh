/*
 * PhotonMapAnalysis.cpp
 *
 *  Created on: 15-jul-2009
 *      Author: amutuberria
 */

#include "PhotonMapAnalysis.h"
#include "Trace.h"


PhotonMapAnalysis::PhotonMapAnalysis()
: m_dataDirectory( 0 ), m_saveDirectory( 0 ), m_matrixWidth( -1 ), m_matrixHeigth ( -1 )
{
	Trace trace( "PhotonMapAnalysis::PhotonMapAnalysis", false);
}


PhotonMapAnalysis::~PhotonMapAnalysis()
{
	Trace trace( "PhotonMapAnalysis::~PhotonMapAnalysis", false);
}

void PhotonMapAnalysis::SetAnalysisData(  QString dataDirectory, QString saveDirectory, int matrixWidth, int matrixHeight )
{
	Trace trace( "PhotonMapAnalysis::PhotonMapAnalysis", false);

	m_dataDirectory = &dataDirectory;
	m_saveDirectory = &saveDirectory;
	m_matrixWidth =  matrixWidth;
	m_matrixHeigth = matrixHeight;
}
