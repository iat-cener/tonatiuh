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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Iñigo Pagola, Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/
#include "SceneModel.h"
#include "ScriptRayTracer.h"

ScriptRayTracer::ScriptRayTracer(  QVector< RandomDeviateFactory* > listRandomDeviateFactory )
:
m_document( 0 ),
m_irradiance( -1 ),
m_numberOfRays( 0 ),
m_photonMap( 0 ),
m_RandomDeviateFactoryList( listRandomDeviateFactory ),
m_randomDeviate( 0 ),
m_sceneModel ( 0 ),
m_widthDivisions(200),
m_heightDivisions(200),
m_sunPosistionChanged( false ),
m_sunAzimuth( 0 ),
m_sunElevation( 0 ),
m_wPhoton( 0 ),
m_dirName( "" )
{

}

ScriptRayTracer::~ScriptRayTracer()
{
	delete m_document;
	delete m_photonMap;
	delete m_randomDeviate;
	delete m_sceneModel;
}

void ScriptRayTracer::Clear()
{
	delete m_document;
	m_document = 0;
	m_irradiance = -1;
	m_numberOfRays = 0;
	delete m_photonMap;
	m_photonMap = 0;
	delete m_randomDeviate;
	m_randomDeviate = 0;
	delete m_sceneModel;
	m_sceneModel = 0;
	m_sunAzimuth = 0;
	m_sunElevation = 0;
	m_wPhoton = 0;
	m_dirName.clear();
}

QString ScriptRayTracer::GetDir()
{
	return m_dirName;
}


bool ScriptRayTracer::IsValidRandomGeneratorType( QString type )
{
	if( m_RandomDeviateFactoryList.size() == 0 )	 return 0;

	QVector< QString > randomGeneratorsNames;
	for( int i = 0; i < m_RandomDeviateFactoryList.size(); i++ )
		randomGeneratorsNames<< m_RandomDeviateFactoryList[i]->RandomDeviateName();

	int selectedRandom = randomGeneratorsNames.indexOf( type );

	if( selectedRandom < 0 )	return 0;

	return 1;
}

bool ScriptRayTracer::IsValidSurface( QString surfaceName )
{
	if( !m_sceneModel )	return false;

	QModelIndex surfaceIndex = m_sceneModel->IndexFromNodeUrl( surfaceName );
	InstanceNode* selectedSurface = m_sceneModel->NodeFromIndex( surfaceIndex );
	if( !selectedSurface )	return false;

	return true;
}

int ScriptRayTracer::SetDir( QString dir )
{
	m_dirName = dir;
	return 1;
}

int ScriptRayTracer::SetIrradiance( double irradiance )
{
	m_irradiance = irradiance;
	return 1;
}

int ScriptRayTracer::SetNumberOfRays( double nrays )
{
	m_numberOfRays = nrays;
	return 1;
}

int ScriptRayTracer::SetNumberOfWidthDivisions( int ndivisions )
{
	m_widthDivisions = ndivisions;
	return 1;
}

int ScriptRayTracer::SetNumberOfHeightDivisions( int ndivisions )
{
	m_heightDivisions = ndivisions;
	return 1;
}

int ScriptRayTracer::SetPhotonMapExportMode( QString typeName )
{
	if( typeName == QLatin1String( "File" ) ) 	m_photonMapToFile = true;
	else if( typeName == QLatin1String( "DB" ) ) 	m_photonMapToFile = false;
	else	return 0;

	return 1;
}

int ScriptRayTracer::SetRandomDeviateType( QString typeName )
{
	QVector< QString > randomGeneratorsNames;
	for( int i = 0; i < m_RandomDeviateFactoryList.size(); i++ )
		randomGeneratorsNames<< m_RandomDeviateFactoryList[i]->RandomDeviateName();

	int selectedRandom = randomGeneratorsNames.indexOf( typeName );
	if(  selectedRandom < 0 )
	{
		m_randomDeviate = 0;
		return 0;
	}

	m_randomDeviate = m_RandomDeviateFactoryList[selectedRandom]->CreateRandomDeviate();
	return 1;
}

/*!
 * Saves the sun position \a azimuth value. \a azimuth  is in degrees.
 */
void ScriptRayTracer::SetSunAzimtuh( double azimuth )
{
	m_sunAzimuth = azimuth * gc::Degree;
	m_sunPosistionChanged = true;

}

/*!
 * Saves the sun position \a elevation value. \a elevation  is in degrees.
 */
void ScriptRayTracer::SetSunElevation( double elevation )
{
	m_sunElevation = elevation * gc::Degree;
	m_sunPosistionChanged = true;
}

int  ScriptRayTracer::SetSunPositionToScene()
{
	if (m_sceneModel)
	{
		QModelIndex sceneIndex;
		InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( sceneIndex );
		SoSceneKit* coinScene =  static_cast< SoSceneKit* >( sceneInstance->GetNode() );

		if ((coinScene)&& ( coinScene->getPart( "lightList[0]", false ) ))
		{
			TLightKit* lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", false ) );
			if( m_sunPosistionChanged )	lightKit->ChangePosition( m_sunAzimuth, gc::Pi/2 - m_sunElevation );
			return 1;
		}
		std::cerr<<"ScriptRayTracer::SetSunPositionToScene() light not found in scene"<<std::endl;
		return 0;
	}
	std::cerr<<"ScriptRayTracer::SetSunPositionToScene() sceneModel not found"<<std::endl;
	return 0;
}

int   ScriptRayTracer::Save( const QString& fileName)
{
 	if( !m_document->WriteFile( fileName ) )
	{
 		std::cerr<< "Saving canceled";
		return 0;
	}

	std::cerr<< "File saved";
	return 1;
}


int ScriptRayTracer::SetTonatiuhModelFile ( QString filename )
{
	delete m_document;
	m_document = 0;

	m_document = new Document;
	if( !m_document->ReadFile( filename ) )	return 0;

	delete m_sceneModel;
	m_sceneModel = 0;
	m_sceneModel = new SceneModel;

	m_sceneModel->SetCoinScene( *m_document->GetSceneKit() );

	return 1;
}

double ScriptRayTracer::GetArea(){
	return m_area;
}

double ScriptRayTracer::GetNumrays(){
	return m_numberOfRays;
}
