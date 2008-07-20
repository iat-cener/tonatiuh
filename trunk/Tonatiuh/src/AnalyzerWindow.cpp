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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez, 
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/
#include <QVBoxLayout>

#include "AnalyzerWindow.h"
#include "tgf.h"
#include "Trace.h"

using namespace Qwt3D;

AnalyzerWindow::AnalyzerWindow( QWidget* parent )
:QDialog( parent )
{
	Trace trace( "AnalyzerWindow::AnalyzerWindow", false );
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

    QVBoxLayout* layout = new QVBoxLayout;
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


