/********************************************************************************
** Form generated from reading UI file 'heliostatfieldwizard.ui'
**
** Created: Wed Oct 17 16:59:11 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELIOSTATFIELDWIZARD_H
#define UI_HELIOSTATFIELDWIZARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWizard>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_HeliostatFieldWizard
{
public:
    QWizardPage *introductionPage;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QWizardPage *heliostatPage;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QLineEdit *heliostatCoordinatesLine;
    QLabel *aimingLabel;
    QLabel *aimingXLabel;
    QDoubleSpinBox *aimingXSpin;
    QPushButton *openComponentdFileButton;
    QPushButton *openCoordFileButton;
    QLabel *aimingYLabel;
    QLabel *aimingZLabel;
    QDoubleSpinBox *aimingYSpin;
    QDoubleSpinBox *aimingZSpin;
    QLabel *label;

    void setupUi(QWizard *HeliostatFieldWizard)
    {
        if (HeliostatFieldWizard->objectName().isEmpty())
            HeliostatFieldWizard->setObjectName(QString::fromUtf8("HeliostatFieldWizard"));
        HeliostatFieldWizard->resize(395, 399);
        HeliostatFieldWizard->setModal(true);
        HeliostatFieldWizard->setWizardStyle(QWizard::ModernStyle);
        introductionPage = new QWizardPage();
        introductionPage->setObjectName(QString::fromUtf8("introductionPage"));
        verticalLayout = new QVBoxLayout(introductionPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(introductionPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setScaledContents(false);
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);

        HeliostatFieldWizard->setPage(1, introductionPage);
        heliostatPage = new QWizardPage();
        heliostatPage->setObjectName(QString::fromUtf8("heliostatPage"));
        gridLayout = new QGridLayout(heliostatPage);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 9, 1, 1, 1);

        heliostatCoordinatesLine = new QLineEdit(heliostatPage);
        heliostatCoordinatesLine->setObjectName(QString::fromUtf8("heliostatCoordinatesLine"));

        gridLayout->addWidget(heliostatCoordinatesLine, 3, 1, 1, 2);

        aimingLabel = new QLabel(heliostatPage);
        aimingLabel->setObjectName(QString::fromUtf8("aimingLabel"));

        gridLayout->addWidget(aimingLabel, 5, 0, 1, 1);

        aimingXLabel = new QLabel(heliostatPage);
        aimingXLabel->setObjectName(QString::fromUtf8("aimingXLabel"));

        gridLayout->addWidget(aimingXLabel, 6, 1, 1, 1);

        aimingXSpin = new QDoubleSpinBox(heliostatPage);
        aimingXSpin->setObjectName(QString::fromUtf8("aimingXSpin"));
        aimingXSpin->setMaximum(1e+09);

        gridLayout->addWidget(aimingXSpin, 6, 2, 1, 1);

        openComponentdFileButton = new QPushButton(heliostatPage);
        openComponentdFileButton->setObjectName(QString::fromUtf8("openComponentdFileButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        openComponentdFileButton->setIcon(icon);

        gridLayout->addWidget(openComponentdFileButton, 4, 3, 1, 1);

        openCoordFileButton = new QPushButton(heliostatPage);
        openCoordFileButton->setObjectName(QString::fromUtf8("openCoordFileButton"));
        openCoordFileButton->setIcon(icon);

        gridLayout->addWidget(openCoordFileButton, 3, 3, 1, 1);

        aimingYLabel = new QLabel(heliostatPage);
        aimingYLabel->setObjectName(QString::fromUtf8("aimingYLabel"));

        gridLayout->addWidget(aimingYLabel, 7, 1, 1, 1);

        aimingZLabel = new QLabel(heliostatPage);
        aimingZLabel->setObjectName(QString::fromUtf8("aimingZLabel"));

        gridLayout->addWidget(aimingZLabel, 8, 1, 1, 1);

        aimingYSpin = new QDoubleSpinBox(heliostatPage);
        aimingYSpin->setObjectName(QString::fromUtf8("aimingYSpin"));
        aimingYSpin->setMaximum(1e+09);

        gridLayout->addWidget(aimingYSpin, 7, 2, 1, 1);

        aimingZSpin = new QDoubleSpinBox(heliostatPage);
        aimingZSpin->setObjectName(QString::fromUtf8("aimingZSpin"));
        aimingZSpin->setMaximum(1e+09);

        gridLayout->addWidget(aimingZSpin, 8, 2, 1, 1);

        label = new QLabel(heliostatPage);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 3);

        HeliostatFieldWizard->setPage(2, heliostatPage);

        retranslateUi(HeliostatFieldWizard);

        QMetaObject::connectSlotsByName(HeliostatFieldWizard);
    } // setupUi

    void retranslateUi(QWizard *HeliostatFieldWizard)
    {
        HeliostatFieldWizard->setWindowTitle(QApplication::translate("HeliostatFieldWizard", "Heliostat Field Wizard", 0, QApplication::UnicodeUTF8));
        introductionPage->setTitle(QApplication::translate("HeliostatFieldWizard", "Introduction", 0, QApplication::UnicodeUTF8));
        introductionPage->setSubTitle(QString());
        label_2->setText(QApplication::translate("HeliostatFieldWizard", "This wizard will help you creating a new heliostat field. You need a file with the heliostat rotation point coordinates and a Tonatiuh component file with the heliostat definition.", 0, QApplication::UnicodeUTF8));
        heliostatPage->setTitle(QApplication::translate("HeliostatFieldWizard", "Heliostats Definition", 0, QApplication::UnicodeUTF8));
        heliostatPage->setSubTitle(QApplication::translate("HeliostatFieldWizard", "Please define the heliostats coordinates and the heliostat type.", 0, QApplication::UnicodeUTF8));
        aimingLabel->setText(QApplication::translate("HeliostatFieldWizard", "Heliostats aiming point:", 0, QApplication::UnicodeUTF8));
        aimingXLabel->setText(QApplication::translate("HeliostatFieldWizard", "x:", 0, QApplication::UnicodeUTF8));
        openComponentdFileButton->setText(QString());
        openCoordFileButton->setText(QString());
        aimingYLabel->setText(QApplication::translate("HeliostatFieldWizard", "y:", 0, QApplication::UnicodeUTF8));
        aimingZLabel->setText(QApplication::translate("HeliostatFieldWizard", "z:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("HeliostatFieldWizard", "Heliostats coordinates and radius:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class HeliostatFieldWizard: public Ui_HeliostatFieldWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELIOSTATFIELDWIZARD_H
