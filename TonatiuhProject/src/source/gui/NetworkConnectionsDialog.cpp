/***************************************************************************
 Copyright (C) 2008 by the Tonatiuh Software Development Team.

 This file is part of Tonatiuh.

 Tonatiuh program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.


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

 Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola, Gilda Jimenez,
 Juana Amieva, Azael Mancillas, Cesar Cantu.
 ***************************************************************************/

#include <QMessageBox>
#include <QProcessEnvironment>
#include <QUrl>

#include "NetworkConnectionsDialog.h"

/*!
 * Creates a dialog to define proxy settings.
 */
NetworkConnectionsDialog::NetworkConnectionsDialog( QWidget * parent, Qt::WindowFlags f )
:QDialog( parent, f)
{
	setupUi( this );
}

/*!
 * Destroys dialog.
 */
NetworkConnectionsDialog::~NetworkConnectionsDialog()
{

}

/*!
 * Returns true if the proxy must be defined to network connections.
 * Otherwise returns false.
 */
bool NetworkConnectionsDialog::IsProxyEnabled() const
{
	if( notProxyRadio->isChecked() )	return false;
	return true;
}

/*!
 * Returns true if the user defines to use system proxy settings.
 * Otherwise, returns false.
 */
bool NetworkConnectionsDialog::IsSystemProxyEnabled() const
{
	return systemProxyRadio->isChecked();
}

/*!
 * Returns the host name for the http proxy.
 * If the proxy is not enabled returns empty name.
 */
QString NetworkConnectionsDialog::GetHostName() const
{
	if( systemProxyRadio->isChecked() )
	{
	    QString urlEnv = QProcessEnvironment::systemEnvironment().value("http_proxy");
	     if (!urlEnv.isEmpty() )
	     {
	          QUrl url = QUrl(urlEnv, QUrl::TolerantMode);
	          return url.host();
	     }
	 	void SetProxyDisabled();
	 	void SetManualProxyConfiguration();
	 	void SetProxyHttpHost( QString name );
	 	void SetProxyHttpPort( int port );
	 	void SetSystemProxyConfiguration();
	}
	else if( manualProxyRadio->isChecked() )
	{
		return httpLine->text();
	}
	return QString();

}

/*!
 * Returns the port number for the http proxy.
 * Returns -1 if the port is unspecified.
 */
int NetworkConnectionsDialog::GetPort() const
{
	if( systemProxyRadio->isChecked() )
	{
		QString urlEnv = QProcessEnvironment::systemEnvironment().value("http_proxy");
		if (!urlEnv.isEmpty())
		{
			QUrl url = QUrl(urlEnv, QUrl::TolerantMode);
			return url.port();
		}
	}
	else if( manualProxyRadio->isChecked() )
	{
		return portSpin->value();
	}
	return -1;
}

/*!
 * Sets the proxy disabled for network connections.
 */
void NetworkConnectionsDialog::SetProxyDisabled()
{
	notProxyRadio->setChecked( true );
}

/*!
 * Sets the manual proxy configuration enabled for network connections.
 */
void NetworkConnectionsDialog::SetManualProxyConfiguration()
{

	manualProxyRadio->setChecked( true );
}

/*
 * Sets the host name for the proxy to \a name.
 */
void NetworkConnectionsDialog::SetProxyHttpHost( QString name )
{
	httpLine->setText( name );
}

/*!
 * Sets the port value for the proxy to \a port.
 */
void NetworkConnectionsDialog::SetProxyHttpPort( int port )
{
	portSpin->setValue( port );
}

/*!
 * Sets the system proxy configuration enabled for network connections.
 */
void NetworkConnectionsDialog::SetSystemProxyConfiguration()
{
	systemProxyRadio->setChecked( true );
}

/*!
 * Verifies that the defined configuration is a valid configuration closes the dialog.
 */
void NetworkConnectionsDialog::accept()
{
	if( manualProxyRadio->isChecked() )
	{
		QUrl httpHostURL( httpLine->text() );
		if( !httpHostURL.isValid() || portSpin->value() < 0 )
		{
			QString message( tr( "Defined proxy configuration is not valid." ) );
			QMessageBox::warning( this, QLatin1String( "Tonatiuh" ),
					message );
			return;
		}

	}
	QDialog::accept();
}
