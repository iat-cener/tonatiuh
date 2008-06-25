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

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoLabel.h>

#include "sunpos.h"
#include "tgc.h"
#include "TLightKit.h"
#include "Trace.h"
#include "TShapeKit.h"
#include "TSquare.h"
#include "TDefaultSunShape.h"

SO_KIT_SOURCE(TLightKit);

void TLightKit::initClass()
{
	SO_KIT_INIT_CLASS(TLightKit, SoLightKit, "LightKit");
}

TLightKit::TLightKit()
{
	Trace trace( "TLightKit::TLightKit()", false );
	SO_KIT_CONSTRUCTOR(TLightKit);	
		
	SO_KIT_ADD_CATALOG_ENTRY(sunshapelabel, SoLabel, TRUE, this, , TRUE);
	SO_KIT_CHANGE_ENTRY_TYPE(icon, TShape, TSquare );
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(tsunshape, SoNode, TDefaultSunShape, TRUE, this, , TRUE);	

	SO_KIT_INIT_INSTANCE();
	
	SoDirectionalLight* light = static_cast<SoDirectionalLight*>( getPart( "light", true ) );
	light->direction.setValue( SbVec3f( 0, 1, 0 ) );
	
	SoTransform* transform = new SoTransform;
	setPart( "transform", transform );
	
	ChangePosition( QDateTime::currentDateTime()  );
}

TLightKit::~TLightKit()
{
	
}

bool TLightKit::ChangePosition( QDateTime newTime )
{
	Trace trace( "TLightKit::ChangePosition" , false );
	m_time = newTime;
	
	int year = m_time.date().year();
	int month = m_time.date().month();
	int day = m_time.date().day();
	
	double hours = m_time.time().hour ();
	double minutes = m_time.time().minute ();
	double seconds = m_time.time().second ();
	cTime myTime = { year, month, day, hours, minutes, seconds };

	//Localization
	double lon = longitude.getValue();
	double lat = latitude.getValue();
    cLocation myLocation = {lon , lat };
    
	//Calculate sun position
	cSunCoordinates results;
	sunpos( myTime, myLocation, &results );
	
	SetSunPosition( results.dAzimuth * tgc::Degree, results.dZenithAngle  * tgc::Degree );
	
	if ( results.dZenithAngle > 90.0 ) return true;
	else return false;
}

QDateTime TLightKit::GetTime()
{
	return 	m_time;
}

void TLightKit::SetSunPosition( double azimuth, double zenith )
{
	Trace trace( "TLightKit::SetSunPosition" , false );
	
	//Trasform calculation
    Vector3D direction = Vector3D( sin( zenith ) * sin( azimuth ), cos( zenith ), -sin( zenith )*cos( azimuth ) );
	
	Point3D center = Point3D( 0, 0, 0 ) + ( direction * 200 );
	SbMatrix translate;
	translate.setTranslate( SbVec3f( center.x, center.y, center.z ) );
	
	SbMatrix rotX;
	rotX.setRotate( SbRotation( SbVec3f( 1, 0, 0 ), -zenith ) );
	
	SbMatrix rotY;
	rotY.setRotate( SbRotation( SbVec3f( 0, 1, 0 ), -azimuth ) );
	
	SbMatrix coinMatrix = rotX* rotY* translate;
	
	
	SbMatrix inverse = coinMatrix.inverse();
	SbVec3f dst;
	coinMatrix.multDirMatrix(  SbVec3f( 0, 1, 0 ), dst );
	
	SoTransform* transform = new SoTransform;
	transform->setMatrix( coinMatrix );

	//Set the new transfor to light
	SoTransform* lightTransform = static_cast< SoTransform* >( this->getPart( "transform", true ) );
	lightTransform->translation.setValue( transform->translation.getValue() );
	lightTransform->rotation.setValue( transform->rotation.getValue() );
	lightTransform->scaleFactor.setValue( transform->scaleFactor.getValue() );
	lightTransform->scaleOrientation.setValue( transform->scaleOrientation.getValue() );
	lightTransform->center.setValue( transform->center.getValue() );
	
}
