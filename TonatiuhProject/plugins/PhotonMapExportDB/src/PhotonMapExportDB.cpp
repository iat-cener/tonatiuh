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
 m_isWPhoton( false ),
 m_pDB( 0 )
{

}

/*!
 *Destroys export object
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
	QString wPhotonQuery;
	if( m_isWPhoton )
	{
		wPhotonQuery = QString( "UPDATE WPhoton SET power = %1;" ).arg( QString::number( wPhoton ) );
	}
	else
	{
		wPhotonQuery = QString( "INSERT INTO WPhoton VALUES( %1 );" ).arg( QString::number( wPhoton ) );
		m_isWPhoton = true;
	}

	char* zErrMsg = 0;

	int rc = sqlite3_exec( m_pDB, wPhotonQuery.toStdString().c_str(), 0, 0, &zErrMsg );
	if( rc != SQLITE_OK )
	{

		QString message = QString( "SQL error: %1 .\n" ).arg( QString( zErrMsg ) );
		QMessageBox::warning( 0, QLatin1String( "Tonatiuh" ), message );
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
	char* sErrMsg = 0;
	sqlite3_exec( m_pDB, surfaces.str().c_str(), 0, 0, &sErrMsg );

}

/*!
 * Opens database
 */
bool PhotonMapExportDB::Open()
{
	try
	{
		QDir dbDirectory( m_dbDirectory );
		QString filename = m_dbFileName;

		QString dbFilename = dbDirectory.absoluteFilePath( filename.append( QLatin1String( ".db" ) ) );

		char* zErrMsg = 0;
		int rc = sqlite3_open( dbFilename.toStdString().c_str() , &m_pDB );
		if( rc != SQLITE_OK )
		{
			QString message = QString( "Error opening %1 file database.\n" ).arg( dbFilename );
			message.append( QString( zErrMsg ) );
			QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
			return 0;
		}

		if( m_exportedPhoton < 1)
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
		else
			m_isWPhoton = true;

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

	if(exportFile.exists() && !exportFile.remove() )
	{
		QString message= QString( "Error deleting database:%1.\n"
				"The database is in use. Please, close it before continuing. \n" ).arg( exportFilename );
		QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
		RemoveExistingFiles();
	}
}

/*!
 * Saves for each photon all the data.
 */
void PhotonMapExportDB::SaveAllData( std::vector< Photon* > raysLists )
{

	const char* tail = 0;
	sqlite3_stmt* stmt;
	char insertSQL[256] ="\0";
	sprintf( insertSQL, "INSERT INTO Photons VALUES( @id, @x, @y, @z, @side, @prev, @next, @surfaceID )" );
	sqlite3_prepare_v2( m_pDB, insertSQL, 256, &stmt, &tail );

	char* sErrMsg = 0;
	sqlite3_exec( m_pDB, "BEGIN TRANSACTION", 0, 0, &sErrMsg );

	unsigned long nPhotonElements = raysLists.size();
	double previousPhotonID = 0;

	if( m_saveCoordinatesInGlobal )
	{
		for( unsigned int i = 0; i < raysLists.size(); i++ )
		{
			Photon* photon = raysLists[i];
			if( photon->id < 1 )	previousPhotonID = 0;

			sqlite3_bind_text( stmt, 1, QString::number(++m_exportedPhoton ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveCoordinates
			Point3D photonPos =  m_concentratorToWorld( photon->pos );
			sqlite3_bind_text( stmt, 2, QString::number( photonPos.x ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, 3, QString::number( photonPos.y ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, 4, QString::number( photonPos.z ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveSide
			sqlite3_bind_text( stmt, 5, QString::number( photon->side ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_savePrevNexID
			sqlite3_bind_text( stmt, 6, QString::number( previousPhotonID ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			int nextPhotonID = 0;
			if( ( i < ( nPhotonElements - 1 ) ) && ( raysLists[i+1]->id > 0  ) )
				nextPhotonID = m_exportedPhoton +1;
			sqlite3_bind_text( stmt, 7, QString::number( nextPhotonID ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveSurfaceID
			unsigned long urlId = 0;
			if( photon->intersectedSurface )
			{
				if( !m_surfaceIdentfier.contains( photon->intersectedSurface ) )
					InsertSurface( photon->intersectedSurface );

				urlId = m_surfaceIdentfier.indexOf( photon->intersectedSurface ) + 1;

			}
			sqlite3_bind_text( stmt, 8, QString::number( urlId ).toStdString().c_str(), -1, SQLITE_TRANSIENT );


			sqlite3_step( stmt );
			sqlite3_clear_bindings( stmt );
			sqlite3_reset( stmt );

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


			sqlite3_bind_text( stmt, 1, QString::number(++m_exportedPhoton ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveCoordinates
			Point3D photonPos = worldToObject(  photon->pos );
			sqlite3_bind_text( stmt, 2, QString::number( photonPos.x ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, 3, QString::number( photonPos.y ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, 4, QString::number( photonPos.z ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveSide
			sqlite3_bind_text( stmt, 5, QString::number( photon->side ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_savePrevNexID
			sqlite3_bind_text( stmt, 6, QString::number( previousPhotonID ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			int nextPhotonID = 0;
			if( ( i < ( nPhotonElements - 1 ) ) && ( raysLists[i+1]->id > 0  ) )
				nextPhotonID = m_exportedPhoton +1;
			sqlite3_bind_text( stmt, 7, QString::number( nextPhotonID ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveSurfaceID
			sqlite3_bind_text( stmt, 8, QString::number( urlId ).toStdString().c_str(), -1, SQLITE_TRANSIENT );


			sqlite3_step( stmt );
			sqlite3_clear_bindings( stmt );
			sqlite3_reset( stmt );
			previousPhotonID = m_exportedPhoton;
		}
	}
	int rc = sqlite3_exec( m_pDB, "END TRANSACTION", 0, 0, &sErrMsg );

	sqlite3_finalize( stmt );
	if( rc != SQLITE_OK )
	{
		std::cout<< "SQL error: "<<sErrMsg<<"\n"<<std::endl;
		sqlite3_free( sErrMsg );
	}
}

/*!
 * Saves for each photon all the data, except previous and next photon identifier.
 */
void PhotonMapExportDB::SaveNotNextPrevID( std::vector< Photon* > raysLists )
{

	const char* tail = 0;
	sqlite3_stmt* stmt;
	char insertSQL[256] ="\0";
	sprintf( insertSQL, "INSERT INTO Photons VALUES( @id, @x, @y, @z, @side, @surfaceID )" );
	sqlite3_prepare_v2( m_pDB, insertSQL, 256, &stmt, &tail );

	char* sErrMsg = 0;
	sqlite3_exec( m_pDB, "BEGIN TRANSACTION", 0, 0, &sErrMsg );

	unsigned long nPhotonElements = raysLists.size();

	if( m_saveCoordinatesInGlobal )
	{

		for( unsigned int i = 0; i < nPhotonElements; i++ )
		{
			Photon* photon = raysLists[i];


			sqlite3_bind_text( stmt, 1, QString::number(++m_exportedPhoton ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveCoordinates
			Point3D photonPos =  m_concentratorToWorld( photon->pos );
			sqlite3_bind_text( stmt, 2, QString::number( photonPos.x ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, 3, QString::number( photonPos.y ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, 4, QString::number( photonPos.z ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveSide
			sqlite3_bind_text( stmt, 5, QString::number( photon->side ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveSurfaceID
			unsigned long urlId = 0;
			if( photon->intersectedSurface )
			{
				if( !m_surfaceIdentfier.contains( photon->intersectedSurface ) )
					InsertSurface( photon->intersectedSurface );

				urlId = m_surfaceIdentfier.indexOf( photon->intersectedSurface ) + 1;

			}
			sqlite3_bind_text( stmt, 6, QString::number( urlId ).toStdString().c_str(), -1, SQLITE_TRANSIENT );


			sqlite3_step( stmt );
			sqlite3_clear_bindings( stmt );
			sqlite3_reset( stmt );
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

			sqlite3_bind_text( stmt, 1, QString::number(++m_exportedPhoton ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveCoordinates
			Point3D localPos = worldToObject( photon->pos );
			sqlite3_bind_text( stmt, 2, QString::number( localPos.x ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, 3, QString::number( localPos.y ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, 4, QString::number( localPos.z ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveSide
			sqlite3_bind_text( stmt, 5, QString::number( photon->side ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			//m_saveSurfaceID
			sqlite3_bind_text( stmt, 6, QString::number( urlId ).toStdString().c_str(), -1, SQLITE_TRANSIENT );


			sqlite3_step( stmt );
			sqlite3_clear_bindings( stmt );
			sqlite3_reset( stmt );


		}
	}
	int rc = sqlite3_exec( m_pDB, "END TRANSACTION", 0, 0, &sErrMsg );

	sqlite3_finalize( stmt );
	if( rc != SQLITE_OK )
	{
		std::cout<< "SQL error: "<<sErrMsg<<"\n"<<std::endl;
		sqlite3_free( sErrMsg );
	}
}

/*!
 * Saves for each photon the selected data.
 */
void PhotonMapExportDB::SaveSelectedData( std::vector< Photon* > raysLists )
{


	const char* tail = 0;
	sqlite3_stmt* stmt;
	QString insertCommand( "INSERT INTO Photons VALUES( @id" );

	if( m_saveCoordinates )	insertCommand.append( ", @x, @y, @z" );
	if( m_saveSide )	insertCommand.append( ", @side" );
	if( m_savePrevNexID )	insertCommand.append( ", @prev, @next" );
	if( m_saveSurfaceID )	insertCommand.append( ", @surfaceID" );
	insertCommand.append( ")" );

	sqlite3_prepare_v2( m_pDB, insertCommand.toStdString().c_str(), 256, &stmt, &tail );

	char* sErrMsg = 0;
	sqlite3_exec( m_pDB, "BEGIN TRANSACTION", 0, 0, &sErrMsg );

	unsigned long nPhotonElements = raysLists.size();
	double previousPhotonID = 0;


	for( unsigned int i = 0; i < raysLists.size(); i++ )
	{
		int parameterIndex = 0;
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

		sqlite3_bind_text( stmt, ++parameterIndex, QString::number(++m_exportedPhoton ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

		if( m_saveCoordinates && m_saveCoordinatesInGlobal )
		{
			Point3D photonPos =  m_concentratorToWorld( photon->pos );
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( photonPos.x ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( photonPos.y ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( photonPos.z ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

		}
		else if( m_saveCoordinates && !m_saveCoordinatesInGlobal )
		{
			Point3D photonPos = worldToObject( photon->pos );
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( photonPos.x ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( photonPos.y ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( photonPos.z ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
		}

		if( m_saveSide )
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( photon->side ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

		if( m_savePrevNexID )
		{
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( previousPhotonID ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

			int nextPhotonID = 0;
			if( ( i < ( nPhotonElements - 1 ) ) && ( raysLists[i+1]->id > 0  ) )
				nextPhotonID = m_exportedPhoton +1;
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( nextPhotonID ).toStdString().c_str(), -1, SQLITE_TRANSIENT );
		}

		if( m_saveSurfaceID )
			sqlite3_bind_text( stmt, ++parameterIndex, QString::number( urlId ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

		sqlite3_bind_text( stmt, 1, QString::number(++m_exportedPhoton ).toStdString().c_str(), -1, SQLITE_TRANSIENT );

		sqlite3_step( stmt );
		sqlite3_clear_bindings( stmt );
		sqlite3_reset( stmt );

		previousPhotonID = m_exportedPhoton;
	}

	int rc = sqlite3_exec( m_pDB, "END TRANSACTION", 0, 0, &sErrMsg );

	sqlite3_finalize( stmt );
	if( rc != SQLITE_OK )
	{
		std::cout<< "SQL error: "<<sErrMsg<<"\n"<<std::endl;
		sqlite3_free( sErrMsg );
	}
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
