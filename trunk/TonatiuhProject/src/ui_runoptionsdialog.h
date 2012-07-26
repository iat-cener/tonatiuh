/********************************************************************************
** Form generated from reading UI file 'runoptionsdialog.ui'
**
** Created: Thu 26. Jul 12:31:24 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNOPTIONSDIALOG_H
#define UI_RUNOPTIONSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RunOptionsDialog
{
public:
    QVBoxLayout *mainLayout;
    QGroupBox *storeTypeGroup;
    QGridLayout *typeStoreLayout;
    QWidget *SQLWidget;
    QGridLayout *gridLayout;
    QLineEdit *SQLNameEdit;
    QPushButton *SQLselectFile;
    QLabel *SQLNameLabel;
    QSpacerItem *horizontalSpacer_3;
    QRadioButton *datFileRadio;
    QSpacerItem *horizontalSpacer;
    QRadioButton *dataBaseRadio;
    QWidget *datWidget;
    QGridLayout *fileStoreLayout;
    QSpinBox *raysPerFileSpinBox;
    QLabel *label_3;
    QPushButton *selectSurfaceButton;
    QSlider *raysPerFileSlider;
    QRadioButton *exportSurfaceRadioButton;
    QSpacerItem *horizontalSpacer_2;
    QRadioButton *exportAllPhotonsRadio;
    QLabel *label_2;
    QRadioButton *oneFileRadio;
    QLabel *raysperFileLabel;
    QRadioButton *moreFilesButton;
    QLabel *label;
    QLineEdit *surfacePathLineEdit;
    QLabel *label_4;
    QLineEdit *fileNameEdit;
    QPushButton *selectFileSave;
    QLabel *storeTypeLabel;
    QComboBox *storeTypeCombo;
    QGroupBox *storeInfoGroup;
    QGridLayout *gridLayout_4;
    QCheckBox *sideBox;
    QCheckBox *coordBox;
    QCheckBox *powerPerPhotonCheck;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;
    QButtonGroup *buttonGroup;

    void setupUi(QDialog *RunOptionsDialog)
    {
        if (RunOptionsDialog->objectName().isEmpty())
            RunOptionsDialog->setObjectName(QString::fromUtf8("RunOptionsDialog"));
        RunOptionsDialog->resize(647, 588);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RunOptionsDialog->sizePolicy().hasHeightForWidth());
        RunOptionsDialog->setSizePolicy(sizePolicy);
        RunOptionsDialog->setMaximumSize(QSize(16777215, 16777215));
        mainLayout = new QVBoxLayout(RunOptionsDialog);
        mainLayout->setSpacing(10);
        mainLayout->setContentsMargins(10, 10, 10, 10);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        storeTypeGroup = new QGroupBox(RunOptionsDialog);
        storeTypeGroup->setObjectName(QString::fromUtf8("storeTypeGroup"));
        typeStoreLayout = new QGridLayout(storeTypeGroup);
        typeStoreLayout->setSpacing(10);
        typeStoreLayout->setContentsMargins(10, 10, 10, 10);
        typeStoreLayout->setObjectName(QString::fromUtf8("typeStoreLayout"));
        SQLWidget = new QWidget(storeTypeGroup);
        SQLWidget->setObjectName(QString::fromUtf8("SQLWidget"));
        gridLayout = new QGridLayout(SQLWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        SQLNameEdit = new QLineEdit(SQLWidget);
        SQLNameEdit->setObjectName(QString::fromUtf8("SQLNameEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(SQLNameEdit->sizePolicy().hasHeightForWidth());
        SQLNameEdit->setSizePolicy(sizePolicy1);
        SQLNameEdit->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(SQLNameEdit, 1, 1, 1, 1);

        SQLselectFile = new QPushButton(SQLWidget);
        SQLselectFile->setObjectName(QString::fromUtf8("SQLselectFile"));
        SQLselectFile->setMinimumSize(QSize(0, 25));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        SQLselectFile->setIcon(icon);

        gridLayout->addWidget(SQLselectFile, 1, 2, 1, 1);

        SQLNameLabel = new QLabel(SQLWidget);
        SQLNameLabel->setObjectName(QString::fromUtf8("SQLNameLabel"));
        SQLNameLabel->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(SQLNameLabel, 1, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 3, 1, 1);


        typeStoreLayout->addWidget(SQLWidget, 4, 0, 1, 3);

        datFileRadio = new QRadioButton(storeTypeGroup);
        datFileRadio->setObjectName(QString::fromUtf8("datFileRadio"));
        datFileRadio->setChecked(true);
        datFileRadio->setAutoExclusive(true);

        typeStoreLayout->addWidget(datFileRadio, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        typeStoreLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        dataBaseRadio = new QRadioButton(storeTypeGroup);
        dataBaseRadio->setObjectName(QString::fromUtf8("dataBaseRadio"));
        dataBaseRadio->setAutoExclusive(true);

        typeStoreLayout->addWidget(dataBaseRadio, 1, 1, 1, 1);

        datWidget = new QWidget(storeTypeGroup);
        datWidget->setObjectName(QString::fromUtf8("datWidget"));
        datWidget->setEnabled(true);
        fileStoreLayout = new QGridLayout(datWidget);
        fileStoreLayout->setSpacing(10);
        fileStoreLayout->setContentsMargins(0, 0, 0, 0);
        fileStoreLayout->setObjectName(QString::fromUtf8("fileStoreLayout"));
        raysPerFileSpinBox = new QSpinBox(datWidget);
        raysPerFileSpinBox->setObjectName(QString::fromUtf8("raysPerFileSpinBox"));
        raysPerFileSpinBox->setEnabled(false);
        raysPerFileSpinBox->setMinimum(10);
        raysPerFileSpinBox->setMaximum(999999999);
        raysPerFileSpinBox->setSingleStep(100000);
        raysPerFileSpinBox->setValue(100000);

        fileStoreLayout->addWidget(raysPerFileSpinBox, 13, 2, 1, 1);

        label_3 = new QLabel(datWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        fileStoreLayout->addWidget(label_3, 5, 0, 1, 3);

        selectSurfaceButton = new QPushButton(datWidget);
        selectSurfaceButton->setObjectName(QString::fromUtf8("selectSurfaceButton"));
        selectSurfaceButton->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(selectSurfaceButton->sizePolicy().hasHeightForWidth());
        selectSurfaceButton->setSizePolicy(sizePolicy2);
        selectSurfaceButton->setCheckable(false);
        selectSurfaceButton->setAutoExclusive(false);

        fileStoreLayout->addWidget(selectSurfaceButton, 3, 2, 1, 1);

        raysPerFileSlider = new QSlider(datWidget);
        raysPerFileSlider->setObjectName(QString::fromUtf8("raysPerFileSlider"));
        raysPerFileSlider->setEnabled(false);
        raysPerFileSlider->setMinimum(10);
        raysPerFileSlider->setMaximum(999999999);
        raysPerFileSlider->setSingleStep(100000);
        raysPerFileSlider->setSliderPosition(100000);
        raysPerFileSlider->setOrientation(Qt::Horizontal);

        fileStoreLayout->addWidget(raysPerFileSlider, 12, 2, 1, 1);

        exportSurfaceRadioButton = new QRadioButton(datWidget);
        buttonGroup = new QButtonGroup(RunOptionsDialog);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(exportSurfaceRadioButton);
        exportSurfaceRadioButton->setObjectName(QString::fromUtf8("exportSurfaceRadioButton"));
        exportSurfaceRadioButton->setChecked(false);
        exportSurfaceRadioButton->setAutoExclusive(true);

        fileStoreLayout->addWidget(exportSurfaceRadioButton, 3, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        fileStoreLayout->addItem(horizontalSpacer_2, 6, 5, 1, 1);

        exportAllPhotonsRadio = new QRadioButton(datWidget);
        buttonGroup->addButton(exportAllPhotonsRadio);
        exportAllPhotonsRadio->setObjectName(QString::fromUtf8("exportAllPhotonsRadio"));
        exportAllPhotonsRadio->setChecked(true);

        fileStoreLayout->addWidget(exportAllPhotonsRadio, 2, 1, 1, 1);

        label_2 = new QLabel(datWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        fileStoreLayout->addWidget(label_2, 7, 0, 1, 5);

        oneFileRadio = new QRadioButton(datWidget);
        oneFileRadio->setObjectName(QString::fromUtf8("oneFileRadio"));
        oneFileRadio->setChecked(true);

        fileStoreLayout->addWidget(oneFileRadio, 8, 1, 1, 1);

        raysperFileLabel = new QLabel(datWidget);
        raysperFileLabel->setObjectName(QString::fromUtf8("raysperFileLabel"));
        raysperFileLabel->setEnabled(false);
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        raysperFileLabel->setFont(font);

        fileStoreLayout->addWidget(raysperFileLabel, 9, 2, 1, 1);

        moreFilesButton = new QRadioButton(datWidget);
        moreFilesButton->setObjectName(QString::fromUtf8("moreFilesButton"));

        fileStoreLayout->addWidget(moreFilesButton, 9, 1, 1, 1);

        label = new QLabel(datWidget);
        label->setObjectName(QString::fromUtf8("label"));

        fileStoreLayout->addWidget(label, 1, 0, 1, 5);

        surfacePathLineEdit = new QLineEdit(datWidget);
        surfacePathLineEdit->setObjectName(QString::fromUtf8("surfacePathLineEdit"));
        surfacePathLineEdit->setEnabled(false);
        QFont font1;
        font1.setPointSize(10);
        surfacePathLineEdit->setFont(font1);

        fileStoreLayout->addWidget(surfacePathLineEdit, 4, 2, 1, 1);

        label_4 = new QLabel(datWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        fileStoreLayout->addWidget(label_4, 6, 1, 1, 1);

        fileNameEdit = new QLineEdit(datWidget);
        fileNameEdit->setObjectName(QString::fromUtf8("fileNameEdit"));
        sizePolicy1.setHeightForWidth(fileNameEdit->sizePolicy().hasHeightForWidth());
        fileNameEdit->setSizePolicy(sizePolicy1);
        fileNameEdit->setMinimumSize(QSize(0, 25));

        fileStoreLayout->addWidget(fileNameEdit, 6, 2, 1, 1);

        selectFileSave = new QPushButton(datWidget);
        selectFileSave->setObjectName(QString::fromUtf8("selectFileSave"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(selectFileSave->sizePolicy().hasHeightForWidth());
        selectFileSave->setSizePolicy(sizePolicy3);
        selectFileSave->setMinimumSize(QSize(0, 25));
        selectFileSave->setLayoutDirection(Qt::LeftToRight);
        selectFileSave->setIcon(icon);

        fileStoreLayout->addWidget(selectFileSave, 6, 4, 1, 1);


        typeStoreLayout->addWidget(datWidget, 2, 0, 1, 3);

        storeTypeLabel = new QLabel(storeTypeGroup);
        storeTypeLabel->setObjectName(QString::fromUtf8("storeTypeLabel"));

        typeStoreLayout->addWidget(storeTypeLabel, 0, 0, 1, 1);

        storeTypeCombo = new QComboBox(storeTypeGroup);
        storeTypeCombo->setObjectName(QString::fromUtf8("storeTypeCombo"));

        typeStoreLayout->addWidget(storeTypeCombo, 0, 1, 1, 1);


        mainLayout->addWidget(storeTypeGroup);

        storeInfoGroup = new QGroupBox(RunOptionsDialog);
        storeInfoGroup->setObjectName(QString::fromUtf8("storeInfoGroup"));
        gridLayout_4 = new QGridLayout(storeInfoGroup);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        sideBox = new QCheckBox(storeInfoGroup);
        sideBox->setObjectName(QString::fromUtf8("sideBox"));
        sideBox->setChecked(true);

        gridLayout_4->addWidget(sideBox, 1, 0, 1, 1);

        coordBox = new QCheckBox(storeInfoGroup);
        coordBox->setObjectName(QString::fromUtf8("coordBox"));
        coordBox->setChecked(true);

        gridLayout_4->addWidget(coordBox, 0, 0, 1, 1);

        powerPerPhotonCheck = new QCheckBox(storeInfoGroup);
        powerPerPhotonCheck->setObjectName(QString::fromUtf8("powerPerPhotonCheck"));
        powerPerPhotonCheck->setChecked(true);

        gridLayout_4->addWidget(powerPerPhotonCheck, 2, 0, 1, 1);


        mainLayout->addWidget(storeInfoGroup);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(RunOptionsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        mainLayout->addWidget(buttonBox);


        retranslateUi(RunOptionsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), RunOptionsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RunOptionsDialog, SLOT(reject()));
        QObject::connect(exportSurfaceRadioButton, SIGNAL(toggled(bool)), selectSurfaceButton, SLOT(setEnabled(bool)));
        QObject::connect(raysPerFileSlider, SIGNAL(valueChanged(int)), raysPerFileSpinBox, SLOT(setValue(int)));
        QObject::connect(raysPerFileSpinBox, SIGNAL(valueChanged(int)), raysPerFileSlider, SLOT(setValue(int)));
        QObject::connect(exportSurfaceRadioButton, SIGNAL(toggled(bool)), surfacePathLineEdit, SLOT(setEnabled(bool)));
        QObject::connect(datFileRadio, SIGNAL(toggled(bool)), datWidget, SLOT(setVisible(bool)));
        QObject::connect(dataBaseRadio, SIGNAL(toggled(bool)), SQLWidget, SLOT(setVisible(bool)));
        QObject::connect(moreFilesButton, SIGNAL(toggled(bool)), raysperFileLabel, SLOT(setEnabled(bool)));
        QObject::connect(moreFilesButton, SIGNAL(toggled(bool)), raysPerFileSlider, SLOT(setEnabled(bool)));
        QObject::connect(moreFilesButton, SIGNAL(toggled(bool)), raysPerFileSpinBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(RunOptionsDialog);
    } // setupUi

    void retranslateUi(QDialog *RunOptionsDialog)
    {
        RunOptionsDialog->setWindowTitle(QApplication::translate("RunOptionsDialog", "Define Run Options", 0, QApplication::UnicodeUTF8));
        storeTypeGroup->setTitle(QApplication::translate("RunOptionsDialog", "Type Of Store", 0, QApplication::UnicodeUTF8));
        SQLselectFile->setText(QString());
        SQLNameLabel->setText(QApplication::translate("RunOptionsDialog", "    SQL BD file       ", 0, QApplication::UnicodeUTF8));
        datFileRadio->setText(QApplication::translate("RunOptionsDialog", ".dat file", 0, QApplication::UnicodeUTF8));
        dataBaseRadio->setText(QApplication::translate("RunOptionsDialog", "SQL", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("RunOptionsDialog", "Selecte the file name to store", 0, QApplication::UnicodeUTF8));
        selectSurfaceButton->setText(QApplication::translate("RunOptionsDialog", "Choose surface...", 0, QApplication::UnicodeUTF8));
        exportSurfaceRadioButton->setText(QApplication::translate("RunOptionsDialog", "Export surface", 0, QApplication::UnicodeUTF8));
        exportAllPhotonsRadio->setText(QApplication::translate("RunOptionsDialog", "Export All Photon Map:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RunOptionsDialog", "Select the number of files to store", 0, QApplication::UnicodeUTF8));
        oneFileRadio->setText(QApplication::translate("RunOptionsDialog", "One file", 0, QApplication::UnicodeUTF8));
        raysperFileLabel->setText(QApplication::translate("RunOptionsDialog", "Max. number of photons per file to store:", 0, QApplication::UnicodeUTF8));
        moreFilesButton->setText(QApplication::translate("RunOptionsDialog", "More than one file", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RunOptionsDialog", "Select photons to store:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("RunOptionsDialog", "File name:", 0, QApplication::UnicodeUTF8));
        selectFileSave->setText(QString());
        storeTypeLabel->setText(QApplication::translate("RunOptionsDialog", "Store type:", 0, QApplication::UnicodeUTF8));
        storeInfoGroup->setTitle(QApplication::translate("RunOptionsDialog", "Please, deselect the information you don't want to export:", 0, QApplication::UnicodeUTF8));
        sideBox->setText(QApplication::translate("RunOptionsDialog", "Surface Side", 0, QApplication::UnicodeUTF8));
        coordBox->setText(QApplication::translate("RunOptionsDialog", "Coordinates", 0, QApplication::UnicodeUTF8));
        powerPerPhotonCheck->setText(QApplication::translate("RunOptionsDialog", "Power Per Photon", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RunOptionsDialog: public Ui_RunOptionsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNOPTIONSDIALOG_H
