/*
 * Vector2ContainerEditor.cpp
 *
 *  Created on: 26/06/2015
 *      Author: amutuberria
 */




#include "Vector2ContainerEditor.h"

Vector2ContainerEditor::Vector2ContainerEditor( QWidget* parent, Qt::WindowFlags f )
:QWidget( parent, f )
{
	setupUi ( this );
	connect( openEditor, SIGNAL( clicked( bool )), this, SLOT( OpenEditor()  ) );
}

Vector2ContainerEditor::~Vector2ContainerEditor()
{

}

QString Vector2ContainerEditor::GetText( ) const
{
	return( parameterEditLine->text());
}

void Vector2ContainerEditor::SetText( QString text )
{
	parameterEditLine->setText( text);
}

void Vector2ContainerEditor::OpenEditor()
{

}
