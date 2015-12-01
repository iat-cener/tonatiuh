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
***************************************************************************/


#ifndef CONTAINEREDITOR_H_
#define CONTAINEREDITOR_H_

#include <QDialog>
#include <QDoubleValidator>
#include <QItemDelegate>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QWidget>

#include "FieldEditor.h"

#include "ui_containerviewer.h"

/***************************************************************
 * CustomPlainTextEdit
 ***************************************************************/
class CustomPlainTextEdit: public QLineEdit //QPlainTextEdit
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

class ContainerEditor: public FieldEditor
{
	Q_OBJECT

public:
	ContainerEditor( QWidget* parent = 0 );
	~ContainerEditor();


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
class ContainerViewer: public QDialog, private Ui::ContainerViewer
{
	Q_OBJECT

public:
	ContainerViewer( QWidget* parent = 0 );
	~ContainerViewer();

public:
	QString GetData( ) const;
	bool SetData( QString value );

	void SetTitles( QStringList titles );
public slots:
	void AddNewRow();
	void CloseViewer();
	void DeleteSelectedRow();

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


#endif /* CONTAINEREDITOR_H_ */
