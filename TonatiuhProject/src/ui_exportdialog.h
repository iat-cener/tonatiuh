/********************************************************************************
** Form generated from reading ui file 'exportdialog.ui'
**
** Created: Fri Jan 15 12:17:53 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EXPORTDIALOG_H
#define UI_EXPORTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExportDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *allMapRadio;
    QRadioButton *surfaceMapRadio;
    QWidget *surfaceWidget;
    QHBoxLayout *horizontalLayout;
    QTreeView *modelView;
    QWidget *coordinatesWidget;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *coordinatesBox;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *globalCoordinatesRadio;
    QRadioButton *localCoordinates;
    QSpacerItem *verticalSpacer;
    QWidget *fileWidget;
    QGridLayout *gridLayout;
    QLabel *fileNameLabel;
    QLineEdit *fileNameEdit;
    QPushButton *openButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ExportDialog)
    {
        if (ExportDialog->objectName().isEmpty())
            ExportDialog->setObjectName(QString::fromUtf8("ExportDialog"));
        ExportDialog->resize(510, 540);
        verticalLayout_2 = new QVBoxLayout(ExportDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(ExportDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        allMapRadio = new QRadioButton(groupBox);
        allMapRadio->setObjectName(QString::fromUtf8("allMapRadio"));
        allMapRadio->setMinimumSize(QSize(0, 25));
        allMapRadio->setChecked(true);

        verticalLayout->addWidget(allMapRadio);

        surfaceMapRadio = new QRadioButton(groupBox);
        surfaceMapRadio->setObjectName(QString::fromUtf8("surfaceMapRadio"));
        surfaceMapRadio->setMinimumSize(QSize(0, 25));

        verticalLayout->addWidget(surfaceMapRadio);

        surfaceWidget = new QWidget(groupBox);
        surfaceWidget->setObjectName(QString::fromUtf8("surfaceWidget"));
        surfaceWidget->setEnabled(false);
        horizontalLayout = new QHBoxLayout(surfaceWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        modelView = new QTreeView(surfaceWidget);
        modelView->setObjectName(QString::fromUtf8("modelView"));

        horizontalLayout->addWidget(modelView);

        coordinatesWidget = new QWidget(surfaceWidget);
        coordinatesWidget->setObjectName(QString::fromUtf8("coordinatesWidget"));
        verticalLayout_3 = new QVBoxLayout(coordinatesWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        coordinatesBox = new QGroupBox(coordinatesWidget);
        coordinatesBox->setObjectName(QString::fromUtf8("coordinatesBox"));
        verticalLayout_4 = new QVBoxLayout(coordinatesBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        globalCoordinatesRadio = new QRadioButton(coordinatesBox);
        globalCoordinatesRadio->setObjectName(QString::fromUtf8("globalCoordinatesRadio"));
        globalCoordinatesRadio->setChecked(true);

        verticalLayout_4->addWidget(globalCoordinatesRadio);

        localCoordinates = new QRadioButton(coordinatesBox);
        localCoordinates->setObjectName(QString::fromUtf8("localCoordinates"));

        verticalLayout_4->addWidget(localCoordinates);


        verticalLayout_3->addWidget(coordinatesBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout->addWidget(coordinatesWidget);


        verticalLayout->addWidget(surfaceWidget);


        verticalLayout_2->addWidget(groupBox);

        fileWidget = new QWidget(ExportDialog);
        fileWidget->setObjectName(QString::fromUtf8("fileWidget"));
        gridLayout = new QGridLayout(fileWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        fileNameLabel = new QLabel(fileWidget);
        fileNameLabel->setObjectName(QString::fromUtf8("fileNameLabel"));
        fileNameLabel->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(fileNameLabel, 0, 0, 1, 1);

        fileNameEdit = new QLineEdit(fileWidget);
        fileNameEdit->setObjectName(QString::fromUtf8("fileNameEdit"));
        fileNameEdit->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(fileNameEdit, 0, 1, 1, 1);

        openButton = new QPushButton(fileWidget);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setMinimumSize(QSize(0, 25));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        openButton->setIcon(icon);

        gridLayout->addWidget(openButton, 0, 2, 1, 1);


        verticalLayout_2->addWidget(fileWidget);

        buttonBox = new QDialogButtonBox(ExportDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(ExportDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ExportDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ExportDialog, SLOT(reject()));
        QObject::connect(surfaceMapRadio, SIGNAL(toggled(bool)), surfaceWidget, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(ExportDialog);
    } // setupUi

    void retranslateUi(QDialog *ExportDialog)
    {
        ExportDialog->setWindowTitle(QApplication::translate("ExportDialog", "Export Photon Map", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ExportDialog", "Select photons", 0, QApplication::UnicodeUTF8));
        allMapRadio->setText(QApplication::translate("ExportDialog", "All photon map", 0, QApplication::UnicodeUTF8));
        surfaceMapRadio->setText(QApplication::translate("ExportDialog", "Surface photon map", 0, QApplication::UnicodeUTF8));
        coordinatesBox->setTitle(QApplication::translate("ExportDialog", "Coordinates System", 0, QApplication::UnicodeUTF8));
        globalCoordinatesRadio->setText(QApplication::translate("ExportDialog", "Global Coordiantes", 0, QApplication::UnicodeUTF8));
        localCoordinates->setText(QApplication::translate("ExportDialog", "Local Coordinates", 0, QApplication::UnicodeUTF8));
        fileNameLabel->setText(QApplication::translate("ExportDialog", "File Name:", 0, QApplication::UnicodeUTF8));
        openButton->setText(QString());
        Q_UNUSED(ExportDialog);
    } // retranslateUi

};

namespace Ui {
    class ExportDialog: public Ui_ExportDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTDIALOG_H
