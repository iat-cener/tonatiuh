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

#ifndef FIELDCONTAINERWIDGET_H_
#define FIELDCONTAINERWIDGET_H_

#include <Inventor/lists/SbStringList.h>

#include <QModelIndex>
#include <QTreeView>

class ParametersDelegate;
class SoField;
class SoNode;
class ParametersModel;

class FieldContainerWidget : public QTreeView
{
    Q_OBJECT

public:
    FieldContainerWidget( QWidget* parent = 0 );
    FieldContainerWidget( SoNode* fieldContainer, QString containerName, QWidget* parent = 0 );
    ~FieldContainerWidget();

    void SetContainer( SoNode* fieldContainer, QString containerName );
    void SetEditable( bool editable );

protected slots:
	void currentChanged ( const QModelIndex & current, const QModelIndex & previous );
	void closeEditor ( QWidget * editor, QAbstractItemDelegate::EndEditHint hint );

signals:
	void valueModificated( SoNode* nodeContainer, QString parameterName, QString newValue );

private:
    void ReadFields( );

   	QString m_containerName;
   	QModelIndex m_currentIndex;
    ParametersDelegate* m_pDelegate;
    SoNode* m_pFieldContainer;
   	ParametersModel* m_pModel;

};

#endif /*SOFIELDCONTAINERWIDGET_H_*/
