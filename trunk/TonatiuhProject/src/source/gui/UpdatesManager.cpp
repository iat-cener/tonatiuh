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

Contributors: Javier Garcia-Barberena, Iï¿½aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>

#include <QApplication>
#include <QAuthenticator>
#include <QBuffer>
#include <QByteArray>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QHttp>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslError>
#include <QUrl>

#include "UpdatesManager.h"

UpdatesManager::UpdatesManager( QString currentVersion )
:QObject(),
 m_networkAccessManager( new QNetworkAccessManager() ),
 m_reply( 0 ),
 m_httpRequestAborted( false ),
 m_osType( QLatin1String( "" ) ),
 m_currentVersion( currentVersion ),
 m_latestVersion( QLatin1String( "" ) ),
 m_filelist( 0 ),
 m_fileReply( 0 ),
 m_fileRequestAborted( false ),
 m_file( 0 ),
 m_filedownloaded( 0 )
{
	DeleteOldFiles();

#if defined( Q_WS_MAC )
	m_osType = QLatin1String( "mac" );
#endif

#if defined( Q_WS_WIN )
	m_osType = QLatin1String( "win" );
#endif

#if defined( Q_WS_X11 )
	m_osType = QLatin1String( "linux" );
#endif


}

/*!
 * Destroyes manager object.
 */
UpdatesManager::~UpdatesManager()
{
	delete m_networkAccessManager;
}

/*!
 * Checks for new Tonatiuh updates.
 */
void UpdatesManager::CheckForUpdates()
{
	//Esta dirección no es válida, solo para probar
	QUrl url( QLatin1String( "http://tonatiuh.googlecode.com/svn/updates/latest.xml" ) );

    // schedule the request
    m_httpRequestAborted = false;

    m_reply = m_networkAccessManager->get( QNetworkRequest( url ) );
    connect( m_reply, SIGNAL( finished() ), this, SLOT( CheckLastUpdate() ) );
    connect( m_reply, SIGNAL( readyRead() ), this, SLOT( Read() ) );

}

/*!
 * Reads latest update version and this version files.
 * If the Sthepanie installed version is oldder, checks if the user want to update to this version.
 */
void UpdatesManager::CheckLastUpdate()
{
	if( m_httpRequestAborted )
	{
		m_reply->deleteLater();
		return;
	}

	// Check file type
	QDomDocument doc( QLatin1String( "XML" ) );
	if( !doc.setContent( m_lastUpdateData, true ) )	return;


	QDomElement root = doc.documentElement();
	if( root.tagName() != QLatin1String( "appname" ) ) return;


	// Loop over main nodes
	m_latestVersion.clear();
	m_filelist.clear();

	QDomNode mainnode = root.firstChild();
	while( !mainnode.isNull() )
	{
		QDomNode subnode = mainnode.firstChild();
        if( !subnode.isNull() )
        {
            // Loop over each elements in subnodes
            while( !subnode.isNull() )
            {
                QDomElement e = subnode.toElement();
                if( !e.isNull() )
                {
                	if( e.tagName() == QLatin1String( "version" ) )
                		m_latestVersion = e.attribute( QLatin1String( "value" ), QLatin1String( "" ));

                	if( ( e.tagName() == QLatin1String( "os" )  ) && ( e.attribute( QLatin1String( "value" ), QLatin1String( "" )  ) == m_osType ) )
                	{
                		QDomNode ossubnode = subnode.firstChild();
                		while( !ossubnode.isNull())
                		{
                			QDomElement e = ossubnode.toElement();
                			if( e.tagName() == QLatin1String( "file" ) )
									m_filelist.push_back( e.attribute( QLatin1String( "value" ), QLatin1String( "" ) ) );

                			ossubnode = ossubnode.nextSibling();
                		}
                	}


				}
				subnode = subnode.nextSibling();
			}
		}
		mainnode = mainnode.nextSibling();
	}

	// Compare current version with the one on the server
	if( m_currentVersion < m_latestVersion )
	{
		QString text =  QString( tr( "New Version of Tonatiuh is available: %1\nWould you like to download and install it?" )  ).arg( m_latestVersion );


		// Check if user wants to download
		if( QMessageBox::question( 0, QLatin1String( "Tonatiuh" ),
				text, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes )
			== QMessageBox::No )	return;

		// Start Download
		m_filedownloaded = 0;
		UpdateDownload( 0 );
	}
	else{
		// Update success dialog
		QMessageBox::information( 0, QLatin1String("Tonatiuh"), tr("You are using the latest version." ) );
	}

	m_lastUpdateData = QLatin1String( "" );
}

/*!
 * Reads reply data to \a m_lastUpdateData.
 */
void UpdatesManager::Read()
{
	m_lastUpdateData.append( QString::fromAscii( m_reply->readAll() ) );
}


/*!
 * Reads reply data to \a m_file.
 */
void UpdatesManager::ReadFile()
{
	if( m_file ) m_file->write( m_fileReply->readAll() );
}

/*!
 * Finishes file download.
 */
void UpdatesManager::FileDownloadComplete()
{
	m_file->close();

	if( m_fileRequestAborted )
	{
		if( m_file )
		{
			m_file->close();
			m_file->remove();
			delete m_file;
			m_file = 0;
		}
		m_fileReply->deleteLater();
		return;
	}
	else
		m_filedownloaded++;

	m_fileReply->deleteLater();
	m_fileReply = 0;
	delete m_file;
	m_file = 0;

	UpdateDownload( m_filedownloaded );

}



void UpdatesManager::UpdateDownload( int fileIndex )
{

	QDir currentDir( qApp->applicationDirPath() );
	currentDir.cdUp();
	currentDir.cdUp();

	// Create a temporary folder
	if( !currentDir.exists( QLatin1String( "tmp" ) ) )	currentDir.mkdir( QLatin1String( "tmp" ) );
	QDir tmpDir( currentDir.absoluteFilePath( QLatin1String( "tmp" ) ) );

	if( fileIndex < m_filelist.size() )
	{
		QString urlPath = QString( QLatin1String( "http://tonatiuh.googlecode.com/svn/updates/release-%1/%2/%3" ) ).arg( m_latestVersion, m_osType, m_filelist[fileIndex] );
		QUrl url( urlPath );
		QString fileName = tmpDir.absoluteFilePath( m_filelist[fileIndex] );
		if( QFile::exists( fileName ) )
		{
			if( QMessageBox::question( 0, QLatin1String("Tonatiuh"), tr("There already exists a file called %1 in "
				"the current directory. Overwrite?").arg(fileName),
				QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel )
				== QMessageBox::Cancel )
				return;
			QFile::remove(fileName);
		}
		else
		{
			QFileInfo fileInfo( fileName );
			if( !fileInfo.dir().exists() )	currentDir.mkpath( fileInfo.dir().path() );

		}

		m_file = new QFile( fileName );

		if( !m_file->open( QIODevice::WriteOnly ) )
		{
			QMessageBox::information( 0, QLatin1String( "Tonatiuh" ),
				tr("Unable to save the file %1: %2.").arg(fileName, m_file->errorString() ) );
			delete m_file;
			m_file = 0;
			return;
		}

		m_fileRequestAborted = false ;
		m_fileReply = m_networkAccessManager->get( QNetworkRequest( url ) );
		connect( m_fileReply, SIGNAL( finished() ), this, SLOT( FileDownloadComplete() ) );
		connect( m_fileReply, SIGNAL( readyRead() ), this, SLOT( ReadFile() ) );

	}
	else if( m_filedownloaded == m_filelist.size() && m_filedownloaded != 0 )
	{

		// Backup old files and rename them
		for( int i = 0; i < m_filelist.size(); i++ )
		{
			QString currentFileName = currentDir.absoluteFilePath( m_filelist[i] );
			QString newFileName = tmpDir.absoluteFilePath( m_filelist[i] );
			QString oldFileName = currentFileName + QLatin1String( ".old" );

			QFile currentFile( currentFileName );
			currentFile.rename( oldFileName );

			QFile newFile( newFileName );
			newFile.rename( currentFileName );

		}
		//Removes tmp dir
		RemoveDir( tmpDir.absolutePath() );

		QMessageBox::information( 0,  QLatin1String( "Tonatiuh"), tr("Please restart Tonatiuh for updates to take effect." ) );
	}
 }


/*!
 * Removes \a dirName directory old files.
 */
void UpdatesManager::DeleteDirOldFiles( QString dirName )
{
	QDir currentDir( dirName );
	currentDir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
	currentDir.setSorting( QDir::DirsFirst  | QDir::Name );

	QStringList fileList = currentDir.entryList();
	for( int i = 0; i < fileList.size(); ++i )
	{
		QFileInfo fileOrDir( currentDir, fileList[i] );
		if( fileOrDir.isDir() )	DeleteDirOldFiles( currentDir.absoluteFilePath( fileList[i] ) );
		else if( ( !fileOrDir.isDir() ) && (  fileOrDir.suffix() == QLatin1String( "old" ) ) )
		{
			QFile oldFile( currentDir.absoluteFilePath( fileList[i] ) );
			oldFile.remove();
		}
	}

}

/*!
 * Removes application old files.
 */
void UpdatesManager::DeleteOldFiles()
{
	QDir appDir( qApp->applicationDirPath() );
	appDir.cdUp();
	appDir.cdUp();
	DeleteDirOldFiles( appDir.absolutePath() );
}

/*!
 * Removes the file hierarchy rooted by \a dirName.
 */
void UpdatesManager::RemoveDir( QString dirName )
{
	QDir currentDir( dirName );
	currentDir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
	currentDir.setSorting( QDir::DirsFirst  | QDir::Name );

	QStringList fileList = currentDir.entryList( );

	for( int i = 0; i < fileList.size(); ++i )
	{
		QFileInfo fileOrDir( currentDir, fileList[i] );
		if( fileOrDir.isDir() )
		{
			RemoveDir( fileOrDir.filePath() );
			currentDir.rmdir( fileList[i] );
		}
		else currentDir.remove( fileList[i] );
	}
	currentDir.cdUp();

	currentDir.rmdir( dirName );
}

