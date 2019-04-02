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

#include "PhotonMapExport.h"
#include "TPhotonMap.h"

/*!
 * Creates a photon map
 */
TPhotonMap::TPhotonMap()
:m_bufferSize( 0 ),
 m_pExportPhotonMap( 0 ),
 m_pSceneModel( 0 ),
 m_storedPhotonsInBuffer( 0 ),
 m_storedAllPhotons( 0 )
{

}

/*
 * Destroys
 */
TPhotonMap::~TPhotonMap()
{
	unsigned int photonListSize = m_photonsInMemory.size();
	for( unsigned int i = 0; i < photonListSize ;++i )
	{
		delete m_photonsInMemory[i];
		m_photonsInMemory[i] = 0;
	}

	m_photonsInMemory.clear();
	std::vector< Photon* >( m_photonsInMemory ).swap( m_photonsInMemory );
	m_storedPhotonsInBuffer = 0;
}

/*!
 * Checks where the photon map has to be saved and saves them.
 */
void TPhotonMap::EndStore( double wPhoton )
{
	if( m_storedPhotonsInBuffer  > 0 )
	{
		if( m_pExportPhotonMap ) m_pExportPhotonMap->SavePhotonMap( m_photonsInMemory );

		unsigned int photonListSize = m_photonsInMemory.size();
		for( unsigned int i = 0; i < photonListSize ;++i )
		{
			delete m_photonsInMemory[i];
			m_photonsInMemory[i] = 0;
		}

		m_photonsInMemory.clear();
    	std::vector< Photon* >( m_photonsInMemory ).swap( m_photonsInMemory );
		m_storedPhotonsInBuffer = 0;

	}
	if( m_pExportPhotonMap )	m_pExportPhotonMap->SetPowerPerPhoton( wPhoton );
	if( m_pExportPhotonMap )	m_pExportPhotonMap->EndExport();
}

/*!
 *
 */
std::vector< Photon* > TPhotonMap::GetAllPhotons() const
{
	return ( m_photonsInMemory );
}

/*!
 * Returns the export mode. If not export mode defined return null.
 */
PhotonMapExport* TPhotonMap::GetExportMode( ) const
{
	if( !m_pExportPhotonMap )	return ( 0 );
	return ( m_pExportPhotonMap );
}

/*!
 * Sets the size of the buffer to \a nPhotons.
 */
void TPhotonMap::SetBufferSize( unsigned long nPhotons )
{
	m_bufferSize = nPhotons;
}

/*!
 * Sets the transformation to change from concentrator coordinates to world coordinates.
 */
void TPhotonMap::SetConcentratorToWorld( Transform concentratorToWorld )
{
	m_concentratorToWorld = concentratorToWorld;
	if( m_pExportPhotonMap ) 	m_pExportPhotonMap->SetConcentratorToWorld( m_concentratorToWorld );
}

/*!
 *Sets the photonmap export mode.
 */
bool TPhotonMap::SetExportMode( PhotonMapExport* pExportPhotonMap )
{
	if( !pExportPhotonMap )	return 0;
	m_pExportPhotonMap = pExportPhotonMap;
	m_pExportPhotonMap->SetConcentratorToWorld( m_concentratorToWorld );

	if( !m_pExportPhotonMap->StartExport() ) return 0;

	return 1;
}

void TPhotonMap::StoreRays( std::vector< Photon >& raysList )
{
	unsigned int raysListSize = raysList.size();
	if( ( m_storedPhotonsInBuffer > 0 ) && ( ( m_storedPhotonsInBuffer + raysListSize )  > m_bufferSize ) )
	{
		if( m_pExportPhotonMap ) m_pExportPhotonMap->SavePhotonMap( m_photonsInMemory );

		unsigned int photonListSize = m_photonsInMemory.size();
		for( unsigned int i = 0; i < photonListSize ;++i )
		{
			delete m_photonsInMemory[i];
			m_photonsInMemory[i] = 0;
		}
		m_photonsInMemory.clear();
    	std::vector< Photon* >( m_photonsInMemory ).swap( m_photonsInMemory );
		m_storedPhotonsInBuffer = 0;
	}

	for( unsigned int photon = 0; photon < raysListSize; photon++ )
		m_photonsInMemory.push_back( new Photon( raysList[photon] ) );

	m_storedPhotonsInBuffer += raysListSize;
	m_storedAllPhotons += raysListSize;
}
