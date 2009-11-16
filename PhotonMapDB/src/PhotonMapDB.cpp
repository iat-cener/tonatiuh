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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <string>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include <db_cxx.h>

#include "PhotonMapDB.h"
#include "tgc.h"
#include "tgf.h"
#include "Trace.h"

/* This is the constructor for the photon map.
 * To create the photon map it is necessary to specify the
 * maximum number of photons that will be stored
*/

PhotonMapDB::PhotonMapDB()
:m_dbDirName( 0 ), m_dbEnv( 0 ), m_photonMapDB( 0 ), m_isSecondary( false ), m_storedPhotons( 0 )
{
	Trace trace( "PhotonMapDB::PhotonMapDB", false );

	u_int32_t env_flags = DB_CREATE |
			DB_INIT_LOCK |
			DB_INIT_MPOOL;// |
			//DB_INIT_TXN;
	//u_int32_t db_flags = DB_CREATE | DB_AUTO_COMMIT;

	m_dbDirName = QFileDialog::getExistingDirectory( 0, QString( "Save Tonatiuh photonMap" ), "." );
	QDir dbDir( m_dbDirName );

	m_dbEnv = new DbEnv( 0 );
	m_dbEnv->open( m_dbDirName.toStdString().c_str(), env_flags, 0 );


	m_surfacesDB= new Db( m_dbEnv, 0 );
	if( dbDir.exists( "surfaces.db" ) )	dbDir.remove( "Surfaces.db" );


	m_photonMapDB = new Db( m_dbEnv, 0 );
	if( dbDir.exists( "PhotonMap.db" ) )	dbDir.remove( "PhotonMap.db" );
	m_photonMapDB->set_re_len( sizeof( PHOTON ) );

	m_secondaryPMDB = new Db( m_dbEnv, 0 );
	m_secondaryPMDB->set_flags( DB_DUPSORT );
	if( dbDir.exists( "PhotonMap.sdb" ) )	dbDir.remove( "PhotonMap.sdb" );

	Open();

}

/**
 * Destroys photon map.
 **/
PhotonMapDB :: ~PhotonMapDB()
{
	Trace trace( "PhotonMapDB::~PhotonMapDB", false );
	Close();
}

/**
 * Returns the icon associated to the photon map.
 **/
QString PhotonMapDB::GetIcon()
{
	Trace trace( "PhotonMapDB::getIcon", false );

	return QString(":icons/photonMapDB.png");
}

/**
 * Return a list with all photons stored at photon map.
 * Is not yet implemented.
 */
QList< Photon* > PhotonMapDB::GetAllPhotons() const
{
	Trace trace( "PhotonMapDB::GetAllPhotons", false );
	QList< Photon* > photonsList;

	//Get a cursor to the photon map db
	Dbc* cursor;
	m_photonMapDB->cursor( NULL, &cursor, 0 );

	//Iterate over the database.
	Dbt key;
	Dbt data;
	int ret;

	while( ( ret = cursor->get( &key, &data, DB_NEXT ) ) == 0 )
	{
		db_recno_t* keyData = (db_recno_t*)key.get_data();
		double photonID = *keyData;

		PHOTON* photonData= (PHOTON*) data.get_data();

		Photon* prev = ( photonData->previous > 0 ) ?  new Photon( Point3D( -tgc::Infinity, -tgc::Infinity, -tgc::Infinity ), 0, 0, photonData->previous ) : 0;
		Photon* next = ( photonData->next > 0 ) ? new Photon( Point3D( -tgc::Infinity, -tgc::Infinity, -tgc::Infinity ), 0, 0, photonData->next ) : 0;
		photonsList.push_back( new Photon( Point3D( photonData->x, photonData->y, photonData->z ), prev, next, photonID ) );
	}

	cursor->close();
	return photonsList;
}

/**
 * Return the photon with \a index id.
 */
Photon* PhotonMapDB::GetPhoton( double photonID ) const
{
	Trace trace( "PhotonMapDB::GetPhoton", false );

	Dbc* cursor;
	m_photonMapDB->cursor( NULL, &cursor, 0 );

	db_recno_t recnoID = ( db_recno_t ) photonID;
	Dbt key( &recnoID, sizeof( recnoID ) );
	Dbt data;
	int ret = cursor->get( &key, &data, DB_SET );
	//cursor->close();

	if( ret == DB_NOTFOUND )
	{
		cursor->close();
		return 0;
	}
	PHOTON* photonData= (PHOTON*) data.get_data();

	Photon* prev = ( photonData->previous > 0 ) ?  new Photon( Point3D( -tgc::Infinity, -tgc::Infinity, -tgc::Infinity ), 0, 0, photonData->previous ) : 0;
	Photon* next = ( photonData->next > 0 ) ? new Photon( Point3D( -tgc::Infinity, -tgc::Infinity, -tgc::Infinity ), 0, 0, photonData->next ) : 0;

	cursor->close();
	return new Photon( Point3D( photonData->x, photonData->y, photonData->z ), prev, next, photonID );

	return 0;
}

/**
 * Not yet implemented.
 */
QList< Photon* > PhotonMapDB::GetSurfacePhotons( InstanceNode* instance ) const
{
	Trace trace( "PhotonMapDB::GetSurfacePhotons", false );

	QList< Photon* > surfacePhotonsList;

	db_recno_t surfaceID = m_surfaceURL_ID.value( instance );


	Dbc* cursor;
	m_secondaryPMDB->cursor( NULL, &cursor, 0 );

	Dbt sKey( &surfaceID, sizeof( db_recno_t) );
	Dbt pKey, pData;

	int ret = cursor->pget( &sKey, &pKey, &pData, DB_SET );
	int contador = 0;

	while( ret == 0 )
	{
		contador++;
		db_recno_t* keyData = (db_recno_t*)pKey.get_data();
		double photonID = *keyData;

		PHOTON* photonData= (PHOTON*) pData.get_data();

		Photon* prev = ( photonData->previous > 0 ) ?  new Photon( Point3D( -tgc::Infinity, -tgc::Infinity, -tgc::Infinity ), 0, 0, photonData->previous ) : 0;
		Photon* next = ( photonData->next > 0 ) ? new Photon( Point3D( -tgc::Infinity, -tgc::Infinity, -tgc::Infinity ), 0, 0, photonData->next ) : 0;
		surfacePhotonsList.push_back( new Photon( Point3D( photonData->x, photonData->y, photonData->z ), prev, next, photonID ) );

		ret = cursor->pget( &sKey, &pKey, &pData, DB_NEXT_DUP );
	}

	cursor->close();

	return surfacePhotonsList;
}
/**
 * Stores new photon at the photon map.
 **/
void PhotonMapDB::StoreRay( Photon* rayFirstPhoton )
{
	Trace trace( "PhotonMapDB::StoreRay", false );

	double nodeID = m_storedPhotons + 1;
	rayFirstPhoton->id =nodeID;
	Photon* currentNode = rayFirstPhoton->next;
	while( currentNode )
	{
		nodeID++;
		currentNode->id = nodeID;
		currentNode = currentNode->next;
	}

	PHOTON photonData;
	currentNode = rayFirstPhoton;

	photonData.x = rayFirstPhoton->pos.x;
	photonData.y = rayFirstPhoton->pos.y;
	photonData.z = rayFirstPhoton->pos.z;
	photonData.previous = ( rayFirstPhoton->prev ) ? rayFirstPhoton->prev->id : 0;
	photonData.next = ( rayFirstPhoton->next ) ? rayFirstPhoton->next->id : 0;

	if( !rayFirstPhoton->intersectedSurface ) photonData.surface = 0;
	else
	{
		if( !m_surfaceURL_ID.contains( rayFirstPhoton->intersectedSurface ) )	InsertSurface( rayFirstPhoton->intersectedSurface );
		photonData.surface = m_surfaceURL_ID.value( rayFirstPhoton->intersectedSurface );
	}

	db_recno_t recnoID = (db_recno_t ) rayFirstPhoton->id;
	Dbt key( &recnoID, sizeof( recnoID ) );
	Dbt data( &photonData, sizeof( PHOTON ) );

	int ret = m_photonMapDB->put( NULL, &key, &data, DB_APPEND );
	if( ret == DB_KEYEXIST )	m_photonMapDB->err( ret, "Put failed because key %f already exists", rayFirstPhoton->id );

	m_storedPhotons++;


	while( currentNode->next )
	{
		currentNode = currentNode->next;
		PHOTON photonData;
		photonData.x = currentNode->pos.x;
		photonData.y = currentNode->pos.y;
		photonData.z = currentNode->pos.z;
		photonData.previous = ( currentNode->prev ) ? currentNode->prev->id : 0;
		photonData.next = ( currentNode->next ) ? currentNode->next->id : 0;
		if( !currentNode->intersectedSurface ) photonData.surface = 0;
		else
		{
			if( !m_surfaceURL_ID.contains( currentNode->intersectedSurface ) )	InsertSurface( currentNode->intersectedSurface );
			photonData.surface = m_surfaceURL_ID.value( currentNode->intersectedSurface );
		}

		db_recno_t recnoID = (db_recno_t ) currentNode->id;
		Dbt key( &recnoID, sizeof( recnoID ) );
				Dbt data( &photonData, sizeof( PHOTON ) );

		int ret = m_photonMapDB->put( NULL, &key, &data, DB_APPEND );

		if( ret == DB_KEYEXIST )	m_photonMapDB->err( ret, "Put failed because key %f already exists", currentNode->id );

		m_storedPhotons++;

		delete currentNode->prev;
	}

	delete currentNode;
}

/**
 * Returns the number of photons stored at the photon map.
 */
unsigned long PhotonMapDB::StoredPhotons() const
{
	Trace trace( "PhotonMapDB::StoredPhotons", false );
	return m_storedPhotons;
}

/**
 * Database close activity.
 */
void PhotonMapDB::Close()
{
	Trace trace( "PhotonMapDB::Close", false );

    // Close the db
    try
    {
    	m_secondaryPMDB->close( 0 );
    	m_photonMapDB->close( 0 );
    	m_surfacesDB->close( 0 );
    	m_dbEnv-> close( 0 );
    	delete m_photonMapDB;
    	delete m_dbEnv;
    }
    catch(DbException &e)
    {
    	QString message = QString( "Error closing database:\n%1" ).arg( QString( e.what() ) );
    	QMessageBox::warning( NULL, "Tonatiuh Action", message );
    }
    catch(std::exception&e)
    {
    	QString message = QString( "Error closing database:\n%1" ).arg( QString( e.what() ) );
    	QMessageBox::warning( NULL, "Tonatiuh Action", message );
    }
}

/**
 * Database open activity.
 * The database is already created, the function doesn't create a new one.
 */
void PhotonMapDB::Open()
{
	Trace trace( "PhotonMapDB::Open", false );

	try
	{
		if( !m_photonMapDB )
		{
			std::cout <<" There is not a db"<< std::endl;
			return;
		}

		QDir dbDir( m_dbDirName );
		QString pmFileName = dbDir.absoluteFilePath( "PhotonMap.db ");
		QString sFileName = dbDir.absoluteFilePath( "Surfaces.db ");
		QString spmFileName = dbDir.absoluteFilePath( "PhotonMap.sdb ");

		//Open the database: concurrently usable by multiple threads
		m_photonMapDB->open( NULL, pmFileName.toStdString().c_str(), NULL, DB_QUEUE, DB_CREATE , 0 );
		m_surfacesDB->open( NULL, sFileName.toStdString().c_str(), NULL, DB_RECNO, DB_CREATE , 0 );
		m_secondaryPMDB->open( NULL, spmFileName.toStdString().c_str(), NULL, DB_BTREE, DB_CREATE, 0 );
		m_photonMapDB->associate( NULL, m_secondaryPMDB, get_photon_surface, 0 );

	}
	// DbException is not a subclass of std::exception, so we
	// need to catch them both.
	catch(DbException &e)
	{
		QString message = QString( "Error opening database:\n%1" ).arg( QString( e.what() ) );
		QMessageBox::warning( NULL, "Tonatiuh Action", message );
	}
	catch(std::exception &e)
	{
		QString message = QString( "Error opening database:\n%2" ).arg( QString( e.what() ) );
		QMessageBox::warning( NULL, "Tonatiuh Action", message );
	}

}


void PhotonMapDB::InsertSurface(  InstanceNode* instance  )
{
	Trace trace( "PhotonMapDB::InsertSurface", false );

	//DbTxn* txn = 0;
	//m_dbEnv->txn_begin( NULL, &txn, 0 );

	//Insert to surfaces database
	db_recno_t surfaceID = (db_recno_t ) m_surfaceURL_ID.count() + 1;
	Dbt key( &surfaceID, sizeof( surfaceID ) );

	QString surfaceURL = instance->GetNodeURL();

	char* surfaceUrl = ( char* ) surfaceURL.toStdString().c_str();
	Dbt data( surfaceUrl, strlen( surfaceUrl ) +1 );
	m_surfacesDB->put( NULL, &key, &data, DB_APPEND );

	m_surfaceURL_ID.insert( instance, m_surfaceURL_ID.count() + 1 );
}

int PhotonMapDB::get_photon_surface( Db* secondaryDB, const Dbt* pKey, const Dbt* pData, Dbt* sKey )
{

	Trace trace( "PhotonMapDB::get_photon_surface", false );
	PHOTON* photon;

	//Extract the photon contained in the PhotonMap.db
	photon = ( PHOTON* )pData->get_data();
	//
	db_recno_t surface = photon->surface;
	sKey->set_data( &surface );
	sKey->set_size( sizeof( db_recno_t ) );

	return 0;
}
