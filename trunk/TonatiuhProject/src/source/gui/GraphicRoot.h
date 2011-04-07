/*
 * GraphicRoot.h
 *
 *  Created on: 07/04/2011
 *      Author: amutuberria
 */

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
	SoSelection* m_pSelectionNode;
	GraphicRootTracker* m_pTracker;

};

#endif /* GRAPHICROOT_H_ */
