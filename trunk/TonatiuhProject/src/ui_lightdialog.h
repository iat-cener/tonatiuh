/********************************************************************************
** Form generated from reading UI file 'lightdialog.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIGHTDIALOG_H
#define UI_LIGHTDIALOG_H

#include <FieldContainerWidget.h>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LightDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *sunShapeTab;
    QGridLayout *sunShpeLayout;
    QLabel *sunshapeTypeLabel;
    QComboBox *sunshapeCombo;
    FieldContainerWidget *sunshapeParameters;
    QSpacerItem *verticalSpacer_2;
    QWidget *sunPositionTab;
    QGridLayout *sunShapePositionLayout;
    QDoubleSpinBox *azimuthSpin;
    QDoubleSpinBox *elevationSpin;
    QLabel *azimuthLabel;
    QLabel *elevationLabel;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LightDialog)
    {
        if (LightDialog->objectName().isEmpty())
            LightDialog->setObjectName(QString::fromUtf8("LightDialog"));
        LightDialog->resize(442, 280);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LightDialog->sizePolicy().hasHeightForWidth());
        LightDialog->setSizePolicy(sizePolicy);
        LightDialog->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout = new QVBoxLayout(LightDialog);
        verticalLayout->setSpacing(10);
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(LightDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        sunShapeTab = new QWidget();
        sunShapeTab->setObjectName(QString::fromUtf8("sunShapeTab"));
        sunShpeLayout = new QGridLayout(sunShapeTab);
        sunShpeLayout->setSpacing(10);
        sunShpeLayout->setContentsMargins(10, 10, 10, 10);
        sunShpeLayout->setObjectName(QString::fromUtf8("sunShpeLayout"));
        sunshapeTypeLabel = new QLabel(sunShapeTab);
        sunshapeTypeLabel->setObjectName(QString::fromUtf8("sunshapeTypeLabel"));
        sunshapeTypeLabel->setMinimumSize(QSize(0, 0));

        sunShpeLayout->addWidget(sunshapeTypeLabel, 0, 1, 1, 1);

        sunshapeCombo = new QComboBox(sunShapeTab);
        sunshapeCombo->setObjectName(QString::fromUtf8("sunshapeCombo"));
        sizePolicy.setHeightForWidth(sunshapeCombo->sizePolicy().hasHeightForWidth());
        sunshapeCombo->setSizePolicy(sizePolicy);
        sunshapeCombo->setMinimumSize(QSize(0, 0));

        sunShpeLayout->addWidget(sunshapeCombo, 0, 2, 1, 3);

        sunshapeParameters = new FieldContainerWidget(sunShapeTab);
        sunshapeParameters->setObjectName(QString::fromUtf8("sunshapeParameters"));

        sunShpeLayout->addWidget(sunshapeParameters, 2, 2, 1, 3);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        sunShpeLayout->addItem(verticalSpacer_2, 3, 0, 1, 4);

        tabWidget->addTab(sunShapeTab, QString());
        sunPositionTab = new QWidget();
        sunPositionTab->setObjectName(QString::fromUtf8("sunPositionTab"));
        sunShapePositionLayout = new QGridLayout(sunPositionTab);
        sunShapePositionLayout->setSpacing(10);
        sunShapePositionLayout->setContentsMargins(10, 10, 10, 10);
        sunShapePositionLayout->setObjectName(QString::fromUtf8("sunShapePositionLayout"));
        azimuthSpin = new QDoubleSpinBox(sunPositionTab);
        azimuthSpin->setObjectName(QString::fromUtf8("azimuthSpin"));
        sizePolicy.setHeightForWidth(azimuthSpin->sizePolicy().hasHeightForWidth());
        azimuthSpin->setSizePolicy(sizePolicy);
        azimuthSpin->setMinimumSize(QSize(0, 0));
        azimuthSpin->setMaximumSize(QSize(16777215, 16777215));
        azimuthSpin->setDecimals(4);
        azimuthSpin->setMaximum(360);

        sunShapePositionLayout->addWidget(azimuthSpin, 0, 1, 1, 1);

        elevationSpin = new QDoubleSpinBox(sunPositionTab);
        elevationSpin->setObjectName(QString::fromUtf8("elevationSpin"));
        sizePolicy.setHeightForWidth(elevationSpin->sizePolicy().hasHeightForWidth());
        elevationSpin->setSizePolicy(sizePolicy);
        elevationSpin->setMinimumSize(QSize(0, 0));
        elevationSpin->setDecimals(4);
        elevationSpin->setMinimum(-90);
        elevationSpin->setMaximum(90);
        elevationSpin->setValue(90);

        sunShapePositionLayout->addWidget(elevationSpin, 1, 1, 1, 1);

        azimuthLabel = new QLabel(sunPositionTab);
        azimuthLabel->setObjectName(QString::fromUtf8("azimuthLabel"));
        azimuthLabel->setMinimumSize(QSize(0, 0));

        sunShapePositionLayout->addWidget(azimuthLabel, 0, 0, 1, 1);

        elevationLabel = new QLabel(sunPositionTab);
        elevationLabel->setObjectName(QString::fromUtf8("elevationLabel"));
        elevationLabel->setMinimumSize(QSize(0, 0));

        sunShapePositionLayout->addWidget(elevationLabel, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        sunShapePositionLayout->addItem(verticalSpacer, 2, 0, 1, 2);

        tabWidget->addTab(sunPositionTab, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(LightDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setMinimumSize(QSize(0, 0));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(LightDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), LightDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LightDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LightDialog);
    } // setupUi

    void retranslateUi(QDialog *LightDialog)
    {
        LightDialog->setWindowTitle(QApplication::translate("LightDialog", "Define Sun Light", 0, QApplication::UnicodeUTF8));
        sunshapeTypeLabel->setText(QApplication::translate("LightDialog", "Sunshape Type:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(sunShapeTab), QApplication::translate("LightDialog", "Sun Shape", 0, QApplication::UnicodeUTF8));
        azimuthLabel->setText(QApplication::translate("LightDialog", "Azimuth (degrees from North):", 0, QApplication::UnicodeUTF8));
        elevationLabel->setText(QApplication::translate("LightDialog", "Elevation (degrees):", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(sunPositionTab), QApplication::translate("LightDialog", "Sun Position", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LightDialog: public Ui_LightDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHTDIALOG_H
