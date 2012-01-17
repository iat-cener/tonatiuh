/********************************************************************************
** Form generated from reading UI file 'sunpositioncalculatordialog.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUNPOSITIONCALCULATORDIALOG_H
#define UI_SUNPOSITIONCALCULATORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCalendarWidget>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTimeEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "CelestialWidget.h"
#include "HorizontalWidget.h"
#include "WorldMap.h"

QT_BEGIN_NAMESPACE

class Ui_SunPositionCalculatorDialog
{
public:
    QVBoxLayout *mainLayout;
    QWidget *mainWidget;
    QHBoxLayout *mainWidgetLayout;
    QTabWidget *viewMap;
    QWidget *mapTab;
    QVBoxLayout *mapTabLayout;
    WorldMap *worldMap;
    QWidget *horizontalTab;
    QVBoxLayout *verticalLayout;
    HorizontalWidget *horizontalWidget;
    QWidget *celestialTab;
    QVBoxLayout *celestialTab_2;
    CelestialWidget *celestialWidget;
    QWidget *parametersWidget;
    QVBoxLayout *parametersLayout;
    QGroupBox *dateGroup;
    QVBoxLayout *dateGroupLayout;
    QCalendarWidget *calendarWidget;
    QGroupBox *timeGroup;
    QGridLayout *timeGroupLayout;
    QRadioButton *utRadio;
    QRadioButton *ctRadio;
    QTimeEdit *utTime;
    QTimeEdit *ctTime;
    QSpinBox *zoneSpin;
    QGroupBox *positionGroup;
    QGridLayout *positionLayout;
    QLabel *latitudeLabel;
    QLabel *longitudeLabel;
    QDoubleSpinBox *latitudeSpin;
    QDoubleSpinBox *longitudeSpin;
    QPushButton *selectButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SunPositionCalculatorDialog)
    {
        if (SunPositionCalculatorDialog->objectName().isEmpty())
            SunPositionCalculatorDialog->setObjectName(QString::fromUtf8("SunPositionCalculatorDialog"));
        SunPositionCalculatorDialog->resize(986, 555);
        mainLayout = new QVBoxLayout(SunPositionCalculatorDialog);
        mainLayout->setSpacing(10);
        mainLayout->setContentsMargins(10, 10, 10, 10);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        mainWidget = new QWidget(SunPositionCalculatorDialog);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        mainWidgetLayout = new QHBoxLayout(mainWidget);
        mainWidgetLayout->setSpacing(10);
        mainWidgetLayout->setContentsMargins(0, 0, 0, 0);
        mainWidgetLayout->setObjectName(QString::fromUtf8("mainWidgetLayout"));
        viewMap = new QTabWidget(mainWidget);
        viewMap->setObjectName(QString::fromUtf8("viewMap"));
        mapTab = new QWidget();
        mapTab->setObjectName(QString::fromUtf8("mapTab"));
        mapTabLayout = new QVBoxLayout(mapTab);
        mapTabLayout->setSpacing(10);
        mapTabLayout->setContentsMargins(0, 0, 0, 0);
        mapTabLayout->setObjectName(QString::fromUtf8("mapTabLayout"));
        worldMap = new WorldMap(mapTab);
        worldMap->setObjectName(QString::fromUtf8("worldMap"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(worldMap->sizePolicy().hasHeightForWidth());
        worldMap->setSizePolicy(sizePolicy);
        worldMap->setMinimumSize(QSize(500, 0));
        worldMap->setMouseTracking(true);

        mapTabLayout->addWidget(worldMap);

        viewMap->addTab(mapTab, QString());
        horizontalTab = new QWidget();
        horizontalTab->setObjectName(QString::fromUtf8("horizontalTab"));
        verticalLayout = new QVBoxLayout(horizontalTab);
        verticalLayout->setSpacing(10);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalWidget = new HorizontalWidget(horizontalTab);
        horizontalWidget->setObjectName(QString::fromUtf8("horizontalWidget"));

        verticalLayout->addWidget(horizontalWidget);

        viewMap->addTab(horizontalTab, QString());
        celestialTab = new QWidget();
        celestialTab->setObjectName(QString::fromUtf8("celestialTab"));
        celestialTab_2 = new QVBoxLayout(celestialTab);
        celestialTab_2->setSpacing(10);
        celestialTab_2->setContentsMargins(0, 0, 0, 0);
        celestialTab_2->setObjectName(QString::fromUtf8("celestialTab_2"));
        celestialWidget = new CelestialWidget(celestialTab);
        celestialWidget->setObjectName(QString::fromUtf8("celestialWidget"));

        celestialTab_2->addWidget(celestialWidget);

        viewMap->addTab(celestialTab, QString());

        mainWidgetLayout->addWidget(viewMap);

        parametersWidget = new QWidget(mainWidget);
        parametersWidget->setObjectName(QString::fromUtf8("parametersWidget"));
        parametersLayout = new QVBoxLayout(parametersWidget);
        parametersLayout->setSpacing(10);
        parametersLayout->setContentsMargins(0, 0, 0, 0);
        parametersLayout->setObjectName(QString::fromUtf8("parametersLayout"));
        dateGroup = new QGroupBox(parametersWidget);
        dateGroup->setObjectName(QString::fromUtf8("dateGroup"));
        dateGroupLayout = new QVBoxLayout(dateGroup);
        dateGroupLayout->setSpacing(10);
        dateGroupLayout->setContentsMargins(10, 10, 10, 10);
        dateGroupLayout->setObjectName(QString::fromUtf8("dateGroupLayout"));
        calendarWidget = new QCalendarWidget(dateGroup);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));

        dateGroupLayout->addWidget(calendarWidget);


        parametersLayout->addWidget(dateGroup);

        timeGroup = new QGroupBox(parametersWidget);
        timeGroup->setObjectName(QString::fromUtf8("timeGroup"));
        timeGroupLayout = new QGridLayout(timeGroup);
        timeGroupLayout->setSpacing(10);
        timeGroupLayout->setContentsMargins(10, 10, 10, 10);
        timeGroupLayout->setObjectName(QString::fromUtf8("timeGroupLayout"));
        utRadio = new QRadioButton(timeGroup);
        utRadio->setObjectName(QString::fromUtf8("utRadio"));
        utRadio->setMinimumSize(QSize(100, 25));

        timeGroupLayout->addWidget(utRadio, 0, 0, 1, 1);

        ctRadio = new QRadioButton(timeGroup);
        ctRadio->setObjectName(QString::fromUtf8("ctRadio"));
        ctRadio->setMinimumSize(QSize(100, 25));

        timeGroupLayout->addWidget(ctRadio, 1, 0, 1, 1);

        utTime = new QTimeEdit(timeGroup);
        utTime->setObjectName(QString::fromUtf8("utTime"));
        utTime->setMinimumSize(QSize(150, 25));

        timeGroupLayout->addWidget(utTime, 0, 1, 1, 1);

        ctTime = new QTimeEdit(timeGroup);
        ctTime->setObjectName(QString::fromUtf8("ctTime"));
        ctTime->setMinimumSize(QSize(150, 25));

        timeGroupLayout->addWidget(ctTime, 1, 1, 1, 1);

        zoneSpin = new QSpinBox(timeGroup);
        zoneSpin->setObjectName(QString::fromUtf8("zoneSpin"));
        zoneSpin->setMinimumSize(QSize(100, 25));
        zoneSpin->setMinimum(-12);
        zoneSpin->setMaximum(12);

        timeGroupLayout->addWidget(zoneSpin, 1, 2, 1, 1);


        parametersLayout->addWidget(timeGroup);

        positionGroup = new QGroupBox(parametersWidget);
        positionGroup->setObjectName(QString::fromUtf8("positionGroup"));
        positionLayout = new QGridLayout(positionGroup);
        positionLayout->setSpacing(10);
        positionLayout->setContentsMargins(10, 10, 10, 10);
        positionLayout->setObjectName(QString::fromUtf8("positionLayout"));
        latitudeLabel = new QLabel(positionGroup);
        latitudeLabel->setObjectName(QString::fromUtf8("latitudeLabel"));
        latitudeLabel->setMinimumSize(QSize(100, 25));

        positionLayout->addWidget(latitudeLabel, 0, 0, 1, 1);

        longitudeLabel = new QLabel(positionGroup);
        longitudeLabel->setObjectName(QString::fromUtf8("longitudeLabel"));
        longitudeLabel->setMinimumSize(QSize(100, 25));

        positionLayout->addWidget(longitudeLabel, 1, 0, 1, 1);

        latitudeSpin = new QDoubleSpinBox(positionGroup);
        latitudeSpin->setObjectName(QString::fromUtf8("latitudeSpin"));
        latitudeSpin->setMinimumSize(QSize(150, 25));
        latitudeSpin->setDecimals(4);
        latitudeSpin->setMinimum(-90);
        latitudeSpin->setMaximum(90);

        positionLayout->addWidget(latitudeSpin, 0, 1, 1, 1);

        longitudeSpin = new QDoubleSpinBox(positionGroup);
        longitudeSpin->setObjectName(QString::fromUtf8("longitudeSpin"));
        longitudeSpin->setMinimumSize(QSize(150, 25));
        longitudeSpin->setDecimals(4);
        longitudeSpin->setMinimum(-180);
        longitudeSpin->setMaximum(180);

        positionLayout->addWidget(longitudeSpin, 1, 1, 1, 1);

        selectButton = new QPushButton(positionGroup);
        selectButton->setObjectName(QString::fromUtf8("selectButton"));
        selectButton->setMinimumSize(QSize(100, 25));

        positionLayout->addWidget(selectButton, 1, 2, 1, 1);


        parametersLayout->addWidget(positionGroup);


        mainWidgetLayout->addWidget(parametersWidget);


        mainLayout->addWidget(mainWidget);

        buttonBox = new QDialogButtonBox(SunPositionCalculatorDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        mainLayout->addWidget(buttonBox);


        retranslateUi(SunPositionCalculatorDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SunPositionCalculatorDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SunPositionCalculatorDialog, SLOT(reject()));

        viewMap->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SunPositionCalculatorDialog);
    } // setupUi

    void retranslateUi(QDialog *SunPositionCalculatorDialog)
    {
        SunPositionCalculatorDialog->setWindowTitle(QApplication::translate("SunPositionCalculatorDialog", "Sun Position Calculator", 0, QApplication::UnicodeUTF8));
        viewMap->setTabText(viewMap->indexOf(mapTab), QApplication::translate("SunPositionCalculatorDialog", "Map", 0, QApplication::UnicodeUTF8));
        viewMap->setTabText(viewMap->indexOf(horizontalTab), QApplication::translate("SunPositionCalculatorDialog", "Horizontal Coordinates", 0, QApplication::UnicodeUTF8));
        viewMap->setTabText(viewMap->indexOf(celestialTab), QApplication::translate("SunPositionCalculatorDialog", "Celestial Coordinates", 0, QApplication::UnicodeUTF8));
        dateGroup->setTitle(QApplication::translate("SunPositionCalculatorDialog", "Date", 0, QApplication::UnicodeUTF8));
        timeGroup->setTitle(QApplication::translate("SunPositionCalculatorDialog", "Time", 0, QApplication::UnicodeUTF8));
        utRadio->setText(QApplication::translate("SunPositionCalculatorDialog", "UT", 0, QApplication::UnicodeUTF8));
        ctRadio->setText(QApplication::translate("SunPositionCalculatorDialog", "CT", 0, QApplication::UnicodeUTF8));
        positionGroup->setTitle(QApplication::translate("SunPositionCalculatorDialog", "Position Coordinates", 0, QApplication::UnicodeUTF8));
        latitudeLabel->setText(QApplication::translate("SunPositionCalculatorDialog", "Latitude:", 0, QApplication::UnicodeUTF8));
        longitudeLabel->setText(QApplication::translate("SunPositionCalculatorDialog", "Longitude:", 0, QApplication::UnicodeUTF8));
        selectButton->setText(QApplication::translate("SunPositionCalculatorDialog", "Select", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SunPositionCalculatorDialog: public Ui_SunPositionCalculatorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUNPOSITIONCALCULATORDIALOG_H
