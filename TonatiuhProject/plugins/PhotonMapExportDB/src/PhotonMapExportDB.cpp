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
void PhotonMapExportDB::StartExport()
{
	if( m_exportedPhoton < 1  )	RemoveExistingFiles();
	if( !m_isDBOpened )	Open();
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
void PhotonMapExportDB::SavePhotonMap( std::vector< std::vector <Photon > > raysLists )
{

	if( !m_isDBOpened )	Open();


	double previousPhotonID = 0;
	for( unsigned int i = 0; i< raysLists.size(); i++ )
	{
		std::vector <Photon > raysList = raysLists[i];
		unsigned long  nPhotonElements = raysList.size() - 1;
		for( unsigned long j = 0; j < raysList.size(); ++j )
		{
			//unsigned int p = 0;
			std::stringstream ss;
			Photon photon = raysList[j];
			if( photon.id < 1 )	previousPhotonID = 0;

			ss << "Insert into photons values("<< (++m_exportedPhoton );
			if( m_saveCoordinates )
				ss <<"," << photon.pos.x <<"," << photon.pos.y <<"," << photon.pos.z;
			if( m_saveSide )	ss << ","<< photon.side;
			if( m_savePrevNexID )
			{
				ss <<"," << previousPhotonID;

				int nextPhotonID = 0;
    		    if( ( j < nPhotonElements ) && ( raysList[j+1].id > 0  ) )	nextPhotonID = m_exportedPhoton +1;
				ss <<"," << nextPhotonID;
			}
			int iSurface;
			if( !photon.intersectedSurface ) iSurface = 0;
			else
			{
				if( !m_surfaceURL_ID.contains( photon.intersectedSurface ) )
					InsertSurface( photon.intersectedSurface );
				iSurface = m_surfaceURL_ID.value( photon.intersectedSurface );
			}
			ss<<"," << iSurface <<");";
			m_sqlTotalInsert+=ss.str();

		}
	}

	char* zErrMsg = 0;
	m_sqlTotalInsert.append( "COMMIT;" );

	int rc = sqlite3_exec( m_pDB, m_sqlTotalInsert.c_str(), 0, 0, &zErrMsg );
	if( rc!=SQLITE_OK )
	{
		std::cout<<"estoy aqui"<<std::endl;
		std::cout<< "SQL error: "<<zErrMsg<<"\n"<<std::endl;
		sqlite3_free(zErrMsg);
	}
	m_sqlTotalInsert.clear();

	m_sqlTotalInsert = "BEGIN;";
}

/*!
 *
 */
void PhotonMapExportDB::SetDBFileName( QString filename )
{
	m_dbFileName = filename;
}


void PhotonMapExportDB::SetDBDirectory( QString filename )
{
	m_dbDirectory = filename;
}

/*!
 * Saves power per photon.
 */
void PhotonMapExportDB::SetPowerPerPhoton( double wPhoton )
{


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
void PhotonMapExportDB::Close()
{
    // Close the db
    try
    {
    	sqlite3_close( m_pDB );

    }
    catch(std::exception&e)
    {
    	QString message = QString( "Error closing database:\n%1" ).arg( QString( e.what() ) );
    	QMessageBox::warning( NULL, "Tonatiuh Action", message );
    }

}
void PhotonMapExportDB::InsertSurface( InstanceNode* instance )
{
	m_surfaceURL_ID.insert( instance, m_surfaceURL_ID.count() );

	int surfaceID = (int) m_surfaceURL_ID.count() - 1;
	QString surfaceURL =QString(" ").append( instance->GetNodeURL() );

	std::stringstream surfaces;
	surfaces << "Insert into Surfaces values(" << surfaceID <<",'"<< surfaceURL.toStdString().c_str() <<"');";
	m_sqlTotalInsert += surfaces.str();

}

/*!
 * Opens database
 */
void PhotonMapExportDB::Open()
{
	try
		{
		//std::cout<<m_dbDirectory.append("/").append(m_dbFileName).append(".db").toStdString().c_str()<<std::endl;
			QDir BDDirectory( m_dbDirectory );
			QString filename = m_dbFileName;

			QString BDFilename = BDDirectory.absoluteFilePath( filename.append( QLatin1String( ".db" ) ) );

		    char* zErrMsg = 0;
		    int rc = sqlite3_open(BDFilename.toStdString().c_str() , &m_pDB );

		    QString createPhotonsTableCmmd( QLatin1String( "create table Photons (ID bigint," ) );
		    if( m_saveCoordinates )
		    	createPhotonsTableCmmd.append( QLatin1String( " x money, y money, z money," ) );

		    if( m_saveSide )
		    	createPhotonsTableCmmd.append( QLatin1String( " side int, " ) );

			if( m_savePrevNexID )
				createPhotonsTableCmmd.append( QLatin1String( " previousID int, nexID int," ) );


			createPhotonsTableCmmd.append( QLatin1String( "  surface int);" ) );
			rc = sqlite3_exec( m_pDB, createPhotonsTableCmmd.toStdString().c_str(), 0, 0, &zErrMsg );
			if( rc!=SQLITE_OK )
			{
				QString message( "Error creating photons table:\n " );
				message.append( QString( zErrMsg ) );
				QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				sqlite3_free( zErrMsg );
			}

			QString createSurfacesTableCmmd( QLatin1String( "create table Surfaces (ID bigint, Path VARCHAR(15000), foreign key(path) references photons(surface));" ) );

			rc = sqlite3_exec( m_pDB, createSurfacesTableCmmd.toStdString().c_str(), 0, 0, &zErrMsg );
			if( rc!=SQLITE_OK )
			{
				QString message( "Error creating surfaces table:\n " );
				message.append( QString( zErrMsg ) );
				QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				sqlite3_free( zErrMsg );
			}

			rc = sqlite3_exec( m_pDB, "PRAGMA synchronous=OFF;", 0, 0, &zErrMsg );
			if( rc!=SQLITE_OK )
			{
				QString message = QString( "SQL error: %1\n%2" ).arg( QString( zErrMsg ) );
				QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
				sqlite3_free( zErrMsg );
			}
			m_sqlTotalInsert="BEGIN;";
	        m_isDBOpened = true;
		}
		catch( std::exception &e )
		{
			QString message = QString( "Error opening database:\n%2" ).arg( QString( e.what() ) );
			QMessageBox::warning( NULL, "Tonatiuh Action", message );
		}
}

void PhotonMapExportDB::RemoveExistingFiles()
{

	QDir exportDirectory( m_dbDirectory );
	QString filename = m_dbFileName;

	QString exportFilename = exportDirectory.absoluteFilePath( filename.append( QLatin1String( ".db" ) ) );
	QFile exportFile( exportFilename );

	if(!exportFile.remove()) {
		QString message= QString( "Error deleting database:\n%1\nThe database is in use. Please, close it before continuing. \n" ).arg(exportFilename);
		QMessageBox::warning( NULL, QLatin1String( "Tonatiuh" ), message );
		RemoveExistingFiles();
	}


}
