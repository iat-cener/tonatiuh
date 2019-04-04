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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef PHOTONMAPEXPORT_H_
#define PHOTONMAPEXPORT_H_

#include <vector>

#include <QStringList>

#include "Photon.h"

class SceneModel;

class PhotonMapExport
{

public:
	PhotonMapExport();
	virtual ~PhotonMapExport();

	virtual void EndExport() = 0;
	virtual void SavePhotonMap( std::vector < Photon* > raysLists ) = 0;
	void SetConcentratorToWorld( Transform concentratorToWorld );
	virtual void SetPowerPerPhoton( double wPhoton ) = 0;

	void SetSaveAllPhotonsEnabled();
	void SetSaveCoordinatesEnabled( bool enabled );
	void SetSaveCoordinatesInGlobalSystemEnabled( bool enabled );
	virtual void SetSaveParameterValue( QString parameterName, QString parameterValue ) = 0;
	void SetSavePreviousNextPhotonsID( bool enabled );
	void SetSaveSideEnabled( bool enabled );
	void SetSaveSurfacesIDEnabled( bool enabled );
	void SetSaveSurfacesURLList( QStringList surfacesURLList );
	void SetSceneModel( SceneModel& sceneModel );
	virtual bool StartExport() = 0;

protected:
    Transform m_concentratorToWorld;
	SceneModel* m_pSceneModel;
	bool m_saveAllPhotonsData;
	bool m_saveCoordinates;
	bool m_saveCoordinatesInGlobal;
	bool m_savePowerPerPhoton;
	bool m_savePrevNexID;
	bool m_saveSide;
	bool m_saveSurfaceID;
	QStringList m_saveSurfacesURLList;

};

#endif /* PHOTONMAPEXPORT_H_ */
