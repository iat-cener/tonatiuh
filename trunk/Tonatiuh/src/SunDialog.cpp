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
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTabWidget>
#include <QTimeEdit>
#include <QVBoxLayout>

#include "FieldContainerWidget.h"
#include "MapDialog.h"
#include "SunDialog.h" 
#include "sunpos.h"
#include "TDefaultMaterial.h"
#include "TLightKit.h"
#include "Trace.h"
#include "tgc.h"
#include "tgf.h"
#include "TShape.h"
#include "TShapeKit.h"
#include "TShapeFactory.h"
#include "TSunShape.h"
#include "TSunShapeFactory.h"

SunDialog::SunDialog(  TLightKit* currentLightKit, std::vector< TShapeFactory* > shapeFactoryList, std::vector< TSunShapeFactory* > m_sunshapeFactoryList, QWidget* parent )
:QDialog( parent ), m_currentLightKit( currentLightKit ), m_sunShape( 0 ), m_shape( 0 ), m_shapeList( shapeFactoryList ), m_sunshapeList( m_sunshapeFactoryList )
{
	Trace trace( "SunDialog::SunDialog", false );
	
	if( currentLightKit )
	{
		if( currentLightKit->getPart( "tsunshape", false ) )	m_sunShape = static_cast< TSunShape* >( currentLightKit->getPart( "tsunshape", false )->copy( true ) );
		if( currentLightKit->getPart( "icon", false ) )	m_shape = static_cast< TShape* >( currentLightKit->getPart( "icon", false )->copy( true ) );
	}
	
	QHBoxLayout* layout = new QHBoxLayout;
    layout->setObjectName( QString::fromUtf8( "hboxLayout" ) );
	layout->setSizeConstraint(QLayout::SetFixedSize);
  	
    QVBoxLayout* vboxLayout = new QVBoxLayout;
    m_tabWidget = new QTabWidget(  );
    vboxLayout->addWidget( m_tabWidget );
      
	//DirectionTab();
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

SunDialog::~SunDialog()
{
	
}

TLightKit* SunDialog::GetTLightKit()
{
	Trace trace( "SunDialog::GetTLightKit", false );
	
	TLightKit* lightKit = new TLightKit;
	lightKit->setPart( "tsunshape", m_sunShape );
	lightKit->setPart( "icon", m_shape );
 	
	/*//Trasform calculation
	SetAngles();  
    Vector3D direction = Vector3D( sin( m_zenith ) * sin( m_azimuth ), cos( m_zenith ), -sin( m_zenith )*cos( m_azimuth ) );

	Point3D center = Point3D( 0, 0, 0 ) + ( direction * 1000 );
	SbMatrix translate;
	translate.setTranslate( SbVec3f( center.x, center.y, center.z ) );
	
	SbMatrix rotX;
	rotX.setRotate( SbRotation( SbVec3f( 1, 0, 0 ), -m_zenith ) );
	
	SbMatrix rotY;
	rotY.setRotate( SbRotation( SbVec3f( 0, 1, 0 ), -m_azimuth ) );
	
	SbMatrix coinMatrix = rotX* rotY* translate;
	
	SoTransform* transform = new SoTransform;
	transform->setMatrix( coinMatrix );
	
	lightKit->setPart( "transform", transform );*/
	
	return lightKit;
	
}

void SunDialog::changeSunshape( int index )
{
	Trace trace( "SunDialog::changeSunshape", false );
	
	QLayout* frameLayout = m_sunshapeParameters->layout();
	if( !frameLayout ) return;
	int children = frameLayout->count();
	for( int i = 0; i< children; i++ )
		frameLayout->removeItem ( frameLayout->itemAt( i ) );
	
	FieldContainerWidget* nodeContainer;	
	if ( index == 0 )
	{
		nodeContainer = new FieldContainerWidget( 0, QString() );
	}
	else
	{
		if( index == m_currentSunShapeIndex)
		{
			TSunShape* sunshape = static_cast< TSunShape* >( m_currentLightKit->getPart( "tsunshape", false ) );
			m_sunShape = static_cast< TSunShape* >( sunshape->copy( true ) );
		}
		else
		{
			TSunShapeFactory* sunshapeFactory = m_sunshapeList[index -1 ];
			m_sunShape =  sunshapeFactory->CreateTSunShape();
		}
		nodeContainer = new FieldContainerWidget( m_sunShape, QString() );

	}
	nodeContainer->setFixedSize( QSize( 200, 100 ) );
	frameLayout->addWidget( nodeContainer );
}

void SunDialog::changeShape( int index )
{
	Trace trace( "SunDialog::changeShape", false );
	
	QLayout* frameLayout = m_shapeParameters->layout();

	if( !frameLayout && frameLayout != 0 ) return;
	int children = frameLayout->count();
	for( int i = 0; i< children; i++ )
		frameLayout->removeItem ( frameLayout->itemAt( i ) );
	FieldContainerWidget* nodeContainer;
	if ( index == 0 )
	{
		nodeContainer = new FieldContainerWidget( 0, QString() );
	}
	else
	{
		if( index == m_currentShapeIndex)
		{
			TShape* shape = static_cast< TShape* >( m_currentLightKit->getPart( "icon", false ) );
			m_shape = static_cast< TShape* >( shape->copy( true ) );
			
		}
		else
		{
			TShapeFactory* shapeFactory = m_shapeList[index -1 ];
			m_shape =  shapeFactory->CreateTShape();
		}
		nodeContainer = new FieldContainerWidget( m_shape, QString() );
	}
	
	nodeContainer->setFixedSize( QSize( 200, 100 ) );
	frameLayout->addWidget( nodeContainer ); 
}

void SunDialog::openMarble()
{
	Trace trace( "SunDialog::openMarble", false );
	MapDialog marbleDialog;
	marbleDialog.SetCoordinates( m_latitudeSpin->value() * ( tgc::Pi / 180 ), m_longitudeSpin->value() * ( tgc::Pi / 180 ) );
	
	if( marbleDialog.exec() )
	{
		double longitude;
		double latitude;
		
		marbleDialog.GetCoordinates( &longitude, &latitude );
		
		m_longitudeSpin->setValue( longitude * ( 180 / tgc::Pi ) );
		m_latitudeSpin->setValue( latitude * ( 180 / tgc::Pi ) );		
	}
}

void SunDialog::DirectionTab()
{
	Trace trace( "SunDialog::DirectionTab", false );
	
    QWidget* direction = new QWidget();
    m_tabWidget->addTab( direction, tr( "Sun Direction") );

   	QGridLayout* gridLayout = new QGridLayout( );
	direction->setLayout( gridLayout );
	
	m_radioAngles = new QRadioButton( );
    m_radioAngles->setChecked(true);
    m_radioAngles->setText( "Define sun Vector" );
    gridLayout->addWidget(m_radioAngles, 0, 0, 1, 2);
    
    QLabel* labelVector = new QLabel( );
    labelVector->setObjectName( QString::fromUtf8( "labelVector" ) );
    labelVector->setText( "Define sun vector angles" );
    gridLayout->addWidget(labelVector, 1, 1, 1, 3);
    connect( m_radioAngles, SIGNAL( toggled( bool ) ), labelVector, SLOT(setEnabled( bool ) ) );

    QLabel* labelAzimuth = new QLabel( );
    labelAzimuth->setObjectName( QString::fromUtf8( "labelAzimuth" ) );
    labelAzimuth->setText( "Azimuth:" );
    gridLayout->addWidget(labelAzimuth, 2, 2, 1, 1);
    connect( m_radioAngles, SIGNAL( toggled( bool ) ), labelAzimuth, SLOT(setEnabled( bool ) ) );
    
    m_azimuthSpin = new QDoubleSpinBox( );
    m_azimuthSpin->setObjectName( QString::fromUtf8( "m_azimuthSpin" ) );
    m_azimuthSpin->setMaximum ( 360.0 );
    m_azimuthSpin->setDecimals( 6 );  
    gridLayout->addWidget(m_azimuthSpin, 2, 3, 1, 1);
    connect( m_radioAngles, SIGNAL( toggled( bool ) ), m_azimuthSpin, SLOT(setEnabled( bool ) ) );

	QLabel* labelZenith = new QLabel( );
    labelZenith->setObjectName( QString::fromUtf8( "labelZenith" ) );
    labelZenith->setText( "Zenith:" );
    gridLayout->addWidget(labelZenith, 3, 2, 1, 1);
    connect( m_radioAngles, SIGNAL( toggled( bool ) ), labelZenith, SLOT(setEnabled( bool ) ) );

    m_zenithSpin = new QDoubleSpinBox( );
    m_zenithSpin->setObjectName( QString::fromUtf8( "m_zenithSpin" ) );
    m_zenithSpin->setMaximum ( 180.0 );
    m_zenithSpin->setDecimals( 6 );   
    gridLayout->addWidget(m_zenithSpin, 3, 3, 1, 1);
    connect( m_radioAngles, SIGNAL( toggled( bool ) ), m_zenithSpin, SLOT(setEnabled( bool ) ) );
    
    m_radioCalculates = new QRadioButton( );
    m_radioCalculates->setObjectName( QString::fromUtf8( "m_radioCalculates" ) );
    m_radioCalculates->setText( "Calculate sun vector" );
    gridLayout->addWidget(m_radioCalculates, 4, 0, 1, 2);
    
    QLabel* informationCalculate = new QLabel( );
    informationCalculate->setObjectName( QString::fromUtf8( "informationCalculate" ) );
    informationCalculate->setText( "Specify information to calculate sun vector" );
    informationCalculate->setEnabled(false);
    gridLayout->addWidget(informationCalculate, 5, 1, 1, 3);
    connect( m_radioCalculates, SIGNAL( toggled( bool ) ), informationCalculate, SLOT(setEnabled( bool ) ) );
    
    QLabel* labelTime = new QLabel( );
    labelTime->setObjectName(QString::fromUtf8( "labelTime" ) );
    labelTime->setEnabled(false);
    labelTime->setText( "Time (univelsal):" );
    gridLayout->addWidget(labelTime, 6, 2, 1, 1);
    connect( m_radioCalculates, SIGNAL( toggled( bool ) ), labelTime, SLOT(setEnabled( bool ) ) );
	
	m_timeEdit = new QTimeEdit( );
    m_timeEdit->setObjectName( QString::fromUtf8( "m_timeEdit" ) );
    m_timeEdit->setEnabled(false);
    gridLayout->addWidget(m_timeEdit, 6, 3, 1, 1);
    connect( m_radioCalculates, SIGNAL( toggled( bool ) ), m_timeEdit, SLOT(setEnabled( bool ) ) );
    
    QLabel* labelDate = new QLabel;
    labelDate->setObjectName( QString::fromUtf8( "labelDate" ) );
    labelDate->setEnabled(false);
    labelDate->setText( "Date:" );
    gridLayout->addWidget(labelDate, 7, 2, 1, 1);
    connect( m_radioCalculates, SIGNAL( toggled( bool ) ), labelDate, SLOT(setEnabled( bool ) ) );
    
    m_dateEdit = new QDateEdit( );
    m_dateEdit->setObjectName( QString::fromUtf8( "m_dateEdit" ) );
    m_dateEdit->setEnabled(false);
    gridLayout->addWidget(m_dateEdit, 7, 3, 1, 1);
    connect( m_radioCalculates, SIGNAL( toggled( bool ) ), m_dateEdit, SLOT(setEnabled( bool ) ) );
    
    QLabel* labelLongitude = new QLabel;
    labelLongitude->setObjectName( QString::fromUtf8( "labelLongitude" ) );
    labelLongitude->setEnabled(false);
    labelLongitude->setText( "Longitude (degrees):");
    gridLayout->addWidget(labelLongitude, 9, 2, 1, 1);
    connect( m_radioCalculates, SIGNAL( toggled( bool ) ), labelLongitude, SLOT(setEnabled( bool ) ) );

    m_longitudeSpin = new QDoubleSpinBox;
    m_longitudeSpin->setObjectName( QString::fromUtf8( "m_longitudeSpin" ) );
    m_longitudeSpin->setEnabled(false);
    m_longitudeSpin->setMaximum ( 180.0 ); 
    m_longitudeSpin->setMinimum ( -180.0 );
    m_longitudeSpin->setDecimals( 6 );   
    gridLayout->addWidget(m_longitudeSpin, 9, 3, 1, 1);
    connect( m_radioCalculates, SIGNAL( toggled( bool ) ), m_longitudeSpin, SLOT(setEnabled( bool ) ) );
    
    QLabel* labelLatitude = new QLabel;
    labelLatitude->setObjectName( QString::fromUtf8( "labelLatitude" ) );
    labelLatitude->setText( "Latitude (degrees):" );
    labelLatitude->setEnabled(false);
    gridLayout->addWidget(labelLatitude, 10, 2, 1, 1);
    connect( m_radioCalculates, SIGNAL( toggled( bool ) ), labelLatitude, SLOT(setEnabled( bool ) ) );

    m_latitudeSpin = new QDoubleSpinBox;
    m_latitudeSpin->setObjectName(QString::fromUtf8( "m_latitudeSpin") );
    m_latitudeSpin->setEnabled(false);
    m_latitudeSpin->setMaximum ( 90.0 ); 
    m_latitudeSpin->setMinimum ( -90.0 );
    m_latitudeSpin->setDecimals( 6 );
    gridLayout->addWidget(m_latitudeSpin, 10, 3, 1, 1);
    connect( m_radioCalculates, SIGNAL( toggled( bool ) ), m_latitudeSpin, SLOT(setEnabled( bool ) ) );
    
    QPushButton* pushSelect = new QPushButton;
    pushSelect->setObjectName(QString::fromUtf8("pushSelect"));
    pushSelect->setEnabled(false);
    pushSelect->setText( "Select" );
    gridLayout->addWidget(pushSelect, 10, 5, 1, 1);
	connect( m_radioCalculates, SIGNAL( toggled( bool ) ), pushSelect, SLOT(setEnabled( bool ) ) );
	connect(pushSelect, SIGNAL( clicked() ), this, SLOT( openMarble() ) );
	
}

void SunDialog::SunshapeTab()
{
	Trace trace( "SunDialog::SunshapeTab", false );
	
   	QWidget* sunShape = new QWidget;
   	sunShape->setObjectName( QString::fromUtf8( "sunShape" ) );
    m_tabWidget->addTab( sunShape, tr( "Sun Shape") );
    
    QVBoxLayout* sunShapeLayout = new QVBoxLayout;
    sunShape->setLayout( sunShapeLayout );
    
    QGroupBox* sunshapeBox = new QGroupBox;
    sunshapeBox->setTitle ( "Sunshape" );
    sunShapeLayout->addWidget( sunshapeBox );
    
   	QGridLayout* gridSunshape = new QGridLayout;
	sunshapeBox->setLayout( gridSunshape );
	
	QLabel* typeLabel = new QLabel;
	typeLabel->setObjectName( QString::fromUtf8( "typeLabel" ) );
	typeLabel->setText( "Sunshape type: " );
	gridSunshape->addWidget(typeLabel, 0, 1, 1, 1);
	
	m_sunshapeCombox = new QComboBox;
	gridSunshape->addWidget(m_sunshapeCombox, 0, 2, 1, 2);
	connect( m_sunshapeCombox, SIGNAL( activated ( int ) ), this, SLOT( changeSunshape( int ) ) );
	m_sunshapeCombox->addItem ( "---" );
	for( int i = 0; i < (int)m_sunshapeList.size(); i++ )
		m_sunshapeCombox->addItem ( m_sunshapeList[i]->TSunShapeIcon(), m_sunshapeList[i]->TSunShapeName() );
	
	m_currentSunShapeIndex = 0;
	if( m_sunShape )
	{
		QString name( m_sunShape->getTypeId().getName() );
		m_currentSunShapeIndex = m_sunshapeCombox->findText ( name );
	}
	
	m_sunshapeParameters = new QFrame;
	gridSunshape->addWidget(m_sunshapeParameters, 1, 2, 3, 2);
	QVBoxLayout* parametersSunShapeLayout = new QVBoxLayout;
	parametersSunShapeLayout->setContentsMargins ( 0, 0, 0, 0 );
	parametersSunShapeLayout->setSizeConstraint( QLayout::SetFixedSize ); 
    m_sunshapeParameters->setLayout( parametersSunShapeLayout );
    changeSunshape( m_currentSunShapeIndex );
    m_sunshapeCombox->setCurrentIndex( m_currentSunShapeIndex );
	 
	QGroupBox* shapeBox = new QGroupBox;
    shapeBox->setTitle ( "Input Aperture" );
    sunShapeLayout->addWidget( shapeBox );
    
    QGridLayout* gridShape = new QGridLayout;
	shapeBox->setLayout( gridShape );
	
	QLabel* shapeLabel = new QLabel;
	shapeLabel->setText( "Shape type: " );
	gridShape->addWidget(shapeLabel, 0, 1, 1, 1);
	
	m_shapeCombox = new QComboBox;
	gridShape->addWidget(m_shapeCombox, 0, 2, 1, 2);
	m_shapeCombox->addItem ( "---" );
	for( int j = 0; j <(int) m_shapeList.size(); j++ )
		m_shapeCombox->addItem ( m_shapeList[j]->TShapeIcon(), m_shapeList[j]->TShapeName() );
		
	connect( m_shapeCombox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( changeShape( int ) ) );
	
	m_currentShapeIndex = 0;
	if( m_shape )
	{
		QString name( m_shape->getTypeId().getName() );
		m_currentShapeIndex = m_shapeCombox->findText ( name );
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

void SunDialog::SetAngles()
{
	Trace trace( "SunDialog::SetAngles", false );
	if( m_radioAngles->isChecked() )
	{
		m_zenith = m_zenithSpin->value() * ( tgc::Pi / 180);
		m_azimuth = m_azimuthSpin->value() * ( tgc::Pi / 180);
	}
	else if( m_radioCalculates->isChecked() )
	{
		double hours, minutes, seconds;
		QTime time = m_timeEdit->time();
	
		hours = time.hour ();
		minutes = time.minute ();
		seconds = time.second ();
		
		QDate date = m_dateEdit->date();
		int year = date.year();
		int month = date.month();
		int day = date.day();
		
		
		cTime myTime = { year, month, day, hours, minutes, seconds };
	
		double longitude = m_longitudeSpin->value();
		double latitude = m_latitudeSpin->value();
	    
		//Calculate sun position
		cLocation myLocation = {longitude , latitude };
		cSunCoordinates results;
		sunpos( myTime, myLocation, &results );
		
		m_zenith = results.dZenithAngle * ( tgc::Pi / 180);
		m_azimuth = results.dAzimuth * ( tgc::Pi / 180);
	}
}
