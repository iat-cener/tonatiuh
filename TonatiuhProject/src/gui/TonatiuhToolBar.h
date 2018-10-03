/*
 * InsertTrackerToolBar.h
 *
 *  Created on: 22/08/2018
 *      Author: oitoiz
 */

#ifndef TONATIUHTOOLBAR_H_
#define TONATIUHTOOLBAR_H_

#include <QObject>
#include <QString>




class TonatiuhToolBar : public QObject
{
    Q_OBJECT

public:
    explicit TonatiuhToolBar();
    Q_INVOKABLE void saveToolbarRef(QObject *toolbaObject, int toolbarSize);
    Q_INVOKABLE void fileToolBarUpdater(int tonatiuhWindowWidth);


private:
    QObject * m_fileToolbar;
    QObject * m_doUndoToolbar;
    QObject * m_editToolbar;
    QObject * m_viewToolbar;
    QObject * m_runToolbar;
    QObject * m_insertElementToolbar;
    QObject * m_insertTrackerToolbar;
    QObject * m_insertMaterialToolbar;
    QObject * m_insertShapeToolbar;

    int m_fileToolbarSize;
    int m_doUndoToolbarSize;
    int m_editToolbarSize;
    int m_viewToolbarSize;
    int m_runToolbarSize;
    int m_insertElementToolbarSize;
    int m_insertTrackerToolbarSize;
    int m_insertMaterialToolbarSize;
    int m_insertShapeToolbarSize;


};

#endif /* TONATIUHTOOLBAR_H_ */
