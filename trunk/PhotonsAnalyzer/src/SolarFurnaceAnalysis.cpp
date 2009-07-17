/*
 * SolarFurnaceAnalysis.cpp
 *
 *  Created on: 16-jul-2009
 *      Author: amutuberria
 */

#include <iostream>
#include <cmath>

#include <QDataStream>
#include <QDir>
#include <QMap>
#include <QMessageBox>
#include <QPair>
#include <QTextStream>
#include <QVector>

#include "SolarFurnaceAnalysis.h"
#include "Point3D.h"
#include "Trace.h"


SolarFurnaceAnalysis::SolarFurnaceAnalysis( )
:PhotonMapAnalysis(), m_rMax( 0.15 )
{
	Trace trace( "SolarFurnaceAnalysis::SolarFurnaceAnalysis", false);
}

SolarFurnaceAnalysis::~SolarFurnaceAnalysis()
{
	Trace trace( "SolarFurnaceAnalysis::~SolarFurnaceAnalysis", false);
}

QString SolarFurnaceAnalysis::ModelName() const
{
	Trace trace( "SolarFurnaceAnalysis::ModelName", false);
	return QString( "SolarFurnace" );
}


void SolarFurnaceAnalysis::RunSolTrace()
{
	Trace trace( "SolarFurnaceAnalysis::RunSolTrace", false);

	QDir photonMapsDir( *m_dataDirectory );
	QStringList filesName =  photonMapsDir.entryList ( QDir::Files );

	QDir resultsDir( *m_saveDirectory );

	QFile saveResultsFile( resultsDir.filePath( "photonMapsAnalysis.txt") );
	if( !saveResultsFile.open( QIODevice::WriteOnly ) )
	{
		std::cout<<"Can not open flux save file" <<std::endl;
		return;
	}
	QTextStream resultsOut( &saveResultsFile );

	for( int index = 0; index < filesName.count(); index ++ )
	{
		QFile dataFile( photonMapsDir.filePath( filesName[index]) );
		if( !dataFile.open( QIODevice::ReadOnly ) )
		{
			std::cout<<"Can not open data file" <<std::endl;
			return;
		}
		QTextStream in( &dataFile );
		for( int line = 0; line < 12; line++ ) in.readLine();
		double wPhoton = in.readLine().toDouble();

		for( int line = 0; line < 9; line++ ) in.readLine();

		double widthCells = ( 2 * m_rMax ) / m_matrixWidth;
		double  heightCells = ( 2 * m_rMax ) / m_matrixHeight;
		double cellArea = widthCells * heightCells;

		double xSum = 0;
		double ySum = 0;
		QMap< QPair< int, int >, Point3D> photonsMatrix;
		while ( !in.atEnd() )
		{
			QString line = in.readLine();
			QStringList photonInfo = line.split( QRegExp( "\\s+" ), QString::SkipEmptyParts );

			if( photonInfo.size() == 8 )
			{
				Point3D photon( photonInfo[0].toDouble(),photonInfo[1].toDouble(),photonInfo[2].toDouble() );

				QPair< int, int > photonCellId;
				photonCellId.first = floor( photon.x /widthCells ) + m_matrixWidth / 2;
				photonCellId.second = floor( photon.y /heightCells ) + m_matrixHeight / 2;
				photonsMatrix.insertMulti( photonCellId, photon );

				xSum += photon.x;
				ySum += photon.y;

				//std::cout<<photonCellId.first<<" - "<<photonCellId.second<<" - "<<photon<<std::endl;
			}
		}

		dataFile.close();

		QList< Point3D > photonsVector = photonsMatrix.values();

		//Power compute
		double targetPower = photonsVector.size() * wPhoton / 1000;


		//Radius StandarDeviation Compute
		QVector< double > radiusList;
		for(int photon = 0; photon < photonsVector.size(); photon++ )
		{
			double radius = Distance( Point3D( 0.0, 0.0, 0.0 ), photonsVector[photon] );
			radiusList << radius;
		}

		double sum = 0;
		for(int photon = 0; photon < photonsVector.size(); photon++ )	sum = sum + radiusList[photon];
		double mean = sum / photonsVector.size();

		double std_dev = 0; // returning zero's

		for(int photon = 0; photon < photonsVector.size(); photon++ )
		{
			std_dev = std_dev + pow( radiusList[photon] - mean, 2 );;
		}
		double radiusStandardDeviation = sqrt( std_dev /( photonsVector.size() - 1 ) );

		//Centro de gravedad
		double xcentro =  xSum / photonsVector.size();
		double ycentro =  ySum / photonsVector.size();


		QDir resultsDir( *m_saveDirectory );
		QFileInfo photonMapFile( filesName[index] );
		QFile saveFluxFile( resultsDir.filePath( photonMapFile.baseName().append(".flx")) );
		if( !saveFluxFile.open( QIODevice::WriteOnly ) )
		{
			std::cout<<"Can not open flux save file" <<std::endl;
			return;
		}

		QTextStream fluxOut( &saveFluxFile );
		//Matrix de flujo
		int maxPhotonsInCell = 0;

		for( int width = 0; width < m_matrixWidth; width++ )
		{
			for( int height = 0; height < m_matrixHeight; height++ )
			{
				QPair< int, int > matrixCellID;
				matrixCellID.first = width;
				matrixCellID.second = height;

				int numberPhotons = photonsMatrix.count( matrixCellID );

				//std::cout<<matrixCellID.first<<" - "<<matrixCellID.second<<" - "<<numberPhotons<<std::endl;
				if( maxPhotonsInCell < numberPhotons ) maxPhotonsInCell = numberPhotons;

				fluxOut<< ( ( numberPhotons * wPhoton ) / cellArea )/1000<<"\t";
			}
			fluxOut<<"\n";
		}

		saveFluxFile.close();
		resultsOut<<filesName[index]<< "\t"<<photonsVector.size()<<"\t"<<targetPower<<"\t"<<radiusStandardDeviation<<"\t"<<xcentro<<"\t"<<ycentro<<"\t"<<( ( maxPhotonsInCell * wPhoton ) / cellArea )/1000<<"\n";

		//Update Analysis Progress
		int progress = ( index * 100 ) / filesName.count();
		emit AnalysisProgressChanged( progress );
	}
	saveResultsFile.close();
}

void SolarFurnaceAnalysis::RunTonatiuh()
{
	Trace trace( "SolarFurnaceAnalysis::RunTonatiuh", false );

	QDir photonMapsDir( *m_dataDirectory );
	QStringList filesName =  photonMapsDir.entryList ( QDir::Files );

	QDir resultsDir( *m_saveDirectory );

	QFile saveResultsFile( resultsDir.filePath( "photonMapsAnalysis.txt") );
	if( !saveResultsFile.open( QIODevice::WriteOnly ) )
	{
		std::cout<<"Can not open flux save file" <<std::endl;
		return;
	}
	QTextStream resultsOut( &saveResultsFile );

	for( int index = 0; index < filesName.count(); index ++ )
	{
		QFile dataFile( photonMapsDir.filePath( filesName[index]) );
		if( !dataFile.open( QIODevice::ReadOnly ) )
		{
			std::cout<<"Can not open data file" <<std::endl;
			return;
		}
		QDataStream in( &dataFile );
		double wPhoton;
		in >>wPhoton;

		double widthCells = ( 2 * m_rMax ) / m_matrixWidth;
		double  heightCells = ( 2 * m_rMax ) / m_matrixHeight;
		double cellArea = widthCells * heightCells;

		double xSum = 0;
		double zSum = 0;

		QMap< QPair< int, int >, Point3D> photonsMatrix;
		while ( !in.atEnd() )
		{

			double id, x, y, z, prev, next;
			in >> id >> x >> y >> z >> prev >> next;
			Point3D photon( x, y, z );

			QPair< int, int > photonCellId;
			photonCellId.first = floor( photon.x /widthCells ) + m_matrixWidth / 2;
			photonCellId.second = floor( photon.z /heightCells ) + m_matrixHeight / 2;
			photonsMatrix.insertMulti( photonCellId, photon );

			xSum += photon.x;
			zSum += photon.z;
		}

		dataFile.close();

		QList< Point3D > photonsVector = photonsMatrix.values();


		//Radius StandarDeviation Compute
		QVector< double > radiusList;
		for(int photon = 0; photon < photonsVector.size(); photon++ )
		{
			double radius = Distance( Point3D( 0.0, 0.0, 0.0 ), photonsVector[photon] );
			radiusList << radius;
		}

		double sum = 0;
		for(int photon = 0; photon < photonsVector.size(); photon++ )	sum = sum + radiusList[photon];
		double mean = sum / photonsVector.size();

		double std_dev = 0; // returning zero's

		for(int photon = 0; photon < photonsVector.size(); photon++ )
		{
			std_dev = std_dev + pow( radiusList[photon] - mean, 2 );;
		}
		double radiusStandardDeviation = sqrt( std_dev /( photonsVector.size() - 1 ) );


		//Centro de gravedad
		double xcentro =  xSum / photonsVector.size();
		double zcentro =  zSum / photonsVector.size();

		//Matrix de flujo
		QDir resultsDir( *m_saveDirectory );
		QFileInfo photonMapFile( filesName[index] );
		QFile saveFluxFile( resultsDir.filePath( photonMapFile.baseName().append(".flx")) );
		if( !saveFluxFile.open( QIODevice::WriteOnly ) )
		{
			std::cout<<"Can not open flux save file" <<std::endl;
			return;
		}

		QTextStream fluxOut( &saveFluxFile );

		int maxPhotonsInCell = 0;

		int totalPhotons = 0;
		for( int width = 0; width < m_matrixWidth; width++ )
		{
			for( int height = 0; height < m_matrixHeight; height++ )
			{
				QPair< int, int > matrixCellID;
				matrixCellID.first = width;
				matrixCellID.second = height;

				int numberPhotons = photonsMatrix.count( matrixCellID );
				totalPhotons += numberPhotons;
				if( maxPhotonsInCell < numberPhotons ) maxPhotonsInCell = numberPhotons;

				fluxOut<< ( ( numberPhotons * wPhoton ) / cellArea )/1000<<"\t";

			}
			fluxOut<<"\n";
		}

		saveFluxFile.close();

		//Power compute
		double targetPower = totalPhotons * wPhoton /1000;

		resultsOut<<filesName[index]<< "\t"<<photonsVector.size()<<"\t"<<targetPower<<"\t"<<radiusStandardDeviation<<"\t"<<xcentro<<"\t"<<zcentro<<"\t"<<( ( maxPhotonsInCell * wPhoton ) / cellArea )/1000<<"\n";

		//Update Analysis Progress
		int progress = ( index * 100 ) / filesName.count();
		emit AnalysisProgressChanged( progress );
	}
	saveResultsFile.close();
}
