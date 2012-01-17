/********************************************************************************
** Form generated from reading UI file 'codeeditorwidget.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CODEEDITORWIDGET_H
#define UI_CODEEDITORWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "CodeEditArea.h"
#include "LineNumberWidget.h"

QT_BEGIN_NAMESPACE

class Ui_CodeEditorWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *editorToolbar;
    QHBoxLayout *horizontalLayout;
    QToolButton *newButton;
    QToolButton *openButton;
    QToolButton *saveButton;
    QToolButton *saveAsButton;
    QToolButton *undoButton;
    QToolButton *redoButton;
    QToolButton *cutButton;
    QToolButton *copyButton;
    QToolButton *pasteButton;
    QToolButton *runButton;
    QSpacerItem *horizontalSpacer;
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
    LineNumberWidget *lineNumberWidget;
    CodeEditArea *codeEditor;

    void setupUi(QWidget *CodeEditorWidget)
    {
        if (CodeEditorWidget->objectName().isEmpty())
            CodeEditorWidget->setObjectName(QString::fromUtf8("CodeEditorWidget"));
        CodeEditorWidget->resize(548, 469);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/LogoTonatiuh.png"), QSize(), QIcon::Normal, QIcon::Off);
        CodeEditorWidget->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(CodeEditorWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        editorToolbar = new QWidget(CodeEditorWidget);
        editorToolbar->setObjectName(QString::fromUtf8("editorToolbar"));
        horizontalLayout = new QHBoxLayout(editorToolbar);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        newButton = new QToolButton(editorToolbar);
        newButton->setObjectName(QString::fromUtf8("newButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        newButton->setIcon(icon1);
        newButton->setIconSize(QSize(30, 30));
        newButton->setAutoRaise(true);

        horizontalLayout->addWidget(newButton);

        openButton = new QToolButton(editorToolbar);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        openButton->setIcon(icon2);
        openButton->setIconSize(QSize(30, 30));
        openButton->setAutoRaise(true);

        horizontalLayout->addWidget(openButton);

        saveButton = new QToolButton(editorToolbar);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveButton->setIcon(icon3);
        saveButton->setIconSize(QSize(30, 30));
        saveButton->setAutoRaise(true);

        horizontalLayout->addWidget(saveButton);

        saveAsButton = new QToolButton(editorToolbar);
        saveAsButton->setObjectName(QString::fromUtf8("saveAsButton"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/saveas.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveAsButton->setIcon(icon4);
        saveAsButton->setIconSize(QSize(30, 30));
        saveAsButton->setAutoRaise(true);

        horizontalLayout->addWidget(saveAsButton);

        undoButton = new QToolButton(editorToolbar);
        undoButton->setObjectName(QString::fromUtf8("undoButton"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        undoButton->setIcon(icon5);
        undoButton->setIconSize(QSize(30, 30));
        undoButton->setAutoRaise(true);

        horizontalLayout->addWidget(undoButton);

        redoButton = new QToolButton(editorToolbar);
        redoButton->setObjectName(QString::fromUtf8("redoButton"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        redoButton->setIcon(icon6);
        redoButton->setIconSize(QSize(30, 30));
        redoButton->setAutoRaise(true);

        horizontalLayout->addWidget(redoButton);

        cutButton = new QToolButton(editorToolbar);
        cutButton->setObjectName(QString::fromUtf8("cutButton"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        cutButton->setIcon(icon7);
        cutButton->setIconSize(QSize(30, 30));
        cutButton->setAutoRaise(true);

        horizontalLayout->addWidget(cutButton);

        copyButton = new QToolButton(editorToolbar);
        copyButton->setObjectName(QString::fromUtf8("copyButton"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        copyButton->setIcon(icon8);
        copyButton->setIconSize(QSize(30, 30));
        copyButton->setAutoRaise(true);

        horizontalLayout->addWidget(copyButton);

        pasteButton = new QToolButton(editorToolbar);
        pasteButton->setObjectName(QString::fromUtf8("pasteButton"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        pasteButton->setIcon(icon9);
        pasteButton->setIconSize(QSize(30, 30));
        pasteButton->setAutoRaise(true);

        horizontalLayout->addWidget(pasteButton);

        runButton = new QToolButton(editorToolbar);
        runButton->setObjectName(QString::fromUtf8("runButton"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/run.png"), QSize(), QIcon::Normal, QIcon::Off);
        runButton->setIcon(icon10);
        runButton->setIconSize(QSize(30, 30));
        runButton->setAutoRaise(true);

        horizontalLayout->addWidget(runButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(editorToolbar);

        mainWidget = new QWidget(CodeEditorWidget);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        mainLayout = new QHBoxLayout(mainWidget);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        lineNumberWidget = new LineNumberWidget(mainWidget);
        lineNumberWidget->setObjectName(QString::fromUtf8("lineNumberWidget"));

        mainLayout->addWidget(lineNumberWidget);

        codeEditor = new CodeEditArea(mainWidget);
        codeEditor->setObjectName(QString::fromUtf8("codeEditor"));

        mainLayout->addWidget(codeEditor);


        verticalLayout->addWidget(mainWidget);


        retranslateUi(CodeEditorWidget);

        QMetaObject::connectSlotsByName(CodeEditorWidget);
    } // setupUi

    void retranslateUi(QWidget *CodeEditorWidget)
    {
        CodeEditorWidget->setWindowTitle(QApplication::translate("CodeEditorWidget", "Code Editor", 0, QApplication::UnicodeUTF8));
        newButton->setText(QApplication::translate("CodeEditorWidget", "New", 0, QApplication::UnicodeUTF8));
        openButton->setText(QApplication::translate("CodeEditorWidget", "Open", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("CodeEditorWidget", "Save", 0, QApplication::UnicodeUTF8));
        saveAsButton->setText(QApplication::translate("CodeEditorWidget", "SaveAs", 0, QApplication::UnicodeUTF8));
        undoButton->setText(QApplication::translate("CodeEditorWidget", "Undo", 0, QApplication::UnicodeUTF8));
        redoButton->setText(QApplication::translate("CodeEditorWidget", "Redo", 0, QApplication::UnicodeUTF8));
        cutButton->setText(QApplication::translate("CodeEditorWidget", "Cut", 0, QApplication::UnicodeUTF8));
        copyButton->setText(QApplication::translate("CodeEditorWidget", "Copy", 0, QApplication::UnicodeUTF8));
        pasteButton->setText(QApplication::translate("CodeEditorWidget", "Paste", 0, QApplication::UnicodeUTF8));
        runButton->setText(QApplication::translate("CodeEditorWidget", "Run", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CodeEditorWidget: public Ui_CodeEditorWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CODEEDITORWIDGET_H
