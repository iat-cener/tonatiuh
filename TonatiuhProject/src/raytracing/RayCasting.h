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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef RAYCASTING_H_
#define RAYCASTING_H_

#include <memory>
#include <string>

#include "RayTracingLibrary.h"
#include "BBox.h"
#include "DifferentialGeometry.h"
#include "Ray.h"
#include "RayTracer.h"
#include "TMaterial.h"
#include "Transform.h"
#include "TShape.h"
#include "TSurfaceNode.h"

class TGroupNode;

struct RayCastingNode
{
	RayCastingNode()
	{

	}

	~RayCastingNode()
	{
		childrenList.clear();
	}
	std::string nodeURL;
	BBox boundingBox; //world coordinates
	Transform wtoTransformation;
	Transform otwTransformation;

	std::shared_ptr< TSurfaceNode > surfaceNode = nullptr;
	std::vector< std::shared_ptr< RayCastingNode > > childrenList;

	//Computes the intersection of the ray with node.
	//Returns true if there is an output ray.
	bool Intersect( const Ray& ray, RandomDeviate& rand, bool* isShapeFront, RayCastingNode** intersectedNode, Ray* outputRay )
	{
		//std::cout<<"RayCastingNode::Intersect "<<nodeURL<<" ray "<<ray.origin<<" - "<<ray.direction()<<std::endl;
		//Check if the ray intersects with the BoundingBox
		if( !boundingBox.IntersectP(ray) ) return ( false );

		if( !surfaceNode )
		{
			bool isOutputRay = false;
			double t = ray.maxt;

			for( unsigned int index = 0; index < childrenList.size(); ++index )
			{
				Ray childOutputRay;
				bool childShapeFront = true;
				RayCastingNode* childIntersectedNode = 0;

				bool isChildOutputRay = childrenList[index]->Intersect( ray, rand, &childShapeFront, &childIntersectedNode, &childOutputRay );
				if( ray.maxt < t )
				{
					t = ray.maxt;

					*isShapeFront = childShapeFront;
					*intersectedNode = childIntersectedNode;
					*outputRay = childOutputRay;
					isOutputRay = isChildOutputRay;
				}
	      }

	      return isOutputRay;

	   }
	   else
	   {
		   Ray childCoordinatesRay( wtoTransformation( ray ) );
		   std::shared_ptr< TShape > tshape = std::dynamic_pointer_cast< TShape >( surfaceNode->GetPart( "shape" ) );
		   std::shared_ptr< TMaterial >  tmaterial = std::dynamic_pointer_cast< TMaterial >( surfaceNode->GetPart( "material" ) );

			if( tshape != nullptr )
			{

				 double thit = 0.0;
				 DifferentialGeometry dg;
		    	 bool shapeFront;
				 if( !tshape->Intersect( childCoordinatesRay, &thit, &dg, &shapeFront ) )	return ( false );

				 ray.maxt = thit;
				 *intersectedNode = this;
				 *isShapeFront = shapeFront;

				 if( tmaterial != nullptr )
				 {
					 Ray surfaceOutputRay;
					 if( tmaterial->OutputRay( childCoordinatesRay, &dg, rand, &surfaceOutputRay ) )
					 {
						 *outputRay = otwTransformation( surfaceOutputRay );
						 return ( true );
					 }
				}
	      }
	   }
		return false;
	}

};

//!  RayCasting is the class to execute the ray tracer using ray casting algorithm.
/*!
  RayTracer class provides an interface for different ray tracing algorithms in order to execute the ray tracer technique to objects scene.
*/

class RAYTRACING_API RayCasting : public RayTracer
{
public:
	RayCasting( );
	~RayCasting();

	bool SetScene( std::shared_ptr< TSceneNode >& scene, std::vector< std::string > notFirstStageNodesURL = std::vector< std::string >() );

protected:
	void RunRaytracer( unsigned long numberOfRays );
	void RunRaytracerWithTransmissivity( unsigned long numberOfRays );

private:
	bool CreateRayTracerNodesTree( std::shared_ptr< TContainerNode >& node, std::shared_ptr< RayCastingNode >& rTRNode, Transform parentWT0, std::string parentURL );
	//void RemoveRayTracerNodesTree( RayCastingNode* rTParentNode );

private:
	std::shared_ptr< RayCastingNode > m_sceneRootNode;
	std::vector< std::shared_ptr< RayCastingNode > > m_surfacesNodeList;

};


#endif /* RAYCASTING_H_ */
