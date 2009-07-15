/*
 * MainWindow.cpp
 *
 *  Created on: Jul 14, 2009
 *      Author: mblanco
 *
 */

#include <QFileDialog>
#include <QMessageBox>

#include "EuroDishAnalysis.h"
#include "MainWindow.h"
#include "Trace.h"


MainWindow::MainWindow( QWidget* parent, Qt::WindowFlags flags )
:QMainWindow( parent, flags )
{
	Trace trace( "MainWindow::MainWindow", false );
	setupUi( this );

	setWindowTitle(  tr( "Photons Analyzer" ) );

	m_modelAnalysisList<< new EuroDishAnalysis();
	for( int index = 0; index < m_modelAnalysisList.count(); index++ )
		modelCombo->addItem( m_modelAnalysisList.at(index)->ModelName() );

	QPushButton* applyButton = buttonBox->button( QDialogButtonBox::Apply );
	connect( applyButton, SIGNAL( clicked( bool ) ), this, SLOT( RunAnalysis( bool ) ) );
}

MainWindow::~MainWindow()
{
	Trace trace( "MainWindow::~MainWindow", false );
}

void MainWindow::on_actionOpen_triggered()
{
    Trace trace( "MainWindow::on_actionOpen_triggered", false );
    if ( OkToContinue() )
    {
        QString fileName = QFileDialog::getOpenFileName( this,
                               tr( "Open binary data files" ), ".",
                               tr( "Binary data files (*.dat)" ) );
        if ( !fileName.isEmpty() ) StartOver( fileName );
    }
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

    QStatusBar* statusbar = new QStatusBar;
    setStatusBar( statusbar );
    statusbar->showMessage( tr( "Running Analysis" ), 0 );

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
    analysisType->Run();

    statusbar->clearMessage();
    delete statusbar;
}

bool MainWindow::OkToContinue()
{
	return true;
}

bool MainWindow::StartOver( const QString& /* fileName */ )
{
    Trace trace( "MainWindow::StartOver", false );
/*
	actionDisplay_rays->setEnabled( false );
	if( m_pRays && ( m_document->GetRoot()->findChild( m_pRays )!= -1 ) )
	{
		m_document->GetRoot()->removeChild(m_pRays);
		while ( m_pRays->getRefCount( ) > 1 ) m_pRays->unref();
		m_pRays = 0;

	}
	m_commandStack->clear();
	SetEnabled_SunPositionCalculator( 0 );

	QStatusBar* statusbar = new QStatusBar;
	setStatusBar( statusbar );

    if( fileName.isEmpty() )
    {
    	m_document->New();
    	statusbar->showMessage( tr( "New file" ), 2000 );
    }
    else
    {
    	if( !m_document->ReadFile( fileName ) )
		{
			statusBar()->showMessage( tr( "Loading canceled" ), 2000 );
			return false;
		}
        //statusBar()->showMessage( tr( "File loaded" ), 2000 );
    	statusbar->showMessage( tr( "File loaded" ), 2000 );
    }

    SetCurrentFile( fileName );
	m_sceneModel->SetCoinScene( *m_document->GetSceneKit() );
*/
    return true;
}
