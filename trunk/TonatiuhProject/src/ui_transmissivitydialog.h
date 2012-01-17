/********************************************************************************
** Form generated from reading UI file 'transmissivitydialog.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSMISSIVITYDIALOG_H
#define UI_TRANSMISSIVITYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include "FieldContainerWidget.h"

QT_BEGIN_NAMESPACE

class Ui_TransmissivityDialog
{
public:
    QGridLayout *mainLayout;
    QLabel *transmissivityLabel;
    QComboBox *transmissivityCombo;
    FieldContainerWidget *transmissivityParameters;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TransmissivityDialog)
    {
        if (TransmissivityDialog->objectName().isEmpty())
            TransmissivityDialog->setObjectName(QString::fromUtf8("TransmissivityDialog"));
        TransmissivityDialog->resize(554, 348);
        mainLayout = new QGridLayout(TransmissivityDialog);
        mainLayout->setSpacing(10);
        mainLayout->setContentsMargins(10, 10, 10, 10);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        transmissivityLabel = new QLabel(TransmissivityDialog);
        transmissivityLabel->setObjectName(QString::fromUtf8("transmissivityLabel"));

        mainLayout->addWidget(transmissivityLabel, 0, 0, 1, 1);

        transmissivityCombo = new QComboBox(TransmissivityDialog);
        transmissivityCombo->setObjectName(QString::fromUtf8("transmissivityCombo"));

        mainLayout->addWidget(transmissivityCombo, 0, 1, 1, 1);

        transmissivityParameters = new FieldContainerWidget(TransmissivityDialog);
        transmissivityParameters->setObjectName(QString::fromUtf8("transmissivityParameters"));

        mainLayout->addWidget(transmissivityParameters, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainLayout->addItem(verticalSpacer, 2, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        mainLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        buttonBox = new QDialogButtonBox(TransmissivityDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        mainLayout->addWidget(buttonBox, 3, 0, 1, 3);


        retranslateUi(TransmissivityDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), TransmissivityDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TransmissivityDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(TransmissivityDialog);
    } // setupUi

    void retranslateUi(QDialog *TransmissivityDialog)
    {
        TransmissivityDialog->setWindowTitle(QApplication::translate("TransmissivityDialog", "Transmissivity Definition", 0, QApplication::UnicodeUTF8));
        transmissivityLabel->setText(QApplication::translate("TransmissivityDialog", "Transmissivity type:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TransmissivityDialog: public Ui_TransmissivityDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSMISSIVITYDIALOG_H
