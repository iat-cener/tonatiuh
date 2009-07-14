/*
 * MainWindow.cpp
 *
 *  Created on: Jul 14, 2009
 *      Author: mblanco
 *
 */

#include <QFileDialog>

#include "Trace.h"
#include "MainWindow.h"


MainWindow::MainWindow( QWidget* parent, Qt::WindowFlags flags )
:QMainWindow( parent, flags )
{
	Trace trace( "MainWindow::MainWindow", false );
	setupUi( this );
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
