/*
 * TLightShapeTests.cpp
 *
 *  Created on: 01/09/2011
 *      Author: sgarayalde
 */

#include "TLightShape.h"
#include "Point3D.h"

#include <gtest/gtest.h>
#include "TestsAuxiliaryFunctions.h"
#include <time.h>

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 60000;

TEST(TLightShapeTests , DefaultConstructor){

	TLightShape* shape= new TLightShape;

	EXPECT_DOUBLE_EQ(double(shape->xMin.getValue()),-0.5);
	EXPECT_DOUBLE_EQ(double(shape->xMax.getValue()),0.5);
	EXPECT_DOUBLE_EQ(double(shape->zMin.getValue()),-0.5);
	EXPECT_DOUBLE_EQ(double(shape->zMax.getValue()),0.5);
	EXPECT_DOUBLE_EQ(double(shape->delta.getValue()),100.0);
}

TEST(TLightShapeTests , TLightShapeGetArea){

	srand ( time(NULL) );

	// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){
		float xmin=float(taf::randomNumber(a,b));
		float xmax=float(taf::randomNumber(a,b));
		float zmin=float(taf::randomNumber(a,b));
		float zmax=float(taf::randomNumber(a,b));

		TLightShape* shape= new TLightShape;

		shape->xMin.setValue(xmin);
		shape->xMax.setValue(xmax);
		shape->zMin.setValue(zmin);
		shape->zMax.setValue(zmax);

		trt::TONATIUH_REAL xminR;
		trt::TONATIUH_REAL xmaxR;
		trt::TONATIUH_REAL zminR;
		trt::TONATIUH_REAL zmaxR;
		xminR.setValue(xmin);
		xmaxR.setValue(xmax);
		zminR.setValue(zmin);
		zmaxR.setValue(zmax);

		double width= xmaxR.getValue()-xminR.getValue();
		double heigh= zmaxR.getValue()-zminR.getValue();

		EXPECT_DOUBLE_EQ(shape->GetArea(),width*heigh);
	}


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

	srand ( time(NULL) );

			// Extension of the testing space
	double b = maximumCoordinate;
	double a = -b;
	double c = 0;
	double d = 1;

	for( unsigned long int i = 0; i < maximumNumberOfTests; i++ ){

		double xmin=taf::randomNumber(a,b);
		double xmax=taf::randomNumber(a,b);
		double zmin=taf::randomNumber(a,b);
		double zmax=taf::randomNumber(a,b);

		double x0=taf::randomNumber(c,d);
		double z0=taf::randomNumber(c,d);

		TLightShape* shape= new TLightShape;

		shape->xMin.setValue(xmin);
		shape->xMax.setValue(xmax);
		shape->zMin.setValue(zmin);
		shape->zMax.setValue(zmax);

		trt::TONATIUH_REAL xminR;
		trt::TONATIUH_REAL xmaxR;
		trt::TONATIUH_REAL zminR;
		trt::TONATIUH_REAL zmaxR;
		xminR.setValue(xmin);
		xmaxR.setValue(xmax);
		zminR.setValue(zmin);
		zmaxR.setValue(zmax);
		/* Este ejemplo produce que el programa mande un mensaje de error y se cierre
		 * ya que los valores x0 y0 no pertenecen al rango {0,1}
		 * EXPECT_FALSE(shape->Sample(x0,y0) == true);
		 */

		/*EXPECT_DOUBLE_EQ(shape->Sample(x0,z0).x,(x0*(xmaxR.getValue()-xminR.getValue())+xminR.getValue()));
		EXPECT_DOUBLE_EQ(shape->Sample(x0,z0).y,0.0);
		EXPECT_DOUBLE_EQ(shape->Sample(x0,z0).z,(z0*(zmaxR.getValue()-zminR.getValue())+zminR.getValue()));

		*/
	}
}
