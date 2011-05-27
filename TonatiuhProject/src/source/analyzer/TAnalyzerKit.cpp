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

Contributors: Javier Garcia-Barberena, Iï¿½aki Perez, Inigo Pagola,  Gilda Jimenez,
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
#include "TShapeKit.h"
#include "TShape.h"
#include "TAnalyzerKit.h"
#include "TAnalyzerParameter.h"
#include "TAnalyzerResult.h"
#include "TAnalyzerLevel.h"
#include "TAnalyzerResultKit.h"
#include "BBox.h"
#include <sstream>

SO_KIT_SOURCE(TAnalyzerKit);

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 */
void TAnalyzerKit::initClass()
{
	SO_KIT_INIT_CLASS(TAnalyzerKit, TSeparatorKit, "AnalyzerKit");
}

/**
 * Constructor.
 *
 */
TAnalyzerKit::TAnalyzerKit()
{

	SO_KIT_CONSTRUCTOR(TAnalyzerKit);
	SO_NODE_ADD_FIELD( previousSplit, (0,0,0) );
	SO_NODE_ADD_FIELD( previousElemSize, (0) );
	

	//SO_KIT_CHANGE_ENTRY_TYPE(shape, TShape, TCube);
	SO_KIT_ADD_CATALOG_ENTRY(parameter, TAnalyzerParameter, FALSE, this, "", TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(result, TAnalyzerResult, FALSE, this, "", TRUE);
	SO_KIT_ADD_CATALOG_LIST_ENTRY(levelList, SoGroup, FALSE, this, "" , TAnalyzerLevel, TRUE);
	SO_KIT_ADD_LIST_ITEM_TYPE(levelList, TAnalyzerLevel);


	SO_KIT_INIT_INSTANCE();


	getPart("parameter",true)->setName("Parameter");
	getPart("result",true)->setName("Result");
	TDefaultMaterial * material1 = new TDefaultMaterial;
	//material1->diffuseColor.setValue(1.0, 0.0, 0.0);
	//material1->transparency.setValue(0.70);

	SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( getPart( "childList", true ) );

	TCube * cube = new TCube;
	TShapeKit * theShapeKit = new TShapeKit;
	theShapeKit->setPart("shape", cube );
	theShapeKit->setPart("material", material1 );
	SoDrawStyle * drawStyle = new SoDrawStyle;
	drawStyle->style = SoDrawStyle::INVISIBLE;
	theShapeKit->setPart("drawStyle",drawStyle);

	SoPickStyle * pickStyle = new SoPickStyle;
	pickStyle->style = SoPickStyle::UNPICKABLE;
	theShapeKit->setPart("pickStyle",pickStyle);

	coinPartList->addChild( theShapeKit );
	//theShapeKit->setSearchingChildren( true );
	TSeparatorKit * analyzerResultList = new TSeparatorKit();
	analyzerResultList->setName("Results");
	coinPartList->addChild( analyzerResultList );

	previousSplit.setValue(0,0,0);
	previousElemSize.setValue(0);

	//setPart("shape", NULL );

	SoNodeKitListPart *levelList = GetLevelList();
	levelList->setName( "Levels" );

	TAnalyzerLevel *level1 = new TAnalyzerLevel;
	level1->setName("L1");
	levelList->addChild(level1);

	TAnalyzerLevel *level2 = new TAnalyzerLevel;
	level2->minValue = 40.0;
	level2->materialColor.setValue(0.66f, 0.33f, 0.0f);
	level2->materialTransparency.setValue(0.65f);
	level2->setName("L2");
	levelList->addChild(level2);

	TAnalyzerLevel *level3 = new TAnalyzerLevel;
	level3->minValue = 30.0;
	level3->materialColor.setValue(0.33f, 0.33f, 0.0f);
	level3->materialTransparency.setValue(0.70f);
	level3->setName("L3");
	levelList->addChild(level3);

	TAnalyzerLevel *level4 = new TAnalyzerLevel;
	level4->minValue = 20.0;
	level4->materialColor.setValue(0.0f, 0.66f, 0.0f);
	level4->materialTransparency.setValue(0.75f);
	level4->setName("L4");
	levelList->addChild(level4);

	TAnalyzerLevel *level5 = new TAnalyzerLevel;
	level5->minValue = 10.0;
	level5->materialColor.setValue(0.0f, 0.33f, 0.0f);
	level5->materialTransparency.setValue(0.80f);
	level5->setName("L5");
	levelList->addChild(level5);

	TAnalyzerLevel *level6 = new TAnalyzerLevel;
	level6->minValue = 5.0;
	level6->materialColor.setValue(0.0f, 0.15f, 0.18f);
	level6->materialTransparency.setValue(0.85f);
	level6->setName("L6");
	levelList->addChild(level6);

	TAnalyzerLevel *level7 = new TAnalyzerLevel;
	level7->minValue = 2.0;
	level7->materialColor.setValue(0.0f, 0.0f, 0.33f);
	level7->materialTransparency.setValue(0.90f);
	level7->setName("L7");
	levelList->addChild(level7);


}
/*!
 * Destroys the TAnalyzerKit object.
 */
TAnalyzerKit::~TAnalyzerKit()
{

}

SoNodeKitListPart * TAnalyzerKit::GetLevelList()
{
	return static_cast<SoNodeKitListPart*> (getPart("levelList", true));
}

TSeparatorKit * TAnalyzerKit::GetResultsList()
{
	SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( getPart( "childList", true ) );
	if (coinPartList->getNumChildren()>1)
		return static_cast< TSeparatorKit* >( coinPartList->getChild(1) );
	else
		return NULL;
}

std::string to_string( const int & Value )
{
    // utiliser un flux de sortie pour créer la chaîne
    std::ostringstream oss;
    // écrire la valeur dans le flux
    oss << Value;
    // renvoyer une string
    return oss.str();
}

void TAnalyzerKit::RemoveResult(SceneModel * pModel)
{
	TSeparatorKit * analyzerResultList = GetResultsList();
	if (analyzerResultList)
	{
		SoNodeKitListPart*analyzerResultListChilds = static_cast< SoNodeKitListPart* >( analyzerResultList->getPart( "childList", true ) );
		//unsigned int nbSplit = xSplit*ySplit*zSplit;
		if ( analyzerResultListChilds )
		{
			while (analyzerResultListChilds->getNumChildren()>0)
			{
					pModel->RemoveCoinNode( 0, *analyzerResultList );
			}
				previousSplit.setValue(0,0,0);
				previousElemSize.setValue(0);

		}
	}
}

void TAnalyzerKit::PrepareCompute(SceneModel * pModel)
{

    TAnalyzerParameter * analyzerParameter;
    TAnalyzerResult * analyzerResult;
    TShape* tshape;
    TMaterial * material ;


	analyzerResult = static_cast<TAnalyzerResult*> (getPart("result", false));

	SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( getPart( "childList", true ) );
	TShapeKit * mainShapeKit = static_cast< TShapeKit* >( coinPartList->getChild(0) );
	material = static_cast< TMaterial* >(mainShapeKit->getPart("material", false) );
	tshape = static_cast< TShape* >( mainShapeKit->getPart("shape", false) );

	analyzerResult->numRayIntersected=0;
	analyzerParameter = static_cast<TAnalyzerParameter*> (getPart("parameter", false));
	analyzerParameter->setParent(this);

	short xSplit,ySplit,zSplit;
	analyzerParameter->split.getValue().getValue(xSplit,ySplit,zSplit);
	if (xSplit<1) xSplit=1;
	if (ySplit<1) ySplit=1;
	if (zSplit<1) zSplit=1;
	analyzerParameter->split.setValue(xSplit,ySplit,zSplit);
	TSeparatorKit * analyzerResultList = GetResultsList();
	if (analyzerResultList)
	{
		SoNodeKitListPart*analyzerResultListChilds = static_cast< SoNodeKitListPart* >( analyzerResultList->getPart( "childList", true ) );

		if ((analyzerParameter->split.getValue()!=previousSplit.getValue())
			||(analyzerParameter->elemSize.getValue()!=previousElemSize.getValue()))
		{
			//unsigned int nbSplit = xSplit*ySplit*zSplit;
			if ( analyzerResultListChilds )
			{
				RemoveResult(pModel);

				BBox box = tshape->GetBBox();
				double incX = (box.pMax.x-box.pMin.x)/(2.0*double(xSplit));
				double incY = (box.pMax.y-box.pMin.y)/(2.0*double(ySplit));
				double incZ = (box.pMax.z-box.pMin.z)/(2.0*double(zSplit));

				for( int indexX = 0; indexX < xSplit; indexX++ )
				{
					for( int indexY = 0; indexY < ySplit; indexY++ )
					{
						for( int indexZ = 0; indexZ < zSplit; indexZ++ )
						{
							TAnalyzerResultKit * tanalyzerResultKit = new TAnalyzerResultKit;

							TDefaultMaterial * material2 = new TDefaultMaterial;
							material2->diffuseColor.setValue(1.0, 0.0, 0.0);
							material2->transparency.setValue(0.70f);
							tanalyzerResultKit->setPart( "material", material2 );

							TShape * tshape2 = static_cast< TShape* >( tshape->copy( false ) );
							tanalyzerResultKit->setPart( "shape", tshape2 );

							SoTransform* subTransform = new SoTransform;
							subTransform->translation.setValue( box.pMin.x + (1.0+2.0*indexX)*incX,
									box.pMin.y + (1.0+2.0*indexY)*incY, box.pMin.z + (1.0+2.0*indexZ)*incZ );
							subTransform->scaleFactor.setValue( 1.0/double(xSplit)*0.95, 1.0/double(ySplit)*.95, 1.0/double(zSplit)*0.95 );

							tanalyzerResultKit->setPart( "transform" , subTransform );
							tanalyzerResultKit->setName(  ("x:" + to_string(indexX)+" - y:"+to_string(indexY)+" - z:"+to_string(indexZ)).c_str() );

							pModel->InsertCoinNode(*tanalyzerResultKit, *analyzerResultList );
						}
					}
				}

	/*			while (analyzerResultListChilds->getNumChildren()>0)
				{
						pModel->RemoveCoinNode( 0, *analyzerResultList );
				}*/
			}
			previousSplit.setValue(xSplit,ySplit,zSplit);
			previousElemSize=analyzerParameter->elemSize.getValue();
		}
	}
}

void TAnalyzerKit::FinalizeCompute(double /*raydensity*/, Transform * /*m_transformWTO*/)
{
	SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( getPart( "childList", true ) );
	TShapeKit * mainShapeKit = static_cast< TShapeKit* >( coinPartList->getChild(0) );
	SoDrawStyle * drawStyle = new SoDrawStyle;
	drawStyle->style = SoDrawStyle::INVISIBLE;
	mainShapeKit->setPart("drawStyle",drawStyle);

	SoPickStyle * pickStyle = new SoPickStyle;
	pickStyle->style = SoPickStyle::UNPICKABLE;
	mainShapeKit->setPart("pickStyle",pickStyle);
}

void TAnalyzerKit::UpdateSize(SceneModel * pModel)
{
	TAnalyzerParameter * analyzerParameter = static_cast<TAnalyzerParameter*> (getPart("parameter", false));
	if (analyzerParameter &&((analyzerParameter->split.getValue()!=previousSplit.getValue())
	||(analyzerParameter->elemSize.getValue()!=previousElemSize.getValue())))
	{
		TShape* tshape;
		SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( getPart( "childList", true ) );
		TShapeKit * mainShapeKit = static_cast< TShapeKit* >( coinPartList->getChild(0) );
		tshape = static_cast< TShape* >( mainShapeKit->getPart("shape", false) );
		if (tshape->getTypeId().isDerivedFrom( TCube::getClassTypeId()))
		{
			short xSplit,ySplit,zSplit;
			analyzerParameter->split.getValue().getValue(xSplit,ySplit,zSplit);

			TCube * tcube = static_cast< TCube* >( tshape );

			tcube->m_width.setValue(analyzerParameter->elemSize.getValue()*xSplit);
			tcube->m_height.setValue(analyzerParameter->elemSize.getValue()*ySplit);
			tcube->m_depth.setValue(analyzerParameter->elemSize.getValue()*zSplit);
			RemoveResult(pModel);
		}
	}
}

void TAnalyzerKit::DisplayResults()
{
	TSeparatorKit * analyzerResultList = GetResultsList();
	if (analyzerResultList)
	{
		SoNodeKitListPart*analyzerResultListChilds = static_cast< SoNodeKitListPart* >( analyzerResultList->getPart( "childList", true ) );
		TAnalyzerParameter * analyzerParameter = static_cast<TAnalyzerParameter*> (getPart("parameter", false));
		analyzerParameter->setParent(this);
		for( int i=0; i<analyzerResultListChilds->getNumChildren();i++)
		{
			TAnalyzerResultKit* resultChild = static_cast< TAnalyzerResultKit* >(analyzerResultListChilds->getChild(i) );
			resultChild->DisplayResult(analyzerParameter->infoDisplayed.getValue(),GetLevelList());
		}
	}
}

void TAnalyzerKit::ResetValues()
{
	TSeparatorKit * analyzerResultList = GetResultsList();
	if (analyzerResultList)
	{
		SoNodeKitListPart*analyzerResultListChilds = static_cast< SoNodeKitListPart* >( analyzerResultList->getPart( "childList", true ) );
		for( int i=0; i<analyzerResultListChilds->getNumChildren();i++)
		{
			TAnalyzerResultKit* resultChild = static_cast< TAnalyzerResultKit* >(analyzerResultListChilds->getChild(i) );
			resultChild->ResetValues();
		}
		DisplayResults();
	}
}


/**
 * Returns the icon that represents TSeparatorKit nodes.
 */
QString TAnalyzerKit::getIcon()
{
    return QString(":/icons/AnalyzerKit.png");

}
