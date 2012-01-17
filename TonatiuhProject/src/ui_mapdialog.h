/********************************************************************************
** Form generated from reading UI file 'mapdialog.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPDIALOG_H
#define UI_MAPDIALOG_H

#include <LegendWidget.h>
#include <MapViewWidget.h>
#include <MarbleWidget.h>
#include <NavigationWidget.h>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *mainWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *mainSplitter;
    QTabWidget *controlTab;
    QWidget *navigationTab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *homeGroup;
    QGridLayout *gridLayout_2;
    QLabel *latitudeLabel;
    QDoubleSpinBox *latitudeSpin;
    QComboBox *latitudeCombo;
    QLabel *longitudeLabel;
    QDoubleSpinBox *longitudeSpin;
    QComboBox *longitudeCombo;
    Marble::NavigationWidget *navigationWidget;
    Marble::MapViewWidget *mapViewTab;
    Marble::LegendWidget *legendTab;
    Marble::MarbleWidget *marbleWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *MapDialog)
    {
        if (MapDialog->objectName().isEmpty())
            MapDialog->setObjectName(QString::fromUtf8("MapDialog"));
        MapDialog->resize(721, 502);
        verticalLayout = new QVBoxLayout(MapDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mainWidget = new QWidget(MapDialog);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        horizontalLayout = new QHBoxLayout(mainWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mainSplitter = new QSplitter(mainWidget);
        mainSplitter->setObjectName(QString::fromUtf8("mainSplitter"));
        mainSplitter->setOrientation(Qt::Horizontal);
        mainSplitter->setHandleWidth(10);
        controlTab = new QTabWidget(mainSplitter);
        controlTab->setObjectName(QString::fromUtf8("controlTab"));
        navigationTab = new QWidget();
        navigationTab->setObjectName(QString::fromUtf8("navigationTab"));
        verticalLayout_2 = new QVBoxLayout(navigationTab);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setContentsMargins(10, 10, 10, 10);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        homeGroup = new QGroupBox(navigationTab);
        homeGroup->setObjectName(QString::fromUtf8("homeGroup"));
        gridLayout_2 = new QGridLayout(homeGroup);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        latitudeLabel = new QLabel(homeGroup);
        latitudeLabel->setObjectName(QString::fromUtf8("latitudeLabel"));

        gridLayout_2->addWidget(latitudeLabel, 0, 0, 1, 1);

        latitudeSpin = new QDoubleSpinBox(homeGroup);
        latitudeSpin->setObjectName(QString::fromUtf8("latitudeSpin"));
        latitudeSpin->setDecimals(4);
        latitudeSpin->setMaximum(90);

        gridLayout_2->addWidget(latitudeSpin, 1, 0, 1, 1);

        latitudeCombo = new QComboBox(homeGroup);
        latitudeCombo->setObjectName(QString::fromUtf8("latitudeCombo"));

        gridLayout_2->addWidget(latitudeCombo, 1, 1, 1, 1);

        longitudeLabel = new QLabel(homeGroup);
        longitudeLabel->setObjectName(QString::fromUtf8("longitudeLabel"));

        gridLayout_2->addWidget(longitudeLabel, 2, 0, 1, 1);

        longitudeSpin = new QDoubleSpinBox(homeGroup);
        longitudeSpin->setObjectName(QString::fromUtf8("longitudeSpin"));
        longitudeSpin->setDecimals(4);
        longitudeSpin->setMaximum(180);

        gridLayout_2->addWidget(longitudeSpin, 3, 0, 1, 1);

        longitudeCombo = new QComboBox(homeGroup);
        longitudeCombo->setObjectName(QString::fromUtf8("longitudeCombo"));

        gridLayout_2->addWidget(longitudeCombo, 3, 1, 1, 1);


        verticalLayout_2->addWidget(homeGroup);

        navigationWidget = new Marble::NavigationWidget(navigationTab);
        navigationWidget->setObjectName(QString::fromUtf8("navigationWidget"));

        verticalLayout_2->addWidget(navigationWidget);

        controlTab->addTab(navigationTab, QString());
        mapViewTab = new Marble::MapViewWidget();
        mapViewTab->setObjectName(QString::fromUtf8("mapViewTab"));
        controlTab->addTab(mapViewTab, QString());
        legendTab = new Marble::LegendWidget();
        legendTab->setObjectName(QString::fromUtf8("legendTab"));
        controlTab->addTab(legendTab, QString());
        mainSplitter->addWidget(controlTab);
        marbleWidget = new Marble::MarbleWidget(mainSplitter);
        marbleWidget->setObjectName(QString::fromUtf8("marbleWidget"));
        mainSplitter->addWidget(marbleWidget);

        horizontalLayout->addWidget(mainSplitter);


        verticalLayout->addWidget(mainWidget);

        buttonBox = new QDialogButtonBox(MapDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(buttonBox, controlTab);

        retranslateUi(MapDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), MapDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MapDialog, SLOT(reject()));

        controlTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MapDialog);
    } // setupUi

    void retranslateUi(QDialog *MapDialog)
    {
        MapDialog->setWindowTitle(QApplication::translate("MapDialog", "Marble - Desktop Globe", 0, QApplication::UnicodeUTF8));
        homeGroup->setTitle(QApplication::translate("MapDialog", "Home Location", 0, QApplication::UnicodeUTF8));
        latitudeLabel->setText(QApplication::translate("MapDialog", "Latitude", 0, QApplication::UnicodeUTF8));
        latitudeCombo->clear();
        latitudeCombo->insertItems(0, QStringList()
         << QApplication::translate("MapDialog", "N", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MapDialog", "S", 0, QApplication::UnicodeUTF8)
        );
        longitudeLabel->setText(QApplication::translate("MapDialog", "Longitude", 0, QApplication::UnicodeUTF8));
        longitudeCombo->clear();
        longitudeCombo->insertItems(0, QStringList()
         << QApplication::translate("MapDialog", "E", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MapDialog", "W", 0, QApplication::UnicodeUTF8)
        );
        controlTab->setTabText(controlTab->indexOf(navigationTab), QApplication::translate("MapDialog", "Navigation", 0, QApplication::UnicodeUTF8));
        controlTab->setTabText(controlTab->indexOf(mapViewTab), QApplication::translate("MapDialog", "MapView", 0, QApplication::UnicodeUTF8));
        controlTab->setTabText(controlTab->indexOf(legendTab), QApplication::translate("MapDialog", "Legend", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MapDialog: public Ui_MapDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPDIALOG_H
