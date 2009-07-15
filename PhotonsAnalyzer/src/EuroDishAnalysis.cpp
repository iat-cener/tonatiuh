/*
 * EuroDishAnalysis.cpp
 *
 *  Created on: 15-jul-2009
 *      Author: amutuberria
 */

#include <iostream>
#include <cmath>

#include <QDataStream>
#include <QDir>
#include <QMessageBox>
#include <QVector>

#include "EuroDishAnalysis.h"
#include "Point3D.h"
#include "Trace.h"


EuroDishAnalysis::EuroDishAnalysis( )
:PhotonMapAnalysis()
{
	Trace trace( "EuroDishAnalysis::EuroDishAnalysis", false);
}

EuroDishAnalysis::~EuroDishAnalysis()
{
	Trace trace( "EuroDishAnalysis::~EuroDishAnalysis", false);
}

QString EuroDishAnalysis::ModelName() const
{
	Trace trace( "EuroDishAnalysis::ModelName", false);
	return QString( "EuroDish" );
}

void EuroDishAnalysis::Run() const
{
	Trace trace( "EuroDishAnalysis::Run", false );

	QDir photonMapsDir( *m_dataDirectory );
	QStringList filesName =  photonMapsDir.entryList ( QDir::Files );
	for( int index = 0; index < filesName.count(); index ++ )
	{
		QVector< Point3D > photonsVector;
		QFile dataFile( photonMapsDir.filePath( filesName[index]) );
		if( !dataFile.open( QIODevice::ReadOnly ) )
		{
			std::cout<<"Can not open data file" <<std::endl;
			return;
		}
		QDataStream in( &dataFile );
		while ( !in.atEnd() )
		{
			double id, x, y, z, prev, next;
			in >> id >> x >> y >> z >> prev >> next;
			Point3D photon( x, y, z );
			photonsVector<< photon;
		}

		//Power compute

		//Radius StandarDeviation Compute
		double sum = 0;
		double STD_DEV = 0; // returning zero's

		for(int photon = 0; photon < photonsVector.size(); photon++ )
		{
			double radius = Distance( Point3D( 0.0, 0.0, 0.0 ), photonsVector[photon] );
			sum = sum +radius;
			STD_DEV = STD_DEV + pow(radius, 2);
		}
		double radiusStandardDeviation = sqrt (( STD_DEV/photonsVector.size() ) - ( pow( sum/photonsVector.size(),2 ) ) );



		std::cout<<filesName[index].toStdString()<< "\t"<<photonsVector.size()<<"\t"<<radiusStandardDeviation<<std::endl;
	}

}
