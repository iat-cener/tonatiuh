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
 
#ifndef NODENAMEDELEGATE_H_
#define NODENAMEDELEGATE_H_

#include <QItemDelegate>

class SceneModel;

//!  NodeNameDelegate class to facility editing the name of a node.
/*!
  NodeNameDelegate can be used to display and edit the name of a selected node. When a node is selected to edit this class shows an editor with a validator to enable only names with a valid regular expresion.
*/

class NodeNameDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	NodeNameDelegate( QObject* parent = 0 );
	~NodeNameDelegate();
	
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
                           
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

	
	
};
#endif /*NODENAMEDELEGATE_H_*/
