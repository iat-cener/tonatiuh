/*
 * ShapeCADFactory.h
 *
 *  Created on: 30/03/2015
 *      Author: amutuberria
 */

#ifndef SHAPECADFACTORY_H_
#define SHAPECADFACTORY_H_

#include "ShapeCAD.h"
#include "Triangle.h"
#include "TShapeFactory.h"

class ShapeCADFactory: public QObject, public TShapeFactory
{
    Q_OBJECT
    Q_INTERFACES(TShapeFactory)
#if QT_VERSION >= 0x050000 // pre Qt 5
    Q_PLUGIN_METADATA(IID "tonatiuh.TShapeFactory")
#endif

public:
   	QString TShapeName() const;
   	QIcon TShapeIcon() const;
   	ShapeCAD* CreateTShape( ) const;
   	bool IsFlat() { return false; }

private:
   	bool ReadSTLFile( QString filename, std::vector< Triangle* >* facetList ) const;
};


#endif /* SHAPECADFACTORY_H_ */
