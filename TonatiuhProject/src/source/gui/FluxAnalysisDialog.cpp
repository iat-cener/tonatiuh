/*
 * FluxAnalysisDialog.cpp
 *
 *  Created on: 08/10/2015
 *      Author: amutuberria
 */


#include "FluxAnalysisDialog.h"
#include "SceneModel.h"
#include "SelectSurfaceDialog.h"


/*!
 * Create dialog object
 */
FluxAnalysisDialog::FluxAnalysisDialog( SceneModel& currentSceneModel/*, QVector< PhotonMapExportFactory* > typeList*/, QWidget* parent  )
:QDialog( parent),
 m_pCurrentSceneModel( &currentSceneModel )
{
	setupUi( this );
	connect( selectButton, SIGNAL( clicked() ), this, SLOT( SelectSurface() ) );

}


/*!
 * Create dialog object
 */
FluxAnalysisDialog::~FluxAnalysisDialog()
{

}


/*!
 * Opens a dialog to select a surface from the system.
 * If the selected surface is ok
 */
void FluxAnalysisDialog::SelectSurface()
{
	SelectSurfaceDialog selectSurfaceDialog( *m_pCurrentSceneModel, false, this );
	if( !selectSurfaceDialog.exec( ) )	return;

	QString selectedSurfaceURL = selectSurfaceDialog.GetSelectedSurfaceURL();
	if( selectedSurfaceURL.isEmpty() )
	{
		surfaceEdit->setText( QLatin1String( "---" ) );
		return;
	}

	surfaceEdit->setText( selectedSurfaceURL );
}
