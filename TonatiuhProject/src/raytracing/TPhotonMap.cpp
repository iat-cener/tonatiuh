#include "TPhotonMap.h"

/*!
 * Creates a photon map
 */
TPhotonMap::TPhotonMap()
:m_bufferSize( 0 ),
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
void TPhotonMap::EndStore( double /*wPhoton*/ )
{

}

/*!
 * Returns all the photons that are currently in the buffer.
 */
std::vector< Photon* > TPhotonMap::GetAllPhotons() const
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
 * Add the photons of the rays to the buffer.
 */

void TPhotonMap::StoreRays( std::vector< Photon >& raysList )
{
	unsigned int raysListSize = raysList.size();
	if( ( m_storedPhotonsInBuffer > 0 ) && ( ( m_storedPhotonsInBuffer + raysListSize )  > m_bufferSize ) )
	{
		//if( m_pExportPhotonMap ) m_pExportPhotonMap->SavePhotonMap( m_photonsInMemory );

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





