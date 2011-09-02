/*
 * TLightShapeTests.cpp
 *
 *  Created on: 01/09/2011
 *      Author: sgarayalde
 */

#include "TLightShape.h"
#include "Point3D.h"

#include <gtest/gtest.h>

TEST(TLightShapeTests , DefaultConstructor){

	TLightShape* shape= new TLightShape;

	EXPECT_DOUBLE_EQ(double(shape->xMin.getValue()),-0.5);
	EXPECT_DOUBLE_EQ(double(shape->xMax.getValue()),0.5);
	EXPECT_DOUBLE_EQ(double(shape->zMin.getValue()),-0.5);
	EXPECT_DOUBLE_EQ(double(shape->zMax.getValue()),0.5);
	EXPECT_DOUBLE_EQ(double(shape->delta.getValue()),100.0);
}

TEST(TLightShapeTests , TLightShapeGetArea){

    double xmin=-25;
    double xmax=40;
    double zmin=7;
    double zmax=32;

	TLightShape* shape= new TLightShape;

	shape->xMin.setValue(xmin);
	shape->xMax.setValue(xmax);
	shape->zMin.setValue(zmin);
	shape->zMax.setValue(zmax);

	EXPECT_DOUBLE_EQ(double(shape->GetArea()),(xmax-xmin)*(zmax-zmin));

}

TEST(TLightShapeTests , TLightShapeIsIntoValidArea){

    double xmin=-25;
    double xmax=40;
    double zmin=7;
    double zmax=32;

    double x=200;
    double y=0.0;
    double z=200;

	TLightShape* shape= new TLightShape;

	shape->xMin.setValue(xmin);
	shape->xMax.setValue(xmax);
	shape->zMin.setValue(zmin);
	shape->zMax.setValue(zmax);
	/*
	 * Las variables son protected y no se pueden asignar
	 * shape->m_heightElements=2;
	 * shape->m_widthElements=4;
	 */


	Point3D point(x,y,z);
	Point3D point2(0,45,15);

	/*
	 * No se pude comparar que salga verdadero porque hay una operacion en
	 * la funcion que hace una division entre 0 y no se puede modificar el valor de esto
	 * porque las variables son de tipo protected
	 *
	 * EXPECT_TRUE(shape->IsIntoValidArea(point) == true);
	 * */

	EXPECT_TRUE(shape->IsIntoValidArea(point2) == false);

}


TEST(TLightShapeTests , TLightShapeSample){

	double xmin=-25;
	double xmax=40;
	double zmin=7;
    double zmax=32;

    double x0=5;
    double z0=40;


    double x1=double(12/150);
    double z1=double(1450/5231);

    TLightShape* shape= new TLightShape;

	shape->xMin.setValue(xmin);
	shape->xMax.setValue(xmax);
	shape->zMin.setValue(zmin);
	shape->zMax.setValue(zmax);
    /* Este ejemplo produce que el programa mande un mensaje de error y se cierre
     * ya que los valores x0 y0 no pertenecen al rango {0,1}
     * EXPECT_FALSE(shape->Sample(x0,y0) == true);
     */

    EXPECT_DOUBLE_EQ(shape->Sample(x1,z1).x,(x1*(xmax-xmin)+xmin));
    EXPECT_DOUBLE_EQ(shape->Sample(x1,z1).y,0.0);
    EXPECT_DOUBLE_EQ(shape->Sample(x1,z1).z,(z1*(zmax-zmin)+zmin));


}
