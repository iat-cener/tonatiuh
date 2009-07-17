/*
 * MainWindow.cpp
 *
 *  Created on: Jul 14, 2009
 *      Author: mblanco
 *
 */

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>
#include <QSettings>


#include "EuroDishAnalysis.h"
#include "LS3Analysis.h"
#include "MainWindow.h"
#include "SolarFurnaceAnalysis.h"
#include "StatusBarWidget.h"
#include "Trace.h"


MainWindow::MainWindow( QWidget* parent, Qt::WindowFlags flags )
:QMainWindow( parent, flags )
{
	Trace trace( "MainWindow::MainWindow", false );
	setupUi( this );

	setWindowTitle(  tr( "Photons Analyzer" ) );

	m_modelAnalysisList<< new EuroDishAnalysis();
	m_modelAnalysisList<< new LS3Analysis();
	m_modelAnalysisList<< new SolarFurnaceAnalysis();
	for( int index = 0; index < m_modelAnalysisList.count(); index++ )
		modelCombo->addItem( m_modelAnalysisList.at(index)->ModelName() );

	QPushButton* applyButton = buttonBox->button( QDialogButtonBox::Apply );
	connect( applyButton, SIGNAL( clicked( bool ) ), this, SLOT( RunAnalysis( bool ) ) );

	ReadSettings();
}

MainWindow::~MainWindow()
{
	Trace trace( "MainWindow::~MainWindow", false );
}

void MainWindow::on_openButton_clicked()
{
    Trace trace( "MainWindow::on_openButton_clicked", false );

    QString fileName = QFileDialog::getExistingDirectory( this, tr("Photon Map Directory"),
       				tr( "." ),
       	            QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks );

    if( !fileName.isEmpty() )	directoryLine->setText( fileName );
}

void MainWindow::on_saveButton_clicked()
{
    Trace trace( "MainWindow::on_saveButton_clicked", false );

    QString fileName = QFileDialog::getExistingDirectory( this, tr("Photon Map Directory"),
    				tr( "." ),
    	            QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks );

    if( !fileName.isEmpty() )	resultsDirectoryLine->setText( fileName );
}

void MainWindow::RunAnalysis( bool )
{
    Trace trace( "MainWindow::RunAnalysis", false );

    QStatusBar* statusbar = statusBar();

    StatusBarWidget* statusBarWidget = new StatusBarWidget;
    statusbar->addWidget( statusBarWidget, width() );
    statusBarWidget->SetText( "Running Analysis" );
    statusBarWidget->SetProgress( 0 );

    if( directoryLine->text().isEmpty() )
	{
		QMessageBox::information( this, "Tonatiuh Action",
									  "Photon Map data directory no defined", 1);
		statusbar->clearMessage();
		delete statusbar;
		return;
	}

    if( resultsDirectoryLine->text().isEmpty() )
	{
		QMessageBox::information( this, "Photons Analyzer",
									  "Directory for results no defined", 1);
		statusbar->clearMessage();
		delete statusbar;
		return;
	}

    PhotonMapAnalysis* analysisType = m_modelAnalysisList.at( modelCombo->currentIndex() );
    analysisType->SetAnalysisData( directoryLine->text(), resultsDirectoryLine->text(), matrixWidthSpin->value(), matrixHeightSpin->value() );

    connect( analysisType, SIGNAL( AnalysisProgressChanged( int ) ), statusBarWidget, SLOT( SetProgress( int ) ) );

    if( tonatiuhRadio->isChecked() )	analysisType->RunTonatiuh();
    else	analysisType->RunSolTrace();

    statusBarWidget->SetProgress( 100 );
    statusBarWidget->SetText( "Analysis successfully finished" );

    statusbar->removeWidget( statusBarWidget );
    delete statusBarWidget;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    Trace trace( "MainWindow::closeEvent", false );

    WriteSettings();
    event->accept();
}

void MainWindow::ReadSettings()
{
    Trace trace( "MainWindow::ReadSettings", false );

    QSettings settings( "NREL UTB CENER", "PhotonsAnalyzer" );
    QRect rect = settings.value( "geometry", QRect(200, 200, 400, 400 ) ).toRect();
    move( rect.topLeft() );
    resize( rect.size() );
    directoryLine->setText( settings.value( "dataDirectory", "" ).toString() );

    modelCombo->setCurrentIndex( settings.value( "modelIndex", 0 ).toInt() );

    int fileType = settings.value( "fileType", 0 ).toInt();
    if( fileType == 0 ) tonatiuhRadio->setChecked( true );
    else solTraceRadio->setChecked( true );

    matrixWidthSpin->setValue( settings.value( "matrixWidth", 50 ).toInt() );
    matrixHeightSpin->setValue( settings.value( "matrixHeight",  50 ).toInt() );

	resultsDirectoryLine->setText( settings.value( "resultsDirectory", "" ).toString() );
}

void MainWindow::WriteSettings()
{
    Trace trace( "MainWindow::WriteSettings", false );

	QSettings settings( "NREL UTB CENER", "PhotonsAnalyzer" );
	settings.setValue( "geometry", geometry() );
    settings.setValue( "dataDirectory", directoryLine->text() );

    settings.setValue( "modelIndex", modelCombo->currentIndex() );

    if( tonatiuhRadio->isChecked() ) settings.setValue( "fileType", 0 );
    else settings.setValue( "fileType", 1 );

    settings.setValue( "matrixWidth", matrixWidthSpin->value() );
    settings.setValue( "matrixHeight", matrixHeightSpin->value() );

    settings.setValue( "resultsDirectory", resultsDirectoryLine->text() );
}
