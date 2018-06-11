#include <mutex>

#include "TPhotonMap.h"

/*!
 * Creates a photon map
 */
TPhotonMap::TPhotonMap()
:m_bufferSize( 5000000 ),
 m_storedPhotonsInBuffer( 0 ),
 m_storedAllPhotons( 0 ),
 m_pPhotonMapExportType( nullptr )
{

	m_mutexPhotonMap.unlock();
}

/*
 * Destroys
 */
TPhotonMap::~TPhotonMap()
{
	m_photonsInMemory.clear();
	m_storedPhotonsInBuffer = 0;
}

/*!
 * Checks where the photon map has to be saved and saves them.
 */
void TPhotonMap::EndStore( double wPhoton )
{
	m_mutexPhotonMap.lock();

	if( m_storedPhotonsInBuffer  > 0 &&  m_pPhotonMapExportType )
		m_pPhotonMapExportType->SavePhotonMap( m_photonsInMemory );

	EmptyBuffer();

	if( m_pPhotonMapExportType )	m_pPhotonMapExportType->SetPowerPerPhoton( wPhoton );
	if( m_pPhotonMapExportType )	m_pPhotonMapExportType->EndSave();

	m_mutexPhotonMap.unlock();
}

/*!
 * Returns all the photons that are currently in the buffer.
 */

std::vector< Photon > TPhotonMap::GetAllPhotons() const
{
	return ( m_photonsInMemory );
}


/*!
 * Sets the size of the buffer to \a nPhotons.
 */
void TPhotonMap::SetBufferSize( unsigned long nPhotons )
{
	m_bufferSize = nPhotons;
}

/*!
 * Defines the export type for the photon map.
 */
void TPhotonMap::SetExportType( std::unique_ptr< PhotonMapExportType > pPhotonMapExportType )
{
	m_pPhotonMapExportType = std::move( pPhotonMapExportType );
	m_pPhotonMapExportType->StartSave();

}

/*!
 * Add the photons of the rays to the buffer.
 */

void TPhotonMap::StoreRays( std::vector< Photon >& raysList )
{
	m_mutexPhotonMap.lock();

	unsigned int raysListSize = raysList.size();
	if( ( m_storedPhotonsInBuffer > 0 ) && ( ( m_storedPhotonsInBuffer + raysListSize )  > m_bufferSize ) )
	{
		if( m_pPhotonMapExportType != nullptr )
			m_pPhotonMapExportType->SavePhotonMap( m_photonsInMemory );

		EmptyBuffer();
	}
	m_photonsInMemory.insert(m_photonsInMemory.end(), raysList.begin(), raysList.end());

	m_storedPhotonsInBuffer += raysListSize;
	m_storedAllPhotons += raysListSize;

	m_mutexPhotonMap.unlock();

}

/*!
 * Checks if there is a export type defined as if so, export the current photons stored at the buffer.
 * Then, the photons are removed.
 */
void TPhotonMap::EmptyBuffer()
{

	m_photonsInMemory.clear();
	std::vector< Photon >( m_photonsInMemory ).swap( m_photonsInMemory );
	m_storedPhotonsInBuffer = 0;
}
