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


#ifndef TRANSMISSIVITYDIALOG_H_
#define TRANSMISSIVITYDIALOG_H_

#include <QDialog>
#include <QMap>

#include "ui_transmissivitydialog.h"

class TTransmissivity;
class TTransmissivityFactory;

class TransmissivityDialog : public QDialog, private Ui::TransmissivityDialog
{
	Q_OBJECT

public:
	TransmissivityDialog( QVector< TTransmissivityFactory* > transmissivityFactoryList, QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~TransmissivityDialog();

	TTransmissivity* GetTransmissivity() const;
	void SetCurrentTransmissivity( TTransmissivity* transmissivity );

public slots:
	void SetValue( SoNode* node, QString paramenterName, QString newValue );

protected slots:
	void ChangeTransmissivityType( int index );

private:
	int m_currentTransmissivityIndex;
	TTransmissivity* m_currentTransmissivity;
	TTransmissivity* m_newTransmissivity;
	QMap< QString, TTransmissivityFactory* > m_transmissivityFactoryList;

};

#endif /* TRANSMISSIVITYDIALOG_H_ */
