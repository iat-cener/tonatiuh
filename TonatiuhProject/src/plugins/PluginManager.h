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


#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <memory>
#include <vector>
#include <map>

class PhotonMapExportType;
class PhotonMapExportFactory;
class PhotonMapExportTypeFactory;
class PluginLoader;
class QDir;
class QStringList;
class RandomDeviate;
class RandomDeviateFactory;
class TMaterial;
class TMaterialFactory;
class TonatiuhPluginDetails;
class TShape;
class TShapeFactory;
class TSunshape;
class TSunshapeFactory;
class TTracker;
class TTrackerFactory;
class TTransmissivity;
class TTransmissivityFactory;

class PluginManager
{

public:
    PluginManager();
    ~PluginManager();

    std::unique_ptr< PhotonMapExportType > CreatePhotonMapExportType( std::string factoryName ) const;
    std::shared_ptr< RandomDeviate > CreateRandomDeviate( std::string factoryName ) const;
    std::shared_ptr< TMaterial > CreateTMaterial( std::string factoryName ) const;
    std::shared_ptr< TShape > CreateTShape( std::string factoryName ) const;
    std::shared_ptr< TSunshape > CreateTSunshape( std::string factoryName ) const;
    std::shared_ptr< TTracker > CreateTTracker( std::string factoryName ) const;
    std::shared_ptr< TTransmissivity > CreateTTransmissivity( std::string factoryName ) const;

    std::vector< std::string > GetPhotonMapExportTypeFactoryNames() const;
    std::vector< std::string > GetRandomDeviateFactoryNames() const;
    std::vector< std::string > GetTMaterialFactoryNames() const;
    std::vector< std::string > GetTShapeFactoryNames() const;
    std::vector< std::string > GetTSunshapeFactoryNames() const;
    std::vector< std::string > GetTTrackerFactoryNames() const;
    std::vector< std::string > GetTTransmissivityFactoryNames() const;
	void LoadAvailablePlugins( const std::string& pluginsDirectory, std::string* error );

private:
	void AddFilesToList( const std::string& directoryPath, std::vector< std::string >& filesList );
	void BuildFileList( const std::string& directoryPath, std::vector< std::string >& filesList );
	void LoadAvailablePlugins( );
	void LoadPhotonMapExportTypePlugin( std::unique_ptr< PluginLoader >& pluginLoader, std::string* error );
	void LoadRandomDeviatePlugin( std::unique_ptr< PluginLoader >& pluginLoader, std::string* error );
	void LoadTMaterialPlugin( std::unique_ptr< PluginLoader >& pluginLoader, std::string* error  );
	void LoadTonatiuhPlugin( const std::string& fileName, std::string* error );
	void LoadTShapePlugin( std::unique_ptr< PluginLoader >& pluginLoader, std::string* error );
	void LoadTSunshapePlugin( std::unique_ptr< PluginLoader >& pluginLoader, std::string* error );
	void LoadTTrackerPlugin( std::unique_ptr< PluginLoader >& pluginLoader, std::string* error );
	void LoadTTransmissivityPlugin( std::unique_ptr< PluginLoader >& pluginLoader, std::string* error );
	bool ValidDirectoryName( std::string& directoryName  );


	std::vector< std::unique_ptr< PhotonMapExportTypeFactory > > m_photonMapExportTypeFactoryList;
	std::vector< std::unique_ptr< RandomDeviateFactory > > m_randomDeviateFactoryList;
	std::vector< std::unique_ptr< TMaterialFactory > > m_tmaterialFactoryList;
	std::vector< std::unique_ptr< TShapeFactory > > m_tshapeFactoryList;
	std::vector< std::unique_ptr< TSunshapeFactory > > m_tsunshapeFactoryList;
	std::vector< std::unique_ptr< TTrackerFactory > > m_ttrackerFactoryList;
	std::vector< std::unique_ptr< TTransmissivityFactory > > m_ttransmissivityFactoryList;
	std::vector< std::unique_ptr< PluginLoader > > m_tonatiuhPluginLibraries;

};

#endif /* PLUGINMANAGER_H_ */
