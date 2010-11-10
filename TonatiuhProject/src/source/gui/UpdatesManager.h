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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef UPDATESMANAGER_H_
#define UPDATESMANAGER_H_

#include <QBuffer>
#include <QObject>
#include <QVector>

class QAuthenticator;
class QFile;
class QHttp;
class QNetworkAccessManager;
class QNetworkReply;
class QSslError;
class QUrl;

//!  UpdateManager class manages Tonatiuh updates.
/*!
  UpdateManager checks if there is new Tonatiuh updates in the web. This class manages updates download and installation.
*/
class UpdatesManager: public QObject
{
	Q_OBJECT

public:
	UpdatesManager( QString currentVersion );
	~UpdatesManager();

	void CheckForUpdates();

public slots:
	void CheckLastUpdate( );
	void FileDownloadComplete();
	void Read();
	void ReadFile();
   void UpdateDownload( int fileIndex );

private:
	void DeleteDirOldFiles( QString dirName );
	void DeleteOldFiles();
	void RemoveDir( QString dirName );

    QNetworkAccessManager* m_networkAccessManager;
    QNetworkReply* m_reply;
    bool m_httpRequestAborted;

	QString m_osType;
	QString m_currentVersion;

    QString m_lastUpdateData;
	QString m_latestVersion;
	QVector< QString > m_filelist; // The list of files to be downloaded

    QNetworkReply* m_fileReply;
    bool m_fileRequestAborted;
	QFile* m_file;
	int m_filedownloaded;

};

#endif /* UPDATESMANAGER_H_ */
