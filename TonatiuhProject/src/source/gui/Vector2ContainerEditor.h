/*
 * Vector2ContainerEditor.h
 *
 *  Created on: 26/06/2015
 *      Author: amutuberria
 */

#ifndef VECTOR2CONTAINEREDITOR_H_
#define VECTOR2CONTAINEREDITOR_H_

#include <QWidget>

#include "ui_vector2containereditor.h"


class Vector2ContainerEditor: public QWidget, private Ui::Vector2ContainerEditor
{
	Q_OBJECT

public:
	Vector2ContainerEditor( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~Vector2ContainerEditor();

	QString GetText( ) const;
	void SetText( QString text );


public slots:
	void OpenEditor();

};


#endif /* VECTOR2CONTAINEREDITOR_H_ */
