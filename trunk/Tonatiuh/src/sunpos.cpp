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
 
#include "sunpos.h"
#include <math.h>

void sunpos(cTime udtTime,cLocation udtLocation, cSunCoordinates *udtSunCoordinates)
{
	// Main variables
	double dJulianDate;
	double dElapsedJulianDays;
	double dDecimalHours;
	double dEclipticLongitude;
	double dEclipticObliquity;
	double dRightAscension;
	double dDeclination;

	// Auxiliary variables
	double dY;
	double dX;

	// Calculate difference in days between the current Julian Day 
	// and JD 2451545.0, which is noon 1 January 2000 Universal Time
	{
		
		long int liAux1;
		long int liAux2;
		// Calculate time of the day in UT decimal hours
		dDecimalHours = udtTime.dHours + (udtTime.dMinutes 
			+ udtTime.dSeconds / 60.0 ) / 60.0;
		// Calculate current Julian Day
		liAux1 =(udtTime.iMonth-14)/12;
		liAux2=(1461*(udtTime.iYear + 4800 + liAux1))/4 + (367*(udtTime.iMonth 
			- 2-12*liAux1))/12- (3*((udtTime.iYear + 4900 
		+ liAux1)/100))/4+udtTime.iDay-32075;
		dJulianDate=(double)(liAux2)-0.5+dDecimalHours/24.0;
		// Calculate difference between current Julian Day and JD 2451545.0 
		dElapsedJulianDays = dJulianDate-2451545.0;
	}

	// Calculate ecliptic coordinates (ecliptic longitude and obliquity of the 
	// ecliptic in radians but without limiting the angle to be less than 2*Pi 
	// (i.e., the result may be greater than 2*Pi)
	{

		double dOmega;
		dOmega=0.0172021241615*dElapsedJulianDays;
		dEclipticLongitude=4.894969024 + 0.017202791697* dElapsedJulianDays 
			+ 5.78262765229295e-9 * dElapsedJulianDays * cos( 2.67824 + dOmega) 
			+ 0.03341656 * cos(4.6693 + dOmega) 
			+ 0.00034894 * cos(4.6261 + 0.03440424832 * dElapsedJulianDays) 
			- 0.00008339 * sin(2.1824 - 0.00092421755 * dElapsedJulianDays)
			+ 0.00003934 * cos(2.7441 + 0.01575190938 * dElapsedJulianDays)
			+ 0.00003461 * cos(2.8289 + 9.64572211e-6 * dElapsedJulianDays)
			+ 0.00003136 * cos(3.6277 + 0.21276871047 * dElapsedJulianDays)
			+ 0.00002676 * cos(4.4181 + 0.02152065544 * dElapsedJulianDays) 
			+ 0.00003046 * cos(6.1352 + 0.01076032772 * dElapsedJulianDays);
			
		dEclipticObliquity =  0.4090928040395968 - 6.21395528282155e-9 * dElapsedJulianDays
			+ 0.00004459673713551599 * cos(2.182954286987892 - 9.24169576470094e-4 * dElapsedJulianDays)
			- 2.782663310837079e-6 * cos(87.60120254057658 - 0.0344055570784386 * dElapsedJulianDays)
			+ 1.0435812166758367e-7 * cos(167.41964504653737 - 0.0698787501985692 * dElapsedJulianDays);
	}

	// Calculate celestial coordinates ( right ascension and declination ) in radians 
	// but without limiting the angle to be less than 2*Pi (i.e., the result may be 
	// greater than 2*Pi)
	{
		double dSin_EclipticLongitude;
		dSin_EclipticLongitude= sin( dEclipticLongitude );
		dY = cos( dEclipticObliquity ) * dSin_EclipticLongitude; //sin correcci�n
		dX = cos( dEclipticLongitude );
		dRightAscension = atan2( dY,dX );
		if( dRightAscension < 0.0 ) dRightAscension = dRightAscension + twopi;
		dDeclination = asin( sin( dEclipticObliquity )*dSin_EclipticLongitude ); //sin correcci�n
	}

	// Calculate local coordinates ( azimuth and zenith angle ) in degrees
	{
		double dGreenwichMeanSiderealTime;
		double dLocalMeanSiderealTime;
		double dLatitudeInRadians;
		double dHourAngle;
		double dCos_Latitude;
		double dSin_Latitude;
		double dCos_HourAngle;
		double dParallax;
		//dGreenwichMeanSiderealTime = 6.6974243242 + 
		//	0.0657098283*dElapsedJulianDays 
		//	+ dDecimalHours;
		dGreenwichMeanSiderealTime = -60053.302628 + 24.065709825 * dElapsedJulianDays 
			- 0.000292222 * sin(0.0009242 * dElapsedJulianDays + 0.9574);
		dLocalMeanSiderealTime = (dGreenwichMeanSiderealTime*15
			+ udtLocation.dLongitude)*rad;
		dHourAngle = dLocalMeanSiderealTime - dRightAscension;
		dLatitudeInRadians = udtLocation.dLatitude*rad;
		dCos_Latitude = cos( dLatitudeInRadians );
		dSin_Latitude = sin( dLatitudeInRadians );
		dCos_HourAngle= cos( dHourAngle );
		udtSunCoordinates->dZenithAngle = (acos( dCos_Latitude*dCos_HourAngle
			*cos(dDeclination) + sin( dDeclination )*dSin_Latitude));
		dY = -sin( dHourAngle );
		dX = tan( dDeclination )*dCos_Latitude - dSin_Latitude*dCos_HourAngle;
		udtSunCoordinates->dAzimuth = atan2( dY, dX );
		if ( udtSunCoordinates->dAzimuth < 0.0 ) 
			udtSunCoordinates->dAzimuth = udtSunCoordinates->dAzimuth + twopi;
		udtSunCoordinates->dAzimuth = udtSunCoordinates->dAzimuth/rad;
		// Parallax Correction
		dParallax=(dEarthMeanRadius/dAstronomicalUnit)
			*sin(udtSunCoordinates->dZenithAngle);
		udtSunCoordinates->dZenithAngle=(udtSunCoordinates->dZenithAngle 
			+ dParallax)/rad;
		udtSunCoordinates->dJuliano=dJulianDate;
	}
}
