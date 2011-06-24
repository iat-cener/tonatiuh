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

#include <iostream>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "tgf.h"
#include "BBox.h"
#include "DifferentialGeometry.h"
#include "InstanceNode.h"
#include "Ray.h"
#include "TMaterial.h"
#include "Transform.h"
#include "TShape.h"
#include "TShapeKit.h"
#include "TLightKit.h"
#include "TAnalyzerKit.h"
#include "TAnalyzerResultKit.h"
#include "TTracker.h"
#include "TTrackerForAiming.h"


InstanceNode::InstanceNode( SoNode* node )
: m_coinNode( node ), m_parent( 0 )
{
}

InstanceNode::InstanceNode( const InstanceNode* node )
{
   m_coinNode = node->m_coinNode;
   node->m_coinNode->ref();
   m_parent = node->m_parent;

   for( int index = 0; index < node->children.count(); ++index )
   {
       InstanceNode* child = new InstanceNode( *(node->children[index]) );
       children.append(child);
    }
}

InstanceNode::~InstanceNode()
{
   qDeleteAll( children );
}

/**
 * Returns node URL.
 */
QString InstanceNode::GetNodeURL() const
{
   QString url;
   if( GetParent() ) url = GetParent()->GetNodeURL();
   url.append( "/" + QString( m_coinNode->getName().getString() ) );
   return url;
}

void InstanceNode::Print( int level ) const
{
   for( int i = 0; i < level; ++i ) std::cout << " ";
      std::cout << m_coinNode->getTypeId().getName().getString()
                << " has " << children.size()
                << " children "<< std::endl;
    for( int index = 0; index < children.count(); ++index )
    	children[index]->Print( level++ );
}

/**
 * Appends new \a child node to the end of the child list.
**/
void InstanceNode::AddChild( InstanceNode* child )
{
    children.push_back( child );
    child->SetParent( this );
}
/**
 * Inserts the \a instanceChild node as child number \a row.
**/
void InstanceNode::InsertChild( int row, InstanceNode* instanceChild)
{
   if( row > children.size() ) row = children.size();
   children.insert( row, instanceChild);
   instanceChild->SetParent(this);
}


//bool InstanceNode::Intersect( const Ray& ray, RandomDeviate& rand, InstanceNode** modelNode, Ray* outputRay )
bool InstanceNode::Intersect( const Ray& ray, RandomDeviate& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* outputRay )
{

	//Check if the ray intersects with the BoundingBox
   if( !m_bbox.IntersectP(ray) ) return false;
   if( GetNode()->getTypeId().isDerivedFrom( TAnalyzerKit::getClassTypeId() ) ) return false;
   if( !GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
   {

      bool isOutputRay = false;
      double t = ray.maxt;
      for( int index = 0; index < children.size(); ++index )
      {
         InstanceNode* intersectedChild = 0;
         Ray childOutputRay;
         bool childShapreFront = true;
         bool isChildOutputRay = children[index]->Intersect( ray, rand, &childShapreFront, &intersectedChild, &childOutputRay );

         if( ray.maxt < t )
         {
            t = ray.maxt;
            *modelNode = intersectedChild;
            *isShapeFront = childShapreFront;

            *outputRay = childOutputRay;
            isOutputRay = isChildOutputRay;

         }
      }

      return isOutputRay;

   }
	else
	{
		Ray childCoordinatesRay( m_transformWTO( ray ) );

		TShape* tshape = 0;
		TMaterial* tmaterial = 0;
		if( children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
		{
			tshape = static_cast< TShape* >( children[0]->GetNode() );
			if( children.size() > 1 )	tmaterial = static_cast< TMaterial* > ( children[1]->GetNode() );
		}
		else if(  children.count() > 1 )
		{
			tmaterial = static_cast< TMaterial* > ( children[0]->GetNode() );
			tshape = static_cast< TShape* >( children[1]->GetNode() );
		}

		if( tshape )
		{
			 double thit = 0.0;
			 DifferentialGeometry dg;
			 if( !tshape->Intersect( childCoordinatesRay, &thit, &dg ) ) return false;

			 ray.maxt = thit;
			 *modelNode = this;

			 *isShapeFront = dg.shapeFrontSide;

			 if( tmaterial )
			 {
				 Ray surfaceOutputRay;
				 if( tmaterial->OutputRay( childCoordinatesRay, &dg, rand, &surfaceOutputRay ) )
				 {
					 *outputRay = m_transformOTW( surfaceOutputRay );
					 return true;
				 }
			}
      }
   }
	return false;
}

void InstanceNode::Analyze(  std::vector<Ray> * raysWays, QMutex* mutex )
{

   if ( GetNode()->getTypeId().isDerivedFrom( TAnalyzerResultKit::getClassTypeId() ) )
   {
	   TAnalyzerResultKit * tanalyzerResult = static_cast< TAnalyzerResultKit* > ( GetNode() );
	   tanalyzerResult->Compute( raysWays, &m_transformWTO, mutex);
   }
   else
   {
      for( int index = 0; index < children.size(); ++index )
      {
         children[index]->Analyze( raysWays, mutex);
      }
   }
}
void InstanceNode::ResetAnalyzeValues()
{
	RecursivlyApply<TAnalyzerKit>(&TAnalyzerKit::ResetValues);
}

void InstanceNode::DisplayAnalyzeResults()
{
	RecursivlyApply<TAnalyzerKit>(&TAnalyzerKit::DisplayResults);
}

void InstanceNode::DisconnectAllTrackers()
{
	RecursivlyApply<TTracker>(&TTracker::Disconnect);
}


void InstanceNode::ReconnectAllTrackers(TLightKit * coinLight)
{
	RecursivlyApply<TTracker,TLightKit *>(&TTracker::SetLightAngles,coinLight);
}

void InstanceNode::UpdateAnalyzerSize(SceneModel* pModel)
{
	RecursivlyApply<TAnalyzerKit,SceneModel*>(&TAnalyzerKit::UpdateSize, pModel);
}
void  InstanceNode::SetAimingPointRelativity( bool relative )
{
	RecursivlyApply<TTrackerForAiming,bool>(&TTrackerForAiming::SetAimingPointRelativity, relative);
}

void InstanceNode::PrepareAnalyze(SceneModel * pModel )
{
	RecursivlyApply<TAnalyzerKit,SceneModel *>(&TAnalyzerKit::PrepareCompute,pModel);
	RecursivlyApply<TAnalyzerResultKit>(&TAnalyzerResultKit::PrepareCompute);
}

void InstanceNode::FinalyzeAnalyze(double raydensity, TAnalyzerKit * analyzerKit )
{

   if ( GetNode()->getTypeId().isDerivedFrom( TAnalyzerResultKit::getClassTypeId() ) )
   {
	   TAnalyzerResultKit * tanalyzerResult = static_cast< TAnalyzerResultKit* > ( GetNode() );
	   tanalyzerResult->FinalizeCompute(raydensity, &m_transformOTW);
   }
   else
   {
	   if( GetNode()->getTypeId().isDerivedFrom( TAnalyzerKit::getClassTypeId() ) )
	   {
		  ((TAnalyzerKit *) GetNode())->FinalizeCompute(raydensity, &m_transformWTO);
		  for( int index = 0; index < children.size(); ++index )
		  {
			 children[index]->FinalyzeAnalyze( raydensity, ((TAnalyzerKit *) GetNode()));
		  }
	   }
	   else
	   {
		  for( int index = 0; index < children.size(); ++index )
		  {
			 children[index]->FinalyzeAnalyze( raydensity, analyzerKit);
		  }
	   }
   }
}

void InstanceNode::extendBoxForLight( SbBox3f * extendedBox )
{
	if( GetNode()->getTypeId().isDerivedFrom( TAnalyzerKit::getClassTypeId() ) )
	{
		return;
	}
	if( !IsTreeContainAnalyzer() )
	{
		SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( SbViewportRegion() ) ;
		GetNode()->getBoundingBox( bbAction );

		SbBox3f box = bbAction->getXfBoundingBox().project();
		delete bbAction;
		extendedBox->extendBy(box);

	}
	else
	{
		SbMatrix matrixOTW = SbMatrix::identity();
		if( GetNode()->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
		{
			SoBaseKit* coinNode = static_cast< SoBaseKit* > ( GetNode() );
			SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart( "transform", true ) );
			matrixOTW = tgf::MatrixFromSoTransform( nodeTransform );
		}

		for( int index = 0; index < children.size(); ++index )
		{
			SbBox3f * box = new SbBox3f;
			children[index]->extendBoxForLight(box);
			if (!box->isEmpty())
			{
				SbVec3f min,max;
				box->getBounds(min,max);
				matrixOTW.multVecMatrix(min,min);
				matrixOTW.multVecMatrix(max,max);
				extendedBox->extendBy(min);
				extendedBox->extendBy(max);
			}
		}
	}
}

bool InstanceNode::IsTreeContainAnalyzer( )
{
   if( GetNode()->getTypeId().isDerivedFrom( TAnalyzerKit::getClassTypeId() ) )
   {
	  return true;
   }
   else
   {
	  for( int index = 0; index < children.size(); ++index )
	  {
		 if (children[index]->IsTreeContainAnalyzer()) return true;
	  }
   }
   return false;
}


BBox InstanceNode::GetIntersectionBBox()
{
	return m_bbox;
}

Transform InstanceNode::GetIntersectionTransform()
{
	return m_transformWTO;
}

void InstanceNode::SetIntersectionBBox( BBox nodeBBox )
{
	m_bbox = nodeBBox;
}
/**
 * Set node world to object transform to \a nodeTransform .
 */
void InstanceNode::SetIntersectionTransform( Transform nodeTransform )
{

	m_transformWTO = nodeTransform;
	m_transformOTW = m_transformWTO.GetInverse();
}

QDataStream& operator<< ( QDataStream & s, const InstanceNode& node )
{
	s << node.GetNode();
	return s;
}

QDataStream& operator>> ( QDataStream & s, const InstanceNode& node )
{
	s >> node;
	return s;
}

bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode)
{
	return ( (thisNode.GetNode() == otherNode.GetNode()) &&
			 (thisNode.GetParent()->GetNode() == otherNode.GetParent()->GetNode()) );
}


template<class T> void InstanceNode::RecursivlyApply(void (T::*func)(void))
{
	if ( GetNode()->getTypeId().isDerivedFrom( T::getClassTypeId() ) )
	{
	   T * elem = static_cast< T* > ( GetNode() );
	   (elem->*func)();
	}
	else
	{
      for( int index = 0; index < children.size(); ++index )
      {
         children[index]->RecursivlyApply<T>(func);
      }
	}
}
template<class T,class Param1> void InstanceNode::RecursivlyApply(void (T::*func)(Param1),Param1 param1)
{
	if (GetNode()->getTypeId().isDerivedFrom( T::getClassTypeId() ) )
	{
	   T * elem = static_cast< T* > ( GetNode() );
	   (elem->*func)(param1);
	}
	else
	{
      for( int index = 0; index < children.size(); ++index )
      {
         children[index]->RecursivlyApply<T,Param1>(func,param1);
      }
	}
}
template<class T,class Param1> void InstanceNode::RecursivlyApplyWithMto(void (T::*func)(Param1),Param1 param1)
{
	if (GetNode()->getTypeId().isDerivedFrom( T::getClassTypeId() ) )
	{
	   T * elem = static_cast< T* > ( GetNode() );
	   (elem->*func)(&m_transformWTO, param1);
	}
	else
	{
      for( int index = 0; index < children.size(); ++index )
      {
         children[index]->RecursivlyApplyWithMto<T,Param1>(func,param1);
      }
	}
}
template<class T,class Param1,class Param2> void InstanceNode::RecursivlyApplyWithMto(void (T::*func)(Param1,Param2),Param1 param1,Param1 param2)
{
	if (GetNode()->getTypeId().isDerivedFrom( T::getClassTypeId() ) )
	{
	   T * elem = static_cast< T* > ( GetNode() );
	   (elem->*func)(&m_transformWTO, param1,param2);
	}
	else
	{
      for( int index = 0; index < children.size(); ++index )
      {
         children[index]->RecursivlyApplyWithMto<T,Param1,Param2>(func,param1,param2);
      }
	}
}
