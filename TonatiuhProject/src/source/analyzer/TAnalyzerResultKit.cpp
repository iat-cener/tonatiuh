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

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoPickStyle.h>




#include "DifferentialGeometry.h"
#include "Ray.h"
#include "TCube.h"
#include "TMaterial.h"
#include "TDefaultMaterial.h"
#include "TShape.h"
#include "TAnalyzerResultKit.h"
#include "TAnalyzerParameter.h"
#include "TAnalyzerResult.h"
#include "TAnalyzerLevel.h"
#include "BBox.h"
#include "tgf.h"

SO_KIT_SOURCE(TAnalyzerResultKit);

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 */
void TAnalyzerResultKit::initClass()
{
	SO_KIT_INIT_CLASS(TAnalyzerResultKit, TShapeKit, "AnalyzerKit");
}

/**
 * Constructor.
 *
 */
TAnalyzerResultKit::TAnalyzerResultKit()
{
	tshape =0;
	analyzerResult = 0;

	SO_KIT_CONSTRUCTOR(TAnalyzerResultKit);
	SO_KIT_ADD_CATALOG_ENTRY(result, TAnalyzerResult, FALSE, this, "", TRUE);
	//SO_KIT_CHANGE_ENTRY_TYPE(shape, TShape, TCube);
	SO_KIT_INIT_INSTANCE();

	setPart("shape", NULL );
}
/*!
 * Destroys the TAnalyzerResultKit object.
 */
TAnalyzerResultKit::~TAnalyzerResultKit()
{

}

void TAnalyzerResultKit::PrepareCompute()
{
	analyzerResult = static_cast<TAnalyzerResult*> (getPart("result", false));
	tshape = static_cast< TShape* >( getPart("shape", false) );
	analyzerResult->numRayIntersected=0;
}


void TAnalyzerResultKit::Compute(std::vector<Ray> * raysWay, Transform *m_transformWTO, QMutex* mutex)
{
	if( tshape && analyzerResult)
	{
		 std::vector<Ray>::iterator itRay;
		itRay = raysWay->begin();
		bool Touched=false;
		while( itRay < raysWay->end()  && !Touched)
		{
			Ray childCoordinatesRay( (*m_transformWTO)( *itRay ) );
			if( tshape->IntersectP( childCoordinatesRay ) )
			{
				mutex->lock();
				analyzerResult->numRayIntersected ++;
				mutex->unlock();
				Touched = true;
			}
			itRay++;
		}
	}
}


void TAnalyzerResultKit::FinalizeCompute(double raydensity, Transform *m_transformWTO)
{
	SbMatrix coinMatrix = tgf::MatrixFromTransform( *m_transformWTO );
	SbVec3f translation;
	SbRotation rotation;
	SbVec3f scaleFactor;
	SbRotation scaleOrientation;
	coinMatrix.getTransform( translation, rotation, scaleFactor, scaleOrientation );

	float scalex,scaley,scalez;
	scaleFactor.getValue(scalex,scaley,scalez);
	if( tshape && analyzerResult )
	{
		analyzerResult->FinalizeCompute(1.0/(raydensity*(tshape->GetArea()*scalex*scalez/6.0)));
	}
	//TODO: add a GetProjecttedArea(scalex,scaley,scalez) function to all tShape
}


void  TAnalyzerResultKit::ResetValues()
{
	analyzerResult = static_cast<TAnalyzerResult*> (getPart("result", false));
	if (analyzerResult) analyzerResult->ResetValues();
}

void TAnalyzerResultKit::DisplayResult(int typeOfDisplay, SoNodeKitListPart *levelList)
{
	analyzerResult = static_cast<TAnalyzerResult*> (getPart("result", false));
	bool levelFind = false;

	float value = 0.0f;
	if (typeOfDisplay == 0 /*CURRENT_VALUE*/) value=analyzerResult->currentValue.getValue();
	else if (typeOfDisplay == 1 /*MIN_VALUE*/) value=analyzerResult->minValue.getValue();
	else if (typeOfDisplay == 2 /*MAX_VALUE*/) value=analyzerResult->maxValue.getValue();


	for (int i=0; i<levelList->getNumChildren();i++)
	{
		TAnalyzerLevel * analyzerLevel = (TAnalyzerLevel *)levelList->getChild(i);
		if (analyzerLevel->isActive.getValue() && (value > analyzerLevel->minValue.getValue()))
		{
			TMaterial * material1 = (TMaterial *) getPart("material",false );
			if (material1)
			{
				material1->ambientColor.setValue(analyzerLevel->materialColor.getValue());
				material1->diffuseColor.setValue(analyzerLevel->materialColor.getValue());
				material1->specularColor.setValue(analyzerLevel->materialColor.getValue());
				material1->emissiveColor.setValue(analyzerLevel->materialColor.getValue());
				material1->transparency.setValue(analyzerLevel->materialTransparency.getValue());
			}
			setPart("drawStyle",NULL);
			setPart("pickStyle",NULL);
			levelFind = true;
			break;
		}
	}
	if (!levelFind)
	{
		TMaterial * material1 = (TMaterial *)  getPart("material",false );
		if (material1)
		{
			material1->ambientColor.setValue(0.0,0.0,0.0);
			material1->diffuseColor.setValue(0.0,0.0,0.0);
			material1->specularColor.setValue(0.0,0.0,0.0);
			material1->emissiveColor.setValue(0.0,0.0,0.0);
			material1->transparency.setValue(0.0);
		}

		SoDrawStyle * drawStyle = new SoDrawStyle;
		drawStyle->style = SoDrawStyle::INVISIBLE;
		setPart("drawStyle",drawStyle);
		
		SoPickStyle * pickStyle = new SoPickStyle;
		pickStyle->style = SoPickStyle::UNPICKABLE;
		setPart("pickStyle",pickStyle);
	}
}

/**
 * Returns the icon that represents TSeparatorKit nodes.
 */
QString TAnalyzerResultKit::getIcon()
{
    return QString(":/icons/AnalyzerKit.png");

}
