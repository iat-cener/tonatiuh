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

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victlor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>

#include <QVector>


#include <Inventor/fields/SoSFBool.h>


#include "TransmissivityDialog.h"
#include "TTransmissivity.h"
#include "TTransmissivityFactory.h"

TransmissivityDialog::TransmissivityDialog( QVector< TTransmissivityFactory* > transmissivityFactoryList, QWidget* parent, Qt::WindowFlags f )
:QDialog( parent, f ),
 m_currentTransmissivityIndex( -1 ),
 m_newTransmissivity( 0 )
{
	setupUi( this );
	transmissivityCombo->addItem ( "---" );
	for( int t = 0; t < (int) transmissivityFactoryList.size(); ++t )
	{
		QString transmissivityTypeName( transmissivityFactoryList[t]->CreateTTransmissivity()->getTypeId().getName().getString() );
		m_transmissivityFactoryList.insert( transmissivityTypeName, transmissivityFactoryList[t] );
		transmissivityCombo->addItem( transmissivityFactoryList[t]->TTransmissivityIcon(), transmissivityFactoryList[t]->TTransmissivityName(), transmissivityTypeName );
	}

	connect( transmissivityCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ChangeTransmissivityType( int ) ) );
	//change parameter signal
	connect( transmissivityParameters, SIGNAL( valueModificated( SoNode*, QString, QString ) ), this, SLOT( SetValue( SoNode*, QString, QString ) ) );

}

TransmissivityDialog::~TransmissivityDialog()
{

}

/*!
 * Returns defined transmissivity node.
 */
TTransmissivity* TransmissivityDialog::GetTransmissivity() const
{
	return m_newTransmissivity;
}
/*
 * Set new new value into the parameter field
 */
void TransmissivityDialog::SetValue( SoNode* node, QString paramenterName, QString newValue )
{
	double value=newValue.toDouble();
	if(value>=0){

	SoField* parameterField = node->getField( SbName( paramenterName.toStdString().c_str() ) );
	if( parameterField )
		parameterField->set( newValue.toStdString().c_str() );
	}
}
void TransmissivityDialog::SetCurrentTransmissivity( TTransmissivity* transmissivity )
{
	if( !transmissivity )	return;
	m_currentTransmissivity = static_cast< TTransmissivity* >( transmissivity->copy( true ) );

	//Select current Transmissivity
	m_currentTransmissivityIndex = 0;
	if( m_currentTransmissivity )
	{
		QString name( m_currentTransmissivity->getTypeId().getName() );
		m_currentTransmissivityIndex = transmissivityCombo->findData( name );
		transmissivityCombo->setCurrentIndex( m_currentTransmissivityIndex );
	}
}

void TransmissivityDialog::ChangeTransmissivityType( int index )
{
	while( ( m_newTransmissivity!= 0 ) && m_newTransmissivity->getRefCount() > 0 )	m_newTransmissivity->unref();

	if( index == 0 ) m_newTransmissivity = 0;
	else if( index == m_currentTransmissivityIndex )
		m_newTransmissivity = static_cast< TTransmissivity* >( m_currentTransmissivity->copy( true ) );
	else
	{
		TTransmissivityFactory* transmissivityFactory = m_transmissivityFactoryList.value( transmissivityCombo->itemData( index ).toString() );
		m_newTransmissivity = transmissivityFactory->CreateTTransmissivity();

	}

	transmissivityParameters->SetContainer( m_newTransmissivity, QString() );

}
