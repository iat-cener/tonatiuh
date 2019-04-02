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


#ifndef GRAPHICROOT_H_
#define GRAPHICROOT_H_

#include <QObject>

class GraphicRootTracker;
class SoPath;
class SoSelection;
class SoSeparator;
class SoTransform;
class TSceneKit;

class GraphicRoot : public QObject
{
    Q_OBJECT

public:
	GraphicRoot();
	~GraphicRoot();

	void AddGrid( SoSeparator* grid );
	void AddRays( SoSeparator* rays );
	void AddModel( TSceneKit* sceneModel);

	void DeselectAll();

	SoSeparator* GetNode() const;

	void RemoveGrid();
	void RemoveRays();
	void RemoveModel();

	void Select( const SoPath* path );
    void SelectionChanged( SoSelection* selection );

	void ShowBackground( bool view );
	void ShowGrid( bool view );
	void ShowRays( bool view );

signals:
	void ChangeSelection( SoSelection* selection );

private:

	SoSeparator* CreateGrid( int xDimension, int zDimension, double xSpacing, double zSpacing );

	SoSeparator* m_graphicsRoot;
	SoSeparator* m_pGrid;
	SoSeparator* m_pRays;
	SoTransform* m_pRootTransform;
	SoSeparator* m_pSceneSeparator;
	SoSelection* m_pSelectionNode;
	GraphicRootTracker* m_pTracker;

};

#endif /* GRAPHICROOT_H_ */
