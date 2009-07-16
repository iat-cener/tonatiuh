/*
 * LS3Analyis.cpp
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

#include "LS3Analysis.h"
#include "Point3D.h"
#include "tgc.h"
#include "Trace.h"


LS3Analysis::LS3Analysis( )
:PhotonMapAnalysis(), m_receiverRadius( 0.035 ), m_receiverLength( 10.0 )
{
	Trace trace( "LS3Analysis::LS3Analysis", false);
}

LS3Analysis::~LS3Analysis()
{
	Trace trace( "LS3Analysis::~LS3Analysis", false);
}

QString LS3Analysis::ModelName() const
{
	Trace trace( "LS3Analysis::ModelName", false);
	return QString( "LS-3" );
}


void LS3Analysis::RunSolTrace() const
{
	Trace trace( "LS3Analysis::ModelName", false);

	QDir photonMapsDir( *m_dataDirectory );
	QStringList filesName =  photonMapsDir.entryList ( QDir::Files );

	QDir resultsDir( *m_saveDirectory );

	int fileNumber = 0;
	QString fileName = QString( "photonMapsAnalysis_%1.txt" ).arg( QString::number( fileNumber ) );
	QFile saveResultsFile( resultsDir.filePath( fileName ) );

	while( saveResultsFile.exists() )
	{
		fileNumber++;
		fileName = QString( "photonMapsAnalysis_%1.txt" ).arg( QString::number( fileNumber ) );
		saveResultsFile.setFileName( resultsDir.filePath( fileName ) );
	}

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

		double widthCells = ( 2 * tgc::Pi ) / m_matrixWidth;
		double heightCells = m_receiverLength / m_matrixHeight;
		double cellArea = ( ( 2 * tgc::Pi * m_receiverRadius ) / m_matrixWidth ) * ( m_receiverLength / m_matrixHeight );

		QMap< QPair< int, int >, QPair< double, double> > photonsMatrix;
		while ( !in.atEnd() )
		{
			QString line = in.readLine();
			QStringList photonInfo = line.split( QRegExp( "\\s+" ), QString::SkipEmptyParts );

			if( photonInfo.size() == 8 )
			{
				Point3D photonPoint( photonInfo[0].toDouble(), photonInfo[1].toDouble(), photonInfo[2].toDouble() );

				QPair< double, double > photonValue;
				photonValue.first = atan2( photonPoint.x, photonPoint.z - m_receiverRadius );
				photonValue.second = photonPoint.y;

				QPair< int, int > photonCellId;
				photonCellId.first = floor( photonValue.first /widthCells ) + m_matrixWidth / 2;
				photonCellId.second = floor( photonValue.second /heightCells )+ m_matrixHeight / 2;
				photonsMatrix.insertMulti( photonCellId, photonValue );

			}
		}

		dataFile.close();

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

				fluxOut<< ( ( numberPhotons * wPhoton ) / cellArea )<<"\t";
			}
			fluxOut<<"\n";
		}

		//Power compute
		double targetPower = totalPhotons * wPhoton;

		saveFluxFile.close();
		resultsOut<<filesName[index]<< "\t"<<totalPhotons<<"\t"<<targetPower<<"\t"<<( ( maxPhotonsInCell * wPhoton ) / cellArea )<<"\n";
	}
	saveResultsFile.close();
}

void LS3Analysis::RunTonatiuh() const
{
	Trace trace( "LS3Analysis::RunTonatiuh", false );

	QDir photonMapsDir( *m_dataDirectory );
	QStringList filesName =  photonMapsDir.entryList ( QDir::Files );

	QDir resultsDir( *m_saveDirectory );

	int fileNumber = 0;
	QString fileName = QString( "photonMapsAnalysis_%1.txt" ).arg( QString::number( fileNumber ) );
	QFile saveResultsFile( resultsDir.filePath( fileName ) );

	while( saveResultsFile.exists() )
	{
		fileNumber++;
		fileName = QString( "photonMapsAnalysis_%1.txt" ).arg( QString::number( fileNumber ) );
		saveResultsFile.setFileName( resultsDir.filePath( fileName ) );
	}

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

		double widthCells = ( 2 * tgc::Pi ) / m_matrixWidth;
		double heightCells = m_receiverLength / m_matrixHeight;
		double cellArea = ( ( 2 * tgc::Pi * m_receiverRadius ) / m_matrixWidth ) * ( m_receiverLength / m_matrixHeight );

		QMap< QPair< int, int >, QPair< double, double> > photonsMatrix;
		while ( !in.atEnd() )
		{

			double id, x, y, z, prev, next;
			in >> id >> x >> y >> z >> prev >> next;

			QPair< double, double > photonValue;
			photonValue.first = atan2( x, y );
			photonValue.second = z;

			QPair< int, int > photonCellId;
			photonCellId.first = floor( photonValue.first /widthCells ) + m_matrixWidth / 2;
			photonCellId.second = floor( photonValue.second /heightCells );
			photonsMatrix.insertMulti( photonCellId, photonValue );
		}

		dataFile.close();

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

				fluxOut<< ( ( numberPhotons * wPhoton ) / cellArea )<<"\t";
			}
			fluxOut<<"\n";
		}

		//Power compute
		double targetPower = totalPhotons * wPhoton;

		saveFluxFile.close();
		resultsOut<<filesName[index]<< "\t"<<totalPhotons<<"\t"<<targetPower<<"\t"<<( ( maxPhotonsInCell * wPhoton ) / cellArea )<<"\n";
	}
	saveResultsFile.close();
}
