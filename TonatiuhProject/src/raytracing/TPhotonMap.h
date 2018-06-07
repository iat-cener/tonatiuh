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


#ifndef TPHOTONMAP_H_
#define TPHOTONMAP_H_

#include <memory>
#include <mutex>
#include <vector>

#include "RayTracingLibrary.h"
#include "Photon.h"
#include "PhotonMapExportType.h"



//!  TPhotonMap is the base class that stores photons generated during the ray tracing.
/*!
  TPhotonMap class is a template class that provides a storage way for the photons generated during a raytracing.
  The base class only stores the photons, there is not export option. So, when the buffer is full, the stored photons are deleted.
*/

class RAYTRACING_API TPhotonMap
{

public:
	TPhotonMap();
	~TPhotonMap();

	void EmptyBuffer();
    void EndStore( double wPhoton );
	std::vector< Photon > GetAllPhotons() const;
	void SetBufferSize( unsigned long nPhotons );
	void SetExportType( std::unique_ptr< PhotonMapExportType > pPhotonMapExportType );
	void StoreRays( std::vector< Photon >& raysList );


private:
    unsigned long m_bufferSize;
    unsigned long m_storedPhotonsInBuffer;
    unsigned long m_storedAllPhotons;
    std::vector< Photon > m_photonsInMemory;

    std::unique_ptr< PhotonMapExportType > m_pPhotonMapExportType;

	std::mutex m_mutexPhotonMap;

};


#endif /* TPHOTONMAP_H_ */