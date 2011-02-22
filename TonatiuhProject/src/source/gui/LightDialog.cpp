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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QMessageBox>

#include "FieldContainerWidget.h"
#include "LightDialog.h"
#include "tgc.h"
#include "TLightKit.h"
#include "TShape.h"
#include "TShapeFactory.h"
#include "TSunShapeFactory.h"

/**
 * Creates a new dialog for the light definition.
 *
 * The dialog present a list given by \a sunshapeFactoryList to define light sunshape, \a shapeFactoryList for the light input
 * aperture and shows the the light parameters defined in the light \a currentLightKit.
 */

LightDialog::LightDialog(  TLightKit* currentLightKit, QVector< TShapeFactory* > shapeFactoryList, QVector< TSunShapeFactory* > sunshapeFactoryList, QWidget* parent )
:QDialog( parent ), m_currentLightKit( currentLightKit ), m_currentSunShapeIndex( -1 ), m_currentShapeIndex( -1 ),
m_newShape( 0 ), m_newSunShape( 0 )
{
	setupUi( this );

	for( int shape = 0; shape < (int) shapeFactoryList.size(); ++shape )
	{
		QString shapeTypeName( shapeFactoryList[shape]->CreateTShape()->getTypeId().getName().getString() );
		m_shapeList.insert( shapeTypeName, shapeFactoryList[shape] );
	}

	for( int sunShape = 0; sunShape < (int) sunshapeFactoryList.size(); ++sunShape )
	{
		QString sunShapeTypeName( sunshapeFactoryList[sunShape]->CreateTSunShape()->getTypeId().getName().getString() );
		m_sunshapeList.insert( sunShapeTypeName, sunshapeFactoryList[sunShape] );
	}

	if( currentLightKit )
	{
		if( currentLightKit->getPart( "tsunshape", false ) )	m_newSunShape = static_cast< TSunShape* >( currentLightKit->getPart( "tsunshape", false )->copy( true ) );
		if( currentLightKit->getPart( "icon", false ) )	m_newShape = static_cast< TShape* >( currentLightKit->getPart( "icon", false )->copy( true ) );

		if( currentLightKit->automaticallyResizable.getValue() ) automaticSizeRadio->setChecked( true );
		else	userSizeRadio->setChecked( true );
	}

	SunPositionTab();
	SunshapeTab();

}

/*!
 * Destroys the LightDialog object.
 */
LightDialog::~LightDialog()
{

}

/*!
 * Returns a lightkit with the parameters defined in the dialog.
 */
TLightKit* LightDialog::GetTLightKit()
{
	TLightKit* lightKit = new TLightKit;

	if( m_newSunShape ) lightKit->setPart( "tsunshape", m_newSunShape );
	if( m_newShape ) lightKit->setPart( "icon", m_newShape );
	if( automaticSizeRadio->isChecked() )	lightKit->automaticallyResizable.setValue( true );
	else	lightKit->automaticallyResizable.setValue( false );

	lightKit->ChangePosition( azimuthSpin->value()* tgc::Degree, ( 90 - elevationSpin->value() ) * tgc::Degree, distanceSpin->value() );
	return lightKit;
}

/*!
 *
 */
void LightDialog::accept()
{
	if( sunshapeCombo->currentIndex() == 0 )
		QMessageBox::warning( this, tr( "Tonatiuh" ), tr( "You must select a sunshape type." ), QMessageBox::Ok );
	else if( shapeCombo->currentIndex() == 0 )
		QMessageBox::warning( this, tr( "Tonatiuh" ), tr( "You must select an input aperture shape before for light." ), QMessageBox::Ok );
	else
		QDialog::accept();
}

/*!
 * Changes parameters of the shunshape paraneters view to sunshape type given by \a index.
 */
void LightDialog::ChangeSunshape( int index )
{
	while( (m_newSunShape!=0) && m_newSunShape->getRefCount() > 0 )	m_newSunShape->unref();
	if( index == 0 ) m_newSunShape = 0;
	else if( index == m_currentSunShapeIndex)	m_newSunShape = static_cast< TSunShape* >( m_currentLightKit->getPart( "tsunshape", false )->copy( true ) );
	else
	{
		TSunShapeFactory* sunshapeFactory = m_sunshapeList.value( sunshapeCombo->itemData( index ).toString() );
		m_newSunShape = sunshapeFactory->CreateTSunShape();

	}

	sunshapeParameters->SetContainer( m_newSunShape, QString() );
}


/*!
 * Changes parameters of the shape paraneters view to shape type given by \a index.
 */

void LightDialog::ChangeShape( int index )
{
	while( ( m_newShape != 0) && ( m_newShape->getRefCount() > 0 ) )	m_newShape->unref();

	if ( index == 0 )	m_newShape = 0;
	else if( index == m_currentShapeIndex )	m_newShape = static_cast< TShape* >( m_currentLightKit->getPart( "icon", false )->copy( true ) );
	else
	{
		TShapeFactory* shapeFactory = m_shapeList.value( shapeCombo->itemData( index ).toString() );
		m_newShape = shapeFactory->CreateTShape();
	}

	shapeParameters->SetContainer( m_newShape, QString() );

}

/*!
 * Updates the sun position tab values to the values of the current light.
 */
void LightDialog::SunPositionTab()
{
	if( m_currentLightKit )
	{
		azimuthSpin->setValue( m_currentLightKit->azimuth.getValue() / tgc::Degree );
		elevationSpin->setValue( 90 - ( m_currentLightKit->zenith.getValue() / tgc::Degree ) );
		distanceSpin->setValue( m_currentLightKit->distance.getValue() );
	}

}

/*!
 * Updates the dialog values to the values of the current light.
 */
void LightDialog::SunshapeTab()
{
	SunshapeBox();
	ShapeBox();
}

void LightDialog::ShapeBox( )
{
	connect( shapeCombo, SIGNAL( activated( int ) ), this, SLOT( ChangeShape( int ) ) );

	//Add elements to sunshape combo
	QList< TShapeFactory* > shapeFactoryList = m_shapeList.values();
	shapeCombo->addItem ( "---" );
	for( int j = 0; j <(int) shapeFactoryList.size(); ++j )
		shapeCombo->addItem( shapeFactoryList[j]->TShapeIcon(), shapeFactoryList[j]->TShapeName(), m_shapeList.key( shapeFactoryList[j] ) );

	//Select current LightKit Shape
	m_currentShapeIndex = 0;
	if( m_newShape )
	{
		QString name( m_newShape->getTypeId().getName() );
		m_currentShapeIndex = shapeCombo->findData( name );
	}

    ChangeShape( m_currentShapeIndex );
    shapeCombo->setCurrentIndex( m_currentShapeIndex );
}

void LightDialog::SunshapeBox()
{
	connect( sunshapeCombo, SIGNAL( activated( int ) ), this, SLOT( ChangeSunshape( int ) ) );

	//Add elements to sunshape combo
	QList< TSunShapeFactory* > sunShapeFactoryList = m_sunshapeList.values();
	sunshapeCombo->addItem ( "---" );
	for( int i = 0; i < sunShapeFactoryList.size(); ++i )
		sunshapeCombo->addItem( sunShapeFactoryList[i]->TSunShapeIcon(), sunShapeFactoryList[i]->TSunShapeName(),m_sunshapeList.key( sunShapeFactoryList[i] ) );

	//Select current LightKit Sunshape
	m_currentSunShapeIndex = 0;
	if( m_newSunShape )
	{
		QString name( m_newSunShape->getTypeId().getName() );
		m_currentSunShapeIndex = sunshapeCombo->findData( name );
	}

    ChangeSunshape( m_currentSunShapeIndex );
    sunshapeCombo->setCurrentIndex( m_currentSunShapeIndex );
}
