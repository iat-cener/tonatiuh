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

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QString>
#include <QTabWidget>

#include "FieldContainerWidget.h"
#include "LightDialog.h"
#include "TLightKit.h"
#include "Trace.h"
#include "TShape.h"
#include "TShapeFactory.h"
#include "TSunShapeFactory.h"

LightDialog::LightDialog(  TLightKit* currentLightKit, QVector< TShapeFactory* > shapeFactoryList, QVector< TSunShapeFactory* > sunshapeFactoryList, QWidget* parent )
:QDialog( parent ), m_tabWidget( 0 ), m_sunshapeCombox( 0 ), m_shapeCombox( 0 ), m_sunshapeParameters( 0 ), m_shapeParameters( 0 ),
m_currentLightKit( currentLightKit ), m_currentSunShapeIndex( -1 ), m_currentShapeIndex( -1 ),
m_newShape( 0 ), m_newSunShape( 0 )
{
	Trace trace( "LightDialog::LightDialog", false );

	for( int shape = 0; shape < (int) shapeFactoryList.size(); shape++ )
	{
		QString shapeTypeName( shapeFactoryList[shape]->CreateTShape()->getTypeId().getName().getString() );
		m_shapeList.insert( shapeTypeName, shapeFactoryList[shape] );
	}

	for( int sunShape = 0; sunShape < (int) sunshapeFactoryList.size(); sunShape++ )
	{
		QString sunShapeTypeName( sunshapeFactoryList[sunShape]->CreateTSunShape()->getTypeId().getName().getString() );
		m_sunshapeList.insert( sunShapeTypeName, sunshapeFactoryList[sunShape] );
	}

	if( currentLightKit )
	{
		if( currentLightKit->getPart( "tsunshape", false ) )	m_newSunShape = static_cast< TSunShape* >( currentLightKit->getPart( "tsunshape", false )->copy( true ) );
		if( currentLightKit->getPart( "icon", false ) )	m_newShape = static_cast< TShape* >( currentLightKit->getPart( "icon", false )->copy( true ) );
	}

	QHBoxLayout* layout = new QHBoxLayout;
    layout->setObjectName( QString::fromUtf8( "hboxLayout" ) );
	layout->setSizeConstraint(QLayout::SetFixedSize);

    QVBoxLayout* vboxLayout = new QVBoxLayout;
    m_tabWidget = new QTabWidget(  );
    vboxLayout->addWidget( m_tabWidget );

   	SunshapeTab();

    QDialogButtonBox* buttonBox = new QDialogButtonBox;
    buttonBox->setObjectName( QString::fromUtf8( "buttonBox" ) );
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    connect( buttonBox, SIGNAL( accepted() ), this, SLOT(accept()));
	connect( buttonBox, SIGNAL( rejected() ), this, SLOT(reject()));
    vboxLayout->addWidget( buttonBox );


    layout->addLayout( vboxLayout );

    setLayout( layout );

}

LightDialog::~LightDialog()
{

}

TLightKit* LightDialog::GetTLightKit()
{
	Trace trace( "LightDialog::GetTLightKit", false );

	TLightKit* lightKit = new TLightKit;
	lightKit->setPart( "tsunshape", m_newSunShape );
	lightKit->setPart( "icon", m_newShape );

	return lightKit;

}

void LightDialog::changeSunshape( int index )
{
	Trace trace( "LightDialog::changeSunshape", false );

	QLayout* frameLayout = m_sunshapeParameters->layout();
	if( !frameLayout ) return;
	int children = frameLayout->count();
	for( int i = 0; i< children; i++ )
		frameLayout->removeItem ( frameLayout->itemAt( i ) );

	while( (m_newSunShape!=0) && m_newSunShape->getRefCount() > 0 )	m_newSunShape->unref();
	if( index == 0 ) m_newSunShape = 0;
	else if( index == m_currentSunShapeIndex)	m_newSunShape = static_cast< TSunShape* >( m_currentLightKit->getPart( "tsunshape", false )->copy( true ) );
	else
	{
		TSunShapeFactory* sunshapeFactory = m_sunshapeList.value( m_sunshapeCombox->itemData( index ).toString() );
		m_newSunShape = sunshapeFactory->CreateTSunShape();

	}
	FieldContainerWidget* nodeContainer = new FieldContainerWidget( m_newSunShape, QString() );
	nodeContainer->setFixedSize( QSize( 200, 100 ) );
	frameLayout->addWidget( nodeContainer );
}

void LightDialog::changeShape( int index )
{
	Trace trace( "LightDialog::changeShape", false );

	QLayout* frameLayout = m_shapeParameters->layout();
	if( !frameLayout && frameLayout != 0 ) return;
	int children = frameLayout->count();
	for( int i = 0; i< children; i++ )
		frameLayout->removeItem ( frameLayout->itemAt( i ) );

	while( ( m_newShape != 0) && ( m_newShape->getRefCount() > 0 ) )	m_newShape->unref();

	if ( index == 0 )	m_newShape = 0;
	else if( index == m_currentShapeIndex )	m_newShape = static_cast< TShape* >( m_currentLightKit->getPart( "icon", false )->copy( true ) );
	else
	{
		TShapeFactory* shapeFactory = m_shapeList.value( m_shapeCombox->itemData( index ).toString() );
		m_newShape = shapeFactory->CreateTShape();
	}

	FieldContainerWidget* nodeContainer = new FieldContainerWidget( m_newShape, QString() );
	nodeContainer->setFixedSize( QSize( 200, 100 ) );
	frameLayout->addWidget( nodeContainer );
}

void LightDialog::SunshapeTab()
{
	Trace trace( "LightDialog::SunshapeTab", false );

   	QWidget* sunShape = new QWidget;
   	sunShape->setObjectName( QString::fromUtf8( "sunShape" ) );
    m_tabWidget->addTab( sunShape, tr( "Sun Shape") );

    QVBoxLayout* sunShapeLayout = new QVBoxLayout;
    sunShape->setLayout( sunShapeLayout );

    QGroupBox* sunshapeBox = new QGroupBox;
    sunshapeBox->setTitle ( "Sunshape" );
    sunShapeLayout->addWidget( sunshapeBox );
    SunshapeBox( sunshapeBox );

    QGroupBox* shapeBox = new QGroupBox;
    shapeBox->setTitle ( "Input Aperture" );
    sunShapeLayout->addWidget( shapeBox );
    ShapeBox( shapeBox );
}


void LightDialog::ShapeBox( QGroupBox* shapeBox )
{
	Trace trace( "LightDialog::ShapeBox", false );

    QGridLayout* gridShape = new QGridLayout;
	shapeBox->setLayout( gridShape );

	QLabel* shapeLabel = new QLabel;
	shapeLabel->setText( "Shape type: " );
	gridShape->addWidget(shapeLabel, 0, 1, 1, 1);

	m_shapeCombox = new QComboBox;
	gridShape->addWidget(m_shapeCombox, 0, 2, 1, 2);
	connect( m_shapeCombox, SIGNAL( activated( int ) ), this, SLOT( changeShape( int ) ) );

	//Add elements to sunshape combo
	QList< TShapeFactory* > shapeFactoryList = m_shapeList.values();
	m_shapeCombox->addItem ( "---" );
	for( int j = 0; j <(int) shapeFactoryList.size(); j++ )
		m_shapeCombox->addItem( shapeFactoryList[j]->TShapeIcon(), shapeFactoryList[j]->TShapeName(), m_shapeList.key( shapeFactoryList[j] ) );

	//Select current LightKit Shape
	m_currentShapeIndex = 0;
	if( m_newShape )
	{
		QString name( m_newShape->getTypeId().getName() );
		m_currentShapeIndex = m_shapeCombox->findData( name );
	}

	m_shapeParameters = new QFrame;
	gridShape->addWidget(m_shapeParameters, 1, 2, 3, 2);
	QVBoxLayout* parametersShapeLayout = new QVBoxLayout;
	parametersShapeLayout->setContentsMargins ( 0, 0, 0, 0 );
	parametersShapeLayout->setSizeConstraint( QLayout::SetFixedSize );
    m_shapeParameters->setLayout( parametersShapeLayout );
    changeShape( m_currentShapeIndex );
    m_shapeCombox->setCurrentIndex( m_currentShapeIndex );
}

void LightDialog::SunshapeBox( QGroupBox* sunshapeBox )
{
	Trace trace( "LightDialog::SunshapeBox", false );

   	QGridLayout* gridSunshape = new QGridLayout;
	sunshapeBox->setLayout( gridSunshape );

	QLabel* typeLabel = new QLabel;
	typeLabel->setObjectName( QString::fromUtf8( "typeLabel" ) );
	typeLabel->setText( "Sunshape type: " );
	gridSunshape->addWidget(typeLabel, 0, 1, 1, 1);

	m_sunshapeCombox = new QComboBox;
	gridSunshape->addWidget(m_sunshapeCombox, 0, 2, 1, 2);
	connect( m_sunshapeCombox, SIGNAL( activated ( int ) ), this, SLOT( changeSunshape( int ) ) );

	//Add elements to sunshape combo
	QList< TSunShapeFactory* > sunShapeFactoryList = m_sunshapeList.values();
	m_sunshapeCombox->addItem ( "---" );
	for( int i = 0; i < sunShapeFactoryList.size(); i++ )
		m_sunshapeCombox->addItem( sunShapeFactoryList[i]->TSunShapeIcon(), sunShapeFactoryList[i]->TSunShapeName(),m_sunshapeList.key( sunShapeFactoryList[i] ) );

	//Select current LightKit Sunshape
	m_currentSunShapeIndex = 0;
	if( m_newSunShape )
	{
		QString name( m_newSunShape->getTypeId().getName() );
		m_currentSunShapeIndex = m_sunshapeCombox->findData( name );
	}

	m_sunshapeParameters = new QFrame;
	gridSunshape->addWidget(m_sunshapeParameters, 1, 2, 3, 2);
	QVBoxLayout* parametersSunShapeLayout = new QVBoxLayout;
	parametersSunShapeLayout->setContentsMargins ( 0, 0, 0, 0 );
	parametersSunShapeLayout->setSizeConstraint( QLayout::SetFixedSize );
    m_sunshapeParameters->setLayout( parametersSunShapeLayout );
    changeSunshape( m_currentSunShapeIndex );
    m_sunshapeCombox->setCurrentIndex( m_currentSunShapeIndex );

}
