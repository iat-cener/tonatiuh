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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
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

#include "Trace.h"

/**
 * Creates a new delegate to facilitate editing node names.
 */
NodeNameDelegate::NodeNameDelegate( SceneModel* sceneModel,  QObject* parent)
 : QItemDelegate(parent), m_pModel( sceneModel )
{
	Trace trace( "NodeNameDelegate::NodeNameDelegate", false );
}

/**
 * Destroys the NodeNameDelegate.
 */
NodeNameDelegate::~NodeNameDelegate()
{
	Trace trace( "NodeNameDelegate::~NodeNameDelegate", false );

}

/**
 * Returns a line edit editor with a validates the input names. The names must start with a alphabetic character and optionally followed with some
 * alphanumeric characters.
 */
QWidget* NodeNameDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
	Trace trace( "NodeNameDelegate::createEditor", false );

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
	Trace trace( "NodeNameDelegate::setEditorData", false );
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QLineEdit  *textEdit = static_cast<QLineEdit *>(editor);

	SoNode* coinNode = m_pModel->NodeFromIndex( index )->GetNode();

	QString nodeName;
	if ( coinNode->getName() == SbName() )
 		nodeName = QString( coinNode->getTypeId().getName().getString() );
	else
		nodeName = QString( coinNode->getName().getString() );

	textEdit->setText( nodeName );

}

/**
 * Takes the editor data and set the name to node if the defined name is a valid name.
 */
void NodeNameDelegate::setModelData(QWidget* editor, QAbstractItemModel* /*model*/, const QModelIndex& index ) const
{
	Trace trace( "NodeNameDelegate::setModelData", false );

	QLineEdit* textEdit = static_cast<QLineEdit *>( editor );

	SoNode* coinNode =m_pModel->NodeFromIndex( index )->GetNode();

	bool changed = m_pModel->SetNodeName( *coinNode, textEdit->text() );
	if( !changed )
	{
		QMessageBox::critical( 0, "Tonatiuh Action", "Tonatiuh can not change the name to node. There is a node with the same name. Define other name" );

	}
}
