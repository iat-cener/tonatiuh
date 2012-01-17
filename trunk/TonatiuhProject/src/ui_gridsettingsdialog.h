/********************************************************************************
** Form generated from reading UI file 'gridsettingsdialog.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDSETTINGSDIALOG_H
#define UI_GRIDSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GridSettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *gridDimensionsBox;
    QGridLayout *gridLayout_2;
    QSpinBox *xDimensionSpin;
    QLabel *xLabel;
    QSpinBox *zDimensionSpin;
    QLabel *dimenstionsLabel;
    QGroupBox *gridSizeBox;
    QGridLayout *gridLayout;
    QDoubleSpinBox *xSpacingSpin;
    QLabel *xSpacionLabel;
    QLabel *zSpacingLabel;
    QDoubleSpinBox *zSpacingSpin;
    QRadioButton *scebeRadio;
    QRadioButton *userDefinedRadio;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GridSettingsDialog)
    {
        if (GridSettingsDialog->objectName().isEmpty())
            GridSettingsDialog->setObjectName(QString::fromUtf8("GridSettingsDialog"));
        GridSettingsDialog->resize(411, 376);
        verticalLayout = new QVBoxLayout(GridSettingsDialog);
        verticalLayout->setSpacing(20);
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridDimensionsBox = new QGroupBox(GridSettingsDialog);
        gridDimensionsBox->setObjectName(QString::fromUtf8("gridDimensionsBox"));
        gridDimensionsBox->setFlat(true);
        gridLayout_2 = new QGridLayout(gridDimensionsBox);
        gridLayout_2->setSpacing(10);
        gridLayout_2->setContentsMargins(10, 10, 10, 10);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        xDimensionSpin = new QSpinBox(gridDimensionsBox);
        xDimensionSpin->setObjectName(QString::fromUtf8("xDimensionSpin"));
        xDimensionSpin->setMinimumSize(QSize(0, 25));
        xDimensionSpin->setMaximum(9999);
        xDimensionSpin->setValue(100);

        gridLayout_2->addWidget(xDimensionSpin, 1, 1, 1, 1);

        xLabel = new QLabel(gridDimensionsBox);
        xLabel->setObjectName(QString::fromUtf8("xLabel"));
        xLabel->setMinimumSize(QSize(0, 25));
        xLabel->setAutoFillBackground(false);
        xLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(xLabel, 1, 2, 1, 1);

        zDimensionSpin = new QSpinBox(gridDimensionsBox);
        zDimensionSpin->setObjectName(QString::fromUtf8("zDimensionSpin"));
        zDimensionSpin->setMinimumSize(QSize(0, 25));
        zDimensionSpin->setMaximum(9999);
        zDimensionSpin->setValue(100);

        gridLayout_2->addWidget(zDimensionSpin, 1, 3, 1, 1);

        dimenstionsLabel = new QLabel(gridDimensionsBox);
        dimenstionsLabel->setObjectName(QString::fromUtf8("dimenstionsLabel"));
        dimenstionsLabel->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(dimenstionsLabel, 1, 0, 1, 1);


        verticalLayout->addWidget(gridDimensionsBox);

        gridSizeBox = new QGroupBox(GridSettingsDialog);
        gridSizeBox->setObjectName(QString::fromUtf8("gridSizeBox"));
        gridSizeBox->setMinimumSize(QSize(0, 25));
        gridSizeBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gridSizeBox->setFlat(true);
        gridLayout = new QGridLayout(gridSizeBox);
        gridLayout->setSpacing(10);
        gridLayout->setContentsMargins(10, 10, 10, 10);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        xSpacingSpin = new QDoubleSpinBox(gridSizeBox);
        xSpacingSpin->setObjectName(QString::fromUtf8("xSpacingSpin"));
        xSpacingSpin->setMinimumSize(QSize(0, 25));
        xSpacingSpin->setMaximum(10000);
        xSpacingSpin->setValue(1);

        gridLayout->addWidget(xSpacingSpin, 3, 2, 1, 1);

        xSpacionLabel = new QLabel(gridSizeBox);
        xSpacionLabel->setObjectName(QString::fromUtf8("xSpacionLabel"));

        gridLayout->addWidget(xSpacionLabel, 3, 1, 1, 1);

        zSpacingLabel = new QLabel(gridSizeBox);
        zSpacingLabel->setObjectName(QString::fromUtf8("zSpacingLabel"));
        zSpacingLabel->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(zSpacingLabel, 4, 1, 1, 1);

        zSpacingSpin = new QDoubleSpinBox(gridSizeBox);
        zSpacingSpin->setObjectName(QString::fromUtf8("zSpacingSpin"));
        zSpacingSpin->setMinimumSize(QSize(0, 25));
        zSpacingSpin->setMaximum(10000);
        zSpacingSpin->setValue(1);

        gridLayout->addWidget(zSpacingSpin, 4, 2, 1, 1);

        scebeRadio = new QRadioButton(gridSizeBox);
        scebeRadio->setObjectName(QString::fromUtf8("scebeRadio"));
        scebeRadio->setChecked(false);

        gridLayout->addWidget(scebeRadio, 1, 0, 1, 1);

        userDefinedRadio = new QRadioButton(gridSizeBox);
        userDefinedRadio->setObjectName(QString::fromUtf8("userDefinedRadio"));
        userDefinedRadio->setMinimumSize(QSize(0, 25));
        userDefinedRadio->setChecked(true);

        gridLayout->addWidget(userDefinedRadio, 2, 0, 1, 1);


        verticalLayout->addWidget(gridSizeBox);

        buttonBox = new QDialogButtonBox(GridSettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(buttonBox, xDimensionSpin);
        QWidget::setTabOrder(xDimensionSpin, zDimensionSpin);
        QWidget::setTabOrder(zDimensionSpin, xSpacingSpin);
        QWidget::setTabOrder(xSpacingSpin, zSpacingSpin);

        retranslateUi(GridSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), GridSettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GridSettingsDialog, SLOT(reject()));
        QObject::connect(userDefinedRadio, SIGNAL(toggled(bool)), xSpacionLabel, SLOT(setEnabled(bool)));
        QObject::connect(userDefinedRadio, SIGNAL(toggled(bool)), xSpacingSpin, SLOT(setEnabled(bool)));
        QObject::connect(userDefinedRadio, SIGNAL(toggled(bool)), zSpacingLabel, SLOT(setEnabled(bool)));
        QObject::connect(userDefinedRadio, SIGNAL(toggled(bool)), zSpacingSpin, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(GridSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *GridSettingsDialog)
    {
        GridSettingsDialog->setWindowTitle(QApplication::translate("GridSettingsDialog", "Grid Settings", 0, QApplication::UnicodeUTF8));
        gridDimensionsBox->setTitle(QApplication::translate("GridSettingsDialog", "Grid Dimensions", 0, QApplication::UnicodeUTF8));
        xLabel->setText(QApplication::translate("GridSettingsDialog", "x", 0, QApplication::UnicodeUTF8));
        dimenstionsLabel->setText(QApplication::translate("GridSettingsDialog", "Dimensions: ", 0, QApplication::UnicodeUTF8));
        gridSizeBox->setTitle(QApplication::translate("GridSettingsDialog", "Grid Size", 0, QApplication::UnicodeUTF8));
        xSpacionLabel->setText(QApplication::translate("GridSettingsDialog", "X Spacing:", 0, QApplication::UnicodeUTF8));
        zSpacingLabel->setText(QApplication::translate("GridSettingsDialog", "Z Spacing:", 0, QApplication::UnicodeUTF8));
        scebeRadio->setText(QApplication::translate("GridSettingsDialog", "Scene Size", 0, QApplication::UnicodeUTF8));
        userDefinedRadio->setText(QApplication::translate("GridSettingsDialog", "User Defined Size", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GridSettingsDialog: public Ui_GridSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDSETTINGSDIALOG_H
