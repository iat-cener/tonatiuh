/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victlor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>

#include <QDataStream>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

#include "PhotonMapExportFile.h"
#include "InstanceNode.h"
#include "SceneModel.h"

/*!
 * Creates export object to export photon map photons to a file.
 */
PhotonMapExportFile::PhotonMapExportFile()
:PhotonMapExport(),
 m_photonsFilename( QLatin1String( "PhotonMap" ) ),
 m_powerPerPhoton( 0.0 ),
 m_currentFile( 1 ),
 m_exportDirecotryName( QLatin1String( "" ) ),
 m_exportedPhotons( 0 ),
 m_nPhotonsPerFile( -1 ),
 m_oneFile( true )
{

}

/*!
 * Destroys export object
 */
PhotonMapExportFile::~PhotonMapExportFile()
{

}

/*!
 * Returns the plugin parameters names.
 */
QStringList PhotonMapExportFile::GetParameterNames()
{
	QStringList parametersNames;
	parametersNames<<QLatin1String( "ExportDirectory" );
	parametersNames<<QLatin1String( "ExportFile" );
	parametersNames<<QLatin1String( "FileSize" );

	return parametersNames;
}

/*!
 *
 */
void PhotonMapExportFile::EndExport()
{

	QDir exportDirectory( m_exportDirecotryName );
	QString exportFilename;
	if( m_oneFile )
		exportFilename = exportDirectory.absoluteFilePath( QString( QLatin1String("%1_parameters.txt" ) ).arg( m_photonsFilename ) );
	else
	{
		QString newName = QString( QLatin1String( "%1_parameters.txt" ) ).arg( m_photonsFilename );
		exportFilename = exportDirectory.absoluteFilePath( newName );
	}

	QString tmpFilename( QLatin1String( "tmpexportpotonmap.dat" ) );
	QFile exportFile( exportFilename );
	//exportFile.copy( tmpFilename ) ;
	//exportFile.remove();


	WriteFileFormat( exportFilename );
	exportFile.open( QIODevice::Append );
	QTextStream out( &exportFile );

	out<<double( m_powerPerPhoton );
	//Append first file data
	/*QFile tmpExportFile( tmpFilename );
	tmpExportFile.open( QIODevice::ReadOnly );
	QDataStream in( &tmpExportFile );

	exportFile.open( QIODevice::Append );
	QDataStream out( &exportFile );
	std::cout<<"."<<std::endl;
	out<<double( m_powerPerPhoton );*/


	/*while( !in.atEnd() )
	{
		double doubleData;
		in>>doubleData;
		out<<doubleData;
	}
	QDataStream inn( &tmpExportFile );
	while( !inn.atEnd() )
		{
			char* string;
			inn>>string;
			std::cout<<string<<"."<<std::endl;
		}
	exportFile.close();
	tmpExportFile.close();
	tmpExportFile.remove();*/

}

/*!
 * Saves \a raysList photons to file.
 */
void PhotonMapExportFile::SavePhotonMap( std::vector< std::vector <Photon > > raysLists )
{

	unsigned long nLists = raysLists.size();
	unsigned long lastListSize = raysLists[nLists-1].size();

	if( m_oneFile )
	{
		QDir exportDirectory( m_exportDirecotryName );
		QString filename = m_photonsFilename;
		QString exportFilename = exportDirectory.absoluteFilePath( filename.append( QLatin1String( ".dat" ) ) );

		if( m_saveCoordinates && m_saveSide && m_savePrevNexID && m_saveSurfaceID )
			ExportPhotonsAllData( exportFilename, raysLists, 0, 0, nLists - 1, lastListSize -1 );
		else if( m_saveCoordinates && m_saveSide && !m_savePrevNexID && m_saveSurfaceID )
			ExportSurfacePhotonsAllData( exportFilename, raysLists, 0, 0, nLists - 1, lastListSize -1 );
		else
			ExportAllPhotonsFile( exportFilename, raysLists, 0, 0, nLists - 1, lastListSize -1 );
	}
	else
		ExportAllPhotonsFiles( raysLists );

}

/*!
 *	Sets the current power per
 */
void PhotonMapExportFile::SetPowerPerPhoton( double wPhoton )
{
	m_powerPerPhoton = wPhoton;
}



/*!
 * Sets to parameter \a parameterName the value \a parameterValue.
 */
void PhotonMapExportFile::SetSaveParameterValue( QString parameterName, QString parameterValue )
{

	QStringList parameters = GetParameterNames();

	//Directory name
	if( parameterName == parameters[0] )
		m_exportDirecotryName = parameterValue;

	//File name
	else if( parameterName == parameters[1] )
	{
		m_photonsFilename = parameterValue;

	}

	//Maximum number of photons that a file can store.
	else if( parameterName == parameters[2] )
	{
		if( parameterValue.toDouble() < 0 )	m_oneFile = true;
		else
		{

			m_oneFile = false;
			m_nPhotonsPerFile = ( unsigned long ) parameterValue.toDouble();
		}

	}
}

/*!
 * Deletes the files that can be uset to export.
 */
void PhotonMapExportFile::StartExport()
{

	if( m_exportedPhotons < 1  )	RemoveExistingFiles();

}

/*!
 * Exports \a raysLists [\a startIndexRaysList, \a endIndexRaysList ] to [\a endIndexRaysList, \a endIndexPhotonList ] photons data
 * to file \a filename.
 */
void PhotonMapExportFile::ExportAllPhotonsFile( QString filename, std::vector< std::vector <Photon > > raysLists,
		unsigned long startIndexRaysList, unsigned long startIndexPhotonList,
		unsigned long endIndexRaysList, unsigned long endIndexPhotonList )
{

	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );


	double previousPhotonID = 0;
	for( unsigned long i = startIndexRaysList; i <= endIndexRaysList; ++i )
	{
		unsigned long startIndex = 0;
		if( i == startIndexRaysList )	startIndex = startIndexPhotonList;

		std::vector <Photon > raysList = raysLists[i];
		unsigned long  nPhotonElements = raysList.size() - 1;
		if( i == endIndexRaysList )	nPhotonElements = endIndexPhotonList;
		for( unsigned long j = startIndex; j <= nPhotonElements; ++j )
		{
			Photon photon = raysList[j];
			QString surfaceURL = "";
			if( photon.intersectedSurface )
			{
				surfaceURL = photon.intersectedSurface->GetNodeURL();
				if( !m_surfaceIdentfier.contains( surfaceURL ) )
				{
					m_surfaceIdentfier.insert( surfaceURL, m_surfaceIdentfier.count() +1 );
					m_surfaceWorldToObject.insert( surfaceURL, photon.intersectedSurface->GetIntersectionTransform() );
				}
			}

			out<<double( ++m_exportedPhotons );
			if( photon.id < 1 )	previousPhotonID = 0;
			//else	previousPhotonID = m_exportedPhotons;
			if( m_saveCoordinates && m_saveCoordinatesInGlobal )	out<<photon.pos.x << photon.pos.y << photon.pos.z;
			else if( m_saveCoordinates && !m_saveCoordinatesInGlobal )
			{
				if( !surfaceURL.isEmpty() && m_surfaceWorldToObject.contains( surfaceURL ) )
				{
					Transform worldToObject = m_surfaceWorldToObject.value( surfaceURL );
					Point3D localPos = worldToObject( photon.pos );
					out<<localPos.x << localPos.y << localPos.z;
				}
				else
					out<<photon.pos.x << photon.pos.y << photon.pos.z;
			}

			if(  m_saveSide )
			{
    			double side = double( photon.side );
    		    out<<side;
			}
			if( m_savePrevNexID )
			{
    		    out<<previousPhotonID;
    		    if( ( j < nPhotonElements ) && ( raysList[j+1].id > 0  ) )	out<< double( m_exportedPhotons +1 );
    		    else out <<0.0;
			}
			//if( m_saveSurfacesURLList.count() > 0 )
			if( m_saveSurfaceID )
			{
				unsigned long urlId = 0;
				if( !surfaceURL.isEmpty() && m_surfaceIdentfier.contains( surfaceURL ) )
					urlId = m_surfaceIdentfier.value( surfaceURL );
				else
				{
					urlId = m_surfaceIdentfier.count()+ 1;
					m_surfaceIdentfier.insert( surfaceURL, urlId );
				}


				out<<double( urlId );
			}
			previousPhotonID = m_exportedPhotons;
		}
	}
	exportFile.close();

}

/*!
 * Exports \a raysLists photons data to files. Each file stores \a m_nPhotonsPerFile photons.
 */
void PhotonMapExportFile::ExportAllPhotonsFiles( std::vector< std::vector <Photon > > raysLists )
{
	unsigned long nLists = raysLists.size();
	unsigned long lastListSize = raysLists[nLists-1].size();

	QDir exportDirectory( m_exportDirecotryName );
	//m_allPhotonsFilename

	unsigned long indexStartList = 0;
	unsigned long indexStartListPhoton = 0;

	unsigned long filePhotons = m_exportedPhotons - ( m_nPhotonsPerFile * ( m_currentFile - 1 ) );
	for( unsigned long i = 0; i < raysLists.size(); i++ )
	{
		std::vector <Photon > currentList = raysLists[i];
		for( unsigned long j = 0; j < currentList.size(); j++ )
		{
			filePhotons++;
			if( !( filePhotons < m_nPhotonsPerFile ) )
			{
				QString newName = QString( QLatin1String( "%1_%2.dat" ) ).arg(
						m_photonsFilename,
						QString::number( m_currentFile ) );

				QString currentFileName = exportDirectory.absoluteFilePath( newName );

				if( m_saveCoordinates && m_saveSide && m_savePrevNexID && m_saveSurfaceID )
					ExportPhotonsAllData( currentFileName, raysLists, indexStartList, indexStartListPhoton, i, j );
				else if( m_saveCoordinates && m_saveSide && !m_savePrevNexID && m_saveSurfaceID )
					ExportSurfacePhotonsAllData( currentFileName, raysLists, 0, 0, nLists - 1, lastListSize -1 );
				else
					ExportAllPhotonsFile( currentFileName, raysLists, indexStartList, indexStartListPhoton, i, j );
				m_currentFile++;
				//m_surfaceIdentfier.insert( m_photonsFilename, m_currentFile );
				filePhotons = 0;

				indexStartList = i;
				indexStartListPhoton = j+1;
				if( indexStartListPhoton == currentList.size() )
				{
					indexStartListPhoton = 0;
					indexStartList++;
				}

			}
		}

	}

	if( filePhotons > 0 )
	{
		QString newName = QString( QLatin1String( "%1_%2.dat" ) ).arg(
				m_photonsFilename,
				QString::number( m_currentFile ) );

		QString currentFileName = exportDirectory.absoluteFilePath( newName );


		if( m_saveCoordinates && m_saveSide && m_savePrevNexID && m_saveSurfaceID )
			ExportPhotonsAllData( currentFileName, raysLists, indexStartList, indexStartListPhoton, nLists - 1, lastListSize - 1 );
		else if( m_saveCoordinates && m_saveSide && !m_savePrevNexID && m_saveSurfaceID )
			ExportSurfacePhotonsAllData( currentFileName, raysLists, 0, 0, nLists - 1, lastListSize -1 );
		else
			ExportAllPhotonsFile( currentFileName, raysLists, indexStartList, indexStartListPhoton, nLists - 1, lastListSize - 1 );
	}
}
/*!
 * Exports \a raysLists [\a startIndexRaysList, \a endIndexRaysList ] to [\a endIndexRaysList, \a endIndexPhotonList ] photons all data
 * to file \a filename.
 */
void PhotonMapExportFile::ExportPhotonsAllData( QString filename, std::vector< std::vector <Photon > > raysLists,
		unsigned long startIndexRaysList, unsigned long startIndexPhotonList,
		unsigned long endIndexRaysList, unsigned long endIndexPhotonList )
{

	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );


	double previousPhotonID = 0;
	for( unsigned long i = startIndexRaysList; i <= endIndexRaysList; ++i )
	{
		unsigned long startIndex = 0;
		if( i == startIndexRaysList )	startIndex = startIndexPhotonList;

		std::vector <Photon > raysList = raysLists[i];
		unsigned long  nPhotonElements = raysList.size() - 1;
		if( i == endIndexRaysList )	nPhotonElements = endIndexPhotonList;
		for( unsigned long j = startIndex; j <= nPhotonElements; ++j )
		{
			Photon photon = raysList[j];
			QString surfaceURL = "";
			if( photon.intersectedSurface )
			{
				surfaceURL = photon.intersectedSurface->GetNodeURL();
				if( !m_surfaceIdentfier.contains( surfaceURL ) )
				{
					m_surfaceIdentfier.insert( surfaceURL, m_surfaceIdentfier.count() +1 );
					m_surfaceWorldToObject.insert( surfaceURL, photon.intersectedSurface->GetIntersectionTransform() );
				}
			}

			out<<double( ++m_exportedPhotons );
			if( photon.id < 1 )	previousPhotonID = 0;


			//m_saveCoordinates
			if( m_saveCoordinatesInGlobal )	out<<photon.pos.x << photon.pos.y << photon.pos.z;
			else
			{
				if( !surfaceURL.isEmpty() && m_surfaceWorldToObject.contains( surfaceURL ) )
				{
					Transform worldToObject = m_surfaceWorldToObject.value( surfaceURL );
					Point3D localPos = worldToObject( photon.pos );
					out<<localPos.x << localPos.y << localPos.z;
				}
				else
					out<<photon.pos.x << photon.pos.y << photon.pos.z;
			}

			//m_saveSide
			double side = double( photon.side );
			out<<side;

			//m_savePrevNexID
			out<<previousPhotonID;
			if( ( j < nPhotonElements ) && ( raysList[j+1].id > 0  ) )	out<< double( m_exportedPhotons +1 );
			else out <<0.0;

			//m_saveSurfaceID
			unsigned long urlId = 0;
			if( !surfaceURL.isEmpty() && m_surfaceIdentfier.contains( surfaceURL ) )
				urlId = m_surfaceIdentfier.value( surfaceURL );
			else
			{
				urlId = m_surfaceIdentfier.count()+ 1;
				m_surfaceIdentfier.insert( surfaceURL, urlId );
			}


			out<<double( urlId );

			previousPhotonID = m_exportedPhotons;
		}
	}
	exportFile.close();

}
/*!
 * Exports \a raysLists [\a startIndexRaysList, \a endIndexRaysList ] to [\a endIndexRaysList, \a endIndexPhotonList ] photons all data (
 * to file \a filename.
 */
void PhotonMapExportFile::ExportSurfacePhotonsAllData( QString filename, std::vector< std::vector <Photon > > raysLists,
		unsigned long startIndexRaysList, unsigned long startIndexPhotonList,
		unsigned long endIndexRaysList, unsigned long endIndexPhotonList )
{

	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );


	double previousPhotonID = 0;
	for( unsigned long i = startIndexRaysList; i <= endIndexRaysList; ++i )
	{
		unsigned long startIndex = 0;
		if( i == startIndexRaysList )	startIndex = startIndexPhotonList;

		std::vector <Photon > raysList = raysLists[i];
		unsigned long  nPhotonElements = raysList.size() - 1;
		if( i == endIndexRaysList )	nPhotonElements = endIndexPhotonList;
		for( unsigned long j = startIndex; j <= nPhotonElements; ++j )
		{
			Photon photon = raysList[j];
			QString surfaceURL = "";
			if( photon.intersectedSurface )
			{
				surfaceURL = photon.intersectedSurface->GetNodeURL();
				if( !m_surfaceIdentfier.contains( surfaceURL ) )
				{
					m_surfaceIdentfier.insert( surfaceURL, m_surfaceIdentfier.count() +1 );
					m_surfaceWorldToObject.insert( surfaceURL, photon.intersectedSurface->GetIntersectionTransform() );
				}
			}

			out<<double( ++m_exportedPhotons );
			if( photon.id < 1 )	previousPhotonID = 0;


			//m_saveCoordinates
			if( m_saveCoordinatesInGlobal )	out<<photon.pos.x << photon.pos.y << photon.pos.z;
			else
			{
				if( !surfaceURL.isEmpty() && m_surfaceWorldToObject.contains( surfaceURL ) )
				{
					Transform worldToObject = m_surfaceWorldToObject.value( surfaceURL );
					Point3D localPos = worldToObject( photon.pos );
					out<<localPos.x << localPos.y << localPos.z;
				}
				else
					out<<photon.pos.x << photon.pos.y << photon.pos.z;
			}

			//m_saveSide
			double side = double( photon.side );
			out<<side;


			//m_saveSurfaceID
			unsigned long urlId = 0;
			if( !surfaceURL.isEmpty() && m_surfaceIdentfier.contains( surfaceURL ) )
				urlId = m_surfaceIdentfier.value( surfaceURL );
			else
			{
				urlId = m_surfaceIdentfier.count()+ 1;
				m_surfaceIdentfier.insert( surfaceURL, urlId );
			}


			out<<double( urlId );

			previousPhotonID = m_exportedPhotons;
		}
	}
	exportFile.close();

}

/*!
 * Remove existing files that this export type can used.
 */
void PhotonMapExportFile::RemoveExistingFiles()
{

	QDir exportDirectory( m_exportDirecotryName );
	QString filename = m_photonsFilename;
	if( m_oneFile )
	{
		QString exportFilename = exportDirectory.absoluteFilePath( filename.append( QLatin1String( ".dat" ) ) );
		QFile exportFile( exportFilename );
		if(exportFile.exists()&&!exportFile.remove()) {
				QString message= QString( "Error deleting %1.\nThe file is in use. Please, close it before continuing. \n" ).arg( QString( exportFilename ) );
				QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				RemoveExistingFiles();
			}
	}
	else
	{

		QStringList filters;
		filters << filename.append( QLatin1String( "_*.dat" ) );
		exportDirectory.setNameFilters(filters);

		QFileInfoList partialFilesList = exportDirectory.entryInfoList();
		for( int i = 0; i< partialFilesList.count(); ++i )
		{
			QFile partialFile( partialFilesList[i].absoluteFilePath() );
			if(partialFile.exists() && !partialFile.remove()) {
					QString message= QString( "Error deleting %1.\nThe file is in use. Please, close it before continuing. \n" ).arg( QString( partialFilesList[i].absoluteFilePath() ) );
					QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
					RemoveExistingFiles();
				}

		}
	}
}

/*!
 * Writes the file or first file header with the format.
 */
void PhotonMapExportFile::WriteFileFormat( QString exportFilename )
{

	QFile exportFile( exportFilename );
	exportFile.open( QIODevice::WriteOnly );
	QTextStream out( &exportFile );
	out<<QString( QLatin1String( "START PARAMETERS\n" ) );
	out<<QString( QLatin1String( "id\n" ) );
	if( m_saveCoordinates  )
	{
		out<<QString( QLatin1String( "x\n" ) );
		out<<QString( QLatin1String( "y\n" ) );
		out<<QString( QLatin1String( "z\n" ) );
	}
	if(  m_saveSide )	out<<QString( QLatin1String( "side\n" ) );
	if( m_savePrevNexID )
	{
		out<<QString( QLatin1String( "previous ID\n" ) );
		out<<QString( QLatin1String( "next ID\n" ) );
	}
	if( m_saveSurfaceID )
	{
		out<<QString( QLatin1String( "surface ID\n" ) );
	}

	out<<QString( QLatin1String( "END PARAMETERS\n" ) );


	out<<QString( QLatin1String( "START SURFACES\n" ) );
	if( m_saveSurfaceID )
	{
		QMap<QString, unsigned long >::const_iterator s = m_surfaceIdentfier.constBegin();
		while( s != m_surfaceIdentfier.constEnd() )
		{
			out<<QString( QLatin1String( "%1 %2\n" ) ).arg( QString::number( s.value() ),
					s.key() );
			++s;
		 }

	}

	out<<QString( QLatin1String( "END SURFACES\n" ) );
}
