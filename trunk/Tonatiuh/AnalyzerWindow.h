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
 
 
#ifndef ANALYZERWINDOW_H_
#define ANALYZERWINDOW_H_

#include <QWidget>
#include <qwt3d_function.h>
#include <qwt3d_surfaceplot.h>
#include "PhotonMap.h"
#include "Point3D.h"
#include "TShape.h"

class QLabel;

/***********************************************************************//**
 * Calculates the flux density of a photon map
***************************************************************************/
class Flux : public Qwt3D::Function
{
public:
	/***********************************************************************//**
 	* Constructor 
	***************************************************************************/
	Flux(Qwt3D::SurfacePlot& pw, PhotonMap* map, TShape* shape)
    	:Function(pw), m_map(map), m_shape(shape)
    	{
    
    	}
    	
	/***********************************************************************//**
 	* Returns the flux density at a given point on the surface.  x,y correspond
 	* to the u,v texture coordinates
	***************************************************************************/
   	double operator()(double x, double y)
   	{
   		int nearestPhotons = 100;
   		double res = 1.0 / 100.0;
   		Point3D point = m_shape->Sample(x*res,y*res);
   		std::cout << "x, y " << x <<", " << y << "  u,v " <<x*res << ", " << y*res << std::endl;
   		std::cout << "Point at " << point << " has a flux of " << m_map->fluxAtPoint( point, nearestPhotons ) << std::endl;
		return m_map->fluxAtPoint( point, nearestPhotons );
   	}
   	
private:
	double* m_data;
	PhotonMap* m_map;
	TShape* m_shape;
};

/***********************************************************************//**
 * Creates a seperate window for analyzing flux density data of a selected surface
***************************************************************************/
class AnalyzerWindow : public QWidget
{
	Q_OBJECT
public:
	AnalyzerWindow( QWidget* parent = 0 );
	~AnalyzerWindow();
	
	/***********************************************************************//**
 	* Creates the graph output of the flux data stored in the photon map for the 
 	* selected shape using u by v resoltion for the output 
	***************************************************************************/	
	void Plot(PhotonMap* map, TShape* shape, unsigned int u, unsigned int v);
	Qwt3D::SurfacePlot* m_plot;
public slots:

};


#endif /*ANALYZERWINDOW_H_*/
