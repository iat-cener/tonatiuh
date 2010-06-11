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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>
#include <QIcon>
#include <QComboBox>
#include <QString>
#include <QVector>

#include "DefineMediumDialog.h"
#include "FieldContainerWidget.h"
#include "TMedium.h"
#include "TMediumFactory.h"


/**
 * Creates a new dialog for the medium definition.
 *
 * The dialog present a list given by \a mediumFactoryList to define medium and shows the the
 * selected medium parameters.
 */

DefineMediumDialog::DefineMediumDialog( QVector< TMediumFactory* > mediumFactoryList, QWidget* parent )
:QDialog( parent ), m_currentMediumIndex( -1 ), m_newMedium( 0 )
{
	setupUi( this );

	for( int medium = 0; medium < (int) mediumFactoryList.size(); ++medium )
	{
		QString mediumTypeName( mediumFactoryList[medium]->TMediumName() );
		m_mediumList.insert( mediumTypeName, mediumFactoryList[medium] );
	}
	MediumBox();
}
/*!
 * Destroys the DefineMediumDialog object.
 */
DefineMediumDialog::~DefineMediumDialog()
{

}

/*!
 * Return a medium with the parameters defined in the dialog.
 */
TMedium* DefineMediumDialog::GetTMedium()
{
	return m_newMedium;
}

/*!
 * Changes parameters of the shape parameters view to medium type given by \a index.
 */

void DefineMediumDialog::changeMedium( int index )
{

	QLayout* frameLayout = mediumParametersFrame->layout();

	if( !frameLayout && frameLayout != 0 ) return;
	int children = frameLayout->count();
	for( int i = 0; i< children; ++i )
		frameLayout->removeItem ( frameLayout->itemAt( i ) );

	if ( index == 0 )	m_newMedium = 0;
	else
	{
		TMediumFactory* mediumFactory = m_mediumList.value( mediumCombo->itemData( index ).toString() );
		m_newMedium = mediumFactory->CreateTMedium();
		m_newMedium->setName( mediumFactory->TMediumName().toStdString().c_str() );
	}

	FieldContainerWidget* nodeContainer = new FieldContainerWidget( m_newMedium, QString() );
	frameLayout->addWidget( nodeContainer );

}
void DefineMediumDialog::MediumBox( )
{
	connect( mediumCombo, SIGNAL( activated( int ) ), this, SLOT( changeMedium( int ) ) );

	//Add elements to sunshape combo
	QList< TMediumFactory* > mediumFactoryList = m_mediumList.values();

	mediumCombo->addItem ( "---" );
	for( int j = 0; j <(int) mediumFactoryList.size(); ++j )
		mediumCombo->addItem( mediumFactoryList[j]->TMediumIcon(), mediumFactoryList[j]->TMediumName(), m_mediumList.key( mediumFactoryList[j] ) );


	//Select current Medium
	/*m_currentMediumIndex = 0;
	if( m_newMedium )
	{
		QString name( m_newMedium->getTypeId().getName() );
		m_currentMediumIndex = shapeCombo->findData( name );
	}*/

	QVBoxLayout* parametersMediumLayout = new QVBoxLayout;
	parametersMediumLayout->setContentsMargins ( 0, 0, 0, 0 );
	mediumParametersFrame->setLayout( parametersMediumLayout );
    //changeMedium( m_currentMediumIndex );
   // mediumCombo->setCurrentIndex( m_currentMediumIndex );
}
