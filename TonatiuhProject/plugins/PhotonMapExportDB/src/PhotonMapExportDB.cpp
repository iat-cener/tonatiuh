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


#include <sstream>
#include <string>
#include <QDir>

#include <QMessageBox>

#include "PhotonMapExportDB.h"

/*!
 *Creates a photonmap export objcet to save the data into a SQL database
 */
PhotonMapExportDB::PhotonMapExportDB()
:PhotonMapExport(),
 m_exportedPhoton( 0 ),
 m_isDBOpened( false ),
 m_pDB( 0 ),
 m_sqlTotalInsert( "" )
{

}

/*!
 *Destorys export object
 */
PhotonMapExportDB::~PhotonMapExportDB()
{

}

/*!
 * Closes database.
 */
void PhotonMapExportDB::EndExport()
{
	if( m_isDBOpened )	Close();
}


/*!
 * Opens database database.
 */
bool  PhotonMapExportDB::StartExport()
{
	if( m_exportedPhoton < 1  )	RemoveExistingFiles();
	if( !m_isDBOpened )	return Open();

	return 1;
}

QStringList PhotonMapExportDB::GetParameterNames()
{
	QStringList parametersNames;
	parametersNames<<QLatin1String( "ExportDirectory" );
	parametersNames<<QLatin1String( "DBFilename" );

	return parametersNames;
}
/*!
 * Saves \a rayLists data into the database.
 */
void PhotonMapExportDB::SavePhotonMap( std::vector< Photon* > raysLists )
{

	if( !m_isDBOpened )	Open();

	if( m_saveCoordinates && m_saveSide && m_savePrevNexID && m_saveSurfaceID )
		SaveAllData( raysLists );
	else if( m_saveCoordinates && m_saveSide && !m_savePrevNexID && m_saveSurfaceID )
		SaveNotNextPrevID( raysLists );
	else
		SaveSelectedData( raysLists );

}


/*!
 * Saves power per photon.
 */
void PhotonMapExportDB::SetPowerPerPhoton( double wPhoton )
{
	std::stringstream w_photon;
	w_photon << "Insert into wphoton values(" << wPhoton <<");";
	char* zErrMsg = 0;

	int rc = sqlite3_exec( m_pDB, w_photon.str().c_str(), 0, 0, &zErrMsg );
	if( rc!=SQLITE_OK )
	{
		std::cout<< "SQL error: "<<zErrMsg<<"\n"<<std::endl;
		sqlite3_free(zErrMsg);
		}

}

/*!
 *Sets to parameter \a parameterName, the value \a parameterValue.
 */
void PhotonMapExportDB::SetSaveParameterValue( QString parameterName, QString parameterValue )
{
	if( parameterName == QLatin1String( "ExportDirectory" ) )
				SetDBDirectory( parameterValue );
	if( parameterName == QLatin1String( "DBFilename" ) )
		SetDBFileName( parameterValue );


}

/*!
 * Closes database.
 */
bool PhotonMapExportDB::Close()
{

    // Close the db
    try
    {
    	if( sqlite3_close( m_pDB ) != SQLITE_OK )
    	{
        	QString message = QString( "Error closing database." );
        	QMessageBox::warning( NULL, "Tonatiuh Action", message );
            return 0;
    	}

    	m_isDBOpened = false;

    }
    catch(std::exception&e)
    {
    	QString message = QString( "Error closing database:\n%1" ).arg( QString( e.what() ) );
    	QMessageBox::warning( NULL, "Tonatiuh Action", message );
        return 0;
    }
    return 1;

}
void PhotonMapExportDB::InsertSurface( InstanceNode* instance )
{
	m_surfaceIdentfier.push_back( instance );
	m_surfaceWorldToObject.push_back( instance->GetIntersectionTransform() );


	int surfaceID = m_surfaceIdentfier.size();
	QString surfaceURL = QString(" ").append( instance->GetNodeURL() );

	std::stringstream surfaces;
	surfaces << "Insert into Surfaces values("
			<< surfaceID << ",'"
			<< surfaceURL.toStdString().c_str() <<"');";
	m_sqlTotalInsert += surfaces.str();

}

/*!
 * Opens database
 */
bool PhotonMapExportDB::Open()
{
	std::cout<<"PhotonMapExportDB::Open"<<std::endl;
	try
	{
		QDir dbDirectory( m_dbDirectory );
		QString filename = m_dbFileName;

		QString dbFilename = dbDirectory.absoluteFilePath( filename.append( QLatin1String( ".db" ) ) );

		std::cout<<"PhotonMapExportDB::Open "<<dbFilename.toStdString()<<std::endl;

		char* zErrMsg = 0;

		int rc = sqlite3_open( dbFilename.toStdString().c_str() , &m_pDB );
		if( rc != SQLITE_OK )
		{
			QString message = QString( "Error opening %1 file database.\n" ).arg( dbFilename );
			message.append( QString( zErrMsg ) );
			QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
			return 0;
		}
		std::cout<<"PhotonMapExportDB::Open "<<rc<<std::endl;

		if( m_exportedPhoton< 1)
		{

			QString createPhotonsTableCmmd( QLatin1String( "CREATE TABLE Photons (id INTEGER PRIMARY KEY" ) );
			if( m_saveCoordinates )
				createPhotonsTableCmmd.append( QLatin1String( ", x REAL, y REAL, z REAL" ) );

			if( m_saveSide )
				createPhotonsTableCmmd.append( QLatin1String( ", side INTEGER " ) );

			if( m_savePrevNexID )
				createPhotonsTableCmmd.append( QLatin1String( ", previousID INTEGER, nextID INTEGER" ) );

			if( m_saveSurfaceID )
				createPhotonsTableCmmd.append( QLatin1String( ", surfaceID INTEGER,"
						" FOREIGN KEY( surfaceID ) REFERENCES surfaces ( id ) " )  );

			createPhotonsTableCmmd.append( QLatin1String( " ); " ) );

			rc = sqlite3_exec( m_pDB, createPhotonsTableCmmd.toStdString().c_str(), 0, 0, &zErrMsg );
			if( rc != SQLITE_OK )
			{
				QString message( "Error creating photons table:\n " );
				message.append( QString( zErrMsg ) );
				QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				sqlite3_free( zErrMsg );
				return 0;
			}

			QString createSurfacesTableCmmd( QLatin1String( "CREATE TABLE Surfaces("
					"	id INTEGER PRIMARY KEY,"
					"	Path VARCHAR(15000)"
					");" ) );

			rc = sqlite3_exec( m_pDB, createSurfacesTableCmmd.toStdString().c_str(), 0, 0, &zErrMsg );
			if( rc != SQLITE_OK )
			{
				QString message( "Error creating surfaces table:\n " );
				message.append( QString( zErrMsg ) );
				QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				sqlite3_free( zErrMsg );
				return 0;
			}

			QString createWPhotonTableCmmd( QLatin1String( "CREATE TABLE wphoton("
					"	power REAL"
					");" ) );

			rc = sqlite3_exec( m_pDB, createWPhotonTableCmmd.toStdString().c_str(), 0, 0, &zErrMsg );
			if( rc != SQLITE_OK )
			{
				QString message( "Error creating wphoton table:\n " );
				message.append( QString( zErrMsg ) );
				QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				sqlite3_free( zErrMsg );
				return 0;
			}


			rc = sqlite3_exec( m_pDB, "PRAGMA synchronous=OFF;", 0, 0, &zErrMsg );
			if( rc != SQLITE_OK )
			{
				QString message = QString( "SQL error: %1\n%2" ).arg( QString( zErrMsg ) );
				QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				sqlite3_free( zErrMsg );
				return 0;
			}
		}
		m_sqlTotalInsert="BEGIN;";
		m_isDBOpened = true;
	}
	catch( std::exception &e )
	{
		QString message = QString( "Error opening database:\n%2" ).arg( QString( e.what() ) );
		QMessageBox::warning( NULL, "Tonatiuh Action", message );
		return 0;
	}
	return 1;
}

void PhotonMapExportDB::RemoveExistingFiles()
{

	QDir exportDirectory( m_dbDirectory );
	QString filename = m_dbFileName;

	QString exportFilename = exportDirectory.absoluteFilePath( filename.append( QLatin1String( ".db" ) ) );
	QFile exportFile( exportFilename );

	if(exportFile.exists() && !exportFile.remove()) {
		QString message= QString( "Error deleting database:\n%1\nThe database is in use. Please, close it before continuing. \n" ).arg(exportFilename);
		QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
		RemoveExistingFiles();
	}


}

/*!
 * Saves for each photon all the data.
 */
void PhotonMapExportDB::SaveAllData( std::vector< Photon* > raysLists )
{

	unsigned long nPhotonElements = raysLists.size();
	double previousPhotonID = 0;

	if( m_saveCoordinatesInGlobal )
	{
		for( unsigned int i = 0; i < raysLists.size(); i++ )
		{
			std::stringstream ss;
			Photon* photon = raysLists[i];
			if( photon->id < 1 )	previousPhotonID = 0;

			//m_saveCoordinates
			ss << "INSERT INTO Photons values("<< (++m_exportedPhoton );
			ss <<"," << photon->pos.x <<"," << photon->pos.y <<"," << photon->pos.z;

			//m_saveSide
			ss << ","<< photon->side;

			//m_savePrevNexID
			ss <<"," << previousPhotonID;

			int nextPhotonID = 0;
			if( ( i < ( nPhotonElements - 1 ) ) && ( raysLists[i+1]->id > 0  ) )
				nextPhotonID = m_exportedPhoton +1;
			ss <<"," << nextPhotonID;

			//m_saveSurfaceID
			unsigned long urlId = 0;
			if( photon->intersectedSurface )
			{
				if( !m_surfaceIdentfier.contains( photon->intersectedSurface ) )
					InsertSurface( photon->intersectedSurface );

				urlId = m_surfaceIdentfier.indexOf( photon->intersectedSurface ) + 1;

			}
			ss<<"," << urlId <<");";

			m_sqlTotalInsert += ss.str();
			previousPhotonID = m_exportedPhoton;
		}
	}
	else
	{
		for( unsigned int i = 0; i < raysLists.size(); i++ )
		{
			std::stringstream ss;
			Photon* photon = raysLists[i];
			if( photon->id < 1 )	previousPhotonID = 0;

			unsigned long urlId = 0;
			Transform worldToObject( 1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0 );
			if( photon->intersectedSurface )
			{

				if( !m_surfaceIdentfier.contains( photon->intersectedSurface ) )
					InsertSurface( photon->intersectedSurface );

				urlId = m_surfaceIdentfier.indexOf( photon->intersectedSurface ) ;
				worldToObject = m_surfaceWorldToObject[urlId];
				urlId++;
			}

			ss << "Insert into photons values("<< (++m_exportedPhoton );

			//m_saveCoordinates
			Point3D localPos = worldToObject( photon->pos );
			ss <<"," << localPos.x <<"," << localPos.y <<"," << localPos.z;

			//m_saveSide
			ss << ","<< photon->side;

			//m_savePrevNexID
			ss <<"," << previousPhotonID;

			int nextPhotonID = 0;
			if( ( i < ( nPhotonElements - 1 ) ) && ( raysLists[i+1]->id > 0  ) )
				nextPhotonID = m_exportedPhoton +1;
			ss <<"," << nextPhotonID;

			//m_saveSurfaceID
			ss<<"," << urlId <<");";

			m_sqlTotalInsert += ss.str();
			previousPhotonID = m_exportedPhoton;
		}
	}

	char* zErrMsg = 0;
	m_sqlTotalInsert.append( "COMMIT;" );

	int rc = sqlite3_exec( m_pDB, m_sqlTotalInsert.c_str(), 0, 0, &zErrMsg );
	if( rc!=SQLITE_OK )
	{
		std::cout<< "SQL error: "<<zErrMsg<<"\n"<<std::endl;
		sqlite3_free(zErrMsg);
	}
	m_sqlTotalInsert.clear();
	m_sqlTotalInsert = "BEGIN;";
}

/*!
 * Saves for each photon all the data, except previous and next photon identifier.
 */
void PhotonMapExportDB::SaveNotNextPrevID( std::vector< Photon* > raysLists )
{

	unsigned long nPhotonElements = raysLists.size();

	if( m_saveCoordinatesInGlobal )
	{
		for( unsigned int i = 0; i < nPhotonElements; i++ )
		{
			std::stringstream ss;
			Photon* photon = raysLists[i];

			ss << "INSERT INTO Photons VALUES("<< (++m_exportedPhoton );

			//m_saveCoordinates
			ss <<"," << photon->pos.x <<"," << photon->pos.y <<"," << photon->pos.z;

			//m_saveSide
			ss << ","<< photon->side;

			//m_saveSurfaceID
			unsigned long urlId = 0;
			if( photon->intersectedSurface )
			{
				if( !m_surfaceIdentfier.contains( photon->intersectedSurface ) )
					InsertSurface( photon->intersectedSurface );

				urlId = m_surfaceIdentfier.indexOf( photon->intersectedSurface ) + 1;

			}
			ss<<"," << urlId <<");";

			m_sqlTotalInsert += ss.str();
		}
	}
	else
	{
		for( unsigned int i = 0; i < nPhotonElements; i++ )
		{
			std::stringstream ss;
			Photon* photon = raysLists[i];

			unsigned long urlId = 0;
			Transform worldToObject( 1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0 );
			if( photon->intersectedSurface )
			{

				if( !m_surfaceIdentfier.contains( photon->intersectedSurface ) )
					InsertSurface( photon->intersectedSurface );

				urlId = m_surfaceIdentfier.indexOf( photon->intersectedSurface ) ;
				worldToObject = m_surfaceWorldToObject[urlId];
				urlId++;
			}

			ss << "INSERT INTO Photons VALUES("<< (++m_exportedPhoton );

			//m_saveCoordinates
			Point3D localPos = worldToObject( photon->pos );
			ss <<"," << localPos.x <<"," << localPos.y <<"," << localPos.z;

			//m_saveSide
			ss << ","<< photon->side;

			//m_saveSurfaceID
			ss<<"," << urlId <<");";

			m_sqlTotalInsert += ss.str();
		}
	}

	char* zErrMsg = 0;
	m_sqlTotalInsert.append( "COMMIT;" );

	int rc = sqlite3_exec( m_pDB, m_sqlTotalInsert.c_str(), 0, 0, &zErrMsg );
	if( rc!=SQLITE_OK )
	{
		std::cout<< "SQL error: "<<zErrMsg<<"\n"<<std::endl;
		sqlite3_free(zErrMsg);
	}
	m_sqlTotalInsert.clear();
	m_sqlTotalInsert = "BEGIN;";
}

/*!
 * Saves for each photon the selected data.
 */
void PhotonMapExportDB::SaveSelectedData( std::vector< Photon* > raysLists )
{

	unsigned long nPhotonElements = raysLists.size();
	double previousPhotonID = 0;

	for( unsigned int i = 0; i < raysLists.size(); i++ )
	{
		std::stringstream ss;
		Photon* photon = raysLists[i];
		if( photon->id < 1 )	previousPhotonID = 0;

		unsigned long urlId = 0;
		Transform worldToObject( 1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0 );
		if( photon->intersectedSurface )
		{

			if( !m_surfaceIdentfier.contains( photon->intersectedSurface ) )
				InsertSurface( photon->intersectedSurface );

			urlId = m_surfaceIdentfier.indexOf( photon->intersectedSurface ) ;
			worldToObject = m_surfaceWorldToObject[urlId];
			urlId++;
		}

		ss << "INSERT INTO Photons VALUES("<< (++m_exportedPhoton );

		if( m_saveCoordinates && m_saveCoordinatesInGlobal )
			ss <<"," << photon->pos.x <<"," << photon->pos.y <<"," << photon->pos.z;
		else if( m_saveCoordinates && !m_saveCoordinatesInGlobal )
		{
			Point3D localPos = worldToObject( photon->pos );
			ss <<"," << localPos.x <<"," << localPos.y <<"," << localPos.z;
		}

		if( m_saveSide )	ss << ","<< photon->side;

		if( m_savePrevNexID )
		{
			ss <<"," << previousPhotonID;
			int nextPhotonID = 0;
			if( ( i < ( nPhotonElements - 1 ) ) && ( raysLists[i+1]->id > 0  ) )
				nextPhotonID = m_exportedPhoton +1;

			ss <<"," << nextPhotonID;
		}

		if( m_saveSurfaceID )	ss<<"," << urlId;

		ss<<" );";

		m_sqlTotalInsert += ss.str();
		previousPhotonID = m_exportedPhoton;
	}

	char* zErrMsg = 0;
	m_sqlTotalInsert.append( "COMMIT;" );

	int rc = sqlite3_exec( m_pDB, m_sqlTotalInsert.c_str(), 0, 0, &zErrMsg );
	if( rc!=SQLITE_OK )
	{
		std::cout<< "SQL error: "<<zErrMsg<<"\n"<<std::endl;
		sqlite3_free(zErrMsg);
	}
	m_sqlTotalInsert.clear();
	m_sqlTotalInsert = "BEGIN;";
}


/*!
 *Sets \a path as the database location.
 */
void PhotonMapExportDB::SetDBDirectory( QString path )
{
	m_dbDirectory = path;
}

/*!
 *Sets \a filename as the file for the database.
 */
void PhotonMapExportDB::SetDBFileName( QString filename )
{
	m_dbFileName = filename;
}
