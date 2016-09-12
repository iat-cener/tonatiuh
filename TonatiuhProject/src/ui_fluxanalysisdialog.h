/********************************************************************************
** Form generated from reading UI file 'fluxanalysisdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLUXANALYSISDIALOG_H
#define UI_FLUXANALYSISDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_FluxAnalysisDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *dialogMainWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QWidget *parametersWidget;
    QVBoxLayout *parametersLayout;
    QGroupBox *inputWidget;
    QGridLayout *gridLayout;
    QLabel *surfaceSideLabel;
    QFrame *line_2;
    QSpacerItem *horizontalSpacer;
    QLabel *nRaysLabel;
    QSpacerItem *horizontalSpacer_6;
    QComboBox *sidesCombo;
    QLineEdit *surfaceEdit;
    QPushButton *selectButton;
    QCheckBox *appendCheck;
    QLabel *surfaceTitleLabel;
    QLabel *surfaceLabel;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_9;
    QFrame *line_3;
    QSpacerItem *horizontalSpacer_16;
    QSpacerItem *horizontalSpacer_17;
    QWidget *runWidget;
    QHBoxLayout *runWidgetLayout;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *runButton;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *nRaysLine;
    QGroupBox *analysisWidget;
    QGridLayout *gridLayout_4;
    QFrame *line;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_8;
    QLabel *widthDivisionsLabel;
    QLabel *heightDivisionsLabel;
    QLineEdit *gridWidthLine;
    QLineEdit *gridHeightLine;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *labelsWidget;
    QGridLayout *gridLayout_6;
    QLabel *lenghtUnitLabel;
    QSpacerItem *horizontalSpacer_18;
    QLabel *powerUnitLabel;
    QLineEdit *lengthUnitLine;
    QSpacerItem *horizontalSpacer_19;
    QSpacerItem *horizontalSpacer_20;
    QLineEdit *powerUnitLine;
    QGroupBox *exportWidget;
    QGridLayout *gridLayout_8;
    QCheckBox *saveCoordsCheckBox;
    QLabel *directoryLabel;
    QLabel *storeTypeLabel;
    QLabel *fileNameLabel;
    QSpacerItem *horizontalSpacer_11;
    QComboBox *storeTypeCombo;
    QSpacerItem *horizontalSpacer_14;
    QFrame *line_4;
    QPushButton *selectFileButton;
    QLineEdit *fileDirEdit;
    QLineEdit *fileNameEdit;
    QSpacerItem *horizontalSpacer_15;
    QWidget *exportButtonWidget;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_12;
    QPushButton *exportButton;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *verticalSpacer_3;
    QWidget *resultsWidget;
    QVBoxLayout *resultsLayout;
    QCustomPlot *contourPlotWidget;
    QWidget *sectorsWidget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *verticalSectorWidget;
    QGridLayout *gridLayout_2;
    QCustomPlot *verticalSectorPlot;
    QLabel *xCoordlabel;
    QDoubleSpinBox *hSectorXCoordSpin;
    QWidget *horizontalSectionWidget;
    QGridLayout *gridLayout_5;
    QCustomPlot *horizontaSectorPlot;
    QDoubleSpinBox *hSectorYCoordSpin;
    QLabel *yCoordlabel;
    QWidget *statisticalWidget;
    QGridLayout *gridLayout_3;
    QLabel *averageFluxLabel;
    QLabel *totalPowerValue;
    QLabel *minimumFluxValue;
    QLabel *maxCoordinatesLabel;
    QLabel *centroidValue;
    QLabel *uniformityValue;
    QLabel *uniformityLabel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *minimumFluxLabel;
    QLabel *totalPowerLabel;
    QLabel *errorLabel;
    QSpacerItem *horizontalSpacer_10;
    QLabel *maximumFluxValue;
    QLabel *averageFluxValue;
    QLabel *maximumFluxLabel;
    QLabel *centroidLabel;
    QLabel *errorValue;
    QLabel *maxCoordinatesValue;

    void setupUi(QDialog *FluxAnalysisDialog)
    {
        if (FluxAnalysisDialog->objectName().isEmpty())
            FluxAnalysisDialog->setObjectName(QStringLiteral("FluxAnalysisDialog"));
        FluxAnalysisDialog->resize(1160, 909);
        FluxAnalysisDialog->setFocusPolicy(Qt::NoFocus);
        verticalLayout = new QVBoxLayout(FluxAnalysisDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        dialogMainWidget = new QWidget(FluxAnalysisDialog);
        dialogMainWidget->setObjectName(QStringLiteral("dialogMainWidget"));
        horizontalLayout = new QHBoxLayout(dialogMainWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(dialogMainWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        parametersWidget = new QWidget(splitter);
        parametersWidget->setObjectName(QStringLiteral("parametersWidget"));
        parametersLayout = new QVBoxLayout(parametersWidget);
        parametersLayout->setObjectName(QStringLiteral("parametersLayout"));
        parametersLayout->setContentsMargins(0, 0, 0, 0);
        inputWidget = new QGroupBox(parametersWidget);
        inputWidget->setObjectName(QStringLiteral("inputWidget"));
        gridLayout = new QGridLayout(inputWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        surfaceSideLabel = new QLabel(inputWidget);
        surfaceSideLabel->setObjectName(QStringLiteral("surfaceSideLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(surfaceSideLabel->sizePolicy().hasHeightForWidth());
        surfaceSideLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(surfaceSideLabel, 4, 1, 1, 1);

        line_2 = new QFrame(inputWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 12, 0, 1, 5);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 5, 0, 1, 5);

        nRaysLabel = new QLabel(inputWidget);
        nRaysLabel->setObjectName(QStringLiteral("nRaysLabel"));
        nRaysLabel->setMinimumSize(QSize(80, 0));

        gridLayout->addWidget(nRaysLabel, 13, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 4, 3, 1, 1);

        sidesCombo = new QComboBox(inputWidget);
        sidesCombo->setObjectName(QStringLiteral("sidesCombo"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sidesCombo->sizePolicy().hasHeightForWidth());
        sidesCombo->setSizePolicy(sizePolicy1);
        sidesCombo->setMinimumSize(QSize(125, 0));

        gridLayout->addWidget(sidesCombo, 4, 2, 1, 1);

        surfaceEdit = new QLineEdit(inputWidget);
        surfaceEdit->setObjectName(QStringLiteral("surfaceEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(surfaceEdit->sizePolicy().hasHeightForWidth());
        surfaceEdit->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(surfaceEdit, 3, 2, 1, 2);

        selectButton = new QPushButton(inputWidget);
        selectButton->setObjectName(QStringLiteral("selectButton"));
        selectButton->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(selectButton, 3, 4, 1, 1);

        appendCheck = new QCheckBox(inputWidget);
        appendCheck->setObjectName(QStringLiteral("appendCheck"));
        appendCheck->setEnabled(false);
        appendCheck->setChecked(false);

        gridLayout->addWidget(appendCheck, 14, 1, 1, 2);

        surfaceTitleLabel = new QLabel(inputWidget);
        surfaceTitleLabel->setObjectName(QStringLiteral("surfaceTitleLabel"));

        gridLayout->addWidget(surfaceTitleLabel, 0, 0, 1, 1);

        surfaceLabel = new QLabel(inputWidget);
        surfaceLabel->setObjectName(QStringLiteral("surfaceLabel"));
        sizePolicy.setHeightForWidth(surfaceLabel->sizePolicy().hasHeightForWidth());
        surfaceLabel->setSizePolicy(sizePolicy);
        surfaceLabel->setMinimumSize(QSize(80, 0));

        gridLayout->addWidget(surfaceLabel, 3, 1, 1, 1);

        label_2 = new QLabel(inputWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 11, 0, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(75, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_9, 13, 0, 1, 1);

        line_3 = new QFrame(inputWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 1, 0, 1, 5);

        horizontalSpacer_16 = new QSpacerItem(75, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_16, 4, 4, 1, 1);

        horizontalSpacer_17 = new QSpacerItem(75, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_17, 3, 0, 1, 1);

        runWidget = new QWidget(inputWidget);
        runWidget->setObjectName(QStringLiteral("runWidget"));
        runWidgetLayout = new QHBoxLayout(runWidget);
        runWidgetLayout->setSpacing(0);
        runWidgetLayout->setObjectName(QStringLiteral("runWidgetLayout"));
        runWidgetLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        runWidgetLayout->addItem(horizontalSpacer_4);

        runButton = new QPushButton(runWidget);
        runButton->setObjectName(QStringLiteral("runButton"));
        runButton->setFocusPolicy(Qt::NoFocus);

        runWidgetLayout->addWidget(runButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        runWidgetLayout->addItem(horizontalSpacer_3);

        runButton->raise();

        gridLayout->addWidget(runWidget, 15, 0, 1, 5);

        nRaysLine = new QLineEdit(inputWidget);
        nRaysLine->setObjectName(QStringLiteral("nRaysLine"));

        gridLayout->addWidget(nRaysLine, 13, 2, 1, 1);


        parametersLayout->addWidget(inputWidget);

        analysisWidget = new QGroupBox(parametersWidget);
        analysisWidget->setObjectName(QStringLiteral("analysisWidget"));
        gridLayout_4 = new QGridLayout(analysisWidget);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        line = new QFrame(analysisWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(line, 1, 0, 1, 5);

        label_3 = new QLabel(analysisWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_4->addWidget(label_3, 0, 0, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(75, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_8, 2, 0, 1, 1);

        widthDivisionsLabel = new QLabel(analysisWidget);
        widthDivisionsLabel->setObjectName(QStringLiteral("widthDivisionsLabel"));
        sizePolicy.setHeightForWidth(widthDivisionsLabel->sizePolicy().hasHeightForWidth());
        widthDivisionsLabel->setSizePolicy(sizePolicy);
        widthDivisionsLabel->setMinimumSize(QSize(80, 0));

        gridLayout_4->addWidget(widthDivisionsLabel, 2, 1, 1, 1);

        heightDivisionsLabel = new QLabel(analysisWidget);
        heightDivisionsLabel->setObjectName(QStringLiteral("heightDivisionsLabel"));

        gridLayout_4->addWidget(heightDivisionsLabel, 4, 1, 1, 1);

        gridWidthLine = new QLineEdit(analysisWidget);
        gridWidthLine->setObjectName(QStringLiteral("gridWidthLine"));

        gridLayout_4->addWidget(gridWidthLine, 2, 2, 1, 1);

        gridHeightLine = new QLineEdit(analysisWidget);
        gridHeightLine->setObjectName(QStringLiteral("gridHeightLine"));

        gridLayout_4->addWidget(gridHeightLine, 4, 2, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_7, 2, 3, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(75, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_5, 2, 4, 1, 1);


        parametersLayout->addWidget(analysisWidget);

        labelsWidget = new QGroupBox(parametersWidget);
        labelsWidget->setObjectName(QStringLiteral("labelsWidget"));
        gridLayout_6 = new QGridLayout(labelsWidget);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        lenghtUnitLabel = new QLabel(labelsWidget);
        lenghtUnitLabel->setObjectName(QStringLiteral("lenghtUnitLabel"));
        lenghtUnitLabel->setMinimumSize(QSize(80, 0));

        gridLayout_6->addWidget(lenghtUnitLabel, 0, 1, 1, 1);

        horizontalSpacer_18 = new QSpacerItem(75, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_18, 0, 0, 1, 1);

        powerUnitLabel = new QLabel(labelsWidget);
        powerUnitLabel->setObjectName(QStringLiteral("powerUnitLabel"));

        gridLayout_6->addWidget(powerUnitLabel, 1, 1, 1, 1);

        lengthUnitLine = new QLineEdit(labelsWidget);
        lengthUnitLine->setObjectName(QStringLiteral("lengthUnitLine"));

        gridLayout_6->addWidget(lengthUnitLine, 0, 2, 1, 1);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_19, 0, 3, 1, 1);

        horizontalSpacer_20 = new QSpacerItem(75, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_20, 0, 4, 1, 1);

        powerUnitLine = new QLineEdit(labelsWidget);
        powerUnitLine->setObjectName(QStringLiteral("powerUnitLine"));

        gridLayout_6->addWidget(powerUnitLine, 1, 2, 1, 1);


        parametersLayout->addWidget(labelsWidget);

        exportWidget = new QGroupBox(parametersWidget);
        exportWidget->setObjectName(QStringLiteral("exportWidget"));
        gridLayout_8 = new QGridLayout(exportWidget);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        saveCoordsCheckBox = new QCheckBox(exportWidget);
        saveCoordsCheckBox->setObjectName(QStringLiteral("saveCoordsCheckBox"));

        gridLayout_8->addWidget(saveCoordsCheckBox, 2, 2, 1, 1);

        directoryLabel = new QLabel(exportWidget);
        directoryLabel->setObjectName(QStringLiteral("directoryLabel"));
        sizePolicy.setHeightForWidth(directoryLabel->sizePolicy().hasHeightForWidth());
        directoryLabel->setSizePolicy(sizePolicy);

        gridLayout_8->addWidget(directoryLabel, 3, 1, 1, 1);

        storeTypeLabel = new QLabel(exportWidget);
        storeTypeLabel->setObjectName(QStringLiteral("storeTypeLabel"));
        storeTypeLabel->setEnabled(true);
        sizePolicy.setHeightForWidth(storeTypeLabel->sizePolicy().hasHeightForWidth());
        storeTypeLabel->setSizePolicy(sizePolicy);
        storeTypeLabel->setMinimumSize(QSize(80, 0));
        storeTypeLabel->setBaseSize(QSize(0, 0));

        gridLayout_8->addWidget(storeTypeLabel, 1, 1, 1, 1);

        fileNameLabel = new QLabel(exportWidget);
        fileNameLabel->setObjectName(QStringLiteral("fileNameLabel"));
        fileNameLabel->setMinimumSize(QSize(0, 25));

        gridLayout_8->addWidget(fileNameLabel, 4, 1, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(75, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_11, 4, 4, 1, 1);

        storeTypeCombo = new QComboBox(exportWidget);
        storeTypeCombo->setObjectName(QStringLiteral("storeTypeCombo"));
        sizePolicy1.setHeightForWidth(storeTypeCombo->sizePolicy().hasHeightForWidth());
        storeTypeCombo->setSizePolicy(sizePolicy1);
        storeTypeCombo->setMinimumSize(QSize(85, 0));
        storeTypeCombo->setMaximumSize(QSize(16777215, 16777215));
        storeTypeCombo->setAcceptDrops(false);

        gridLayout_8->addWidget(storeTypeCombo, 1, 2, 1, 1);

        horizontalSpacer_14 = new QSpacerItem(75, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_14, 1, 0, 1, 1);

        line_4 = new QFrame(exportWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_8->addWidget(line_4, 0, 0, 1, 5);

        selectFileButton = new QPushButton(exportWidget);
        selectFileButton->setObjectName(QStringLiteral("selectFileButton"));
        selectFileButton->setMinimumSize(QSize(0, 25));
        selectFileButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        selectFileButton->setIcon(icon);

        gridLayout_8->addWidget(selectFileButton, 3, 4, 1, 1);

        fileDirEdit = new QLineEdit(exportWidget);
        fileDirEdit->setObjectName(QStringLiteral("fileDirEdit"));
        fileDirEdit->setMinimumSize(QSize(0, 0));

        gridLayout_8->addWidget(fileDirEdit, 3, 2, 1, 2);

        fileNameEdit = new QLineEdit(exportWidget);
        fileNameEdit->setObjectName(QStringLiteral("fileNameEdit"));

        gridLayout_8->addWidget(fileNameEdit, 4, 2, 1, 2);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_15, 1, 3, 1, 1);

        exportButtonWidget = new QWidget(exportWidget);
        exportButtonWidget->setObjectName(QStringLiteral("exportButtonWidget"));
        horizontalLayout_5 = new QHBoxLayout(exportButtonWidget);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_12 = new QSpacerItem(196, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_12);

        exportButton = new QPushButton(exportButtonWidget);
        exportButton->setObjectName(QStringLiteral("exportButton"));
        exportButton->setMaximumSize(QSize(16777215, 16777215));
        exportButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_5->addWidget(exportButton);

        horizontalSpacer_13 = new QSpacerItem(196, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_13);


        gridLayout_8->addWidget(exportButtonWidget, 7, 0, 7, 5);


        parametersLayout->addWidget(exportWidget);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        parametersLayout->addItem(verticalSpacer_3);

        splitter->addWidget(parametersWidget);
        resultsWidget = new QWidget(splitter);
        resultsWidget->setObjectName(QStringLiteral("resultsWidget"));
        resultsLayout = new QVBoxLayout(resultsWidget);
        resultsLayout->setObjectName(QStringLiteral("resultsLayout"));
        resultsLayout->setContentsMargins(0, 0, 0, 0);
        contourPlotWidget = new QCustomPlot(resultsWidget);
        contourPlotWidget->setObjectName(QStringLiteral("contourPlotWidget"));
        sizePolicy.setHeightForWidth(contourPlotWidget->sizePolicy().hasHeightForWidth());
        contourPlotWidget->setSizePolicy(sizePolicy);
        contourPlotWidget->setMinimumSize(QSize(0, 0));

        resultsLayout->addWidget(contourPlotWidget);

        sectorsWidget = new QWidget(resultsWidget);
        sectorsWidget->setObjectName(QStringLiteral("sectorsWidget"));
        sectorsWidget->setMinimumSize(QSize(0, 0));
        horizontalLayout_2 = new QHBoxLayout(sectorsWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalSectorWidget = new QWidget(sectorsWidget);
        verticalSectorWidget->setObjectName(QStringLiteral("verticalSectorWidget"));
        verticalSectorWidget->setMinimumSize(QSize(0, 0));
        gridLayout_2 = new QGridLayout(verticalSectorWidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, -1, 0, 0);
        verticalSectorPlot = new QCustomPlot(verticalSectorWidget);
        verticalSectorPlot->setObjectName(QStringLiteral("verticalSectorPlot"));

        gridLayout_2->addWidget(verticalSectorPlot, 1, 0, 1, 2);

        xCoordlabel = new QLabel(verticalSectorWidget);
        xCoordlabel->setObjectName(QStringLiteral("xCoordlabel"));

        gridLayout_2->addWidget(xCoordlabel, 0, 0, 1, 1);

        hSectorXCoordSpin = new QDoubleSpinBox(verticalSectorWidget);
        hSectorXCoordSpin->setObjectName(QStringLiteral("hSectorXCoordSpin"));

        gridLayout_2->addWidget(hSectorXCoordSpin, 0, 1, 1, 1);


        horizontalLayout_2->addWidget(verticalSectorWidget);

        horizontalSectionWidget = new QWidget(sectorsWidget);
        horizontalSectionWidget->setObjectName(QStringLiteral("horizontalSectionWidget"));
        gridLayout_5 = new QGridLayout(horizontalSectionWidget);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, -1, 0, 0);
        horizontaSectorPlot = new QCustomPlot(horizontalSectionWidget);
        horizontaSectorPlot->setObjectName(QStringLiteral("horizontaSectorPlot"));

        gridLayout_5->addWidget(horizontaSectorPlot, 1, 0, 1, 2);

        hSectorYCoordSpin = new QDoubleSpinBox(horizontalSectionWidget);
        hSectorYCoordSpin->setObjectName(QStringLiteral("hSectorYCoordSpin"));
        hSectorYCoordSpin->setMinimum(0);
        hSectorYCoordSpin->setValue(0);

        gridLayout_5->addWidget(hSectorYCoordSpin, 0, 1, 1, 1);

        yCoordlabel = new QLabel(horizontalSectionWidget);
        yCoordlabel->setObjectName(QStringLiteral("yCoordlabel"));

        gridLayout_5->addWidget(yCoordlabel, 0, 0, 1, 1);


        horizontalLayout_2->addWidget(horizontalSectionWidget);


        resultsLayout->addWidget(sectorsWidget);

        statisticalWidget = new QWidget(resultsWidget);
        statisticalWidget->setObjectName(QStringLiteral("statisticalWidget"));
        gridLayout_3 = new QGridLayout(statisticalWidget);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        averageFluxLabel = new QLabel(statisticalWidget);
        averageFluxLabel->setObjectName(QStringLiteral("averageFluxLabel"));
        averageFluxLabel->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(averageFluxLabel, 3, 0, 1, 1);

        totalPowerValue = new QLabel(statisticalWidget);
        totalPowerValue->setObjectName(QStringLiteral("totalPowerValue"));

        gridLayout_3->addWidget(totalPowerValue, 1, 1, 1, 1);

        minimumFluxValue = new QLabel(statisticalWidget);
        minimumFluxValue->setObjectName(QStringLiteral("minimumFluxValue"));

        gridLayout_3->addWidget(minimumFluxValue, 2, 1, 1, 1);

        maxCoordinatesLabel = new QLabel(statisticalWidget);
        maxCoordinatesLabel->setObjectName(QStringLiteral("maxCoordinatesLabel"));
        maxCoordinatesLabel->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(maxCoordinatesLabel, 6, 0, 1, 1);

        centroidValue = new QLabel(statisticalWidget);
        centroidValue->setObjectName(QStringLiteral("centroidValue"));

        gridLayout_3->addWidget(centroidValue, 9, 1, 1, 1);

        uniformityValue = new QLabel(statisticalWidget);
        uniformityValue->setObjectName(QStringLiteral("uniformityValue"));

        gridLayout_3->addWidget(uniformityValue, 8, 1, 1, 1);

        uniformityLabel = new QLabel(statisticalWidget);
        uniformityLabel->setObjectName(QStringLiteral("uniformityLabel"));
        uniformityLabel->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(uniformityLabel, 8, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 10, 3, 1, 1);

        minimumFluxLabel = new QLabel(statisticalWidget);
        minimumFluxLabel->setObjectName(QStringLiteral("minimumFluxLabel"));
        minimumFluxLabel->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(minimumFluxLabel, 2, 0, 1, 1);

        totalPowerLabel = new QLabel(statisticalWidget);
        totalPowerLabel->setObjectName(QStringLiteral("totalPowerLabel"));
        sizePolicy.setHeightForWidth(totalPowerLabel->sizePolicy().hasHeightForWidth());
        totalPowerLabel->setSizePolicy(sizePolicy);
        totalPowerLabel->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(totalPowerLabel, 1, 0, 1, 1);

        errorLabel = new QLabel(statisticalWidget);
        errorLabel->setObjectName(QStringLiteral("errorLabel"));
        errorLabel->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(errorLabel, 7, 0, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_10, 0, 0, 1, 1);

        maximumFluxValue = new QLabel(statisticalWidget);
        maximumFluxValue->setObjectName(QStringLiteral("maximumFluxValue"));

        gridLayout_3->addWidget(maximumFluxValue, 4, 1, 1, 1);

        averageFluxValue = new QLabel(statisticalWidget);
        averageFluxValue->setObjectName(QStringLiteral("averageFluxValue"));

        gridLayout_3->addWidget(averageFluxValue, 3, 1, 1, 1);

        maximumFluxLabel = new QLabel(statisticalWidget);
        maximumFluxLabel->setObjectName(QStringLiteral("maximumFluxLabel"));
        maximumFluxLabel->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(maximumFluxLabel, 4, 0, 1, 1);

        centroidLabel = new QLabel(statisticalWidget);
        centroidLabel->setObjectName(QStringLiteral("centroidLabel"));
        centroidLabel->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(centroidLabel, 9, 0, 1, 1);

        errorValue = new QLabel(statisticalWidget);
        errorValue->setObjectName(QStringLiteral("errorValue"));

        gridLayout_3->addWidget(errorValue, 7, 1, 1, 1);

        maxCoordinatesValue = new QLabel(statisticalWidget);
        maxCoordinatesValue->setObjectName(QStringLiteral("maxCoordinatesValue"));

        gridLayout_3->addWidget(maxCoordinatesValue, 6, 1, 1, 1);


        resultsLayout->addWidget(statisticalWidget);

        splitter->addWidget(resultsWidget);

        horizontalLayout->addWidget(splitter);


        verticalLayout->addWidget(dialogMainWidget);

        QWidget::setTabOrder(surfaceEdit, sidesCombo);
        QWidget::setTabOrder(sidesCombo, appendCheck);
        QWidget::setTabOrder(appendCheck, storeTypeCombo);
        QWidget::setTabOrder(storeTypeCombo, saveCoordsCheckBox);
        QWidget::setTabOrder(saveCoordsCheckBox, fileDirEdit);
        QWidget::setTabOrder(fileDirEdit, fileNameEdit);
        QWidget::setTabOrder(fileNameEdit, hSectorXCoordSpin);
        QWidget::setTabOrder(hSectorXCoordSpin, hSectorYCoordSpin);

        retranslateUi(FluxAnalysisDialog);

        QMetaObject::connectSlotsByName(FluxAnalysisDialog);
    } // setupUi

    void retranslateUi(QDialog *FluxAnalysisDialog)
    {
        FluxAnalysisDialog->setWindowTitle(QApplication::translate("FluxAnalysisDialog", "Tonatiuh", 0));
        inputWidget->setTitle(QApplication::translate("FluxAnalysisDialog", "Settings", 0));
        surfaceSideLabel->setText(QApplication::translate("FluxAnalysisDialog", "Active side:", 0));
        nRaysLabel->setText(QApplication::translate("FluxAnalysisDialog", "Number of rays:", 0));
        surfaceEdit->setText(QApplication::translate("FluxAnalysisDialog", "---", 0));
        selectButton->setText(QApplication::translate("FluxAnalysisDialog", "Select", 0));
        appendCheck->setText(QApplication::translate("FluxAnalysisDialog", "Append to previous simulation", 0));
        surfaceTitleLabel->setText(QApplication::translate("FluxAnalysisDialog", "Surface", 0));
        surfaceLabel->setText(QApplication::translate("FluxAnalysisDialog", "Surface URL:", 0));
        label_2->setText(QApplication::translate("FluxAnalysisDialog", "Ray tracing", 0));
        runButton->setText(QApplication::translate("FluxAnalysisDialog", "Run", 0));
        nRaysLine->setText(QApplication::translate("FluxAnalysisDialog", "10000", 0));
        analysisWidget->setTitle(QApplication::translate("FluxAnalysisDialog", "Analysis Settings", 0));
        label_3->setText(QApplication::translate("FluxAnalysisDialog", "Grid divisions", 0));
        widthDivisionsLabel->setText(QApplication::translate("FluxAnalysisDialog", "Width:", 0));
        heightDivisionsLabel->setText(QApplication::translate("FluxAnalysisDialog", "Height:", 0));
        gridWidthLine->setInputMask(QString());
        gridWidthLine->setText(QApplication::translate("FluxAnalysisDialog", "20", 0));
        gridHeightLine->setInputMask(QString());
        gridHeightLine->setText(QApplication::translate("FluxAnalysisDialog", "20", 0));
        labelsWidget->setTitle(QApplication::translate("FluxAnalysisDialog", "Labels", 0));
        lenghtUnitLabel->setText(QApplication::translate("FluxAnalysisDialog", "Length:", 0));
        powerUnitLabel->setText(QApplication::translate("FluxAnalysisDialog", "Power:", 0));
        exportWidget->setTitle(QApplication::translate("FluxAnalysisDialog", "Export Settings", 0));
        saveCoordsCheckBox->setText(QApplication::translate("FluxAnalysisDialog", "Save coordinates of the grid", 0));
        directoryLabel->setText(QApplication::translate("FluxAnalysisDialog", "Directory:", 0));
        storeTypeLabel->setText(QApplication::translate("FluxAnalysisDialog", "Store type:", 0));
        fileNameLabel->setText(QApplication::translate("FluxAnalysisDialog", "File Name:", 0));
        storeTypeCombo->clear();
        storeTypeCombo->insertItems(0, QStringList()
         << QApplication::translate("FluxAnalysisDialog", "ASCII", 0)
         << QApplication::translate("FluxAnalysisDialog", "IMAGE.PNG", 0)
         << QApplication::translate("FluxAnalysisDialog", "IMAGE.JPG", 0)
        );
        selectFileButton->setText(QString());
        exportButton->setText(QApplication::translate("FluxAnalysisDialog", "Export", 0));
        xCoordlabel->setText(QApplication::translate("FluxAnalysisDialog", "X Coord:", 0));
        yCoordlabel->setText(QApplication::translate("FluxAnalysisDialog", "Y Coord:", 0));
        averageFluxLabel->setText(QApplication::translate("FluxAnalysisDialog", "Average Flux: ", 0));
        totalPowerValue->setText(QApplication::translate("FluxAnalysisDialog", "0.0", 0));
        minimumFluxValue->setText(QApplication::translate("FluxAnalysisDialog", "0.0", 0));
        maxCoordinatesLabel->setText(QApplication::translate("FluxAnalysisDialog", "Maximum Flux Coordinates:", 0));
        centroidValue->setText(QApplication::translate("FluxAnalysisDialog", ";", 0));
        uniformityValue->setText(QApplication::translate("FluxAnalysisDialog", "0.0", 0));
        uniformityLabel->setText(QApplication::translate("FluxAnalysisDialog", "Uniformity:", 0));
        minimumFluxLabel->setText(QApplication::translate("FluxAnalysisDialog", "Minimum Flux:", 0));
        totalPowerLabel->setText(QApplication::translate("FluxAnalysisDialog", "Total Power:", 0));
        errorLabel->setText(QApplication::translate("FluxAnalysisDialog", "Error:", 0));
        maximumFluxValue->setText(QApplication::translate("FluxAnalysisDialog", "0.0", 0));
        averageFluxValue->setText(QApplication::translate("FluxAnalysisDialog", "0.0", 0));
        maximumFluxLabel->setText(QApplication::translate("FluxAnalysisDialog", "Maximum Flux:", 0));
        centroidLabel->setText(QApplication::translate("FluxAnalysisDialog", "Centroid:", 0));
        errorValue->setText(QApplication::translate("FluxAnalysisDialog", "0.0", 0));
        maxCoordinatesValue->setText(QApplication::translate("FluxAnalysisDialog", ";", 0));
    } // retranslateUi

};

namespace Ui {
    class FluxAnalysisDialog: public Ui_FluxAnalysisDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLUXANALYSISDIALOG_H
