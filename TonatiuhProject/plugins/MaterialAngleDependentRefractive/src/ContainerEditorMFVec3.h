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


#ifndef CONTAINEREDITORMFVEC3_H_
#define CONTAINEREDITORMFVEC3_H_

#include <QDialog>
#include <QDoubleValidator>
#include <QItemDelegate>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QWidget>

#include "FieldEditor.h"

#include "ui_containerviewermfvec3.h"

/***************************************************************
 * CustomPlainTextEdit
 ***************************************************************/
class CustomPlainTextEdit: public QLineEdit
{
	Q_OBJECT
public:
	CustomPlainTextEdit( QWidget* parent = 0 );
	~CustomPlainTextEdit();

signals:
	void editingFinished( );

protected:
    void focusOutEvent(QFocusEvent* event);
};


/***************************************************************
 * ContainerEditor
 ***************************************************************/

class ContainerEditorMFVec3: public FieldEditor
{
	Q_OBJECT

public:
	ContainerEditorMFVec3( QWidget* parent = 0 );
	~ContainerEditorMFVec3();


	QString GetData( ) const;
	void SetData( QString value );

	void SetTitles( QStringList titles );
signals:
	void editingFinished( );


protected:
    void focusOutEvent(QFocusEvent* event);

protected slots:
		void CloseEditor();
    	void OpenContainerViewer();

private:
    QHBoxLayout* m_horizontalLayout;
    CustomPlainTextEdit* m_valuesEdit;
    QPushButton* m_editButton;
    QStringList m_titlesList;
};


/***************************************************************
 * ContainerViewer
 ***************************************************************/

class DoubleValuesDelegate;
class ContainerViewerMFVec3: public QDialog, private Ui::ContainerViewerMFVec3
{
	Q_OBJECT

public:
	ContainerViewerMFVec3( QWidget* parent = 0 );
	~ContainerViewerMFVec3();

public:
	QString GetData( ) const;
	bool SetData( QString value );

	void SetTitles( QStringList titles );
public slots:
	void AddNewRow();
	void CloseViewer();
	void DeleteSelectedRow();
	void HelpMenu();

protected:
	bool OkToContinue();

private:
	DoubleValuesDelegate* m_delegate;
};

/***************************************************************
 * DoubleValuesDelegate
 ***************************************************************/

class DoubleValuesDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    DoubleValuesDelegate(QObject *parent = 0);
    ~DoubleValuesDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

};


#endif /* CONTAINEREDITORMFVEC3_H_ */
