/********************************************************************************
** Form generated from reading UI file 'scripteditordialog.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCRIPTEDITORDIALOG_H
#define UI_SCRIPTEDITORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>
#include "CodeEditorWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ScriptEditorDialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *runButton;
    QPushButton *closeButton;
    QSpacerItem *horizontalSpacer;
    QSplitter *dialogVSplitter;
    QWidget *mainWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *mainSplitter;
    QTreeView *fileTree;
    CodeEditorWidget *codeEditorWidget;
    QPlainTextEdit *logWidget;

    void setupUi(QDialog *ScriptEditorDialog)
    {
        if (ScriptEditorDialog->objectName().isEmpty())
            ScriptEditorDialog->setObjectName(QString::fromUtf8("ScriptEditorDialog"));
        ScriptEditorDialog->resize(789, 520);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScriptEditorDialog->sizePolicy().hasHeightForWidth());
        ScriptEditorDialog->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/LogoTonatiuh.png"), QSize(), QIcon::Normal, QIcon::Off);
        ScriptEditorDialog->setWindowIcon(icon);
        gridLayout = new QGridLayout(ScriptEditorDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        runButton = new QPushButton(ScriptEditorDialog);
        runButton->setObjectName(QString::fromUtf8("runButton"));
        runButton->setMinimumSize(QSize(0, 25));
        runButton->setMaximumSize(QSize(16777215, 25));

        gridLayout->addWidget(runButton, 2, 1, 1, 1);

        closeButton = new QPushButton(ScriptEditorDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setMinimumSize(QSize(0, 25));
        closeButton->setMaximumSize(QSize(16777215, 25));

        gridLayout->addWidget(closeButton, 2, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 1, 1);

        dialogVSplitter = new QSplitter(ScriptEditorDialog);
        dialogVSplitter->setObjectName(QString::fromUtf8("dialogVSplitter"));
        dialogVSplitter->setOrientation(Qt::Vertical);
        mainWidget = new QWidget(dialogVSplitter);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        horizontalLayout = new QHBoxLayout(mainWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mainSplitter = new QSplitter(mainWidget);
        mainSplitter->setObjectName(QString::fromUtf8("mainSplitter"));
        mainSplitter->setOrientation(Qt::Horizontal);
        fileTree = new QTreeView(mainSplitter);
        fileTree->setObjectName(QString::fromUtf8("fileTree"));
        mainSplitter->addWidget(fileTree);
        codeEditorWidget = new CodeEditorWidget(mainSplitter);
        codeEditorWidget->setObjectName(QString::fromUtf8("codeEditorWidget"));
        mainSplitter->addWidget(codeEditorWidget);

        horizontalLayout->addWidget(mainSplitter);

        dialogVSplitter->addWidget(mainWidget);
        logWidget = new QPlainTextEdit(dialogVSplitter);
        logWidget->setObjectName(QString::fromUtf8("logWidget"));
        dialogVSplitter->addWidget(logWidget);

        gridLayout->addWidget(dialogVSplitter, 1, 0, 1, 3);


        retranslateUi(ScriptEditorDialog);

        QMetaObject::connectSlotsByName(ScriptEditorDialog);
    } // setupUi

    void retranslateUi(QDialog *ScriptEditorDialog)
    {
        ScriptEditorDialog->setWindowTitle(QApplication::translate("ScriptEditorDialog", "Script Editor", 0, QApplication::UnicodeUTF8));
        runButton->setText(QApplication::translate("ScriptEditorDialog", "Run", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("ScriptEditorDialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ScriptEditorDialog: public Ui_ScriptEditorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCRIPTEDITORDIALOG_H
