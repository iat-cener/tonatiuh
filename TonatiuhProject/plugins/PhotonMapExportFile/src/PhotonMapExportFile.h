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


#ifndef EXPORTPHOTONMAPFILE_H_
#define EXPORTPHOTONMAPFILE_H_

#include <QMap>
#include <QString>

#include "PhotonMapExport.h"

class Photon;

class PhotonMapExportFile : public PhotonMapExport
{

public:
	PhotonMapExportFile();
	virtual ~PhotonMapExportFile();

	static QStringList GetParameterNames();

	void EndExport();
	void SavePhotonMap( std::vector< Photon* > raysLists );
	void SetPowerPerPhoton( double wPhoton );
	void SetSaveParameterValue( QString parameterName, QString parameterValue );
	bool StartExport();

private:
	void ExportAllPhotonsAllData( QString filename, std::vector< Photon* > raysLists );
	void ExportAllPhotonsNotNextPrevID( QString filename, std::vector< Photon* > raysLists );
	void ExportAllPhotonsSelectedData( QString filename, std::vector< Photon* > raysLists );
	void ExportSelectedPhotonsAllData( QString filename, std::vector< Photon* > raysLists,
			unsigned long startIndex, 	unsigned long numberOfPhotons );
	void ExportSelectedPhotonsNotNextPrevID( QString filename, std::vector< Photon* > raysLists,
			unsigned long startIndex, 	unsigned long numberOfPhotons );
	void ExportSelectedPhotonsSelectedData( QString filename, std::vector <Photon* > raysLists,
			unsigned long startIndex, 	unsigned long numberOfPhotons );


    void RemoveExistingFiles();
    void SaveToVariousFiles( std::vector <Photon* > raysLists );
    void WriteFileFormat( QString exportFilename );


	QString m_photonsFilename;
	double m_powerPerPhoton;
	QVector< InstanceNode* > m_surfaceIdentfier;
	QVector< Transform > m_surfaceWorldToObject;
	int m_currentFile;
	QString m_exportDirecotryName;
	unsigned long m_exportedPhotons;
	unsigned long m_nPhotonsPerFile;
	bool m_oneFile;

};


#endif /* EXPORTPHOTONMAPFILE_H_ */
