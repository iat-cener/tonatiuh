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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

/*!
 * \class PluginManager
 * \brief PluginManager class manages plugin loading.
 * PluginManager is used to load plugins, manage the list of loaded plugins.
 */
#include "Trace.h"

#include <iostream>

#include <QDir>
#include <QPluginLoader>

#include "PhotonMapExportType.h"
#include "PhotonMapExportTypeFactory.h"
#include "PluginManager.h"
#include "RandomDeviateFactory.h"
#include "TMaterialFactory.h"
#include "TShapeFactory.h"
#include "TSunshapeFactory.h"
#include "TTrackerFactory.h"
#include "TTransmissivityFactory.h"

/*!
 * Creates a new PluginManager object.
 */
PluginManager::PluginManager()
{

}

/*!
 * Destroyes PluginManager object.
 */
PluginManager::~PluginManager()
{
	Trace{ "PluginManager::~PluginManager", false };
}

/*!
 * Returns an object of the photon map export type \a factoryName.
 */
std::unique_ptr< PhotonMapExportType > PluginManager::CreatePhotonMapExportType( std::string factoryName ) const
{
	for( unsigned int p = 0; p < m_photonMapExportTypeFactoryList.size(); p++ )
		if( m_photonMapExportTypeFactoryList[p]->PhotonMapExportTypName() == factoryName )
			return ( std::move( m_photonMapExportTypeFactoryList[p]->CreatePhotonMapExportType() ) );

	return nullptr;
}

/*!
 * Returns an object of the random generator \a factoryName.
 */
std::shared_ptr< RandomDeviate > PluginManager::CreateRandomDeviate( std::string factoryName ) const
{
	for( unsigned int r = 0; r < m_randomDeviateFactoryList.size(); r++ )
		if( m_randomDeviateFactoryList[r]->RandomDeviateName() == factoryName )
			return ( m_randomDeviateFactoryList[r]->CreateRandomDeviate() );

	return nullptr;
}

/*!
 * Returns an object of the material \a factoryName.
 */
std::shared_ptr< TMaterial > PluginManager::CreateTMaterial( std::string factoryName ) const
{
	for( unsigned int m = 0; m < m_tmaterialFactoryList.size(); m++ )
		if( m_tmaterialFactoryList[m]->TMaterialName() == factoryName )
			return ( m_tmaterialFactoryList[m]->CreateTMaterial() );

	return nullptr;
}

/*!
 * Returns an object of the shape \a factoryName.
 */
std::shared_ptr< TShape > PluginManager::CreateTShape( std::string factoryName ) const
{
	for( unsigned int s = 0; s < m_tshapeFactoryList.size(); s++ )
		if( m_tshapeFactoryList[s]->TShapeName() == factoryName )
			return ( m_tshapeFactoryList[s]->CreateTShape() );

	return nullptr;
}

/*!
 * Returns an object of the sunshape \a factoryName.
 */
std::shared_ptr< TSunshape > PluginManager::CreateTSunshape( std::string factoryName ) const
{
	for( unsigned int s = 0; s < m_tsunshapeFactoryList.size(); s++ )
		if( m_tsunshapeFactoryList[s]->TSunshapeName() == factoryName )
			return ( m_tsunshapeFactoryList[s]->CreateTSunshape() );

	return nullptr;
}

/*!
 * Returns an object of the tracker \a factoryName.
 */
std::shared_ptr< TTracker > PluginManager::CreateTTracker( std::string factoryName ) const
{
	for( unsigned int t = 0; t < m_ttrackerFactoryList.size(); t++ )
		if( m_ttrackerFactoryList[t]->TTrackerName() == factoryName )
			return ( m_ttrackerFactoryList[t]->CreateTTracker() );

	return nullptr;
}


/*!
 * Returns an object of the transmissivity model \a factoryName.
 */
std::shared_ptr< TTransmissivity > PluginManager::CreateTTransmissivity( std::string factoryName ) const
{
	for( unsigned int t = 0; t < m_ttransmissivityFactoryList.size(); t++ )
		if( m_ttransmissivityFactoryList[t]->TTransmissivityName() == factoryName )
			return ( m_ttransmissivityFactoryList[t]->CreateTTransmissivity() );

	return nullptr;
}

/*!
 * Returns available photon map export type plugins factory name list.
 */
std::vector< std::string > PluginManager::GetPhotonMapExportTypeFactoryNames() const
{
	std::vector< std::string > photonMapExportTypeFactoryName;
	for( unsigned int p = 0; p < m_photonMapExportTypeFactoryList.size(); p++ )
		photonMapExportTypeFactoryName.push_back( m_photonMapExportTypeFactoryList[p]->PhotonMapExportTypName() );
	return ( photonMapExportTypeFactoryName );
}

/*!
 * Returns available random deviates plugins factory name list.
 */
std::vector< std::string >  PluginManager::GetRandomDeviateFactoryNames() const
{
	std::vector< std::string > randomDeviateFactoryName;
	for( unsigned int r = 0; r < m_randomDeviateFactoryList.size(); r++ )
		randomDeviateFactoryName.push_back( m_randomDeviateFactoryList[r]->RandomDeviateName() );
	return ( randomDeviateFactoryName );

}

/*!
 * Returns available material plugin name list.
 */
std::vector< std::string > PluginManager::GetTMaterialFactoryNames() const
{
	std::vector< std::string > materialFactoryName;
	for( unsigned int p = 0; p < m_tmaterialFactoryList.size(); p++ )
		materialFactoryName.push_back( m_tmaterialFactoryList[p]->TMaterialName() );
	return ( materialFactoryName );
}

/*!
 * Returns available shape plugins factory list.
 */
std::vector< std::string > PluginManager::GetTShapeFactoryNames() const
{
	std::vector< std::string > shapeFactoryName;
	for( unsigned int s = 0; s < m_tshapeFactoryList.size(); s++ )
		shapeFactoryName.push_back( m_tshapeFactoryList[s]->TShapeName() );
	return ( shapeFactoryName );
}

/*!
 * Returns available sunshape plugins factory list.
 */
std::vector< std::string > PluginManager::GetTSunshapeFactoryNames() const
{
	std::vector< std::string > sunshapeFactoryName;
	for( unsigned int p = 0; p < m_tsunshapeFactoryList.size(); p++ )
		sunshapeFactoryName.push_back( m_tsunshapeFactoryList[p]->TSunshapeName() );
	return ( sunshapeFactoryName );
}

/*!
 * Returns available tracker plugins factory list.
 */
std::vector< std::string > PluginManager::GetTTrackerFactoryNames() const
{
	std::vector< std::string > trackerFactoryName;
	for( unsigned int t = 0; t < m_ttrackerFactoryList.size(); t++ )
		trackerFactoryName.push_back( m_ttrackerFactoryList[t]->TTrackerName() );
	return ( trackerFactoryName );
}

/*!
 * Returns available transmissivity plugins factory list.
 */
std::vector< std::string > PluginManager::GetTTransmissivityFactoryNames() const
{
	std::vector< std::string > transmissivityFactoryName;
	for( unsigned int t = 0; t < m_ttransmissivityFactoryList.size(); t++ )
		transmissivityFactoryName.push_back( m_ttransmissivityFactoryList[t]->TTransmissivityName() );
	return ( transmissivityFactoryName );
}

/*!
 * Loads all the valid plugins from "plugins" subdirecotry of the directory in
 * which the running version of Tonatiuh is located.
 */
void PluginManager::LoadAvailablePlugins( QDir pluginsDirectory, QString* error )
{
	Trace{ "PluginManager::LoadAvailablePlugins", false };
	QStringList filesList;
	BuildFileList( pluginsDirectory, filesList );
	foreach( QString fileName, filesList ) LoadTonatiuhPlugin( fileName, error );
}

/*!
 * Appends to \a fileList directory files.
 */
void PluginManager::AddFilesToList( QDir directory, QStringList& filesList )
{
	Trace{ "PluginManager::AddFilesToList", false };
	QString directoryPath( directory.absolutePath().append( QLatin1String( "/" ) ) );

    QStringList filenamesList = directory.entryList( QDir::Files, QDir::Unsorted );
    for( int i = 0; i < filenamesList.size(); ++i )
    	filesList << ( directoryPath + filenamesList[i] );
}


/*!
 * Creates a list with the files al files in the defined \a directory
 * and its subdirectories.
 */
void PluginManager::BuildFileList( QDir directory, QStringList& filesList )
{
	Trace{ "PluginManager::BuildFileList", false };
	AddFilesToList( directory, filesList );

	QString directoryPath( directory.absolutePath().append( QLatin1String( "/" ) ) );
    QStringList subdirectoriesList = directory.entryList( QDir::Dirs, QDir::Unsorted );

   for( int i = 0; i< subdirectoriesList.size(); ++i )
   {
    	QString subdirectoryName = subdirectoriesList[i];
   		if( ValidDirectoryName( subdirectoryName ) )
   			BuildFileList( QDir( directoryPath + subdirectoryName ), filesList );
   	}
}


/*!
 * Loads the \a plugin as PhotonMapExportType type.
 */
void PluginManager::LoadPhotonMapExportTypePlugin( QObject* plugin, QString* error  )
{
	Trace{ "PluginManager::LoadPhotonMapExportTypePlugin", false };
	std::unique_ptr< PhotonMapExportTypeFactory > pPhotonMapExportTypeFactory{ qobject_cast<PhotonMapExportTypeFactory* >( plugin )};
	if( !pPhotonMapExportTypeFactory )
	{
		*error = QString("LoadPlugins: PhotonMapExportType plug-in not recognized" );
		return;
	}
	m_photonMapExportTypeFactoryList.push_back( std::move( pPhotonMapExportTypeFactory ) );
}

/*!
 * Loads the \a plugin as RandomDeviatePlugin type.
 */
void PluginManager::LoadRandomDeviatePlugin( QObject* plugin, QString* error  )
{
	Trace{ "PluginManager::LoadRandomDeviatePlugin", false };
	std::unique_ptr< RandomDeviateFactory > pRamdomDeviateFactory{ qobject_cast<RandomDeviateFactory* >( plugin )};
	if( !pRamdomDeviateFactory )
	{
		*error = QString("LoadPlugins: RandomDeviate plug-in not recognized" );
		return;
	}
	m_randomDeviateFactoryList.push_back( std::move( pRamdomDeviateFactory ) );
}

/*!
 * Loads the \a plugin as material type.
 */
void PluginManager::LoadTMaterialPlugin( QObject* plugin, QString* error  )
{
	Trace{ "PluginManager::LoadMaterialPlugin", false };

	std::unique_ptr< TMaterialFactory > pTMaterialFactory{ qobject_cast<TMaterialFactory* >( plugin )};

	if( !pTMaterialFactory )
	{
		*error = QString("LoadPlugins: Material plug-in not recognized" );
		return;//gf::SevereError( "LoadPlugins: Material plug-in not recognized" );
	}
	pTMaterialFactory->Init();
	m_tmaterialFactoryList.push_back( std::move( pTMaterialFactory ) );
}

/*!
 * Loads the plugin is defined in \a fileName, if it is a valid plugin.
 */
void PluginManager::LoadTonatiuhPlugin( const QString& fileName, QString* error )
{
	Trace{ "PluginManager::LoadTonatiuhPlugin " + fileName.toStdString(), false };

 	QPluginLoader loader( fileName );
 	QObject* plugin = loader.instance();
    if ( plugin != 0)
    {
    	if( plugin->inherits( "PhotonMapExportTypeFactory" ) ) LoadPhotonMapExportTypePlugin( plugin, error  );
    	if( plugin->inherits( "RandomDeviateFactory" ) ) LoadRandomDeviatePlugin( plugin, error  );
    	if( plugin->inherits( "TMaterialFactory" ) ) LoadTMaterialPlugin( plugin, error  );
    	if( plugin->inherits( "TShapeFactory" ) ) LoadTShapePlugin( plugin, error );
    	if( plugin->inherits( "TSunshapeFactory" ) ) LoadTSunshapePlugin( plugin, error );
    	if( plugin->inherits( "TTrackerFactory" ) ) LoadTTrackerPlugin( plugin, error );
    	if( plugin->inherits( "TTransmissivityFactory" ) ) LoadTTransmissivityPlugin( plugin, error );
	}
}

/*!
 * Loads the \a plugin as shape type.
 */
void PluginManager::LoadTShapePlugin( QObject* plugin, QString* error )
{
	Trace{ "PluginManager::LoadShapePlugin", false };
	std::unique_ptr< TShapeFactory > pTShapeFactory{ qobject_cast<TShapeFactory* >( plugin )};
	if ( !pTShapeFactory )
	{
		*error = QString("LoadPlugins: Shape plug-in not recognized" );
		return;//gf::SevereError( "LoadPlugins: Shape plug-in not recognized" );
	}
	pTShapeFactory->Init();
	m_tshapeFactoryList.push_back( std::move( pTShapeFactory ) );
}

/*!
 * Loads the \a plugin as shape type.
 */
void PluginManager::LoadTSunshapePlugin( QObject* plugin, QString* error )
{
	Trace{ "PluginManager::LoadSunshapePlugin", false };
	std::unique_ptr< TSunshapeFactory > pTSunshapeFactory{ qobject_cast<TSunshapeFactory* >( plugin )};

	if ( !pTSunshapeFactory )
	{
		*error = QString("LoadPlugins: Sunshape plug-in not recognized" );
		return;
	}
	pTSunshapeFactory->Init();
	m_tsunshapeFactoryList.push_back( std::move( pTSunshapeFactory ) );
}


/*!
 * Loads the \a plugin as tracker type.
 */
void PluginManager::LoadTTrackerPlugin( QObject* plugin, QString* error )
{
	Trace{ "PluginManager::LoadTrackerPlugin", false };
	std::unique_ptr< TTrackerFactory > pTTrackerFactory{ qobject_cast<TTrackerFactory* >( plugin )};
	if ( !pTTrackerFactory )
	{
		*error = QString("LoadPlugins: Tracker plug-in not recognized" );
		return;
	}
	pTTrackerFactory->Init();
	m_ttrackerFactoryList.push_back( std::move( pTTrackerFactory ) );
}

/*!
 * Loads the \a plugin as transmissivity type.
 */
void PluginManager::LoadTTransmissivityPlugin( QObject* plugin, QString* error )
{
	Trace{ "PluginManager::LoadTransmissivityPlugin", false };
	std::unique_ptr< TTransmissivityFactory > pTTransmissivityFactory{ qobject_cast<TTransmissivityFactory* >( plugin )};
	if ( !pTTransmissivityFactory )
	{
		*error = QString("LoadPlugins: Transmissivity plug-in not recognized" );
		return;
	}
	pTTransmissivityFactory->Init();
	m_ttransmissivityFactoryList.push_back( std::move( pTTransmissivityFactory ) );
}

/*!
 * Checks if the \a directoryName is a valid directory name.
 * '.' and '..' are not valid names.
 */
bool PluginManager::ValidDirectoryName( QString& directoryName  )
{
	return ( directoryName != QLatin1String( "." ) ) && ( directoryName != QLatin1String( ".." ) );
}
