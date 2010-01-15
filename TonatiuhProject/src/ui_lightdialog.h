/********************************************************************************
** Form generated from reading ui file 'lightdialog.ui'
**
** Created: Fri Jan 15 12:17:53 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LIGHTDIALOG_H
#define UI_LIGHTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
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
    QVBoxLayout *verticalLayout_2;
    QGroupBox *sunshapeBox;
    QGridLayout *gridLayout;
    QLabel *sunshapeLabel;
    QComboBox *sunshapeCombo;
    QFrame *sunshapeParametersFrame;
    QGroupBox *apertureBox;
    QGridLayout *gridLayout_2;
    QLabel *shapeLabel;
    QComboBox *shapeCombo;
    QFrame *shapeParametersFrame;
    QWidget *sunPositionTab;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLabel *azimuthLabel;
    QDoubleSpinBox *azimuthSpin;
    QLabel *elevationLabel;
    QDoubleSpinBox *elevationSpin;
    QLabel *distanceLabel;
    QDoubleSpinBox *distanceSpin;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LightDialog)
    {
        if (LightDialog->objectName().isEmpty())
            LightDialog->setObjectName(QString::fromUtf8("LightDialog"));
        LightDialog->resize(380, 450);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LightDialog->sizePolicy().hasHeightForWidth());
        LightDialog->setSizePolicy(sizePolicy);
        LightDialog->setMaximumSize(QSize(380, 450));
        verticalLayout = new QVBoxLayout(LightDialog);
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
        verticalLayout_2 = new QVBoxLayout(sunShapeTab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        sunshapeBox = new QGroupBox(sunShapeTab);
        sunshapeBox->setObjectName(QString::fromUtf8("sunshapeBox"));
        gridLayout = new QGridLayout(sunshapeBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        sunshapeLabel = new QLabel(sunshapeBox);
        sunshapeLabel->setObjectName(QString::fromUtf8("sunshapeLabel"));

        gridLayout->addWidget(sunshapeLabel, 0, 0, 1, 1);

        sunshapeCombo = new QComboBox(sunshapeBox);
        sunshapeCombo->setObjectName(QString::fromUtf8("sunshapeCombo"));
        sunshapeCombo->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(sunshapeCombo, 0, 1, 1, 1);

        sunshapeParametersFrame = new QFrame(sunshapeBox);
        sunshapeParametersFrame->setObjectName(QString::fromUtf8("sunshapeParametersFrame"));
        sunshapeParametersFrame->setFrameShape(QFrame::StyledPanel);
        sunshapeParametersFrame->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(sunshapeParametersFrame, 1, 1, 1, 1);


        verticalLayout_2->addWidget(sunshapeBox);

        apertureBox = new QGroupBox(sunShapeTab);
        apertureBox->setObjectName(QString::fromUtf8("apertureBox"));
        gridLayout_2 = new QGridLayout(apertureBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        shapeLabel = new QLabel(apertureBox);
        shapeLabel->setObjectName(QString::fromUtf8("shapeLabel"));

        gridLayout_2->addWidget(shapeLabel, 0, 0, 1, 1);

        shapeCombo = new QComboBox(apertureBox);
        shapeCombo->setObjectName(QString::fromUtf8("shapeCombo"));
        shapeCombo->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(shapeCombo, 0, 1, 1, 1);

        shapeParametersFrame = new QFrame(apertureBox);
        shapeParametersFrame->setObjectName(QString::fromUtf8("shapeParametersFrame"));
        shapeParametersFrame->setFrameShape(QFrame::StyledPanel);
        shapeParametersFrame->setFrameShadow(QFrame::Raised);

        gridLayout_2->addWidget(shapeParametersFrame, 1, 1, 1, 1);


        verticalLayout_2->addWidget(apertureBox);

        tabWidget->addTab(sunShapeTab, QString());
        sunPositionTab = new QWidget();
        sunPositionTab->setObjectName(QString::fromUtf8("sunPositionTab"));
        verticalLayout_3 = new QVBoxLayout(sunPositionTab);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(sunPositionTab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        azimuthLabel = new QLabel(groupBox);
        azimuthLabel->setObjectName(QString::fromUtf8("azimuthLabel"));

        gridLayout_3->addWidget(azimuthLabel, 0, 0, 1, 1);

        azimuthSpin = new QDoubleSpinBox(groupBox);
        azimuthSpin->setObjectName(QString::fromUtf8("azimuthSpin"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(azimuthSpin->sizePolicy().hasHeightForWidth());
        azimuthSpin->setSizePolicy(sizePolicy2);
        azimuthSpin->setMinimumSize(QSize(0, 25));
        azimuthSpin->setMaximumSize(QSize(16777215, 16777215));
        azimuthSpin->setDecimals(4);
        azimuthSpin->setMaximum(360);

        gridLayout_3->addWidget(azimuthSpin, 0, 1, 1, 1);

        elevationLabel = new QLabel(groupBox);
        elevationLabel->setObjectName(QString::fromUtf8("elevationLabel"));

        gridLayout_3->addWidget(elevationLabel, 1, 0, 1, 1);

        elevationSpin = new QDoubleSpinBox(groupBox);
        elevationSpin->setObjectName(QString::fromUtf8("elevationSpin"));
        sizePolicy2.setHeightForWidth(elevationSpin->sizePolicy().hasHeightForWidth());
        elevationSpin->setSizePolicy(sizePolicy2);
        elevationSpin->setMinimumSize(QSize(0, 25));
        elevationSpin->setDecimals(4);
        elevationSpin->setMinimum(-90);
        elevationSpin->setMaximum(90);
        elevationSpin->setValue(90);

        gridLayout_3->addWidget(elevationSpin, 1, 1, 1, 1);

        distanceLabel = new QLabel(groupBox);
        distanceLabel->setObjectName(QString::fromUtf8("distanceLabel"));

        gridLayout_3->addWidget(distanceLabel, 2, 0, 1, 1);

        distanceSpin = new QDoubleSpinBox(groupBox);
        distanceSpin->setObjectName(QString::fromUtf8("distanceSpin"));
        sizePolicy2.setHeightForWidth(distanceSpin->sizePolicy().hasHeightForWidth());
        distanceSpin->setSizePolicy(sizePolicy2);
        distanceSpin->setMinimumSize(QSize(0, 25));
        distanceSpin->setDecimals(4);
        distanceSpin->setMaximum(1e+09);
        distanceSpin->setValue(200);

        gridLayout_3->addWidget(distanceSpin, 2, 1, 1, 1);


        verticalLayout_3->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        tabWidget->addTab(sunPositionTab, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(LightDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
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
        sunshapeBox->setTitle(QApplication::translate("LightDialog", "Sun Shape", 0, QApplication::UnicodeUTF8));
        sunshapeLabel->setText(QApplication::translate("LightDialog", "Sunshape Type:", 0, QApplication::UnicodeUTF8));
        apertureBox->setTitle(QApplication::translate("LightDialog", "Input Aperture", 0, QApplication::UnicodeUTF8));
        shapeLabel->setText(QApplication::translate("LightDialog", "Shape Type:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(sunShapeTab), QApplication::translate("LightDialog", "Sun Shape", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("LightDialog", "Definition of sun position", 0, QApplication::UnicodeUTF8));
        azimuthLabel->setText(QApplication::translate("LightDialog", "Azimuth:", 0, QApplication::UnicodeUTF8));
        elevationLabel->setText(QApplication::translate("LightDialog", "Elevation:", 0, QApplication::UnicodeUTF8));
        distanceLabel->setText(QApplication::translate("LightDialog", "Distance:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(sunPositionTab), QApplication::translate("LightDialog", "Sun Position", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(LightDialog);
    } // retranslateUi

};

namespace Ui {
    class LightDialog: public Ui_LightDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHTDIALOG_H
