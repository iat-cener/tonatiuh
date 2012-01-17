/********************************************************************************
** Form generated from reading UI file 'raytracedialog.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RAYTRACEDIALOG_H
#define UI_RAYTRACEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
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

class Ui_RayTraceDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *settingsGroup;
    QGridLayout *gridLayout;
    QLabel *numberRaysLabel;
    QSpinBox *raysSpinBox;
    QLabel *randomLabel;
    QComboBox *randomCombo;
    QSpinBox *widthDivisionsSpinBox;
    QSpinBox *heightDivisionsSpinBox;
    QLabel *heightdivisionsQLabel;
    QLabel *widthdivisionsQLabel;
    QGroupBox *drawBox;
    QGridLayout *gridLayout_2;
    QLabel *drawFractionLabel;
    QDoubleSpinBox *drawSpin;
    QCheckBox *photonsCheck;
    QGroupBox *photonmapGroup;
    QGridLayout *gridLayout_3;
    QLabel *photonmapTypeLabel;
    QComboBox *photonmapTypeCombo;
    QLabel *constructionLabel;
    QRadioButton *newMapRadio;
    QRadioButton *increaseMapRadio;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RayTraceDialog)
    {
        if (RayTraceDialog->objectName().isEmpty())
            RayTraceDialog->setObjectName(QString::fromUtf8("RayTraceDialog"));
        RayTraceDialog->resize(342, 543);
        RayTraceDialog->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(RayTraceDialog);
        verticalLayout->setSpacing(20);
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        settingsGroup = new QGroupBox(RayTraceDialog);
        settingsGroup->setObjectName(QString::fromUtf8("settingsGroup"));
        gridLayout = new QGridLayout(settingsGroup);
        gridLayout->setSpacing(10);
        gridLayout->setContentsMargins(10, 10, 10, 10);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        numberRaysLabel = new QLabel(settingsGroup);
        numberRaysLabel->setObjectName(QString::fromUtf8("numberRaysLabel"));
        numberRaysLabel->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(numberRaysLabel, 0, 0, 1, 1);

        raysSpinBox = new QSpinBox(settingsGroup);
        raysSpinBox->setObjectName(QString::fromUtf8("raysSpinBox"));
        raysSpinBox->setMinimumSize(QSize(0, 25));
        raysSpinBox->setLayoutDirection(Qt::LeftToRight);
        raysSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        raysSpinBox->setMinimum(1);
        raysSpinBox->setMaximum(999999999);
        raysSpinBox->setSingleStep(10000);
        raysSpinBox->setValue(10000);

        gridLayout->addWidget(raysSpinBox, 0, 1, 1, 1);

        randomLabel = new QLabel(settingsGroup);
        randomLabel->setObjectName(QString::fromUtf8("randomLabel"));
        randomLabel->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(randomLabel, 1, 0, 1, 1);

        randomCombo = new QComboBox(settingsGroup);
        randomCombo->setObjectName(QString::fromUtf8("randomCombo"));
        randomCombo->setMinimumSize(QSize(0, 25));
        randomCombo->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

        gridLayout->addWidget(randomCombo, 1, 1, 1, 1);

        widthDivisionsSpinBox = new QSpinBox(settingsGroup);
        widthDivisionsSpinBox->setObjectName(QString::fromUtf8("widthDivisionsSpinBox"));
        widthDivisionsSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        widthDivisionsSpinBox->setMinimum(1);
        widthDivisionsSpinBox->setMaximum(5000);
        widthDivisionsSpinBox->setSingleStep(50);
        widthDivisionsSpinBox->setValue(200);

        gridLayout->addWidget(widthDivisionsSpinBox, 2, 1, 2, 1);

        heightDivisionsSpinBox = new QSpinBox(settingsGroup);
        heightDivisionsSpinBox->setObjectName(QString::fromUtf8("heightDivisionsSpinBox"));
        heightDivisionsSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        heightDivisionsSpinBox->setMinimum(1);
        heightDivisionsSpinBox->setMaximum(5000);
        heightDivisionsSpinBox->setSingleStep(50);
        heightDivisionsSpinBox->setValue(200);

        gridLayout->addWidget(heightDivisionsSpinBox, 4, 1, 1, 1);

        heightdivisionsQLabel = new QLabel(settingsGroup);
        heightdivisionsQLabel->setObjectName(QString::fromUtf8("heightdivisionsQLabel"));

        gridLayout->addWidget(heightdivisionsQLabel, 4, 0, 1, 1);

        widthdivisionsQLabel = new QLabel(settingsGroup);
        widthdivisionsQLabel->setObjectName(QString::fromUtf8("widthdivisionsQLabel"));

        gridLayout->addWidget(widthdivisionsQLabel, 2, 0, 1, 1);


        verticalLayout->addWidget(settingsGroup);

        drawBox = new QGroupBox(RayTraceDialog);
        drawBox->setObjectName(QString::fromUtf8("drawBox"));
        gridLayout_2 = new QGridLayout(drawBox);
        gridLayout_2->setSpacing(10);
        gridLayout_2->setContentsMargins(10, 10, 10, 10);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        drawFractionLabel = new QLabel(drawBox);
        drawFractionLabel->setObjectName(QString::fromUtf8("drawFractionLabel"));
        drawFractionLabel->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(drawFractionLabel, 0, 0, 1, 1);

        drawSpin = new QDoubleSpinBox(drawBox);
        drawSpin->setObjectName(QString::fromUtf8("drawSpin"));
        drawSpin->setMinimumSize(QSize(0, 25));
        drawSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        drawSpin->setMaximum(100);
        drawSpin->setSingleStep(5);
        drawSpin->setValue(1);

        gridLayout_2->addWidget(drawSpin, 0, 1, 1, 1);

        photonsCheck = new QCheckBox(drawBox);
        photonsCheck->setObjectName(QString::fromUtf8("photonsCheck"));
        photonsCheck->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(photonsCheck, 1, 0, 1, 1);


        verticalLayout->addWidget(drawBox);

        photonmapGroup = new QGroupBox(RayTraceDialog);
        photonmapGroup->setObjectName(QString::fromUtf8("photonmapGroup"));
        gridLayout_3 = new QGridLayout(photonmapGroup);
        gridLayout_3->setSpacing(10);
        gridLayout_3->setContentsMargins(10, 10, 10, 10);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        photonmapTypeLabel = new QLabel(photonmapGroup);
        photonmapTypeLabel->setObjectName(QString::fromUtf8("photonmapTypeLabel"));
        photonmapTypeLabel->setMinimumSize(QSize(0, 25));

        gridLayout_3->addWidget(photonmapTypeLabel, 0, 0, 1, 1);

        photonmapTypeCombo = new QComboBox(photonmapGroup);
        photonmapTypeCombo->setObjectName(QString::fromUtf8("photonmapTypeCombo"));
        photonmapTypeCombo->setMinimumSize(QSize(0, 25));
        photonmapTypeCombo->setLayoutDirection(Qt::LeftToRight);

        gridLayout_3->addWidget(photonmapTypeCombo, 0, 1, 1, 1);

        constructionLabel = new QLabel(photonmapGroup);
        constructionLabel->setObjectName(QString::fromUtf8("constructionLabel"));
        constructionLabel->setMinimumSize(QSize(0, 25));

        gridLayout_3->addWidget(constructionLabel, 1, 0, 1, 1);

        newMapRadio = new QRadioButton(photonmapGroup);
        newMapRadio->setObjectName(QString::fromUtf8("newMapRadio"));
        newMapRadio->setMinimumSize(QSize(0, 25));
        newMapRadio->setChecked(true);

        gridLayout_3->addWidget(newMapRadio, 1, 1, 1, 1);

        increaseMapRadio = new QRadioButton(photonmapGroup);
        increaseMapRadio->setObjectName(QString::fromUtf8("increaseMapRadio"));
        increaseMapRadio->setMinimumSize(QSize(0, 25));

        gridLayout_3->addWidget(increaseMapRadio, 2, 1, 1, 1);


        verticalLayout->addWidget(photonmapGroup);

        buttonBox = new QDialogButtonBox(RayTraceDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setMinimumSize(QSize(0, 30));
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(raysSpinBox, randomCombo);
        QWidget::setTabOrder(randomCombo, drawSpin);
        QWidget::setTabOrder(drawSpin, photonsCheck);
        QWidget::setTabOrder(photonsCheck, photonmapTypeCombo);
        QWidget::setTabOrder(photonmapTypeCombo, newMapRadio);
        QWidget::setTabOrder(newMapRadio, increaseMapRadio);
        QWidget::setTabOrder(increaseMapRadio, buttonBox);

        retranslateUi(RayTraceDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), RayTraceDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RayTraceDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(RayTraceDialog);
    } // setupUi

    void retranslateUi(QDialog *RayTraceDialog)
    {
        RayTraceDialog->setWindowTitle(QApplication::translate("RayTraceDialog", "Options", 0, QApplication::UnicodeUTF8));
        settingsGroup->setTitle(QApplication::translate("RayTraceDialog", "Settings", 0, QApplication::UnicodeUTF8));
        numberRaysLabel->setText(QApplication::translate("RayTraceDialog", "Number of Rays:", 0, QApplication::UnicodeUTF8));
        randomLabel->setText(QApplication::translate("RayTraceDialog", "Random Generator:", 0, QApplication::UnicodeUTF8));
        heightdivisionsQLabel->setText(QApplication::translate("RayTraceDialog", "Height divisions:", 0, QApplication::UnicodeUTF8));
        widthdivisionsQLabel->setText(QApplication::translate("RayTraceDialog", "Width divisions:", 0, QApplication::UnicodeUTF8));
        drawBox->setTitle(QApplication::translate("RayTraceDialog", "Draw", 0, QApplication::UnicodeUTF8));
        drawFractionLabel->setText(QApplication::translate("RayTraceDialog", "Rays to draw (%):", 0, QApplication::UnicodeUTF8));
        photonsCheck->setText(QApplication::translate("RayTraceDialog", "Draw photons", 0, QApplication::UnicodeUTF8));
        photonmapGroup->setTitle(QApplication::translate("RayTraceDialog", "PhotonMap", 0, QApplication::UnicodeUTF8));
        photonmapTypeLabel->setText(QApplication::translate("RayTraceDialog", "Type:", 0, QApplication::UnicodeUTF8));
        constructionLabel->setText(QApplication::translate("RayTraceDialog", "Contruction:", 0, QApplication::UnicodeUTF8));
        newMapRadio->setText(QApplication::translate("RayTraceDialog", "New Photon Map", 0, QApplication::UnicodeUTF8));
        increaseMapRadio->setText(QApplication::translate("RayTraceDialog", "Increase Photon Map", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RayTraceDialog: public Ui_RayTraceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAYTRACEDIALOG_H
