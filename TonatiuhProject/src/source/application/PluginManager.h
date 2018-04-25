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


class PhotonMapExportFactory;
class QDir;
class QStringList;
class RandomDeviate;
class RandomDeviateFactory;
class TMaterial;
class TMaterialFactory;
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


    std::shared_ptr< TMaterial > CreateTMaterial( std::string factoryName ) const;
    std::shared_ptr< RandomDeviate > CreateRandomDeviate( std::string factoryName ) const;
    std::shared_ptr< TShape > CreateTShape( std::string factoryName ) const;
    std::shared_ptr< TSunshape > CreateTSunshape( std::string factoryName ) const;
    std::shared_ptr< TTracker > CreateTTracker( std::string factoryName ) const;
    std::shared_ptr< TTransmissivity > CreateTTransmissivity( std::string factoryName ) const;

    std::vector< std::string > GetMaterialFactoryNames() const;
    std::vector< std::string > GetRandomDeviateFactoryNames() const;
    std::vector< std::string > GetShapeFactoryNames() const;
    std::vector< std::string > GetSunshapeFactoryNames() const;
    std::vector< std::string > GetTrackerFactoryNames() const;
    std::vector< std::string > GetTransmissivityFactoryNames() const;
	void LoadAvailablePlugins( QDir pluginsDirectory, QString* error );

private:
	void AddFilesToList( QDir directory, QStringList& filesList );
	void BuildFileList( QDir directory, QStringList& filesList );
	void LoadAvailablePlugins( );
	void LoadMaterialPlugin( QObject* plugin, QString* error  );
	void LoadRandomDeviatePlugin( QObject* plugin, QString* error );
	void LoadShapePlugin( QObject* plugin, QString* error );
	void LoadSunshapePlugin( QObject* plugin, QString* error );
	void LoadTonatiuhPlugin( const QString& fileName, QString* error );
	void LoadTrackerPlugin( QObject* plugin, QString* error );
	void LoadTransmissivityPlugin( QObject* plugin, QString* error );
	bool ValidDirectoryName( QString& directoryName  );


	std::vector< std::unique_ptr< TMaterialFactory > > m_materialFactoryList;
	std::vector< std::unique_ptr< RandomDeviateFactory > > m_randomDeviateFactoryList;
	std::vector< std::unique_ptr< TShapeFactory > > m_shapeFactoryList;
	std::vector< std::unique_ptr< TSunshapeFactory > > m_sunshapeFactoryList;
	std::vector< std::unique_ptr< TTrackerFactory > > m_trackerFactoryList;
	std::vector< std::unique_ptr< TTransmissivityFactory > > m_transmissivityFactoryList;

};

#endif /* PLUGINMANAGER_H_ */
