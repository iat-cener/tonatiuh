/********************************************************************************
** Form generated from reading UI file 'heliostatfieldwizard.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELIOSTATFIELDWIZARD_H
#define UI_HELIOSTATFIELDWIZARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>
#include <QtGui/QWizard>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_Wizard
{
public:
    QWizardPage *heliostatPage;
    QGridLayout *gridLayout;
    QLabel *label;
    QFrame *line;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_2;
    QLabel *label_4;
    QFrame *line_2;
    QLabel *label_5;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_3;
    QDoubleSpinBox *doubleSpinBox_4;
    QLabel *label_7;
    QComboBox *comboBox;
    QLabel *label_8;
    QFrame *line_3;
    QComboBox *comboBox_2;
    QLabel *label_9;
    QWizardPage *targetPage;
    QGridLayout *gridLayout_2;
    QLabel *label_10;
    QLabel *label_11;
    QFrame *line_4;
    QLabel *label_12;
    QDoubleSpinBox *doubleSpinBox_5;
    QComboBox *comboBox_3;
    QLabel *label_13;
    QDoubleSpinBox *doubleSpinBox_6;
    QLabel *label_14;
    QWidget *widget;
    QWizardPage *solarFieldPage;
    QGridLayout *gridLayout_3;
    QTableWidget *positioonsTable;
    QPushButton *loadButton;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWizard *Wizard)
    {
        if (Wizard->objectName().isEmpty())
            Wizard->setObjectName(QString::fromUtf8("Wizard"));
        Wizard->resize(629, 504);
        heliostatPage = new QWizardPage();
        heliostatPage->setObjectName(QString::fromUtf8("heliostatPage"));
        gridLayout = new QGridLayout(heliostatPage);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(heliostatPage);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        line = new QFrame(heliostatPage);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 0, 1, 1, 3);

        doubleSpinBox = new QDoubleSpinBox(heliostatPage);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));

        gridLayout->addWidget(doubleSpinBox, 1, 2, 1, 1);

        label_2 = new QLabel(heliostatPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        label_3 = new QLabel(heliostatPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 1, 1, 1);

        doubleSpinBox_2 = new QDoubleSpinBox(heliostatPage);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));

        gridLayout->addWidget(doubleSpinBox_2, 2, 2, 1, 1);

        label_4 = new QLabel(heliostatPage);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        line_2 = new QFrame(heliostatPage);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 3, 1, 1, 3);

        label_5 = new QLabel(heliostatPage);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 5, 1, 1, 1);

        label_6 = new QLabel(heliostatPage);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 6, 1, 1, 1);

        doubleSpinBox_3 = new QDoubleSpinBox(heliostatPage);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));

        gridLayout->addWidget(doubleSpinBox_3, 6, 2, 1, 1);

        doubleSpinBox_4 = new QDoubleSpinBox(heliostatPage);
        doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));

        gridLayout->addWidget(doubleSpinBox_4, 5, 2, 1, 1);

        label_7 = new QLabel(heliostatPage);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 4, 1, 1, 1);

        comboBox = new QComboBox(heliostatPage);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout->addWidget(comboBox, 4, 2, 1, 1);

        label_8 = new QLabel(heliostatPage);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 7, 0, 1, 1);

        line_3 = new QFrame(heliostatPage);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 7, 1, 1, 3);

        comboBox_2 = new QComboBox(heliostatPage);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        gridLayout->addWidget(comboBox_2, 8, 2, 1, 1);

        label_9 = new QLabel(heliostatPage);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 8, 1, 1, 1);

        Wizard->addPage(heliostatPage);
        targetPage = new QWizardPage();
        targetPage->setObjectName(QString::fromUtf8("targetPage"));
        gridLayout_2 = new QGridLayout(targetPage);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_10 = new QLabel(targetPage);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 0, 0, 1, 1);

        label_11 = new QLabel(targetPage);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 3, 0, 1, 1);

        line_4 = new QFrame(targetPage);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_4, 3, 1, 1, 3);

        label_12 = new QLabel(targetPage);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_2->addWidget(label_12, 4, 1, 1, 1);

        doubleSpinBox_5 = new QDoubleSpinBox(targetPage);
        doubleSpinBox_5->setObjectName(QString::fromUtf8("doubleSpinBox_5"));

        gridLayout_2->addWidget(doubleSpinBox_5, 4, 2, 1, 1);

        comboBox_3 = new QComboBox(targetPage);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));

        gridLayout_2->addWidget(comboBox_3, 1, 2, 1, 1);

        label_13 = new QLabel(targetPage);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 5, 1, 1, 1);

        doubleSpinBox_6 = new QDoubleSpinBox(targetPage);
        doubleSpinBox_6->setObjectName(QString::fromUtf8("doubleSpinBox_6"));

        gridLayout_2->addWidget(doubleSpinBox_6, 5, 2, 1, 1);

        label_14 = new QLabel(targetPage);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 1, 1, 1, 1);

        widget = new QWidget(targetPage);
        widget->setObjectName(QString::fromUtf8("widget"));

        gridLayout_2->addWidget(widget, 2, 2, 1, 1);

        Wizard->addPage(targetPage);
        solarFieldPage = new QWizardPage();
        solarFieldPage->setObjectName(QString::fromUtf8("solarFieldPage"));
        gridLayout_3 = new QGridLayout(solarFieldPage);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        positioonsTable = new QTableWidget(solarFieldPage);
        if (positioonsTable->columnCount() < 3)
            positioonsTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        positioonsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        positioonsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        positioonsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        positioonsTable->setObjectName(QString::fromUtf8("positioonsTable"));
        positioonsTable->setAlternatingRowColors(true);
        positioonsTable->setShowGrid(false);
        positioonsTable->setSortingEnabled(false);

        gridLayout_3->addWidget(positioonsTable, 0, 0, 3, 1);

        loadButton = new QPushButton(solarFieldPage);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));

        gridLayout_3->addWidget(loadButton, 0, 1, 1, 1);

        pushButton_2 = new QPushButton(solarFieldPage);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout_3->addWidget(pushButton_2, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 2, 1, 1, 1);

        Wizard->addPage(solarFieldPage);

        retranslateUi(Wizard);

        QMetaObject::connectSlotsByName(Wizard);
    } // setupUi

    void retranslateUi(QWizard *Wizard)
    {
        Wizard->setWindowTitle(QApplication::translate("Wizard", "Wizard", 0, QApplication::UnicodeUTF8));
        heliostatPage->setTitle(QApplication::translate("Wizard", "Heliostat Definition", 0, QApplication::UnicodeUTF8));
        heliostatPage->setSubTitle(QApplication::translate("Wizard", "Define heliostat paramenters.", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Wizard", "Heliostat size", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Wizard", "Height:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Wizard", "Width:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Wizard", "Facet", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Wizard", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Wizard", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Wizard", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Wizard", "Canting", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Wizard", "TextLabel", 0, QApplication::UnicodeUTF8));
        targetPage->setTitle(QApplication::translate("Wizard", "Target", 0, QApplication::UnicodeUTF8));
        targetPage->setSubTitle(QApplication::translate("Wizard", "Target definition parameters.", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Wizard", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Wizard", "Position", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Wizard", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Wizard", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Wizard", "TextLabel", 0, QApplication::UnicodeUTF8));
        solarFieldPage->setTitle(QApplication::translate("Wizard", "Solar Field", 0, QApplication::UnicodeUTF8));
        solarFieldPage->setSubTitle(QApplication::translate("Wizard", "Define the  solar field heliostats positions.", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = positioonsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Wizard", "X", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = positioonsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Wizard", "y", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = positioonsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Wizard", "Z", 0, QApplication::UnicodeUTF8));
        loadButton->setText(QApplication::translate("Wizard", "Load", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Wizard", "Clear", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Wizard: public Ui_Wizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELIOSTATFIELDWIZARD_H
