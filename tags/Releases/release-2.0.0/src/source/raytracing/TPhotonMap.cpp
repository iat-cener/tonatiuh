
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
    	std::vector< Photon* >( m_photonsInMemory ).swap( m_photonsInMemory );
		m_storedPhotonsInBuffer = 0;

	}
	m_pExportPhotonMap->SetPowerPerPhoton( wPhoton );


	m_pExportPhotonMap->EndExport();
}

/*!
 *
 */
std::vector< Photon* > TPhotonMap::GetAllPhotons() const
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
		m_photonsInMemory.clear();
    	std::vector< Photon* >( m_photonsInMemory ).swap( m_photonsInMemory );
		m_storedPhotonsInBuffer = 0;
	}

	for( unsigned int photon = 0; photon < raysListSize; photon++ )
		m_photonsInMemory.push_back( new Photon( raysList[photon] ) );

	m_storedPhotonsInBuffer += raysListSize;
	m_storedAllPhotons += raysListSize;
}
