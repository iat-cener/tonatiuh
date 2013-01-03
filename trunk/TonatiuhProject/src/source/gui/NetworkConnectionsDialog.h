/*
 * NetworkConnectionsDialog.h
 *
 *  Created on: 02/01/2013
 *      Author: amutuberria
 */

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
