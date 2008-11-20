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

#ifndef ANALYZERWINDOW_H_
#define ANALYZERWINDOW_H_


#include <QDialog>

#include <qwt3d_function.h>
#include <qwt3d_surfaceplot.h>

#include "PhotonMap.h"
#include "Point3D.h"
#include "TShape.h"

class QLabel;

class Flux : public Qwt3D::Function
{
public:
	Flux(Qwt3D::SurfacePlot& pw, PhotonMap* map, TShape* shape)
    	:Function(pw), m_map(map), m_shape(shape)
    	{

    	}

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

class AnalyzerWindow : public QDialog
{
	Q_OBJECT

public:
	AnalyzerWindow( QWidget* parent = 0 );
	~AnalyzerWindow();

	void Plot(PhotonMap* map, TShape* shape, unsigned int u, unsigned int v);
	Qwt3D::SurfacePlot* m_plot;

};

#endif /*ANALYZERWINDOW_H_*/
