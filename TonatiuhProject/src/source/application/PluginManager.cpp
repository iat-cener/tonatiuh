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
 * Returns an object of the material \a factoryName.
 */
std::shared_ptr< TMaterial > PluginManager::CreateTMaterial( std::string factoryName ) const
{
	for( unsigned int m = 0; m < m_materialFactoryList.size(); m++ )
		if( m_materialFactoryList[m]->TMaterialName() == factoryName )
			return ( m_materialFactoryList[m]->CreateTMaterial() );

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
 * Returns an object of the shape \a factoryName.
 */
std::shared_ptr< TShape > PluginManager::CreateTShape( std::string factoryName ) const
{
	for( unsigned int s = 0; s < m_shapeFactoryList.size(); s++ )
		if( m_shapeFactoryList[s]->TShapeName() == factoryName )
			return ( m_shapeFactoryList[s]->CreateTShape() );

	return nullptr;
}

/*!
 * Returns an object of the sunshape \a factoryName.
 */
std::shared_ptr< TSunshape > PluginManager::CreateTSunshape( std::string factoryName ) const
{
	for( unsigned int s = 0; s < m_sunshapeFactoryList.size(); s++ )
		if( m_sunshapeFactoryList[s]->TSunshapeName() == factoryName )
			return ( m_sunshapeFactoryList[s]->CreateTSunshape() );

	return nullptr;
}

/*!
 * Returns an object of the tracker \a factoryName.
 */
std::shared_ptr< TTracker > PluginManager::CreateTTracker( std::string factoryName ) const
{
	for( unsigned int t = 0; t < m_trackerFactoryList.size(); t++ )
		if( m_trackerFactoryList[t]->TTrackerName() == factoryName )
			return ( m_trackerFactoryList[t]->CreateTTracker() );

	return nullptr;
}


/*!
 * Returns an object of the transmissivity model \a factoryName.
 */
std::shared_ptr< TTransmissivity > PluginManager::CreateTTransmissivity( std::string factoryName ) const
{
	for( unsigned int t = 0; t < m_transmissivityFactoryList.size(); t++ )
		if( m_transmissivityFactoryList[t]->TTransmissivityName() == factoryName )
			return ( m_transmissivityFactoryList[t]->CreateTTransmissivity() );

	return nullptr;
}

/*!
 * Returns available material plugin  name list.
 */
std::vector< std::string > PluginManager::GetMaterialFactoryNames() const
{
	std::vector< std::string > materialFactoryName;
	for( unsigned int p = 0; p < m_materialFactoryList.size(); p++ )
		materialFactoryName.push_back( m_materialFactoryList[p]->TMaterialName() );
	return ( materialFactoryName );
}


/*!
 * Returns available random deviates plugins factory list.
 */
std::vector< std::string >  PluginManager::GetRandomDeviateFactoryNames() const
{
	std::vector< std::string > randomDeviateFactoryName;
	for( unsigned int r = 0; r < m_randomDeviateFactoryList.size(); r++ )
		randomDeviateFactoryName.push_back( m_randomDeviateFactoryList[r]->RandomDeviateName() );
	return ( randomDeviateFactoryName );

}

/*!
 * Returns available shape plugins factory list.
 */
std::vector< std::string > PluginManager::GetShapeFactoryNames() const
{
	std::vector< std::string > shapeFactoryName;
	for( unsigned int s = 0; s < m_shapeFactoryList.size(); s++ )
		shapeFactoryName.push_back( m_shapeFactoryList[s]->TShapeName() );
	return ( shapeFactoryName );
}

/*!
 * Returns available sunshape plugins factory list.
 */
std::vector< std::string > PluginManager::GetSunshapeFactoryNames() const
{
	std::vector< std::string > sunshapeFactoryName;
	for( unsigned int p = 0; p < m_sunshapeFactoryList.size(); p++ )
		sunshapeFactoryName.push_back( m_sunshapeFactoryList[p]->TSunshapeName() );
	return ( sunshapeFactoryName );
}

/*!
 * Returns available tracker plugins factory list.
 */
std::vector< std::string > PluginManager::GetTrackerFactoryNames() const
{
	std::vector< std::string > trackerFactoryName;
	for( unsigned int t = 0; t < m_trackerFactoryList.size(); t++ )
		trackerFactoryName.push_back( m_trackerFactoryList[t]->TTrackerName() );
	return ( trackerFactoryName );
}

/*!
 * Returns available transmissivity plugins factory list.
 */
std::vector< std::string > PluginManager::GetTransmissivityFactoryNames() const
{
	std::vector< std::string > transmissivityFactoryName;
	for( unsigned int t = 0; t < m_transmissivityFactoryList.size(); t++ )
		transmissivityFactoryName.push_back( m_transmissivityFactoryList[t]->TTransmissivityName() );
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
 * Loads the \a plugin as material type.
 */
void PluginManager::LoadMaterialPlugin( QObject* plugin, QString* error  )
{
	Trace{ "PluginManager::LoadMaterialPlugin", false };

	std::unique_ptr< TMaterialFactory > pTMaterialFactory{ qobject_cast<TMaterialFactory* >( plugin )};

	if( !pTMaterialFactory )
	{
		*error = QString("LoadPlugins: Material plug-in not recognized" );
		return;//gf::SevereError( "LoadPlugins: Material plug-in not recognized" );
	}
	pTMaterialFactory->Init();
	m_materialFactoryList.push_back( std::move( pTMaterialFactory ) );
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
 * Loads the \a plugin as shape type.
 */
void PluginManager::LoadShapePlugin( QObject* plugin, QString* error )
{
	Trace{ "PluginManager::LoadShapePlugin", false };
	std::unique_ptr< TShapeFactory > pTShapeFactory{ qobject_cast<TShapeFactory* >( plugin )};
	if ( !pTShapeFactory )
	{
		*error = QString("LoadPlugins: Shape plug-in not recognized" );
		return;//gf::SevereError( "LoadPlugins: Shape plug-in not recognized" );
	}
	pTShapeFactory->Init();
	m_shapeFactoryList.push_back( std::move( pTShapeFactory ) );
}

/*!
 * Loads the \a plugin as shape type.
 */
void PluginManager::LoadSunshapePlugin( QObject* plugin, QString* error )
{
	Trace{ "PluginManager::LoadSunshapePlugin", false };
	std::unique_ptr< TSunshapeFactory > pTSunshapeFactory{ qobject_cast<TSunshapeFactory* >( plugin )};

	if ( !pTSunshapeFactory )
	{
		*error = QString("LoadPlugins: Sunshape plug-in not recognized" );
		return;
	}
	pTSunshapeFactory->Init();
	m_sunshapeFactoryList.push_back( std::move( pTSunshapeFactory ) );
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
    	if( plugin->inherits( "TMaterialFactory" ) ) LoadMaterialPlugin( plugin, error  );
    	if( plugin->inherits( "RandomDeviateFactory" ) ) LoadRandomDeviatePlugin( plugin, error  );
    	if( plugin->inherits( "TShapeFactory" ) ) LoadShapePlugin( plugin, error );
    	if( plugin->inherits( "TSunshapeFactory" ) ) LoadSunshapePlugin( plugin, error );
    	if( plugin->inherits( "TTrackerFactory" ) ) LoadTrackerPlugin( plugin, error );
    	if( plugin->inherits( "TTransmissivityFactory" ) ) LoadTransmissivityPlugin( plugin, error );
	}
}

/*!
 * Loads the \a plugin as tracker type.
 */
void PluginManager::LoadTrackerPlugin( QObject* plugin, QString* error )
{
	Trace{ "PluginManager::LoadTrackerPlugin", false };
	std::unique_ptr< TTrackerFactory > pTTrackerFactory{ qobject_cast<TTrackerFactory* >( plugin )};
	if ( !pTTrackerFactory )
	{
		*error = QString("LoadPlugins: Tracker plug-in not recognized" );
		return;
	}
	pTTrackerFactory->Init();
	m_trackerFactoryList.push_back( std::move( pTTrackerFactory ) );
}

/*!
 * Loads the \a plugin as transmissivity type.
 */
void PluginManager::LoadTransmissivityPlugin( QObject* plugin, QString* error )
{
	Trace{ "PluginManager::LoadTransmissivityPlugin", false };
	std::unique_ptr< TTransmissivityFactory > pTTransmissivityFactory{ qobject_cast<TTransmissivityFactory* >( plugin )};
	if ( !pTTransmissivityFactory )
	{
		*error = QString("LoadPlugins: Transmissivity plug-in not recognized" );
		return;
	}
	pTTransmissivityFactory->Init();
	m_transmissivityFactoryList.push_back( std::move( pTTransmissivityFactory ) );
}

/*!
 * Checks if the \a directoryName is a valid directory name.
 * '.' and '..' are not valid names.
 */
bool PluginManager::ValidDirectoryName( QString& directoryName  )
{
	return ( directoryName != QLatin1String( "." ) ) && ( directoryName != QLatin1String( ".." ) );
}
