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

#ifndef PHOTONMAPEXPORTDB_H_
#define PHOTONMAPEXPORTDB_H_

#include <QMap>
#include <QString>

#include <sqlite3.h>

#include "PhotonMapExport.h"

class PhotonMapExportDB : public PhotonMapExport
{

public:
	PhotonMapExportDB();
	virtual ~PhotonMapExportDB();

	void EndExport();
	static QStringList GetParameterNames();
	void SavePhotonMap( std::vector< Photon* > raysLists );
	void SetPowerPerPhoton( double wPhoton );
	void SetSaveParameterValue( QString parameterName, QString parameterValue );
	bool StartExport();

private:
    bool Close();
    void InsertSurface( InstanceNode* instance );
	bool Open();
	void SaveAllData( std::vector< Photon* > raysLists );
	void SaveNotNextPrevID( std::vector< Photon* > raysLists );
	void SaveSelectedData( std::vector< Photon* > raysLists );
	void SetDBDirectory( QString path );
	void SetDBFileName( QString filename );
	void RemoveExistingFiles();

	QString m_dbFileName;
	QString m_dbDirectory;
	unsigned long m_exportedPhoton;
	bool m_isDBOpened;
	bool m_isWPhoton;
    sqlite3* m_pDB;
	QVector< InstanceNode* > m_surfaceIdentfier;
	QVector< Transform > m_surfaceWorldToObject;

};

#endif /* PHOTONMAPEXPORTDB_H_ */
