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

#ifndef PHOTONMAPEXPORTTYPE_H_
#define PHOTONMAPEXPORTTYPE_H_


//!  PhotonMapExportType class provides an interface for safely writing simulation data to file.
/*!
  PhotonMapExportType is an I/O device that provides the base functionality for writing simulation data to a files, the specific file format depends on the PhotonMapExportType derived class used.
*/

#include <string>
#include <vector>


class Photon;

class PhotonMapExportType
{

public:
	/*!
	 * Creates a PhotonMapExportType object that saves simulations data into the file \a filename
	 */
	PhotonMapExportType()
	:m_saveCoordinates( false ),
	 m_saveCoordinatesInGlobal( true ),
	 m_savePrevNextID( true ),
	 m_saveSide( true ),
	 m_saveSurfaceID( true ),
	 m_saveIsAbsorbed( true ),
	 m_saveRayDirection( true ),
	 m_saveSurfacesURLList()
	{

	}


	/*!
	 * Destroys object
	 */
	virtual ~PhotonMapExportType()
	{

	}

	/*!
	 * Ends save process.
	 */
	virtual void EndSave() = 0;

	/*!
	 * Saves the rays in the list \a raysLists in the file.
	 */
	virtual void SavePhotonMap( std::vector< Photon >& raysLists ) = 0;
	//void SetConcentratorToWorld( Transform concentratorToWorld );

	/*!
	 * Sets file name to \a filename.
	 */
	/*void SetFilename( QString filename )
	{
		m_photonsFilename = filename;
	}*/

	/*!
	 * Saves the power per photon value \a wPhoton in the file.
	 */
	virtual void SetPowerPerPhoton( double wPhoton ) = 0;


	//void SetSaveAllPhotonsEnabled();

	/*!
	 * Sets to \a enabled if the information about the ray is absorbed or not is saved.
	 */
	void SetSaveAbsorbedEnabled( bool enabled )
	{
		m_saveIsAbsorbed = enabled;
	}

	/*!
	 * Sets to \a enabled if the photons coordinates are saved.
	 */
	void SetSaveCoordinatesEnabled( bool enabled )
	{
		m_saveCoordinates = enabled;
	}

	/*!
	 * Sets to \a enabled if the photons coordinates are saved in global or in local coordinates system.
	 * If the local coordinates system is selected each photon transformed to the it surface local coordinate system.
	 */
	void SetSaveCoordinatesInGlobalSystemEnabled( bool enabled )
	{
		m_saveCoordinatesInGlobal = enabled;
	}

	/*!
	 * If \a enabled is true for each photon the identifier of the previous and next photon is saved in the file.
	 * Otherwise, this information will not be included.
	 */
	void SetSavePreviousNextPhotonsID( bool enabled )
	{
		m_savePrevNextID = enabled;
	}

	/*!
	 * If \a enabled is true for each photon the information about the ray direnction before the intersection is saved is saved.
	 * Otherwise, this information will not be included.
	 */
	void SetSaveRayDirection( bool enabled )
	{
		m_saveRayDirection = enabled;
	}

	/*!
	 * If \a enabled is true for each photon the identifier of the previous and next photon is saved in the file.
	 * Otherwise, this information will not be included.
	 */
	void  SetSaveSideEnabled( bool enabled )
	{
		m_saveSide = enabled;
	}

	/*!
	 * If \a enabled is true for each photon the information about the side in which the ray intersect will be included.
	 * Otherwise, this information will not be included.
	 */
	void SetSaveSurfacesIDEnabled( bool enabled )
	{
		m_saveSurfaceID = enabled;
	}

	/*!
	 * Only the photons related to intersections in the surfaced defined in \a surfacesURLList will be saved.
	 * If \a surfacesURLList is an empty list all the photons will be saved.
	 */
	void SetSaveSurfacesURLList( std::vector<std::string> surfacesURLList )
	{
		m_saveSurfacesURLList = surfacesURLList;
	}

	/*!
	 * Sets current SaveFile parameters.
	 */
	virtual void SetSaveParameterValue( std::string parameterName, std::string parameterValue ) = 0;

	/*!
	 * Starts save process.
	 */
	virtual bool StartSave() = 0;


protected:
	bool m_saveCoordinates;
	bool m_saveCoordinatesInGlobal;
	bool m_savePrevNextID;
	bool m_saveSide;
	bool m_saveSurfaceID;
	bool m_saveIsAbsorbed;
	bool m_saveRayDirection;
	std::vector<std::string> m_saveSurfacesURLList;


};

#endif /* PHOTONMAPEXPORTTYPE_H_ */
