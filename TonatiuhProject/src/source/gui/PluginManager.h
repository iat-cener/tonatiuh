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
***************************************************************************/


#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <QVector>

class PhotonMapExportFactory;
class QDir;
class QStringList;
class RandomDeviateFactory;
class TComponentFactory;
class TMaterialFactory;
class TShapeFactory;
class TSunShapeFactory;
class TTrackerFactory;
class TTransmissivityFactory;

class PluginManager
{

public:
    PluginManager();
    ~PluginManager();

    QVector< TComponentFactory* > GetComponentFactories() const;
    QVector< PhotonMapExportFactory* > GetExportPMModeFactories() const;
    QVector< TMaterialFactory* > GetMaterialFactories() const;
    QVector< RandomDeviateFactory* > GetRandomDeviateFactories() const;
    QVector< TShapeFactory* > GetShapeFactories() const;
    QVector< TSunShapeFactory* > GetSunShapeFactories() const;
    QVector< TTrackerFactory* > GetTrackerFactories() const;
    QVector< TTransmissivityFactory* > GetTransmissivityFactories() const;
	void LoadAvailablePlugins( QDir pluginsDirectory );

private:
	void AddFilesToList( QDir directory, QStringList& filesList );
	void BuildFileList( QDir directory, QStringList& filesList );
	void LoadAvailablePlugins( );
	void LoadComponentPlugin( QObject* plugin );
	void LoadExportPhotonMapModePlugin( QObject* plugin );
	void LoadMaterialPlugin( QObject* plugin );
	void LoadRandomDeviatePlugin( QObject* plugin );
	void LoadShapePlugin( QObject* plugin );
	void LoadSunshapePlugin( QObject* plugin );
	void LoadTrackerPlugin( QObject* plugin );
	void LoadTransmissivityPlugin( QObject* plugin );
	void LoadTonatiuhPlugin( const QString& fileName );
	bool ValidDirectoryName( QString& directoryName  );


	QVector< TComponentFactory* > m_componentFactoryList;
	QVector< PhotonMapExportFactory* > m_exportPMModeFactoryList;
	QVector< TMaterialFactory* > m_materialFactoryList;
	QVector< RandomDeviateFactory* > m_randomDeviateFactoryList;
	QVector< TShapeFactory* > m_shapeFactoryList;
	QVector< TSunShapeFactory* > m_sunshapeFactoryList;
	QVector< TTrackerFactory* > m_trackerFactoryList;
	QVector< TTransmissivityFactory* > m_transmissivityFactoryList;
};

#endif /* PLUGINMANAGER_H_ */
