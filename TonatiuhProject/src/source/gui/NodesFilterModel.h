/*
 * NodesFilterModel.h
 *
 *  Created on: 09/10/2015
 *      Author: amutuberria
 */

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
