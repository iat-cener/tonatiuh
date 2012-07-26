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
	std::vector< std::vector<Photon> >GetAllPhotons() const;
	PhotonMapExport* GetExportMode( ) const;
	std::vector< std::vector<Photon> >GetSurfacePhotons(InstanceNode* instance) const;
	void SetBufferSize( unsigned long nPhotons );
	void SetExportMode( PhotonMapExport* pExportPhotonMap );
	void StoreRays( std::vector< Photon >& ray );


private:
    unsigned long m_bufferSize;
    PhotonMapExport* m_pExportPhotonMap;
	const SceneModel* m_pSceneModel;
    unsigned long m_storedPhotonsInBuffer;
    unsigned long m_storedAllPhotons;
	std::vector< std::vector <Photon > > m_photonsInMemory;


};

#endif /*TPHOTONMAP_H_*/
