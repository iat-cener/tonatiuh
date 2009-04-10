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

#include "CmdInsertPhotonMap.h"
#include "SceneModel.h"
#include "tgf.h"
#include "TPhotonMap.h"
#include "Trace.h"
#include "TShapeKit.h"


CmdInsertPhotonMap::CmdInsertPhotonMap( TShapeKit* shapeKit, TPhotonMap* photonMap, SceneModel* model, QUndoCommand * parent )
: QUndoCommand("InsertPhotonMap", parent), m_shapeKit( shapeKit ), m_previousPhotonMap( 0 ), m_photonMap( photonMap ), m_pModel( model ), m_row( -1 )
{
	Trace trace( "CmdInsertPhotonMap::CmdInsertPhotonMap", false );

	if( m_shapeKit == 0 ) tgf::SevereError( "CmdInsertPhotonMap called with NULL TShapeKit" );
	if( m_photonMap == 0 ) tgf::SevereError( "CmdInsertPhotonMap called with NULL TPhotonMap" );
	m_photonMap->ref();
    m_previousPhotonMap = dynamic_cast< TPhotonMap* >( m_shapeKit->getPart( "photonMap", false ) );
}

CmdInsertPhotonMap::~CmdInsertPhotonMap()
{
	Trace trace( "CmdInsertPhotonMap::~CmdInsertPhotonMap", false );
    m_photonMap->unref();
}

void CmdInsertPhotonMap::undo()
{
	Trace trace( "CmdInsertPhotonMap::undo", false );

    m_shapeKit->setPart( "photonMap", NULL );
	m_pModel->RemoveCoinNode( m_row, *m_shapeKit );
}

void CmdInsertPhotonMap::redo( )
{
	Trace trace( "CmdInsertPhotonMap::redo", false );

    m_shapeKit->setPart( "photonMap", m_photonMap );
    m_row = m_pModel->InsertCoinNode( *m_photonMap, *m_shapeKit );
}
