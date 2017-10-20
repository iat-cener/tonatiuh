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

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <stdlib.h>     /* strtoul */

#include <QDataStream>
#include <QDir>
#include <QString>
#include <QTextStream>


#include "Photon.h"
#include "SaveBinaryFile.h"

/*!
 * Creates an object.
 */
SaveBinaryFile::SaveBinaryFile()
:SaveFile(),
 m_currentFileID( 1 ),
 m_exportDirecotryName( std::string("") ),
m_exportedPhotons( 0 ),
m_nPhotonsPerFile( -1 ),
m_photonsFilename( std::string("") ),
m_powerPerPhoton ( -1 ),
m_oneFile(true)
{

}

/*!
 * Destroys the object.
 */
SaveBinaryFile::~SaveBinaryFile()
{

}

/*!
 * Ends save process.
 */
void SaveBinaryFile::EndSave()
{

	QDir exportDirectory( m_exportDirecotryName.c_str() );
	QString exportFilename;
	if( m_oneFile )
		exportFilename = exportDirectory.absoluteFilePath( QString( QLatin1String("%1_parameters.txt" ) ).arg( m_photonsFilename.c_str() ) );
	else
	{
		QString newName = QString( QLatin1String( "%1_parameters.txt" ) ).arg( m_photonsFilename.c_str() );
		exportFilename = exportDirectory.absoluteFilePath( newName );
	}

	QString tmpFilename( QLatin1String( "tmpexportpotonmap.dat" ) );
	QFile exportFile( exportFilename );


	WriteFileFormat( exportFilename );
	exportFile.open( QIODevice::Append );
	QTextStream out( &exportFile );

	out<<double( m_powerPerPhoton );
}

/*!
 * Saves \a raysList photons to file.
 */
void SaveBinaryFile::SavePhotonMap( std::vector < Photon* > raysLists )
{
	if( m_oneFile )
	{
		QDir exportDirectory( m_exportDirecotryName.c_str() );
		QString filename( m_photonsFilename.c_str() );
		QString exportFilename = exportDirectory.absoluteFilePath( filename.append( QLatin1String( ".dat" ) ) );

		if( m_saveCoordinates && m_saveSide && m_savePrevNextID && m_saveSurfaceID )
			ExportAllPhotonsAllData( exportFilename, raysLists );
		else if( m_saveCoordinates && m_saveSide && !m_savePrevNextID && m_saveSurfaceID )
			ExportAllPhotonsNotNextPrevID( exportFilename, raysLists );
		else
			ExportAllPhotonsSelectedData( exportFilename, raysLists );
	}
	else
		SaveToVariousFiles( raysLists );
}

/*!
 *	Sets the current power per photon to \a wPhoton.
 */
void SaveBinaryFile::SetPowerPerPhoton( double wPhoton )
{

	m_powerPerPhoton = wPhoton;
}

/*!
 * Sets the parameters to define the binary files to save.
 */
void SaveBinaryFile::SetSaveParameterValue( std::string parameterName, std::string parameterValue )
{
	//Directory name

	if (parameterName.compare( "ExportDirectory" ) == 0)
		m_exportDirecotryName = parameterValue;

	//File name
	else if (parameterName.compare( "ExportFile" ) == 0)
	{
		m_photonsFilename = parameterValue;

	}

	//Maximum number of photons that a file can store.
	else if (parameterName.compare( "FileSize" ) == 0)
	{

		unsigned long photonsPerFile = strtoul( parameterValue.c_str(), NULL, 0 );
		if( photonsPerFile < 1 )	m_oneFile = true;
		else
		{

			m_oneFile = false;
			m_nPhotonsPerFile = photonsPerFile;
		}

	}
}

/*!
 * Before start saving the current photon map in the defined file or files if a previous results have been saved in the files they are removed.
 */
bool SaveBinaryFile::StartSave()
{
	if( m_exportedPhotons < 1  )	RemoveExistingFiles();
	return 1;
}

/*!
 * Export \a a raysList all data to file \a filename.
 */
void SaveBinaryFile::ExportAllPhotonsAllData( QString filename, std::vector< Photon* > raysLists )
{
	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );

	unsigned long nPhotonElements = raysLists.size();

	double previousPhotonID = 0;
	for( unsigned long i = 0; i < nPhotonElements; ++i )
	{

		Photon* photon = raysLists[i];
		unsigned long urlId = 0;
		if( !photon->intersectedSurfaceURL.empty() )
		{
			auto it = std::find( m_saveSurfacesURLList.begin(), m_saveSurfacesURLList.end(), photon->intersectedSurfaceURL );
			if (it == m_saveSurfacesURLList.end())
			{
				// url is not in vector
				m_saveSurfacesURLList.push_back( photon->intersectedSurfaceURL );
				urlId = m_saveSurfacesURLList.size();

			} else
			{
				//urlId = m_saveSurfacesURLList.indexOf( photon->intersectedSurfaceURL ) + 1;
				urlId = std::distance( m_saveSurfacesURLList.begin(), it);
			}


		}

		out<<double( ++m_exportedPhotons );
		if( photon->id < 1 )	previousPhotonID = 0;


		//m_saveCoordinates
		Point3D scenePos = photon->posLocal;
		if( m_saveCoordinatesInGlobal )
			scenePos = photon->posWorld;
		out<<scenePos.x << scenePos.y << scenePos.z;

		//m_saveSide
		double side = double( photon->side );
		out<<side;

		//m_savePrevNexID
		out<<previousPhotonID;
		if( ( i < ( nPhotonElements - 1 ) ) && ( raysLists[i+1]->id > 0  ) )
			out<< double( m_exportedPhotons +1 );
		else
			out <<0.0;

		//m_saveSurfaceID
		out<<double( urlId );

		previousPhotonID = m_exportedPhotons;
	}
	exportFile.close();

}

/*!
 * Exports \a raysLists photons data except previous and next photon identifier to file \a filename.
 */
void SaveBinaryFile::ExportAllPhotonsNotNextPrevID( QString filename, std::vector< Photon* > raysLists )
{

	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );

	unsigned long nPhotons = raysLists.size();
	for( unsigned long i = 0; i < nPhotons; ++i )
	{
		Photon* photon = raysLists[i];
		unsigned long urlId = 0;
		if( !photon->intersectedSurfaceURL.empty() )
		{
			auto it = std::find( m_saveSurfacesURLList.begin(), m_saveSurfacesURLList.end(), photon->intersectedSurfaceURL );
			if (it == m_saveSurfacesURLList.end())
			{
				// url is not in vector
				m_saveSurfacesURLList.push_back( photon->intersectedSurfaceURL );
				urlId = m_saveSurfacesURLList.size();

			} else
			{
				//urlId = m_saveSurfacesURLList.indexOf( photon->intersectedSurfaceURL ) + 1;
				urlId = std::distance( m_saveSurfacesURLList.begin(), it);
			}
		}


		out<<double( ++m_exportedPhotons );

		//m_saveCoordinates
		Point3D scenePos = photon->posLocal;
		if( m_saveCoordinatesInGlobal )
			scenePos = photon->posWorld;
		out<<scenePos.x << scenePos.y << scenePos.z;

		//m_saveSide
		out<<double( photon->side );

		//m_saveSurfaceID
		out<<double( urlId );
	}

	exportFile.close();

}

/*!
 * Exports \a raysLists all photons data to file \a filename.
 * For each photon only selected parameters will be exported.
 */
void SaveBinaryFile::ExportAllPhotonsSelectedData( QString filename, std::vector< Photon* > raysLists )
{

	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );

	double previousPhotonID = 0;
	unsigned long nPhotons = raysLists.size();
	for( unsigned long i = 0; i < nPhotons; ++i )
	{
		Photon* photon = raysLists[i];
		unsigned long urlId = 0;
		if( !photon->intersectedSurfaceURL.empty() )
		{
			auto it = std::find( m_saveSurfacesURLList.begin(), m_saveSurfacesURLList.end(), photon->intersectedSurfaceURL );
			if (it == m_saveSurfacesURLList.end())
			{
				// url is not in vector
				m_saveSurfacesURLList.push_back( photon->intersectedSurfaceURL );
				urlId = m_saveSurfacesURLList.size();

			} else
			{
				//urlId = m_saveSurfacesURLList.indexOf( photon->intersectedSurfaceURL ) + 1;
				urlId = std::distance( m_saveSurfacesURLList.begin(), it);
			}
		}

		out<<double( ++m_exportedPhotons );
		if( photon->id < 1 )	previousPhotonID = 0;

		if( m_saveCoordinates && m_saveCoordinatesInGlobal )	out<<photon->posWorld.x << photon->posWorld.y << photon->posWorld.z;
		else if( m_saveCoordinates && !m_saveCoordinatesInGlobal )	out<<photon->posLocal.x << photon->posLocal.y << photon->posLocal.z;

		if(  m_saveSide )
		{
			double side = double( photon->side );
			out<<side;
		}
		if( m_savePrevNextID )
		{
			out<<previousPhotonID;
			if( ( i < nPhotons - 1 ) && ( raysLists[i+1]->id > 0  ) )	out<< double( m_exportedPhotons +1 );
			else out <<0.0;
		}

		if( m_saveSurfaceID )
			out<<double( urlId );

		previousPhotonID = m_exportedPhotons;

	}
	exportFile.close();
}

/*!
 * Exports \a numberOfPhotons photons from \a raysLists to file \a filename starting from [\a startIndexRaysList, \a endIndexRaysList ].
 */
void SaveBinaryFile::ExportSelectedPhotonsAllData( QString filename, std::vector< Photon* > raysLists,
		unsigned long startIndex, 	unsigned long numberOfPhotons )
{

	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );

	unsigned int nPhotonElements = raysLists.size();

	double previousPhotonID = 0;
	unsigned long exportedPhotonsToFile = 0;
	while( exportedPhotonsToFile < numberOfPhotons )
	{
		Photon* photon = raysLists[startIndex + exportedPhotonsToFile];
		unsigned long urlId = 0;
		if( !photon->intersectedSurfaceURL.empty() )
		{
			auto it = std::find( m_saveSurfacesURLList.begin(), m_saveSurfacesURLList.end(), photon->intersectedSurfaceURL );
			if (it == m_saveSurfacesURLList.end())
			{
				// url is not in vector
				m_saveSurfacesURLList.push_back( photon->intersectedSurfaceURL );
				urlId = m_saveSurfacesURLList.size();

			} else
			{
				//urlId = m_saveSurfacesURLList.indexOf( photon->intersectedSurfaceURL ) + 1;
				urlId = std::distance( m_saveSurfacesURLList.begin(), it);
			}
		}

		out<<double( ++m_exportedPhotons );
		if( photon->id < 1 )	previousPhotonID = 0;

		//m_saveCoordinates
		Point3D scenePos = photon->posLocal;
		if( m_saveCoordinatesInGlobal )
			scenePos = photon->posWorld;
		out<<scenePos.x << scenePos.y << scenePos.z;

		//m_saveSide
		double side = double( photon->side );
		out<<side;

		//m_savePrevNexID
		out<<previousPhotonID;
		if( ( ( startIndex + exportedPhotonsToFile ) < ( nPhotonElements - 1 ) ) && ( raysLists[startIndex + exportedPhotonsToFile + 1]->id > 0  ) )
			out<< double( m_exportedPhotons +1 );
		else
			out <<0.0;


		//m_saveSurfaceID
		out<<double( urlId );

		previousPhotonID = m_exportedPhotons;
		exportedPhotonsToFile++;

	}


	exportFile.close();

}

/*!
 * Exports \a numberOfPhotons photons from \a raysLists to file \a filename starting from [\a startIndexRaysList, \a endIndexRaysList ].
 */
void SaveBinaryFile::ExportSelectedPhotonsNotNextPrevID( QString filename, std::vector< Photon* > raysLists,
		unsigned long startIndex, unsigned long numberOfPhotons )
{
	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );


	unsigned long exportedPhotonsToFile = 0;
	while( exportedPhotonsToFile < numberOfPhotons )
	{
		Photon* photon = raysLists[startIndex + exportedPhotonsToFile];
		unsigned long urlId = 0;
		if( !photon->intersectedSurfaceURL.empty() )
		{
			auto it = std::find( m_saveSurfacesURLList.begin(), m_saveSurfacesURLList.end(), photon->intersectedSurfaceURL );
			if (it == m_saveSurfacesURLList.end())
			{
				// url is not in vector
				m_saveSurfacesURLList.push_back( photon->intersectedSurfaceURL );
				urlId = m_saveSurfacesURLList.size();

			} else
			{
				//urlId = m_saveSurfacesURLList.indexOf( photon->intersectedSurfaceURL ) + 1;
				urlId = std::distance( m_saveSurfacesURLList.begin(), it);
			}
		}

		out<<double( ++m_exportedPhotons );

		//m_saveCoordinates
		Point3D scenePos = photon->posLocal;
		if( m_saveCoordinatesInGlobal )
			scenePos = photon->posWorld;
		out<<scenePos.x << scenePos.y << scenePos.z;

		//m_saveSide
		double side = double( photon->side );
		out<<side;

		//m_saveSurfaceID
		out<<double( urlId );

		exportedPhotonsToFile++;
	}

	exportFile.close();

}

/*!
 * Exports \a numberOfPhotons photons from \a raysLists to file \a filename starting from [\a startIndexRaysList, \a endIndexRaysList ].
 *  * For each photon only selected parameters will be exported.
 */
void SaveBinaryFile::ExportSelectedPhotonsSelectedData( QString filename, std::vector< Photon* > raysLists,
		unsigned long startIndex, 	unsigned long numberOfPhotons )
{

	QFile exportFile( filename );
	exportFile.open( QIODevice::Append );

	QDataStream out( &exportFile );

	double previousPhotonID = 0;
	unsigned long nPhotonElements = raysLists.size();
	unsigned long exportedPhotonsToFile = 0;
	while( exportedPhotonsToFile < numberOfPhotons )
	{
		Photon* photon = raysLists[startIndex + exportedPhotonsToFile];
		unsigned long urlId = 0;
		if( !photon->intersectedSurfaceURL.empty() )
		{
			auto it = std::find( m_saveSurfacesURLList.begin(), m_saveSurfacesURLList.end(), photon->intersectedSurfaceURL );
			if (it == m_saveSurfacesURLList.end())
			{
				// url is not in vector
				m_saveSurfacesURLList.push_back( photon->intersectedSurfaceURL );
				urlId = m_saveSurfacesURLList.size();

			} else
			{
				//urlId = m_saveSurfacesURLList.indexOf( photon->intersectedSurfaceURL ) + 1;
				urlId = std::distance( m_saveSurfacesURLList.begin(), it);
			}
		}

		out<<double( ++m_exportedPhotons );
		if( photon->id < 1 )	previousPhotonID = 0;


		if( m_saveCoordinates && m_saveCoordinatesInGlobal )	out<<photon->posWorld.x << photon->posWorld.y << photon->posWorld.z;
		else if( m_saveCoordinates && !m_saveCoordinatesInGlobal )	out<<photon->posLocal.x << photon->posLocal.y << photon->posLocal.z;

		if(  m_saveSide )
		{
			double side = double( photon->side );
			out<<side;
		}
		if( m_savePrevNextID )
		{
			out<<previousPhotonID;
			if( ( ( startIndex + exportedPhotonsToFile ) < nPhotonElements )
					&& ( raysLists[startIndex + exportedPhotonsToFile + 1]->id > 0  ) )
				out<< double( m_exportedPhotons +1 );
			else out <<0.0;
		}

		if( m_saveSurfaceID )
			out<<double( urlId );

		previousPhotonID = m_exportedPhotons;
		exportedPhotonsToFile++;
	}
	exportFile.close();

}

/*!
 * Remove existing files that this export type can used.
 */
void SaveBinaryFile::RemoveExistingFiles()
{

	QDir exportDirectory( m_exportDirecotryName.c_str() );
	QString filename( m_photonsFilename.c_str() );
	if( m_oneFile )
	{
		QString exportFilename = exportDirectory.absoluteFilePath( filename.append( QLatin1String( ".dat" ) ) );
		QFile exportFile( exportFilename );
		if(exportFile.exists()&&!exportFile.remove()) {
				QString message= QString( "Error deleting %1.\nThe file is in use. Please, close it before continuing. \n" ).arg( QString( exportFilename ) );
				//QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				std::cerr<<message.toStdString()<<std::endl;
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
					//QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );

					std::cerr<<message.toStdString()<<std::endl;
					RemoveExistingFiles();
				}

		}
	}
}

/*!
 * Exports \a raysLists photons data to files with the same number of photons in each file.
 * Each file stores \a m_nPhotonsPerFile photons.
 */
void SaveBinaryFile::SaveToVariousFiles( std::vector <Photon* > raysLists )
{

	QDir exportDirectory( m_exportDirecotryName.c_str() );

	unsigned long nPhotons = raysLists.size();

	unsigned long startIndex = 0;
	unsigned long filePhotons = m_exportedPhotons - ( m_nPhotonsPerFile * ( m_currentFileID - 1 ) );

	unsigned long nPhotonsToExport = 0;
	for( unsigned long i = 0; i < nPhotons; i++ )
	{
		if( !( filePhotons < m_nPhotonsPerFile ) )
		{
			std::string newName = m_photonsFilename + std::string( "_" ) + std::to_string(m_currentFileID) + std::string( ".dat" );

			QString currentFileName = exportDirectory.absoluteFilePath( newName.c_str() );

			if( m_saveCoordinates && m_saveSide && m_savePrevNextID && m_saveSurfaceID )
				ExportSelectedPhotonsAllData( currentFileName, raysLists, startIndex, nPhotonsToExport );
			else if( m_saveCoordinates && m_saveSide && !m_savePrevNextID && m_saveSurfaceID )
				ExportSelectedPhotonsNotNextPrevID( currentFileName, raysLists, startIndex, nPhotonsToExport );
			else
				ExportSelectedPhotonsSelectedData( currentFileName, raysLists, startIndex, nPhotonsToExport );
			m_currentFileID++;

			filePhotons = 0;
			nPhotonsToExport = 0;
			startIndex = i;

		}
		filePhotons++;
		nPhotonsToExport++;

	}

	if( filePhotons > 0 )
	{
		std::string newName = m_photonsFilename + std::string( "_" ) + std::to_string(m_currentFileID) + std::string( ".dat" );

		QString currentFileName = exportDirectory.absoluteFilePath( newName.c_str() );

		if( m_saveCoordinates && m_saveSide && m_savePrevNextID && m_saveSurfaceID )
			ExportSelectedPhotonsAllData( currentFileName, raysLists, startIndex, nPhotonsToExport );
		else if( m_saveCoordinates && m_saveSide && !m_savePrevNextID && m_saveSurfaceID )
			ExportSelectedPhotonsNotNextPrevID( currentFileName, raysLists, startIndex, nPhotonsToExport );
		else
			ExportSelectedPhotonsSelectedData( currentFileName, raysLists, startIndex, nPhotonsToExport );
	}

}

/*!
 * Writes the file or first file header with the format.
 */
void SaveBinaryFile::WriteFileFormat( QString exportFilename )
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
	if( m_savePrevNextID )
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
	for( unsigned int s = 0; s < m_saveSurfacesURLList.size(); s++ )
	{
		QString surfaceURL( m_saveSurfacesURLList[s].c_str() );
		out<<QString( QLatin1String( "%1 %2\n" ) ).arg( QString::number( s+1 ),
				surfaceURL);
	}

	out<<QString( QLatin1String( "END SURFACES\n" ) );
}

