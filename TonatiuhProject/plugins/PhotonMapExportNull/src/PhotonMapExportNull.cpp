/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This Null is part of Tonatiuh.

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

#include "PhotonMapExportNull.h"

/*!
 * Creates an object that dont export photons.
 */
PhotonMapExportNull::PhotonMapExportNull()
:PhotonMapExport()
{

}

/*!
 * Destroys the object
 */
PhotonMapExportNull::~PhotonMapExportNull()
{

}

/*!
 * Returns an empty parameters name list.
 */
QStringList PhotonMapExportNull::GetParameterNames()
{
	return QStringList();
}

/*!
 * Nothing is done
 */
void PhotonMapExportNull::EndExport()
{

}

/*!
 * Nothing is done
 */
void PhotonMapExportNull::SavePhotonMap( std::vector< Photon* > /*raysLists*/ )
{

}

/*!
 * Nothing is done
 */
void PhotonMapExportNull::SetPowerPerPhoton( double /*wPhoton*/ )
{

}

/*!
 * Nothing is done
 */
void PhotonMapExportNull::SetSaveParameterValue( QString /*parameterName*/, QString /*parameterValue*/ )
{

}

/*!
 * Nothing is done
 */
bool PhotonMapExportNull::StartExport()
{
	return 1;
}
