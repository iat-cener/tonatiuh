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

#ifndef NETWORKCONNECTIONSDIALOG_H_
#define NETWORKCONNECTIONSDIALOG_H_

#include <QDialog>

#include "ui_networkconnectionsdialog.h"

class NetworkConnectionsDialog: public QDialog, private Ui::NetworkConnectionsDialog
{
	Q_OBJECT

public:
	NetworkConnectionsDialog( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~NetworkConnectionsDialog();

	bool IsProxyEnabled() const;
	bool IsSystemProxyEnabled() const;
	QString GetHostName() const;
	int GetPort() const;

	void SetProxyDisabled();
	void SetManualProxyConfiguration();
	void SetProxyHttpHost( QString name );
	void SetProxyHttpPort( int port );
	void SetSystemProxyConfiguration();

public slots:
	void accept();

};

#endif /* NETWORKCONNECTIONSDIALOG_H_ */
