/********************************************************************************
** Form generated from reading ui file 'sunpositioncalculatordialog.ui'
**
** Created: Fri Jan 15 12:17:53 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SUNPOSITIONCALCULATORDIALOG_H
#define UI_SUNPOSITIONCALCULATORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCalendarWidget>
#include <QtGui/QDoubleSpinBox>
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
    QTabWidget *viewWidget;
    QWidget *mapTab;
    QHBoxLayout *hboxLayout;
    WorldMap *worldMap;
    QWidget *horizontalTab;
    QVBoxLayout *vboxLayout;
    HorizontalWidget *horizontalWidget;
    QWidget *celestial;
    QVBoxLayout *vboxLayout1;
    CelestialWidget *celestialWidget;
    QWidget *parametersWidget;
    QGroupBox *Hora;
    QRadioButton *UT;
    QRadioButton *CT;
    QTimeEdit *uTimeSpin;
    QTimeEdit *cTimeSpin;
    QSpinBox *zoneSpin;
    QGroupBox *GBPosicion;
    QLabel *longitudeLabel;
    QLabel *latitudeLabel;
    QDoubleSpinBox *longitudeSpin;
    QDoubleSpinBox *latitudeSpin;
    QPushButton *selectButton;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout1;
    QCalendarWidget *calendarWidget;
    QPushButton *updateButton;
    QPushButton *closeButton;

    void setupUi(QWidget *SunPositionCalculatorDialog)
    {
        if (SunPositionCalculatorDialog->objectName().isEmpty())
            SunPositionCalculatorDialog->setObjectName(QString::fromUtf8("SunPositionCalculatorDialog"));
        SunPositionCalculatorDialog->resize(841, 460);
        viewWidget = new QTabWidget(SunPositionCalculatorDialog);
        viewWidget->setObjectName(QString::fromUtf8("viewWidget"));
        viewWidget->setGeometry(QRect(9, 9, 531, 441));
        QFont font;
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        viewWidget->setFont(font);
        viewWidget->setMouseTracking(true);
        mapTab = new QWidget();
        mapTab->setObjectName(QString::fromUtf8("mapTab"));
        hboxLayout = new QHBoxLayout(mapTab);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        worldMap = new WorldMap(mapTab);
        worldMap->setObjectName(QString::fromUtf8("worldMap"));
        QFont font1;
        font1.setPointSize(8);
        worldMap->setFont(font1);
        worldMap->setMouseTracking(true);

        hboxLayout->addWidget(worldMap);

        viewWidget->addTab(mapTab, QString());
        horizontalTab = new QWidget();
        horizontalTab->setObjectName(QString::fromUtf8("horizontalTab"));
        vboxLayout = new QVBoxLayout(horizontalTab);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        horizontalWidget = new HorizontalWidget(horizontalTab);
        horizontalWidget->setObjectName(QString::fromUtf8("horizontalWidget"));

        vboxLayout->addWidget(horizontalWidget);

        viewWidget->addTab(horizontalTab, QString());
        celestial = new QWidget();
        celestial->setObjectName(QString::fromUtf8("celestial"));
        vboxLayout1 = new QVBoxLayout(celestial);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        celestialWidget = new CelestialWidget(celestial);
        celestialWidget->setObjectName(QString::fromUtf8("celestialWidget"));

        vboxLayout1->addWidget(celestialWidget);

        viewWidget->addTab(celestial, QString());
        parametersWidget = new QWidget(SunPositionCalculatorDialog);
        parametersWidget->setObjectName(QString::fromUtf8("parametersWidget"));
        parametersWidget->setGeometry(QRect(540, 10, 290, 435));
        parametersWidget->setFont(font1);
        Hora = new QGroupBox(parametersWidget);
        Hora->setObjectName(QString::fromUtf8("Hora"));
        Hora->setGeometry(QRect(10, 195, 270, 90));
        QFont font2;
        font2.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font2.setPointSize(8);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setUnderline(false);
        font2.setWeight(50);
        font2.setStrikeOut(false);
        Hora->setFont(font2);
        UT = new QRadioButton(Hora);
        UT->setObjectName(QString::fromUtf8("UT"));
        UT->setGeometry(QRect(20, 30, 50, 25));
        UT->setFont(font);
        UT->setChecked(true);
        CT = new QRadioButton(Hora);
        CT->setObjectName(QString::fromUtf8("CT"));
        CT->setGeometry(QRect(20, 60, 50, 25));
        CT->setFont(font);
        uTimeSpin = new QTimeEdit(Hora);
        uTimeSpin->setObjectName(QString::fromUtf8("uTimeSpin"));
        uTimeSpin->setGeometry(QRect(100, 30, 100, 25));
        uTimeSpin->setFont(font);
        cTimeSpin = new QTimeEdit(Hora);
        cTimeSpin->setObjectName(QString::fromUtf8("cTimeSpin"));
        cTimeSpin->setEnabled(false);
        cTimeSpin->setGeometry(QRect(100, 60, 100, 25));
        cTimeSpin->setFont(font);
        zoneSpin = new QSpinBox(Hora);
        zoneSpin->setObjectName(QString::fromUtf8("zoneSpin"));
        zoneSpin->setEnabled(false);
        zoneSpin->setGeometry(QRect(210, 60, 50, 25));
        zoneSpin->setFont(font);
        zoneSpin->setMinimum(-12);
        zoneSpin->setMaximum(12);
        GBPosicion = new QGroupBox(parametersWidget);
        GBPosicion->setObjectName(QString::fromUtf8("GBPosicion"));
        GBPosicion->setGeometry(QRect(9, 290, 270, 90));
        GBPosicion->setFont(font);
        longitudeLabel = new QLabel(GBPosicion);
        longitudeLabel->setObjectName(QString::fromUtf8("longitudeLabel"));
        longitudeLabel->setGeometry(QRect(20, 30, 100, 25));
        longitudeLabel->setFont(font2);
        latitudeLabel = new QLabel(GBPosicion);
        latitudeLabel->setObjectName(QString::fromUtf8("latitudeLabel"));
        latitudeLabel->setGeometry(QRect(20, 60, 80, 25));
        latitudeLabel->setFont(font2);
        longitudeSpin = new QDoubleSpinBox(GBPosicion);
        longitudeSpin->setObjectName(QString::fromUtf8("longitudeSpin"));
        longitudeSpin->setGeometry(QRect(100, 30, 100, 25));
        longitudeSpin->setFont(font);
        longitudeSpin->setDecimals(4);
        longitudeSpin->setMinimum(-180);
        longitudeSpin->setMaximum(180);
        latitudeSpin = new QDoubleSpinBox(GBPosicion);
        latitudeSpin->setObjectName(QString::fromUtf8("latitudeSpin"));
        latitudeSpin->setGeometry(QRect(100, 60, 100, 25));
        latitudeSpin->setFont(font);
        latitudeSpin->setDecimals(4);
        latitudeSpin->setMinimum(-90);
        latitudeSpin->setMaximum(90);
        selectButton = new QPushButton(GBPosicion);
        selectButton->setObjectName(QString::fromUtf8("selectButton"));
        selectButton->setGeometry(QRect(210, 60, 50, 25));
        selectButton->setFont(font);
        groupBox = new QGroupBox(parametersWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 270, 182));
        groupBox->setFont(font1);
        hboxLayout1 = new QHBoxLayout(groupBox);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        calendarWidget = new QCalendarWidget(groupBox);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));
        calendarWidget->setFont(font1);
        calendarWidget->setFirstDayOfWeek(Qt::Sunday);

        hboxLayout1->addWidget(calendarWidget);

        updateButton = new QPushButton(parametersWidget);
        updateButton->setObjectName(QString::fromUtf8("updateButton"));
        updateButton->setGeometry(QRect(120, 400, 75, 25));
        updateButton->setFont(font);
        updateButton->setDefault(true);
        closeButton = new QPushButton(parametersWidget);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(200, 400, 75, 25));

        retranslateUi(SunPositionCalculatorDialog);
        QObject::connect(UT, SIGNAL(toggled(bool)), uTimeSpin, SLOT(setEnabled(bool)));
        QObject::connect(CT, SIGNAL(toggled(bool)), cTimeSpin, SLOT(setEnabled(bool)));
        QObject::connect(CT, SIGNAL(toggled(bool)), zoneSpin, SLOT(setEnabled(bool)));
        QObject::connect(closeButton, SIGNAL(clicked()), SunPositionCalculatorDialog, SLOT(close()));

        viewWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SunPositionCalculatorDialog);
    } // setupUi

    void retranslateUi(QWidget *SunPositionCalculatorDialog)
    {
        SunPositionCalculatorDialog->setWindowTitle(QApplication::translate("SunPositionCalculatorDialog", "Sun Position Calculator", 0, QApplication::UnicodeUTF8));
        viewWidget->setTabText(viewWidget->indexOf(mapTab), QApplication::translate("SunPositionCalculatorDialog", "Map", 0, QApplication::UnicodeUTF8));
        viewWidget->setTabText(viewWidget->indexOf(horizontalTab), QApplication::translate("SunPositionCalculatorDialog", "Horizontal Coordinates", 0, QApplication::UnicodeUTF8));
        viewWidget->setTabText(viewWidget->indexOf(celestial), QApplication::translate("SunPositionCalculatorDialog", "Celestial Coordinates", 0, QApplication::UnicodeUTF8));
        Hora->setTitle(QApplication::translate("SunPositionCalculatorDialog", "Time", 0, QApplication::UnicodeUTF8));
        UT->setText(QApplication::translate("SunPositionCalculatorDialog", "UT", 0, QApplication::UnicodeUTF8));
        CT->setText(QApplication::translate("SunPositionCalculatorDialog", "CT", 0, QApplication::UnicodeUTF8));
        uTimeSpin->setDisplayFormat(QApplication::translate("SunPositionCalculatorDialog", "hh:mm:ss", 0, QApplication::UnicodeUTF8));
        cTimeSpin->setDisplayFormat(QApplication::translate("SunPositionCalculatorDialog", "hh:mm:ss", 0, QApplication::UnicodeUTF8));
        GBPosicion->setTitle(QApplication::translate("SunPositionCalculatorDialog", "Position Coordinates", 0, QApplication::UnicodeUTF8));
        longitudeLabel->setText(QApplication::translate("SunPositionCalculatorDialog", "Longitude:", 0, QApplication::UnicodeUTF8));
        latitudeLabel->setText(QApplication::translate("SunPositionCalculatorDialog", "Latitude:", 0, QApplication::UnicodeUTF8));
        selectButton->setText(QApplication::translate("SunPositionCalculatorDialog", "Select", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SunPositionCalculatorDialog", "Date", 0, QApplication::UnicodeUTF8));
        updateButton->setText(QApplication::translate("SunPositionCalculatorDialog", "Update Light", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("SunPositionCalculatorDialog", "Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SunPositionCalculatorDialog);
    } // retranslateUi

};

namespace Ui {
    class SunPositionCalculatorDialog: public Ui_SunPositionCalculatorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUNPOSITIONCALCULATORDIALOG_H
