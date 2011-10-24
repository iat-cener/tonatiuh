/*
 * TransmissivityDialog.h
 *
 *  Created on: 24/10/2011
 *      Author: amutuberria
 */

#ifndef TRANSMISSIVITYDIALOG_H_
#define TRANSMISSIVITYDIALOG_H_

#include <QDialog>

class TransmissivityDialog : public QDialog, private Ui::TransmissivityDialog
{
	Q_OBJECT

public:
	TransmissivityDialog( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~TransmissivityDialog();
};

#endif /* TRANSMISSIVITYDIALOG_H_ */
