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

#ifndef PHOTONMAPDEFAULT_H_
#define PHOTONMAPDEFAULT_H_

#include <QList>
#include <QMap>

#include "InstanceNode.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "BBox.h"
#include "NormalVector.h"
#include "Photon.h"
#include "TPhotonMap.h"
#include <vector>


typedef struct NearestPhotons
{
	long unsigned m_max;
	long unsigned m_found;
	long unsigned m_gotHeap;
	Point3D m_pos;
	double* m_dist2;
	Photon** m_index;

}NearestPhotons;


class PhotonMapDefault : public TPhotonMap
{

public:
	PhotonMapDefault();
	PhotonMapDefault( long unsigned maxPhotons );
	~PhotonMapDefault();

	QString GetIcon();

	Photon* GetPhoton( double photonID ) const;
	QList< Photon* > GetAllPhotons() const;
	QList< Photon* > GetSurfacePhotons( InstanceNode* instance ) const;
	void StoreRay( Photon* rayFirstPhoton );
	unsigned long StoredPhotons() const { return m_storedPhotons; };

private:


	QMap< double, Photon*> m_photons;
	unsigned long  m_storedPhotons;
	long unsigned m_maxPhotons;

};

#endif /* PHOTONMAPDEFAULT_H_ */
