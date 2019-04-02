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

#ifndef NODESFILTERMODEL_H_
#define NODESFILTERMODEL_H_

#include <QSortFilterProxyModel>
#include <QVector>


class NodesFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    NodesFilterModel(QObject *parent = 0);

    void AddShapeTypeFilter( QString shapeType );
	void SetShapeTypeFilters( QVector< QString >  shapeTypeFilters );

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;


private:
    QVector< QString > m_shapeTypeList;

};





#endif /* NODESFILTERMODEL_H_ */
