/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include <QFileDialog>
#include <QPixmap>
#include <QSettings>
#include <QStringList>
#include <QMessageBox>

#include "Point3D.h"
#include "InputTable.h"
#include "HeliostatFieldWizard.h"

HeliostatFieldWizard::HeliostatFieldWizard( QWidget * parent, Qt::WindowFlags flags )
:QWizard( parent, flags )
{
	setupUi( this );

	QPixmap waterMarkPixmap( ":/icons/ComponentHeliostatField.png" );
	setPixmap( QWizard::WatermarkPixmap, waterMarkPixmap.scaled(  width()*0.3, waterMarkPixmap.height() ) );

	connect( fileNameHelRadio, SIGNAL( toggled( bool ) ), this, SLOT( ChangedFileNameHelState( bool ) ) );
	connect( tableHelRadio, SIGNAL( toggled( bool ) ), this, SLOT( ChangedTableHelState( bool ) ) );
	connect( editHelCoordsButton, SIGNAL( clicked() ), this, SLOT( ActivateHeliostatCoordsTable() ) );
	connect( componentRadio, SIGNAL( toggled( bool ) ), this, SLOT( ChangedComponentHelState( bool ) ) );
	connect( newDesignRadio, SIGNAL( toggled( bool ) ), this, SLOT( ChangedNewDesignState( bool ) ) );
	connect( shapeBox, SIGNAL( currentIndexChanged(const QString ) ), this, SLOT( ChangedShapeHel( const QString ) ) );
	connect( slantCheck, SIGNAL( toggled( bool ) ), this, SLOT( ChangedRadiusHelState( bool ) ) );
	connect( fileNameStrategyRadio, SIGNAL( toggled( bool ) ), this, SLOT( ChangedFileNameStrategyState( bool ) ) );
	connect( oneDimensionalStrategyRadio, SIGNAL( toggled( bool ) ), this, SLOT( ChangedOneDimensionsinalStrategyState( bool ) ) );
	connect( pointStrategyRadio, SIGNAL( toggled( bool ) ), this, SLOT( ChangedPointStrategyState( bool ) ) );
	connect( tableStrategyRadio, SIGNAL( toggled( bool ) ), this, SLOT( ChangedTableStrategyState( bool ) ) );
	connect( editTableStrategyButton, SIGNAL( clicked() ), this, SLOT( ActivateStrategyTable() ) );

	heliostatCoordinatesLine->setEnabled( false );
	openCoordFileButton->setEnabled( false );
	editHelCoordsButton->setEnabled( false );
	heliostatComponentLine->setEnabled( false );
	openHelComponentButton->setEnabled( false );
	hShapeLabel->setEnabled( false );
	shapeBox->setEnabled( false );
	hWidthLabel->setEnabled( false );
	widthSpin->setEnabled( false );
	hWidthUnitLabel->setEnabled( false );
	hWidthLabel->setEnabled( false );
	widthSpin->setEnabled( false );
	hWidthUnitLabel->setEnabled( false );
	hHeightLabel->setEnabled( false );
	heightSpin->setEnabled( false );
	hHeightUnitLabel->setEnabled( false );
	radiusLabel->setEnabled( false );
	slantCheck->setEnabled( false );
	radioCheckButton->setEnabled( false );
	hRadiusSpin->setEnabled( false );
	hRadiusUnitLabel->setEnabled( false );
	hReflectivityLabel->setEnabled( false );
	hReflectivitySpin->setEnabled( false );
	hReflectivityUnitLabel->setEnabled( false );
	rSigmaLabel->setEnabled( false );
	hSigmaSpin->setEnabled( false );
	mRadLabel->setEnabled( false );
	strategyLine->setEnabled( false );
	openStrategyButton->setEnabled( false );
	rElevationLabel->setEnabled( false );
	rElevationSpin->setEnabled( false );
	rElevationUnitLabel->setEnabled( false );
	rRadiusLabel->setEnabled( false );
	rRadiusSpin->setEnabled( false );
	rRadiusUnitLabel->setEnabled( false );
	xPointLabel->setEnabled( false );
	xPointSpin->setEnabled( false );
	xPointUnitLabel->setEnabled( false );
	yPointLabel->setEnabled( false );
	yPointSpin->setEnabled( false );
	yPointUnitLabel->setEnabled( false );
	zPointLabel->setEnabled( false );
	zPointSpin->setEnabled( false );
	zPointUnitLabel->setEnabled( false );
	editTableStrategyButton->setEnabled( false );

}

HeliostatFieldWizard::~HeliostatFieldWizard( )
{

}


//HELIOSTAT COORDINATES SECTION

void HeliostatFieldWizard::on_openCoordFileButton_clicked()
{
	QSettings settings( "NREL UTB CENER", "Tonatiuh" );
	QString directoryPath = settings.value( QLatin1String("HeliostatFieldWizard.dirname" ) ,QLatin1String( "/home") ).toString();

	QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
			directoryPath,
	            tr( "Text file (*.txt)" ) );

	if( fileName.isEmpty() )	return;

	QFileInfo coordFileInfo( fileName );
	settings.setValue( QLatin1String("HeliostatFieldWizard.dirname"), coordFileInfo.absolutePath() );

	if( !fileName.isEmpty() )	heliostatCoordinatesLine->setText( fileName );
}


void HeliostatFieldWizard::ChangedFileNameHelState( bool checked )
{
	if( checked )
	{
		heliostatCoordinatesLine->setEnabled( true );
		openCoordFileButton->setEnabled( true );
		tableHelRadio->setChecked( false );
		editHelCoordsButton->setEnabled( false );
	}
	else
	{
		heliostatCoordinatesLine->setEnabled( false );
		openCoordFileButton->setEnabled( false );
	}
}


void HeliostatFieldWizard::ChangedTableHelState( bool checked )
{
	if( checked )
	{
		heliostatCoordinatesLine->setEnabled( false );
		openCoordFileButton->setEnabled( false );
		fileNameHelRadio->setChecked( false );
		editHelCoordsButton->setEnabled( true );
	}
	else
	{
		editHelCoordsButton->setEnabled( false );
	}
}


void HeliostatFieldWizard::ActivateHeliostatCoordsTable()
{
	InputTableWidget helCoordsTable( m_helCoordsTable, this );

	if( !helCoordsTable.exec( ) )	return;
	m_helCoordsTable = helCoordsTable.GetCoordsTable();
}


//HELIOSTAT SECTION

void HeliostatFieldWizard::on_openHelComponentButton_clicked()
{
	QSettings settings( "NREL UTB CENER", "Tonatiuh" );
	QString directoryPath = settings.value( QLatin1String("HeliostatFieldWizard.dirname" ) ,QLatin1String( "/home") ).toString();

	QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
			directoryPath,
	            tr( "Tonatiuh component ( *.tcmp )" ) );

	if( fileName.isEmpty() )	return;

	QFileInfo helComponentInfo( fileName );
	settings.setValue( QLatin1String("HeliostatFieldWizard.dirname"), helComponentInfo.absolutePath() );

	if( !fileName.isEmpty() )	heliostatComponentLine->setText( fileName );
}


void HeliostatFieldWizard::ChangedComponentHelState( bool checked )
{
	if( checked )
	{
		heliostatComponentLine->setEnabled( true );
		openHelComponentButton->setEnabled( true );

		newDesignRadio->setChecked( false );
		hShapeLabel->setEnabled( false );
		shapeBox->setEnabled( false );
		hWidthLabel->setEnabled( false );
		widthSpin->setEnabled( false );
		hWidthUnitLabel->setEnabled( false );
		hHeightLabel->setEnabled( false );
		heightSpin->setEnabled( false );
		hHeightUnitLabel->setEnabled( false );
		radiusLabel->setEnabled( false );
		slantCheck->setEnabled( false );
		radioCheckButton->setEnabled( false );
		hRadiusSpin->setEnabled( false );
		hRadiusUnitLabel->setEnabled( false );
		hReflectivityLabel->setEnabled( false );
		hReflectivitySpin->setEnabled( false );
		hReflectivityUnitLabel->setEnabled( false );
		rSigmaLabel->setEnabled( false );
		hSigmaSpin->setEnabled( false );
		mRadLabel->setEnabled( false );
	}
	else
	{
		heliostatComponentLine->setEnabled( false );
		openHelComponentButton->setEnabled( false );
	}
}


void HeliostatFieldWizard::ChangedNewDesignState( bool checked )
{
	if( checked )
	{
		componentRadio->setChecked( false );
		heliostatComponentLine->setEnabled( false );
		openHelComponentButton->setEnabled( false );

		hShapeLabel->setEnabled( true );
		shapeBox->setEnabled( true );

		hReflectivityLabel->setEnabled( true );
		hReflectivitySpin->setEnabled( true );
		hReflectivityUnitLabel->setEnabled( true );
		rSigmaLabel->setEnabled( true );
		hSigmaSpin->setEnabled( true );
		mRadLabel->setEnabled( true );

		if( shapeBox->currentText() == QString("Flat_Rectangle") )
		{
			hWidthLabel->setEnabled( true );
			widthSpin->setEnabled( true );
			hWidthUnitLabel->setEnabled( true );
			hHeightLabel->setEnabled( true );
			heightSpin->setEnabled( true );
			hHeightUnitLabel->setEnabled( true );
		}
		else if( shapeBox->currentText() == QString("Spherical_rectangle") )
		{
			slantCheck->setEnabled( true );
			radioCheckButton->setEnabled( true );
			hWidthLabel->setEnabled( true );
			widthSpin->setEnabled( true );
			hWidthUnitLabel->setEnabled( true );
			hHeightLabel->setEnabled( true );
			heightSpin->setEnabled( true );
			hHeightUnitLabel->setEnabled( true );
			radiusLabel->setEnabled( true );
			hRadiusSpin->setEnabled( false );
			if( slantCheck->isChecked() )
			{
				radioCheckButton->setEnabled( false );
				hRadiusSpin->setEnabled( false );
				hRadiusUnitLabel->setEnabled( false );
			}
			else if( radioCheckButton->isChecked() )
			{
				radioCheckButton->setEnabled( true );
				hRadiusSpin->setEnabled( true );
				hRadiusUnitLabel->setEnabled( true );
			}
		}
	}
	else
	{
		hShapeLabel->setEnabled( false );
		shapeBox->setEnabled( false );
		hWidthLabel->setEnabled( false );
		widthSpin->setEnabled( false );
		hWidthUnitLabel->setEnabled( false );
		hHeightLabel->setEnabled( false );
		heightSpin->setEnabled( false );
		hHeightUnitLabel->setEnabled( false );
		radiusLabel->setEnabled( false );
		slantCheck->setEnabled( false );
		radioCheckButton->setEnabled( false );
		hRadiusSpin->setEnabled( false );
		hRadiusUnitLabel->setEnabled( false );
		hReflectivityLabel->setEnabled( false );
		hReflectivitySpin->setEnabled( false );
		hReflectivityUnitLabel->setEnabled( false );
		rSigmaLabel->setEnabled( false );
		hSigmaSpin->setEnabled( false );
		mRadLabel->setEnabled( false );
	}
}


void HeliostatFieldWizard::ChangedShapeHel( const QString text)
{
	if( text == QString("Flat_Rectangle") )
	{
		radiusLabel->setEnabled( false );
		slantCheck->setEnabled( false );
		radioCheckButton->setEnabled( false );
		hRadiusSpin->setEnabled( false );
		hRadiusUnitLabel->setEnabled( false );
	}
	else if ( text == QString("Spherical_rectangle") )
	{
		radiusLabel->setEnabled( true );
		slantCheck->setEnabled( true );
		radioCheckButton->setEnabled( true );
		if( radioCheckButton->isChecked() )
		{
			hRadiusSpin->setEnabled( true );
			hRadiusUnitLabel->setEnabled( true );
		}
		else
		{
			hRadiusSpin->setEnabled( false );
			hRadiusUnitLabel->setEnabled( false );
		}
	}
}


void HeliostatFieldWizard::ChangedRadiusHelState( bool checked )
{
	if( checked )
	{
		hRadiusSpin->setEnabled( false );
		hRadiusUnitLabel->setEnabled( false );
	}
	else
	{
		radioCheckButton->setChecked( true );
		hRadiusSpin->setEnabled( true );
		hRadiusUnitLabel->setEnabled( true );
	}
}


//AIMING STRATEGY STATE

void HeliostatFieldWizard::on_openStrategyButton_clicked()
{
	QSettings settings( "NREL UTB CENER", "Tonatiuh" );
	QString directoryPath = settings.value( QLatin1String("HeliostatFieldWizard.dirname" ) ,QLatin1String( "/home") ).toString();

	QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
			directoryPath,
	            tr( "Text file (*.txt)" ) );

	if( fileName.isEmpty() )	return;

	QFileInfo strategyInfo( fileName );
	settings.setValue( QLatin1String("HeliostatFieldWizard.dirname"), strategyInfo.absolutePath() );

	if( !fileName.isEmpty() )	strategyLine->setText( fileName );
}


void HeliostatFieldWizard::ChangedFileNameStrategyState( bool checked )
{
	if( checked )
	{
		strategyLine->setEnabled( true );
		openStrategyButton->setEnabled( true );

		oneDimensionalStrategyRadio->setChecked( false );
		rElevationLabel->setEnabled( false );
		rElevationSpin->setEnabled( false );
		rElevationUnitLabel->setEnabled( false );
		rRadiusLabel->setEnabled( false );
		rRadiusSpin->setEnabled( false );
		rRadiusUnitLabel->setEnabled( false );

		pointStrategyRadio->setChecked( false );
		xPointLabel->setEnabled( false );
		yPointLabel->setEnabled( false );
		zPointLabel->setEnabled( false );
		xPointSpin->setEnabled( false );
		yPointSpin->setEnabled( false );
		zPointSpin->setEnabled( false );
		xPointUnitLabel->setEnabled( false );
		yPointUnitLabel->setEnabled( false );
		zPointUnitLabel->setEnabled( false );

		tableStrategyRadio->setChecked( false );
		editTableStrategyButton->setEnabled( false );
	}
	else
	{
		strategyLine->setEnabled( false );
		openStrategyButton->setEnabled( false );
	}

}

void HeliostatFieldWizard::ChangedOneDimensionsinalStrategyState( bool checked )
{
	if( checked )
	{
		fileNameStrategyRadio->setChecked( false );
		strategyLine->setEnabled( false );
		openStrategyButton->setEnabled( false );

		rElevationLabel->setEnabled( true );
		rElevationSpin->setEnabled( true );
		rElevationUnitLabel->setEnabled( true );
		rRadiusLabel->setEnabled( true );
		rRadiusSpin->setEnabled( true );
		rRadiusUnitLabel->setEnabled( true );

		pointStrategyRadio->setChecked( false );
		xPointLabel->setEnabled( false );
		yPointLabel->setEnabled( false );
		zPointLabel->setEnabled( false );
		xPointSpin->setEnabled( false );
		yPointSpin->setEnabled( false );
		zPointSpin->setEnabled( false );
		xPointUnitLabel->setEnabled( false );
		yPointUnitLabel->setEnabled( false );
		zPointUnitLabel->setEnabled( false );

		tableStrategyRadio->setChecked( false );
		editTableStrategyButton->setEnabled( false );
	}
	else
	{
		rElevationLabel->setEnabled( false );
		rElevationSpin->setEnabled( false );
		rElevationUnitLabel->setEnabled( false );
		rRadiusLabel->setEnabled( false );
		rRadiusSpin->setEnabled( false );
		rRadiusUnitLabel->setEnabled( false );
	}
}


void HeliostatFieldWizard::ChangedPointStrategyState( bool checked )
{

	if( checked )
	{
		fileNameStrategyRadio->setChecked( false );
		strategyLine->setEnabled( false );
		openStrategyButton->setEnabled( false );

		oneDimensionalStrategyRadio->setChecked( false );
		rElevationLabel->setEnabled( false );
		rElevationSpin->setEnabled( false );
		rElevationUnitLabel->setEnabled( false );
		rRadiusLabel->setEnabled( false );
		rRadiusSpin->setEnabled( false );
		rRadiusUnitLabel->setEnabled( false );

		xPointLabel->setEnabled( true );
		yPointLabel->setEnabled( true );
		zPointLabel->setEnabled( true );
		xPointSpin->setEnabled( true );
		yPointSpin->setEnabled( true );
		zPointSpin->setEnabled( true );
		xPointUnitLabel->setEnabled( true );
		yPointUnitLabel->setEnabled( true );
		zPointUnitLabel->setEnabled( true );

		tableStrategyRadio->setChecked( false );
		editTableStrategyButton->setEnabled( false );
	}
	else
	{
		xPointLabel->setEnabled( false );
		yPointLabel->setEnabled( false );
		zPointLabel->setEnabled( false );
		xPointSpin->setEnabled( false );
		yPointSpin->setEnabled( false );
		zPointSpin->setEnabled( false );
		xPointUnitLabel->setEnabled( false );
		yPointUnitLabel->setEnabled( false );
		zPointUnitLabel->setEnabled( false );
	}
}


void HeliostatFieldWizard::ChangedTableStrategyState( bool checked )
{
	if( checked )
	{
		fileNameStrategyRadio->setChecked( false );
		strategyLine->setEnabled( false );
		openStrategyButton->setEnabled( false );

		oneDimensionalStrategyRadio->setChecked( false );
		rElevationLabel->setEnabled( false );
		rElevationSpin->setEnabled( false );
		rElevationUnitLabel->setEnabled( false );
		rRadiusLabel->setEnabled( false );
		rRadiusSpin->setEnabled( false );
		rRadiusUnitLabel->setEnabled( false );

		pointStrategyRadio->setChecked( false );
		xPointLabel->setEnabled( false );
		yPointLabel->setEnabled( false );
		zPointLabel->setEnabled( false );
		xPointSpin->setEnabled( false );
		yPointSpin->setEnabled( false );
		zPointSpin->setEnabled( false );
		xPointUnitLabel->setEnabled( false );
		yPointUnitLabel->setEnabled( false );
		zPointUnitLabel->setEnabled( false );

		editTableStrategyButton->setEnabled( true );
	}
	else
	{
		editTableStrategyButton->setEnabled( false );

	}

}

void HeliostatFieldWizard::ActivateStrategyTable()
{
	InputTableWidget aiminPointsTable( m_aimingPointsTable, this );

	if( !aiminPointsTable.exec( ) )	return;
	m_aimingPointsTable = aiminPointsTable.GetCoordsTable();
}

//FINISH BUTTON

void HeliostatFieldWizard::accept()
{
	if( (!fileNameHelRadio->isChecked() && !tableHelRadio->isChecked() )
			|| ( !componentRadio->isChecked() && !newDesignRadio->isChecked() )
			|| ( !fileNameStrategyRadio->isChecked() && !oneDimensionalStrategyRadio->isChecked() && !pointStrategyRadio->isChecked() && !tableStrategyRadio->isChecked() ) )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
				QString( "Missing information." ) );
			return;
		}
	QDialog::accept();
}


//TRANSFER DATA FUNCTIONS

int HeliostatFieldWizard::GetHelCoordinates() const
{
	if( fileNameHelRadio->isChecked() )
		return ( 1 );
	else if( tableHelRadio->isChecked() )
		return ( 2 );
	return ( 0 );
}


QString HeliostatFieldWizard::GetCoordinatesFile() const
{
	return heliostatCoordinatesLine->text();
}


std::vector< Point3D > HeliostatFieldWizard::GetHeliostatCoordsTable() const
{

	return m_helCoordsTable;
}


QString HeliostatFieldWizard::GetComponentFile() const
{
	return heliostatComponentLine->text();
}


int HeliostatFieldWizard::GetHeliostat() const
{
	if( componentRadio->isChecked() )
		return ( 1 );
	else if( newDesignRadio->isChecked() )
		return ( 2 );
	return ( 0 );
}


QString HeliostatFieldWizard::GetHeliostatShape() const
{
	return shapeBox->currentText();
}


double HeliostatFieldWizard::GetHeliostatWidth() const
{
	return widthSpin->value();
}


double HeliostatFieldWizard::GetHeliostatHeight() const
{
	return heightSpin->value();
}


double HeliostatFieldWizard::GetHeliostatRadius() const
{
	if( slantCheck->isChecked() )	return ( -1 );
	else return hRadiusSpin->value();
}


double HeliostatFieldWizard::GetHeliostatReflectivity() const
{
	return ( hReflectivitySpin->value() /100 );
}


double HeliostatFieldWizard::GetHeliostatSigma() const
{
	return hSigmaSpin->value();
}


QString HeliostatFieldWizard::GetStrategyFile()const
{
	return strategyLine->text();
}


int HeliostatFieldWizard::GetAimingStrategy() const
{
	if( fileNameStrategyRadio->isChecked() )
		return ( 4 );
	else if( oneDimensionalStrategyRadio->isChecked() )
		return ( 1 );
	else if( pointStrategyRadio->isChecked() )
		return ( 2 );
	else if( tableStrategyRadio->isChecked() )
		return ( 3 );
	return ( 0 );
}


Point3D HeliostatFieldWizard::GetAimingStrategyPoint() const
{
	Point3D aimingPoint( -1, -1, -1 );
	if( pointStrategyRadio->isChecked() )
		aimingPoint = Point3D( xPointSpin->value(), yPointSpin->value(), zPointSpin->value() );

	return ( aimingPoint );
}


double HeliostatFieldWizard::GetReceiverElevation() const
{
	return ( rElevationSpin->value() );
}


double HeliostatFieldWizard::GetReceiverRadius() const
{
	return ( rRadiusSpin->value() );
}


std::vector< Point3D > HeliostatFieldWizard::GetAimingPointsTable() const
{
	return m_aimingPointsTable;
}
