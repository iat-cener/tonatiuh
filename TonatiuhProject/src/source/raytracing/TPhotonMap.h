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
***************************************************************************/

#ifndef TPHOTONMAP_H_
#define TPHOTONMAP_H_

#include "Photon.h"

class PhotonMapExport;

class TPhotonMap
{
public:
	TPhotonMap( );
	~TPhotonMap();

    void EndStore( double wPhoton );
	std::vector< Photon* > GetAllPhotons() const;
	PhotonMapExport* GetExportMode( ) const;
	void SetBufferSize( unsigned long nPhotons );
	void SetConcentratorToWorld( Transform concentratorToWorld );
	bool SetExportMode( PhotonMapExport* pExportPhotonMap );
	void StoreRays( std::vector< Photon >& ray );


private:
    unsigned long m_bufferSize;
    Transform m_concentratorToWorld;
    PhotonMapExport* m_pExportPhotonMap;
	const SceneModel* m_pSceneModel;
    unsigned long m_storedPhotonsInBuffer;
    unsigned long m_storedAllPhotons;
    std::vector< Photon* > m_photonsInMemory;


};

#endif /*TPHOTONMAP_H_*/
