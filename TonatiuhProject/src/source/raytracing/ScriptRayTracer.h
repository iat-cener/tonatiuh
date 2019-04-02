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


#ifndef SCRIPTRAYTRACER_H_
#define SCRIPTRAYTRACER_H_

#include <QMap>
#include <QObject>
#include <QPair>
#include <QString>
#include <QVector>

class Document;
class GraphicRoot;
class InstanceNode;
class RandomDeviate;
class RandomDeviateFactory;
class QScriptContext;
class SceneModel;
class TPhotonMap;
class Transform;

class ScriptRayTracer : public QObject
{
	Q_OBJECT

public:
	ScriptRayTracer( QVector< RandomDeviateFactory* > listRandomDeviateFactory  );
	~ScriptRayTracer();

	void Clear();

	QString GetDir();

	bool IsValidRandomGeneratorType( QString type );
	bool IsValidSurface( QString surfaceName );

	double GetArea();
	double GetNumrays();

	int SetDir( QString dir );

	int SetIrradiance( double irradiance );

	int SetNumberOfRays( double nrays );

	int SetNumberOfWidthDivisions( int wdivisions );
	int SetNumberOfHeightDivisions( int hdivisions );

	int SetPhotonMapExportMode( QString typeName );

	int SetRandomDeviateType( QString typeName );

	void SetSunAzimtuh( double azimuth);
	void SetSunElevation( double elevation );

	void SetupGraphcisRoot();
	void SetupModels();
	int SetTonatiuhModelFile ( QString filename );

	int Trace();

	int SetSunPositionToScene();
	int SetDisconnectAllTrackers(bool disconnect);
	int Save( const QString& fileName);

private:
	Document* m_document;

	double m_irradiance;

	unsigned long m_numberOfRays;

	TPhotonMap* m_photonMap;
	bool m_photonMapToFile;

	QVector< RandomDeviateFactory* > m_RandomDeviateFactoryList;
	RandomDeviate* m_randomDeviate;

	SceneModel* m_sceneModel;

	double m_area;
	int m_widthDivisions;
	int m_heightDivisions;
	bool m_sunPosistionChanged;
	double m_sunAzimuth;
	double m_sunElevation;

	double m_wPhoton;

	QString m_dirName;

};
#endif /* SCRIPTRAYTRACER_H_ */
