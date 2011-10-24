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

#include <QVector>

class QDir;
class QStringList;
class RandomDeviateFactory;
class TMaterialFactory;
class TPhotonMapFactory;
class TShapeFactory;
class TSunShapeFactory;
class TTrackerFactory;
class TTransmissivityFactory;

class PluginManager
{

public:
    PluginManager();
    ~PluginManager();

    QVector< RandomDeviateFactory* > GetRandomDeviateFactories() const;
    QVector< TMaterialFactory* > GetMaterialFactories() const;
    QVector< TPhotonMapFactory* > GetPhotonMapFactories() const;
    QVector< TShapeFactory* > GetShapeFactories() const;
    QVector< TSunShapeFactory* > GetSunShapeFactories() const;
    QVector< TTrackerFactory* > GetTrackerFactories() const;
    QVector< TTransmissivityFactory* > GetTransmissivityFactories() const;
	void LoadAvailablePlugins( QDir pluginsDirectory );

private:
	void AddFilesToList( QDir directory, QStringList& filesList );
	void BuildFileList( QDir directory, QStringList& filesList );
	void LoadAvailablePlugins( );
	void LoadMaterialPlugin( QObject* plugin );
	void LoadPhotonMapPlugin( QObject* plugin );
	void LoadRandomDeviatePlugin( QObject* plugin );
	void LoadShapePlugin( QObject* plugin );
	void LoadSunshapePlugin( QObject* plugin );
	void LoadTrackerPlugin( QObject* plugin );
	void LoadTransmissivityPlugin( QObject* plugin );
	void LoadTonatiuhPlugin( const QString& fileName );
	bool ValidDirectoryName( QString& directoryName  );


	QVector< RandomDeviateFactory* > m_RandomDeviateFactoryList;
	QVector< TMaterialFactory* > m_materialFactoryList;
	QVector< TPhotonMapFactory* > m_photonmapFactoryList;
	QVector< TShapeFactory* > m_shapeFactoryList;
	QVector< TSunShapeFactory* > m_sunshapeFactoryList;
	QVector< TTrackerFactory* > m_trackerFactoryList;
	QVector< TTransmissivityFactory* > m_transmissivityFactoryList;
};

#endif /* PLUGINMANAGER_H_ */
