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
:m_shapeFactoryList( 0 )
{

}

/*!
 * Destroyes PluginManager object.
 */
PluginManager::~PluginManager()
{

}

/*!
 * Returns available material plugins factory list.
 */
QVector< TMaterialFactory* > PluginManager::GetMaterialFactories() const
{
	return m_materialFactoryList;
}

/*!
 * Returns available random deviates plugins factory list.
 */
QVector< RandomDeviateFactory* > PluginManager::GetRandomDeviateFactories() const
{
	return	m_randomDeviateFactoryList;
}

/*!
 * Returns available shape plugins factory list.
 */
QVector< TShapeFactory* > PluginManager::GetShapeFactories() const
{
	return m_shapeFactoryList;
}

/*!
 * Returns available sunshape plugins factory list.
 */
QVector< TSunshapeFactory* > PluginManager::GetSunshapeFactories() const
{
	return ( m_sunshapeFactoryList );
}

/*!
 * Returns available tracker plugins factory list.
 */
QVector< TTrackerFactory* > PluginManager::GetTrackerFactories() const
{
	return ( m_trackerFactoryList );
}

/*!
 * Returns available transmissivity plugins factory list.
 */
QVector< TTransmissivityFactory* > PluginManager::GetTransmissivityFactories() const
{
	return ( m_transmissivityFactoryList );
}

/*!
 * Loads all the valid plugins from "plugins" subdirecotry of the directory in
 * which the running version of Tonatiuh is located.
 */
void PluginManager::LoadAvailablePlugins( QDir pluginsDirectory, QString* error )
{
	QStringList filesList;
	BuildFileList( pluginsDirectory, filesList );
	foreach( QString fileName, filesList ) LoadTonatiuhPlugin( fileName, error );
}

/*!
 * Appends to \a fileList directory files.
 */
void PluginManager::AddFilesToList( QDir directory, QStringList& filesList )
{
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

	TMaterialFactory* pTMaterialFactory = qobject_cast<TMaterialFactory* >( plugin );
	if( !pTMaterialFactory )
	{
		*error = QString("LoadPlugins: Material plug-in not recognized" );
		return;//gf::SevereError( "LoadPlugins: Material plug-in not recognized" );
	}
	pTMaterialFactory->Init();
	m_materialFactoryList.push_back( pTMaterialFactory );
}

/*!
 * Loads the \a plugin as RandomDeviatePlugin type.
 */
void PluginManager::LoadRandomDeviatePlugin( QObject* plugin, QString* error  )
{
	RandomDeviateFactory* pRamdomDeviateFactory = qobject_cast<RandomDeviateFactory* >( plugin );
	if( !pRamdomDeviateFactory )
	{
		*error = QString("LoadPlugins: RandomDeviate plug-in not recognized" );
		return;
	}
	m_randomDeviateFactoryList.push_back( pRamdomDeviateFactory );
}

/*!
 * Loads the \a plugin as shape type.
 */
void PluginManager::LoadShapePlugin( QObject* plugin, QString* error )
{
	TShapeFactory* pTShapeFactory = qobject_cast<TShapeFactory* >( plugin );
	if ( !pTShapeFactory )
	{
		*error = QString("LoadPlugins: Shape plug-in not recognized" );
		return;//gf::SevereError( "LoadPlugins: Shape plug-in not recognized" );
	}
	pTShapeFactory->Init();
	m_shapeFactoryList.push_back( pTShapeFactory );
}

/*!
 * Loads the \a plugin as shape type.
 */
void PluginManager::LoadSunshapePlugin( QObject* plugin, QString* error )
{
	TSunshapeFactory* pTSunshapeFactory = qobject_cast<TSunshapeFactory* >( plugin );
	if ( !pTSunshapeFactory )
	{
		*error = QString("LoadPlugins: Sunshape plug-in not recognized" );
		return;
	}
	pTSunshapeFactory->Init();
	m_sunshapeFactoryList.push_back( pTSunshapeFactory );
}


/*!
 * Loads the plugin is defined in \a fileName, if it is a valid plugin.
 */
void PluginManager::LoadTonatiuhPlugin( const QString& fileName, QString* error )
{
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
	TTrackerFactory* pTTrackerFactory = qobject_cast<TTrackerFactory* >( plugin );
	if ( !pTTrackerFactory )
	{
		*error = QString("LoadPlugins: Tracker plug-in not recognized" );
		return;
	}
	pTTrackerFactory->Init();
	m_trackerFactoryList.push_back( pTTrackerFactory );
}

/*!
 * Loads the \a plugin as transmissivity type.
 */
void PluginManager::LoadTransmissivityPlugin( QObject* plugin, QString* error )
{
	TTransmissivityFactory* pTTransmissivityFactory = qobject_cast< TTransmissivityFactory* >( plugin );
	if ( !pTTransmissivityFactory )
	{
		*error = QString("LoadPlugins: Transmissivity plug-in not recognized" );
		return;
	}
	pTTransmissivityFactory->Init();
	m_transmissivityFactoryList.push_back( pTTransmissivityFactory );
}

/*!
 * Checks if the \a directoryName is a valid directory name.
 * '.' and '..' are not valid names.
 */
bool PluginManager::ValidDirectoryName( QString& directoryName  )
{
	return ( directoryName != QLatin1String( "." ) ) && ( directoryName != QLatin1String( ".." ) );
}
