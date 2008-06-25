/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
 ***************************************************************************/
 
#include <Inventor/actions/SoGetMatrixAction.h> 
#include <Inventor/nodes/SoTransform.h> 
#include <Inventor/nodes/SoMaterial.h> 

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "TDefaultTracker.h"
#include "Trace.h"
#include "TSeparatorKit.h"
#include "tgf.h"


SO_KIT_SOURCE(TSeparatorKit);

void TSeparatorKit::initClass()
{
	Trace trace("TSeparatorKit::initClass", true);
	SO_KIT_INIT_CLASS(TSeparatorKit, SoSeparatorKit, "SeparatorKit");
}

TSeparatorKit::TSeparatorKit()
{
	Trace trace("TSeparatorKit::TSeparatorKit", true);
	SO_KIT_CONSTRUCTOR(TSeparatorKit);
	SO_KIT_INIT_INSTANCE();
	
	SoTransform* transform = new SoTransform; 
	setPart("transform", transform);
}

TSeparatorKit::~TSeparatorKit()
{
    Trace trace("TSeparatorKit exiting destructor", false);
}

void TSeparatorKit::SetBoundigBox( SbXfBox3f boundingBox )
{
	m_boundigBox = boundingBox;
}

bool TSeparatorKit::IntersectP( const Ray& /*ray*/ ) const
{
	return false;
}

Ray* TSeparatorKit::Intersect( const Ray& ray, RandomDeviate& rand ) const
{
	Trace trace( "TSeparatorKit::Intersect", false );
	
	//Test if the ray intersects with this bounding box
	SbBox3f box= m_boundigBox.project();
	
	double t0 = ray.mint;
	double t1 = ray.maxt;

    for( int i = 0; i < 3; ++i )
    {
    	double invRayDir = 1.0 / ray.direction[i];
    	double tNear = ( box.getMin()[i] - ray.origin[i] ) * invRayDir;
    	double tFar = ( box.getMax()[i] - ray.origin[i] ) * invRayDir;
    	if( tNear > tFar ) std::swap( tNear, tFar );
    	t0 = tNear > t0 ? tNear : t0;
    	t1 = tFar < t1 ? tFar : t1;
    	if( t0 > t1 ) return 0;
    }
   
	Ray* result = 0;
	 
	//The ray intersects with the BoundingBox
	//Transform the ray to call children intersect
	SoTransform* coinTransform = static_cast< SoTransform* > ( transform.getValue() );
	if ( coinTransform )
	{
		Transform objectToWorld( *coinTransform );
		Transform worldToObject = objectToWorld.GetInverse();
		
		Ray objectRay( worldToObject( ray ) );					 
		objectRay.maxt = ray.maxt;
		
		Ray* reflected = 0;
		Ray* childReflected = 0;
		
		SoNodeKitListPart* coinPartList = dynamic_cast< SoNodeKitListPart* >( childList.getValue() );
		
	  	if( coinPartList )
	  	{ 
			for( int index = 0; index < coinPartList->getNumChildren(); index++ )
			{
				SoBaseKit* coinChild = dynamic_cast< SoBaseKit* >( coinPartList->getChild( index ) );
				if( coinChild->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
				{
					TSeparatorKit* child = dynamic_cast< TSeparatorKit* >( coinChild );
					childReflected = child->Intersect( objectRay, rand );
				}
				else if(coinChild->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) 
				{
					TShapeKit* child = dynamic_cast< TShapeKit* >( coinChild );
					childReflected = child->Intersect( objectRay, rand );
					
				}
				
				if( ray.maxt != objectRay.maxt )
				{
					delete reflected;
					reflected = 0;			
				    ray.maxt = objectRay.maxt;
				}
				
				if( childReflected ) 
				{
				    reflected = new Ray( *childReflected );
				    delete childReflected;
				    childReflected = 0;
				}
			}
			
			if( reflected ) 
			{	
				result = new Ray( objectToWorld( *reflected ) );	
				delete reflected;
				reflected = 0;
	  		}
	  	}	
	}		
	return result;	
}
