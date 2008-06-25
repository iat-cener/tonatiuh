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

#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoTransform.h>  
#include <Inventor/sensors/SoFieldSensor.h> 

#include <QString>

#include <cmath>

#include "TDefaultTracker.h"
#include "Trace.h"

SO_KIT_SOURCE( TDefaultTracker );

void TDefaultTracker::initClass()
{
	Trace trace( "TDefaultTracker::initClass", false );
	SO_KIT_INIT_CLASS( TDefaultTracker, TTracker, "Tracker" );
}

TDefaultTracker::TDefaultTracker()
{
	Trace trace( "TDefaultTracker::TDefaultTracker", false );
	
	SO_KIT_CONSTRUCTOR(TDefaultTracker);
	m_rotSensor = new SoFieldSensor( fielSensorCB, this );
	m_rotSensor->attach( &sunRotation );
}

TDefaultTracker::~TDefaultTracker()
{
	Trace trace( "TDefaultTracker::~TDefaultTracker", false );
}
QString TDefaultTracker::getIcon()
{
	Trace trace( "TDefaultTracker::~TDefaultTracker", false );
	return QString(":/icons/HorizontalTracker.png");
}

void TDefaultTracker::fielSensorCB(void* data, SoSensor* sensor )
{
	Trace trace( "TDefaultTracker::fielSensorCB", false );
	TDefaultTracker* tracker = (TDefaultTracker* ) data;
	if( tracker ) tracker->setRotation();
}

void TDefaultTracker::setRotation()
{
	Trace trace( "TDefaultTracker::setRotation", false );
		
	
	SbVec3f dir;
	float angle;
	
	sunRotation.getValue (dir, angle );
	SoTransform* trackerTransform = static_cast< SoTransform* > ( transform.getValue() );
	trackerTransform->rotation.setValue( dir, angle );

}

