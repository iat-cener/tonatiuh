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
 
#ifndef PARAMETERSITEM_H_
#define PARAMETERSITEM_H_

#include <QStandardItem>
#include <QList>
#include <QString>

class SoField;
class SoFieldSensor;

class ParametersItem: public QStandardItem
{
	
public:

	ParametersItem( QString text, bool editable, SoField* field );
	~ParametersItem();
	SoField* GetField() const { return m_pField; };
	
	QVariant data ( int role = Qt::UserRole + 1 ) const;
	void setData ( const QVariant & value, int role = Qt::UserRole + 1 );
	QString text () const; 
	
private:
	SoField* m_pField;
	SoFieldSensor* m_pFieldSensor;
	QString m_text;
	

};


#endif /*PARAMETERSITEM_H_*/
