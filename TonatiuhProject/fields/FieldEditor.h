/*
 * FieldEditor.h
 *
 *  Created on: 30/11/2015
 *      Author: amutuberria
 */

#ifndef FIELDEDITOR_H_
#define FIELDEDITOR_H_


#include <QWidget>

class FieldEditor: public QWidget
{
	Q_OBJECT

public:
	FieldEditor( QWidget* parent = 0 )
	:QWidget( parent )
	{

	}
	virtual QString GetData() const = 0;
	virtual void SetData( QString data) = 0;

};

#endif /* FIELDEDITOR_H_ */
