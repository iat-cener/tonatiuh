/********************************************************************************
** Form generated from reading ui file 'raytracedialog.ui'
**
** Created: Fri Jan 15 12:17:53 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
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
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_RayTraceDialog
{
public:
    QGroupBox *raysGroup;
    QLabel *numberRaysLabel;
    QSpinBox *raysSpinBox;
    QGroupBox *photonmapGroup;
    QRadioButton *newMapRadio;
    QRadioButton *increaseMapRadio;
    QComboBox *photonmapTypeCombo;
    QLabel *photonmapTypeLabel;
    QLabel *constructionLabel;
    QDialogButtonBox *buttonBox;
    QGroupBox *drawBox;
    QLabel *drawFractionLabel;
    QCheckBox *photonsCheck;
    QDoubleSpinBox *drawSpin;

    void setupUi(QDialog *RayTraceDialog)
    {
        if (RayTraceDialog->objectName().isEmpty())
            RayTraceDialog->setObjectName(QString::fromUtf8("RayTraceDialog"));
        RayTraceDialog->resize(395, 405);
        RayTraceDialog->setSizeGripEnabled(true);
        raysGroup = new QGroupBox(RayTraceDialog);
        raysGroup->setObjectName(QString::fromUtf8("raysGroup"));
        raysGroup->setGeometry(QRect(10, 10, 375, 75));
        numberRaysLabel = new QLabel(raysGroup);
        numberRaysLabel->setObjectName(QString::fromUtf8("numberRaysLabel"));
        numberRaysLabel->setGeometry(QRect(20, 30, 100, 25));
        raysSpinBox = new QSpinBox(raysGroup);
        raysSpinBox->setObjectName(QString::fromUtf8("raysSpinBox"));
        raysSpinBox->setGeometry(QRect(160, 30, 150, 25));
        raysSpinBox->setLayoutDirection(Qt::LeftToRight);
        raysSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        raysSpinBox->setMinimum(1);
        raysSpinBox->setMaximum(99999999);
        raysSpinBox->setSingleStep(1000);
        raysSpinBox->setValue(1000);
        photonmapGroup = new QGroupBox(RayTraceDialog);
        photonmapGroup->setObjectName(QString::fromUtf8("photonmapGroup"));
        photonmapGroup->setGeometry(QRect(10, 195, 375, 135));
        newMapRadio = new QRadioButton(photonmapGroup);
        newMapRadio->setObjectName(QString::fromUtf8("newMapRadio"));
        newMapRadio->setGeometry(QRect(160, 60, 150, 25));
        newMapRadio->setChecked(true);
        increaseMapRadio = new QRadioButton(photonmapGroup);
        increaseMapRadio->setObjectName(QString::fromUtf8("increaseMapRadio"));
        increaseMapRadio->setGeometry(QRect(160, 90, 150, 25));
        photonmapTypeCombo = new QComboBox(photonmapGroup);
        photonmapTypeCombo->setObjectName(QString::fromUtf8("photonmapTypeCombo"));
        photonmapTypeCombo->setGeometry(QRect(160, 30, 150, 25));
        photonmapTypeCombo->setLayoutDirection(Qt::LeftToRight);
        photonmapTypeLabel = new QLabel(photonmapGroup);
        photonmapTypeLabel->setObjectName(QString::fromUtf8("photonmapTypeLabel"));
        photonmapTypeLabel->setGeometry(QRect(20, 30, 46, 25));
        constructionLabel = new QLabel(photonmapGroup);
        constructionLabel->setObjectName(QString::fromUtf8("constructionLabel"));
        constructionLabel->setGeometry(QRect(20, 60, 150, 25));
        buttonBox = new QDialogButtonBox(RayTraceDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(110, 350, 275, 25));
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        drawBox = new QGroupBox(RayTraceDialog);
        drawBox->setObjectName(QString::fromUtf8("drawBox"));
        drawBox->setGeometry(QRect(10, 90, 375, 100));
        drawFractionLabel = new QLabel(drawBox);
        drawFractionLabel->setObjectName(QString::fromUtf8("drawFractionLabel"));
        drawFractionLabel->setGeometry(QRect(20, 30, 100, 25));
        photonsCheck = new QCheckBox(drawBox);
        photonsCheck->setObjectName(QString::fromUtf8("photonsCheck"));
        photonsCheck->setGeometry(QRect(20, 60, 110, 25));
        drawSpin = new QDoubleSpinBox(drawBox);
        drawSpin->setObjectName(QString::fromUtf8("drawSpin"));
        drawSpin->setGeometry(QRect(160, 30, 150, 25));
        drawSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        drawSpin->setMaximum(100);
        drawSpin->setSingleStep(5);
        drawSpin->setValue(10);
        QWidget::setTabOrder(raysSpinBox, drawSpin);
        QWidget::setTabOrder(drawSpin, photonsCheck);
        QWidget::setTabOrder(photonsCheck, newMapRadio);
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
        raysGroup->setTitle(QApplication::translate("RayTraceDialog", "Rays", 0, QApplication::UnicodeUTF8));
        numberRaysLabel->setText(QApplication::translate("RayTraceDialog", "Number of Rays:", 0, QApplication::UnicodeUTF8));
        photonmapGroup->setTitle(QApplication::translate("RayTraceDialog", "PhotonMap", 0, QApplication::UnicodeUTF8));
        newMapRadio->setText(QApplication::translate("RayTraceDialog", "New Photon Map", 0, QApplication::UnicodeUTF8));
        increaseMapRadio->setText(QApplication::translate("RayTraceDialog", "Increase Photon Map", 0, QApplication::UnicodeUTF8));
        photonmapTypeLabel->setText(QApplication::translate("RayTraceDialog", "Type:", 0, QApplication::UnicodeUTF8));
        constructionLabel->setText(QApplication::translate("RayTraceDialog", "Contruction:", 0, QApplication::UnicodeUTF8));
        drawBox->setTitle(QApplication::translate("RayTraceDialog", "Draw", 0, QApplication::UnicodeUTF8));
        drawFractionLabel->setText(QApplication::translate("RayTraceDialog", "Rays to draw (%):", 0, QApplication::UnicodeUTF8));
        photonsCheck->setText(QApplication::translate("RayTraceDialog", "Draw photons", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(RayTraceDialog);
    } // retranslateUi

};

namespace Ui {
    class RayTraceDialog: public Ui_RayTraceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAYTRACEDIALOG_H
