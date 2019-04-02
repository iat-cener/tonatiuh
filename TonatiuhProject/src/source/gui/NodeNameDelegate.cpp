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

#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndex>
#include <QObject>

#include <Inventor/SbName.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include "InstanceNode.h"
#include "NodeNameDelegate.h"
#include "SceneModel.h"

/**
 * Creates a new delegate to facilitate editing node names.
 */
NodeNameDelegate::NodeNameDelegate( QObject* parent)
 : QItemDelegate(parent)
{

}

/**
 * Destroys the NodeNameDelegate.
 */
NodeNameDelegate::~NodeNameDelegate()
{

}

/**
 * Returns a line edit editor with a validates the input names. The names must start with a alphabetic character and optionally followed with some
 * alphanumeric characters.
 */
QWidget* NodeNameDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
	QLineEdit * editor = new QLineEdit (parent);
	QRegExp rx("[a-zA-Z]\\S*");
	QValidator* validator = new QRegExpValidator( rx, 0 );
	editor->setValidator(validator);

	return editor;

}

/**
 * Takes the name of the selected node and sets to de editor to display it.
 */
void NodeNameDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
	const SceneModel* model = static_cast< const SceneModel* >( index.model() );

	QString value = model->data(index, Qt::DisplayRole).toString();

	QLineEdit  *textEdit = static_cast<QLineEdit *>(editor);

	SoNode* coinNode = model->NodeFromIndex( index )->GetNode();

	QString nodeName;
	if ( coinNode->getName() == SbName() )
 		nodeName = QString( coinNode->getTypeId().getName().getString() );
	else
		nodeName = QString( coinNode->getName().getString() );

	textEdit->setText( nodeName );

}
