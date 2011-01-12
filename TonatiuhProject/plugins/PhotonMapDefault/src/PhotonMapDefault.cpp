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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QString>

#include "PhotonMapDefault.h"
#include "tgc.h"

/** This is the constructor for the photon map.
 * To create the photon map it is necessary to specify the
 * maximum number of photons that will be stored
*/
PhotonMapDefault::PhotonMapDefault()
{
	m_storedPhotons = 0;
	BBox m_bbox();

}

/**
 * Destroys the photon map.
 */
PhotonMapDefault :: ~PhotonMapDefault()
{
	for( int index = 0; index < m_photons.size(); ++index )
		delete m_photons[index];

	m_photons.clear();
}

QString PhotonMapDefault::GetIcon()
{
	return QString(":icons/eclipse32.png");
}

Photon* PhotonMapDefault::GetPhoton( double photonID ) const
{
	//if( !m_photons.contains( photonID ) ) return 0;
	return m_photons[photonID-1];
}


QList< Photon* > PhotonMapDefault::GetAllPhotons() const
{
	return QVector<Photon*>::fromStdVector( m_photons ).toList();

}

QList< Photon* > PhotonMapDefault::GetSurfacePhotons( InstanceNode* instance ) const
{
	QList< Photon* > surfacePhotonsList;

	//QList< Photon* > mapPhotons = m_photons.values();

	for( int index = 0; index < m_photons.size(); ++index )
	{
		if( m_photons[index]->intersectedSurface == instance )
			surfacePhotonsList.push_back( m_photons[index] );
	}
	return surfacePhotonsList;
}


void PhotonMapDefault::StoreRay( std::vector< Photon >* raysPhotonsList )
{
	std::vector< Photon >::iterator it = raysPhotonsList->begin();

	while( it < raysPhotonsList->end() )
	{
		Photon* first = new Photon( *it );
		first->id = ++m_storedPhotons;
		m_photons.push_back( first );

		it++;

		while( ( it <  raysPhotonsList->end() ) && ( (*it).id > 0 ) )
		{
			Photon* photon = new Photon( *it );
			photon->id = ++m_storedPhotons;
			m_photons.push_back( photon );

			first->next = photon;
			photon->prev = first;

			first = photon;
			it++;

		}
	}

	/*for( unsigned int ray = 0; ray < raysList.size(); ray++ )
	{
		if( ( m_storedPhotons + raysList[ray].size() ) >= m_maxPhotons )	return;

		Photon* first = new Photon( raysList[ray][0] );
  	  	m_photons.push_back( first );

		first->id  = ++m_storedPhotons;
		for( unsigned int photon = 1; photon < raysList[ray].size(); photon++ )
		{
			Photon* currentPhoton = new Photon( raysList[ray][photon] );
	  	  	m_photons.push_back( currentPhoton );

			currentPhoton->id  = ++m_storedPhotons;
			first->next = currentPhoton;
			currentPhoton->prev = first;

			first = currentPhoton;

		}

	}*/
	/*rayFirstPhoton->id = m_storedPhotons + 1;
	 * Photon* currentNode = rayFirstPhoton->next;
	while( currentNode )
	{
		currentNode->id = currentNode->prev->id +1;
		currentNode = currentNode->next;
	}

	if ( m_storedPhotons >= m_maxPhotons ) return;
	m_photons.push_back( rayFirstPhoton );
	++m_storedPhotons;
  	currentNode = rayFirstPhoton->next;

  	while( currentNode )
  	{
		if ( m_storedPhotons >= m_maxPhotons ) return;
  		m_storedPhotons += 1;
  	  	m_photons.push_back( currentNode );

  	  	currentNode = currentNode->next;
  	}*/


}
