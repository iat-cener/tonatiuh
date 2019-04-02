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

#include <QAbstractButton>
#include <QFileDialog>
#include <QDir>

#include "RandomDeviateFactory.h"
#include "RayTraceDialog.h"

/**
 * Creates a dialog to ray tracer options with the given \a parent and \a f flags.
 *
 * The variables take the default values.
 */
RayTraceDialog::RayTraceDialog( QWidget * parent, Qt::WindowFlags f )
:QDialog ( parent, f ),
 m_drawPhotons( false ),
 m_drawRays( false ),
 m_heightDivisions( 200 ),
 m_increasePhotonMap( false ),
 m_numRays( 0 ),
 m_photonMapBufferSize( 1000000 ),
 m_selectedRandomFactory( -1 ),
 m_widthDivisions( 200 )
{
	setupUi( this );
	connect( this, SIGNAL( accepted() ), this, SLOT( saveChanges() ) );
	connect( buttonBox, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( applyChanges( QAbstractButton* ) ) );
}

/**
 * Creates a dialog to ray tracer options with the given \a parent and \a f flags.
 *
 * The variables take the values specified by \a numRats, \a faction, \a drawPhotons and \a increasePhotonMap.
 */
RayTraceDialog::RayTraceDialog( int numRays,
		QVector< RandomDeviateFactory* > randomFactoryList, int selectedRandomFactory,
		int widthDivisions, int heightDivisions,
		bool drawRays, bool drawPhotons,
		int photonMapSize, bool increasePhotonMap,
		QWidget * parent, Qt::WindowFlags f )
:QDialog ( parent, f ),
 m_drawPhotons( drawPhotons ),
 m_drawRays( drawRays ),
 m_heightDivisions( heightDivisions ),
 m_increasePhotonMap( increasePhotonMap ),
 m_numRays( numRays ),
 m_photonMapBufferSize( photonMapSize ),
 m_selectedRandomFactory( selectedRandomFactory ),
 m_widthDivisions( widthDivisions )
{
	setupUi( this );
	raysSpinBox->setValue( m_numRays );
	for( int index = 0; index < randomFactoryList.size(); ++index )
	{
		randomCombo->addItem( randomFactoryList[index]->RandomDeviateIcon(), randomFactoryList[index]->RandomDeviateName() );
	}
	if( m_selectedRandomFactory < 0 && randomFactoryList.size() > 0 )
		m_selectedRandomFactory =0;
	randomCombo->setCurrentIndex( m_selectedRandomFactory );


	widthDivisionsSpinBox->setValue( m_widthDivisions );
	heightDivisionsSpinBox->setValue( m_heightDivisions );

	showRaysCheck->setChecked( m_drawRays );
	showPhotonsCheck->setChecked( m_drawPhotons );

	bufferSizeSpin->setValue( m_photonMapBufferSize );
	if ( m_increasePhotonMap )
		increaseMapRadio->setChecked( true );
	else
		newMapRadio->setChecked( true );

	connect( this, SIGNAL( accepted() ), this, SLOT( saveChanges() ) );
	connect( buttonBox, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( applyChanges( QAbstractButton* ) ) );
}

/*!
 * Destroys the RayTraceDialog object.
 */
RayTraceDialog::~RayTraceDialog()
{

}

/**
 * Returns if the photons are going to be represented.
 */
bool RayTraceDialog::DrawPhotons() const
{
	return m_drawPhotons;
}

/*!
 * Returns if the rays are going to be represented.
 */
bool RayTraceDialog::DrawRays() const
{
	return m_drawRays;
}

/**
 * Returns the the height divisions applied to the sun shape.
 */
int RayTraceDialog::GetHeightDivisions() const
{
	return m_heightDivisions;
};

/**
 * Returns the number of rays to trace.
 */
int RayTraceDialog::GetNumRays() const
{
	return m_numRays;
}

/*!
 * Returns maximum number of photons that the photonmap buffer can save.
 */
int RayTraceDialog::GetPhotonMapBufferSize() const
{
	return m_photonMapBufferSize;
}

/**
 * Returns the factory to create a new random generator.
 */
int RayTraceDialog::GetRandomDeviateFactoryIndex() const
{
	return m_selectedRandomFactory;
}

/**
 * Returns the the width divisions applied to the sun shape.
 */
int RayTraceDialog::GetWidthDivisions() const
{
	return m_widthDivisions;
};

/**
 * Returns if the the tracer use the same photon map used in the previous tracer process.
 */
bool RayTraceDialog::IncreasePhotonMap() const
{
	return m_increasePhotonMap;
}

/**
 * If the applyChanges button is clicked the dialog values are saved.
 */
void RayTraceDialog::applyChanges( QAbstractButton* button  )
{
	if( buttonBox->buttonRole( button ) == QDialogButtonBox::ApplyRole )
		saveChanges();
}

/**
 * Saves the values of the dialog.
 */
void RayTraceDialog::saveChanges()
{
	m_numRays = raysSpinBox->value();
	m_selectedRandomFactory = randomCombo->currentIndex();
	m_widthDivisions= widthDivisionsSpinBox->value();
	m_heightDivisions= heightDivisionsSpinBox->value();

	m_drawRays = showRaysCheck->isChecked();
	m_drawPhotons = showPhotonsCheck->isChecked();

	m_photonMapBufferSize = bufferSizeSpin->value();
	if( newMapRadio->isChecked() )
		m_increasePhotonMap = false;
	else
		m_increasePhotonMap = true;
}

