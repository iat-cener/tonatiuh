/***************************************************************************
 Copyright (C) 2008 by the Tonatiuh Software Development Team.

 This file is part of Tonatiuh.

 Tonatiuh program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.


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

 Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola, Gilda Jimenez,
 Juana Amieva, Azael Mancillas, Cesar Cantu.
 ***************************************************************************/

 #include <QLabel>
 #include <QVBoxLayout>

 #include <Inventor/nodes/SoComplexity.h>
 #include <Inventor/nodes/SoCoordinate3.h>
 #include <Inventor/nodes/SoDrawStyle.h>
 #include <Inventor/nodes/SoIndexedFaceSet.h>
 #include <Inventor/nodes/SoLineSet.h>
 #include <Inventor/nodes/SoMaterial.h>
 #include <Inventor/nodes/SoPointSet.h>
 #include <Inventor/nodes/SoSeparator.h>
 #include <Inventor/nodes/SoSphere.h>
 #include <Inventor/nodes/SoText2.h>
 #include <Inventor/nodes/SoTranslation.h>
 #include <Inventor/nodes/SoTransform.h>
 #include <Inventor/nodes/SoShapeHints.h>
 #include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include "CelestialWidget.h"
#include "Point3D.h"
#include "Ray.h"
#include "tgc.h"
#include "Vector3D.h"


CelestialWidget::CelestialWidget( QWidget* parent )
:QWidget( parent ), sphereRadio( 120.0 ), m_declination( 0 ), m_rightAscension( 0 )
{
	QVBoxLayout* mainLayout = new QVBoxLayout;
	setLayout( mainLayout );

	QWidget* examinerWidget = new QWidget;
	examinerWidget->setFixedSize( 490, 300 );
	mainLayout->addWidget(examinerWidget);

	m_rootNode = new SoSeparator;

	m_rootNode->addChild( Ejes() );
	m_rootNode->addChild( Sphere() );
	m_rootNode->addChild( CelestialEquator() );
	m_rootNode->addChild( Ecliptic() );
	m_rootNode->addChild( Points() );
	m_rootNode->addChild( RightAscension() );
	m_rootNode->addChild( Declination() );
	m_rootNode->addChild( Star() );

	SoQtExaminerViewer* myRenderArea = new SoQtExaminerViewer( examinerWidget );
	myRenderArea->setSceneGraph( m_rootNode );
	SbColor col( 0.86f, 0.86f, 0.86f );
	myRenderArea->setBackgroundColor(col);
	myRenderArea->show( );

	QWidget* labelsWidget = new QWidget;
	mainLayout->addWidget( labelsWidget );
	QGridLayout* labelsLayout = new QGridLayout;
	labelsWidget->setLayout( labelsLayout );

	QLabel* m_rightLabel = new QLabel;
	m_rightLabel->setText( "Right Ascension:" );
	labelsLayout->addWidget( m_rightLabel, 0, 0, 1, 1 );

	m_rightValue = new QLabel;
	m_rightValue->setText( QString::number( m_rightAscension ) );
	labelsLayout->addWidget( m_rightValue, 0, 1, 1, 3 );

	QLabel* m_declinationLabel = new QLabel;
	m_declinationLabel->setText( "Declination:" );
	labelsLayout->addWidget( m_declinationLabel, 1, 0, 1, 1 );

	m_declinationValue = new QLabel;
	m_declinationValue->setText( QString::number( m_declination ) );
	labelsLayout->addWidget( m_declinationValue, 1, 1, 1, 3 );
}

CelestialWidget::~CelestialWidget()
{
}

void CelestialWidget::CoordinatesChanged( cSunCoordinates coordinates )
{
	m_rightAscension = coordinates.dRightAscension;
	m_rightValue->setText( QString::number( m_rightAscension ) );
	m_declination = coordinates.dDeclination;
	m_declinationValue->setText( QString::number( m_declination ) );

	//Right Ascension
	SoSeparator* ascension = RightAscension();
	m_rootNode->replaceChild( 5, ascension );

	//Declination
	SoSeparator* declination = Declination();
	m_rootNode->replaceChild( 6, declination );

	//Star
	SoSeparator* star = Star();
	m_rootNode->replaceChild( 7, star );
}

SoSeparator* CelestialWidget::CelestialEquator() const
{
	SoSeparator* ecuador = new SoSeparator;

	SoMaterial *myMaterial = new SoMaterial;
	myMaterial->diffuseColor.setValue(0.0f, 0.0f, 0.80f); // Olive
	ecuador->addChild(myMaterial);

	SoDrawStyle * drawstyle = new SoDrawStyle;
	drawstyle->lineWidth = 2;
	ecuador->addChild( drawstyle );

	float p[360][3];
	for( int fi = 0; fi<360; fi++ )
	{
		p[fi][0] = -sin( fi * (tgc::Pi / 180) ) * sphereRadio ;
		p[fi][1] = 0.0;
		p[fi][2] = -cos( fi * (tgc::Pi / 180) ) * sphereRadio ;

	}

	int lines[1]={360};
	SoCoordinate3 * coord3 = new SoCoordinate3;
	coord3->point.setValues(0, 360, p);
	ecuador->addChild(coord3);

	SoLineSet* lineset=new SoLineSet;
	lineset->numVertices.setValues(0,1,lines);
	ecuador->addChild(lineset);

	return ecuador;
}

SoSeparator* CelestialWidget::Declination()
{
	//Decliantion
	SoSeparator* declination = new SoSeparator;

	SoMaterial* declinationMaterial = new SoMaterial;
	declinationMaterial->diffuseColor.setValue( 1.0f, 0.5f, 0.0 ); // Green
	declination->addChild( declinationMaterial );

	SoDrawStyle * drawstyle = new SoDrawStyle;
	drawstyle->lineWidth = 4;
	declination->addChild( drawstyle );

	float declinationPoints[360][3];

	for( int theta = 0; theta< 360; theta++ )
	{
		double grad = ( m_declination / 360 ) * theta;
		declinationPoints[theta][0] = sin( m_rightAscension ) * cos( grad )* sphereRadio;
		declinationPoints[theta][1] = sin( grad ) * sphereRadio;
		declinationPoints[theta][2] = cos( m_rightAscension ) * cos( grad ) * sphereRadio ;
	}

	int declinationLines[1] = {360};
	SoCoordinate3 * declinationCoord3 = new SoCoordinate3;
	declinationCoord3->point.setValues( 0, 360, declinationPoints );
	declination->addChild( declinationCoord3 );

	SoLineSet* decliantionLine=new SoLineSet;
	decliantionLine->numVertices.setValues( 0, 1, declinationLines );
	declination->addChild( decliantionLine );


	SoSeparator* curve = new SoSeparator;
	declination->addChild( curve );

	SoMaterial* curveMaterial = new SoMaterial;
	curveMaterial->diffuseColor.setValue( 1.0, 0.50f, 0.0 ); // Orange
	curve->addChild( curveMaterial );

	float curvePoints[270][3];
	Point3D center( 0, 0, 0 );

	int numPoints = 0;
	int indexes[360];
	for( int index = 1; index <= 90; index ++ )
	{
		curvePoints[numPoints][0]= center.x;
		curvePoints[numPoints][1]= center.y;
		curvePoints[numPoints][2]= center.z;
		indexes[ ( index -1 ) * 4 ] = numPoints;
		numPoints++;

		double grad1 = ( m_declination / 360 ) * 4 * ( index -1 );
		curvePoints[numPoints][0] = sin( m_rightAscension ) * cos( grad1 )* sphereRadio;
		curvePoints[numPoints][1] = sin( grad1 ) * sphereRadio;
		curvePoints[numPoints][2] = cos( m_rightAscension ) * cos( grad1 ) * sphereRadio ;
		indexes[ ( ( index -1 ) * 4 ) +1 ] = numPoints;
		numPoints++;

		double grad2 = ( m_declination / 360 ) * 4 * index;
		curvePoints[numPoints][0] = sin( m_rightAscension ) * cos( grad1 )* sphereRadio;
		curvePoints[numPoints][1] = sin( grad2 ) * sphereRadio;
		curvePoints[numPoints][2] = cos( m_rightAscension ) * cos( grad2 ) * sphereRadio ;
		indexes[ ( ( index -1 ) * 4 ) +2 ] = numPoints;
		numPoints++;

		indexes[ ( ( index -1 ) * 4 ) + 3 ] = -1;

	 }

	SoCoordinate3 * curveCoord3 = new SoCoordinate3;
	curveCoord3->point.setValues( 0, 270, curvePoints );
	curve->addChild( curveCoord3 );

	SoIndexedFaceSet* facet = new SoIndexedFaceSet;
	facet->coordIndex.setValues( 0, 360, indexes );
	curve->addChild( facet );

	return declination;
}

SoSeparator* CelestialWidget::Ecliptic() const
{
	SoSeparator* ecliptic = new SoSeparator;

	SoMaterial *myMaterial = new SoMaterial;
	myMaterial->diffuseColor.setValue( 0.80f, 0.80f, 0.0f); // Olive
	ecliptic->addChild(myMaterial);

	SoDrawStyle * drawstyle = new SoDrawStyle;
	drawstyle->lineWidth = 2;
	ecliptic->addChild( drawstyle );

	float p[360][3];
	for( int fi = 0; fi<360; fi++ )
	{
		p[fi][0] = cos( tgc::Ecliptic ) * sin( fi * (tgc::Pi / 180) ) * sphereRadio;
		p[fi][1] = sin( tgc::Ecliptic ) * sin( fi * (tgc::Pi / 180) ) * sphereRadio;
		p[fi][2] = -cos( fi * (tgc::Pi / 180) ) * sphereRadio;
	}

	int lines[1]={360};
	SoCoordinate3 * coord3 = new SoCoordinate3;
	coord3->point.setValues(0, 360, p);
	ecliptic->addChild(coord3);

	SoLineSet* lineset=new SoLineSet;
	lineset->numVertices.setValues(0,1,lines);
	ecliptic->addChild(lineset);

	return ecliptic;
}

SoSeparator* CelestialWidget::Ejes() const
{
	 SoSeparator* polos = new SoSeparator;

	SoSeparator* eje = new SoSeparator;
	polos->addChild( eje );

	SoMaterial *myMaterial = new SoMaterial;
	myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0); // Red
	eje->addChild(myMaterial);

	SoDrawStyle * drawstyle = new SoDrawStyle;
	drawstyle->lineWidth = 2;
	eje->addChild( drawstyle );

	float p[2][3]={ {0.0, -sphereRadio-40, 0.0}, {0.0, sphereRadio+40, 0.0} };

	SoCoordinate3* coord3 = new SoCoordinate3;
	coord3->point.setValues(0, 2, p);
	eje->addChild(coord3);

	int lines[1]={2};
	SoLineSet* lineset=new SoLineSet;
	lineset->numVertices.setValues(0,1,lines);
	eje->addChild(lineset);

	//North celestial pole
	SoSeparator* norte = new SoSeparator;

	SoMaterial* norteMaterial = new SoMaterial;
	norteMaterial->diffuseColor.setValue( 0.0, 0.0, 0.0 ); // Red
	norte->addChild( norteMaterial );

	SoTransform* norteTransf = new SoTransform;
	norteTransf->translation.setValue( SbVec3f( 0.0, sphereRadio+30, 0.0 ) );
	norte->addChild( norteTransf );

	SoText2* northText= new SoText2();
	northText->string.setValue("North celestial pole");
	northText->justification = SoText2::CENTER;
	norte->addChild( northText );
	polos->addChild( norte );

	//South celestial pole
	SoSeparator* south = new SoSeparator;

	SoMaterial* southMaterial = new SoMaterial;
	southMaterial->diffuseColor.setValue( 0.0, 0.0, 0.0 ); // Red
	south->addChild( southMaterial );

	SoTransform* southTransf = new SoTransform;
	southTransf->translation.setValue( SbVec3f( 0.0, -sphereRadio-30, 0.0 ) );
	south->addChild( southTransf );

	SoText2* southText= new SoText2();
	southText->string.setValue("South celestial pole");
	southText->justification = SoText2::CENTER;
	south->addChild( southText );
	polos->addChild( south );

	return polos;
}

SoSeparator* CelestialWidget::Points() const
{
	SoSeparator* points = new SoSeparator;

	//Earth
	SoSeparator* earth = new SoSeparator;
	points->addChild(earth);

	SoMaterial *myMaterial = new SoMaterial;
	myMaterial->diffuseColor.setValue( 0.8f, 0.8f, 0.9f); // Blue
	earth->addChild(myMaterial);

	SoSphere* earthSphere=new SoSphere;
	earthSphere->radius=5;
	earth->addChild(earthSphere);

	//AriesPoint
	SoSeparator* aries = new SoSeparator;
	points->addChild(aries);

	SoSeparator* ariesPoint = new SoSeparator;
	aries->addChild(ariesPoint);

	SoMaterial* ariesMaterial = new SoMaterial;
	ariesMaterial->diffuseColor.setValue( 0.0f, 0.0f, 0.9f); // Blue
	ariesPoint->addChild(ariesMaterial);

	SoTranslation* ariesTransform = new SoTranslation;
	ariesTransform->translation.setValue( 0.0f, 0.0f, sphereRadio );
	ariesPoint->addChild( ariesTransform);

	SoSphere* ariesSphere=new SoSphere;
	ariesSphere->radius=5;
	ariesPoint->addChild(ariesSphere);

	SoSeparator* ariesText = new SoSeparator;
	aries->addChild( ariesText );

	SoMaterial* ariesTextMaterial = new SoMaterial;
	ariesTextMaterial->diffuseColor.setValue( 0.0, 0.0, 0.0 ); // Red
	ariesText->addChild( ariesTextMaterial );

	SoTransform* ariesTextTransf = new SoTransform;
	ariesTextTransf->translation.setValue( SbVec3f( 0.0, 0.0, sphereRadio+30 ) );
	ariesText->addChild( ariesTextTransf );

	SoText2* textNode= new SoText2();
	textNode->string.setValue("Aries");
	textNode->justification = SoText2::CENTER;
	ariesText->addChild(textNode);

	//Libra Point
	SoSeparator* libra = new SoSeparator;
	points->addChild(libra);

	SoSeparator* libraPoint = new SoSeparator;
	libra->addChild(libraPoint);

	SoMaterial* libraMaterial = new SoMaterial;
	libraMaterial->diffuseColor.setValue( 0.0f, 0.0f, 0.9f); // Blue
	libraPoint->addChild(libraMaterial);

	SoTranslation* libraTransform = new SoTranslation;
	libraTransform->translation.setValue( 0.0f, 0.0f, -sphereRadio );
	libraPoint->addChild( libraTransform);

	SoSphere* libraSphere=new SoSphere;
	libraSphere->radius=5;
	libraPoint->addChild(libraSphere);

	SoSeparator* libraText = new SoSeparator;
	libra->addChild( libraText );

	SoMaterial* libraTextMaterial = new SoMaterial;
	libraTextMaterial->diffuseColor.setValue( 0.0, 0.0, 0.0 ); // Red
	libraText->addChild( libraTextMaterial );

	SoTransform* libraTextTransf = new SoTransform;
	libraTextTransf->translation.setValue( SbVec3f( 0.0, 0.0, -sphereRadio-30 ) );
	libraText->addChild( libraTextTransf );

	SoText2* text= new SoText2();
	text->string.setValue("Libra");
	text->justification = SoText2::CENTER;
	libraText->addChild(text);

	return points;
}


SoSeparator* CelestialWidget::RightAscension()
{
	//Right Ascension
	SoSeparator* ascension = new SoSeparator;

	SoSeparator* right = new SoSeparator;
	ascension->addChild( right );

	SoMaterial* ascensionMaterial = new SoMaterial;
	ascensionMaterial->diffuseColor.setValue( 1.0, 0.50f, 0.0 ); // Orange
	right->addChild( ascensionMaterial );

	SoDrawStyle * drawstyle = new SoDrawStyle;
	drawstyle->lineWidth = 3;
	right->addChild( drawstyle );

	float ascensionPoints[360][3];
	for( int fi = 0; fi < 360; fi++ )
	{
		double grad = ( m_rightAscension / 360 ) * fi;
		ascensionPoints[fi][0] = sin( grad )* sphereRadio ;
		ascensionPoints[fi][1] = 0.0;
		ascensionPoints[fi][2] = cos( grad ) * sphereRadio ;
	}

	int ascensionLines[1] = {360};
	SoCoordinate3 * ascensionCoord3 = new SoCoordinate3;
	ascensionCoord3->point.setValues( 0, 360, ascensionPoints );
	right->addChild( ascensionCoord3 );

	SoLineSet* ascensionLine=new SoLineSet;
	ascensionLine->numVertices.setValues( 0, 1, ascensionLines );
	right->addChild( ascensionLine );

	SoSeparator* curve = new SoSeparator;

	SoMaterial* curveMaterial = new SoMaterial;
	curveMaterial->diffuseColor.setValue( 1.0, 0.50f, 0.0 ); // Orange
	curve->addChild( curveMaterial );

	float curvePoints[270][3];
	Point3D center( 0, 0, 0 );

	int numPoints = 0;
	int indexes[360];
	for( int index = 1; index <= 90; index ++ )
	{
		curvePoints[numPoints][0]= center.x;
		curvePoints[numPoints][1]= center.y;
		curvePoints[numPoints][2]= center.z;
		indexes[ ( index -1 ) * 4 ] = numPoints;
		numPoints++;

		double grad1 = ( m_rightAscension / 360 ) * 4 * ( index -1 );
		curvePoints[numPoints][0] = sin( grad1 )* sphereRadio ;
		curvePoints[numPoints][1] = 0.0;
		curvePoints[numPoints][2] = cos( grad1 ) * sphereRadio ;
		indexes[ ( ( index -1 ) * 4 ) +1 ] = numPoints;
		numPoints++;

		double grad2 = ( m_rightAscension / 360 ) * 4 * index;
		curvePoints[numPoints][0] = sin( grad2 )* sphereRadio ;
		curvePoints[numPoints][1] = 0.0;
		curvePoints[numPoints][2] = cos( grad2 ) * sphereRadio ;
		indexes[ ( ( index -1 ) * 4 ) +2 ] = numPoints;
		numPoints++;

		indexes[ ( ( index -1 ) * 4 ) + 3 ] = -1;
	}

	SoCoordinate3 * curveCoord3 = new SoCoordinate3;
	curveCoord3->point.setValues( 0, 270, curvePoints );
	curve->addChild( curveCoord3 );

	SoIndexedFaceSet* facet = new SoIndexedFaceSet;
	facet->coordIndex.setValues( 0, 360, indexes );
	curve->addChild( facet );

	ascension->addChild( curve );
	return ascension;
}

SoSeparator* CelestialWidget::Sphere() const
{
	SoSeparator* sph = new SoSeparator;

	SoMaterial *myMaterial = new SoMaterial;
	myMaterial->diffuseColor.setValue( 0.0f, 0.0f, 1.0f ); // Blue
	myMaterial->transparency.setValue( 0.3f );
	sph->addChild( myMaterial );

	SoComplexity* complexity = new SoComplexity;
	complexity->value = 0.5;
	sph->addChild( complexity );

	SoSphere* sphere = new SoSphere;
	sphere->radius = sphereRadio;
	sph->addChild( sphere );

	return sph;
}

SoSeparator* CelestialWidget::Star()
{
	//Star definition
	SoSeparator* star = new SoSeparator;

	SoMaterial *myMaterial = new SoMaterial;
	myMaterial->diffuseColor.setValue(1.0, 1.0, 0.0); // yellow
	star->addChild(myMaterial);

	SoTranslation* starTransform = new SoTranslation;
	starTransform->translation.setValue( cos( m_declination )* sin( m_rightAscension ) * sphereRadio, sin( m_declination ) * sphereRadio, cos( m_declination )* cos( m_rightAscension) * sphereRadio ) ;
	star->addChild( starTransform);

	SoSphere* starSphere=new SoSphere;
	starSphere->radius=10;
	star->addChild(starSphere);

	return star;
}
