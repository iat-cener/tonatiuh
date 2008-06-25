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

#include "AnalyzerWindow.h"

#include <QtGui>
#include "Trace.h"
#include "tgf.h"

using namespace Qwt3D;

AnalyzerWindow::AnalyzerWindow( QWidget* parent )
:QWidget( parent )
{
	Trace trace( "AnalyzerWindow::AnalyzerWindow" );
	setWindowTitle(tr("Results Analyzer"));
/*
	m_pDataWidget = new SurfacePlot(this);
    Rosenbrock rosenbrock(*m_pDataWidget);

    rosenbrock.setMesh(41,31);
    rosenbrock.setDomain(-1.73,1.5,-1.5,1.5);
    rosenbrock.setMinZ(-10);
	
    rosenbrock.create();
	
    for (unsigned i=0; i!=m_pDataWidget->coordinates()->axes.size(); ++i)
    {
		m_pDataWidget->coordinates()->axes[i].setMajors(7);
    	m_pDataWidget->coordinates()->axes[i].setMinors(4);
	}

    m_pDataWidget->coordinates()->axes[Qwt3D::X1].setLabelString("x-axis");
    m_pDataWidget->coordinates()->axes[Qwt3D::Y1].setLabelString("y-axis");
    m_pDataWidget->setCoordinateStyle(Qwt3D::BOX);
    
    resize(600,400);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_pDataWidget);
    setLayout(layout);
*/    
}

AnalyzerWindow::~AnalyzerWindow()      
{
	delete m_plot;
}

void AnalyzerWindow::Plot(PhotonMap* map, TShape* shape, unsigned int u, unsigned int v)
{

	m_plot = new SurfacePlot(this);

    Flux flux(*m_plot, map, shape);

    flux.setMesh(u,v);
    flux.setDomain(0,100,0,100);
    flux.setMinZ(0);
	
    flux.create();
	

	
    for (unsigned i=0; i!=m_plot->coordinates()->axes.size(); ++i)
    {
		m_plot->coordinates()->axes[i].setMajors(7);
    	m_plot->coordinates()->axes[i].setMinors(4);
	}

    m_plot->coordinates()->axes[Qwt3D::X1].setLabelString("x-axis");
    m_plot->coordinates()->axes[Qwt3D::Y1].setLabelString("y-axis");
    m_plot->coordinates()->axes[Qwt3D::Z1].setLabelString("z-axis");
    m_plot->setCoordinateStyle(Qwt3D::BOX);

	m_plot->setScale(2,2,.1);

    resize(600,400);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_plot);
    setLayout(layout);
/*
	m_plot->loadFromData(fluxarray, 10, 10);
	
    for (unsigned i=0; i!=m_plot->coordinates()->axes.size(); ++i)
    {
		m_plot->coordinates()->axes[i].setMajors(7);
    	m_plot->coordinates()->axes[i].setMinors(4);
	}

    m_plot->coordinates()->axes[Qwt3D::X1].setLabelString("x-axis");
    m_plot->coordinates()->axes[Qwt3D::Y1].setLabelString("y-axis");
    m_plot->setCoordinateStyle(Qwt3D::BOX);
	resize(600,400);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_plot);
    setLayout(layout);
    */
}


