
#include "PhotonMapExport.h"
#include "TPhotonMap.h"

/*!
 * Creates a photon map
 */
TPhotonMap::TPhotonMap()
:m_bufferSize( 0 ),
 m_pExportPhotonMap( 0 ),
 m_storedPhotonsInBuffer( 0 ),
 m_storedAllPhotons( 0 )
{

}

/*
 * Destroys
 */
TPhotonMap::~TPhotonMap()
{
}

/*!
 * Checks where the photon map has to be saved and saves them.
 */
void TPhotonMap::EndStore( double wPhoton )
{
	if( m_storedPhotonsInBuffer  > 0 )
	{
		if( m_pExportPhotonMap ) m_pExportPhotonMap->SavePhotonMap( m_photonsInMemory );
		m_photonsInMemory.clear();
		m_storedPhotonsInBuffer = 0;
	}
	m_pExportPhotonMap->SetPowerPerPhoton( wPhoton );

	m_pExportPhotonMap->EndExport();
}

/*!
 *
 */
std::vector<std::vector<Photon> > TPhotonMap::GetAllPhotons() const
{
	return m_photonsInMemory;
}

/*!
 * Returns the export mode. If not export mode defined return null.
 */
PhotonMapExport* TPhotonMap::GetExportMode( ) const
{
	if( !m_pExportPhotonMap )	return 0;
	return m_pExportPhotonMap;
}

/*!
 * Sets the size of the buffer to \a nPhotons.
 */
void TPhotonMap::SetBufferSize( unsigned long nPhotons )
{
	m_bufferSize = nPhotons;
}

/*!
 *Sets the photonmap export mode.
 */
void TPhotonMap::SetExportMode( PhotonMapExport* pExportPhotonMap )
{
	m_pExportPhotonMap = pExportPhotonMap;
	m_pExportPhotonMap->StartExport();
}

void TPhotonMap::StoreRays( std::vector< Photon >& raysList )
{
	unsigned int raysListSize = raysList.size();

	if( ( m_storedPhotonsInBuffer > 0 ) && ( ( m_storedPhotonsInBuffer + raysListSize )  > m_bufferSize ) )
	{
		if( m_pExportPhotonMap ) m_pExportPhotonMap->SavePhotonMap( m_photonsInMemory );
		m_photonsInMemory.clear();
    	std::vector< std::vector< Photon > >( m_photonsInMemory ).swap( m_photonsInMemory );
		m_storedPhotonsInBuffer = 0;
	}
	m_photonsInMemory.reserve( m_photonsInMemory.size()+1 );
	m_photonsInMemory.push_back( raysList );
	m_storedPhotonsInBuffer += raysListSize;
	m_storedAllPhotons += raysListSize;
}
