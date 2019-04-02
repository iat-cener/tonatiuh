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
 
#ifndef SCENEMODELVIEW_H_
#define SCENEMODELVIEW_H_

#include <QTreeView>

#include "NodeNameDelegate.h"

class SceneModelView : public QTreeView
{
    Q_OBJECT

public:
    SceneModelView(QWidget *parent = 0);
    ~SceneModelView( );

signals:
	void dragAndDrop(const QModelIndex& newParent, const QModelIndex& item);
	void dragAndDropCopy(const QModelIndex& newParent, const QModelIndex& item);
	void nodeNameModificated( const QModelIndex& index, const QString& newName );
	void showMenu( const QModelIndex& index );
	
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

protected slots:
	void currentChanged ( const QModelIndex & current, const QModelIndex & previous );
	void closeEditor ( QWidget * editor, QAbstractItemDelegate::EndEditHint hint );
	void resizeViewToContents( const QModelIndex& index );

protected slots:

private:
    void startDrag(QMouseEvent *event);
    
    QModelIndex m_currentIndex;
    NodeNameDelegate* m_iteimsDelegate;
    QPoint startPos;
    
};

#endif /*SCENEMODELVIEW_H_*/
