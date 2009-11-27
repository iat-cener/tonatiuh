TEMPLATE      = lib
CONFIG       += plugin debug_and_release

INCLUDEPATH += 	. \
				src \
				$$(TONATIUH_ROOT)/src \
				$$(TDE_ROOT)/local/include

win32{
	DEFINES+= COIN_DLL SOQT_DLL
}

# Input
HEADERS = src/*.h \
			$$(TONATIUH_ROOT)/src/BBox.h \
           	$$(TONATIUH_ROOT)/src/DifferentialGeometry.h \
 			$$(TONATIUH_ROOT)/src/InstanceNode.h \
           	$$(TONATIUH_ROOT)/src/Matrix4x4.h \
			$$(TONATIUH_ROOT)/src/NormalVector.h \
           	$$(TONATIUH_ROOT)/src/Photon.h \
			$$(TONATIUH_ROOT)/src/Point3D.h \
           	$$(TONATIUH_ROOT)/src/Ray.h \
           	$$(TONATIUH_ROOT)/src/RefCount.h \
			$$(TONATIUH_ROOT)/src/tgf.h \
			$$(TONATIUH_ROOT)/src/Trace.h \
           	$$(TONATIUH_ROOT)/src/Transform.h \
			$$(TONATIUH_ROOT)/src/TShape.h  \
           	$$(TONATIUH_ROOT)/src/TShapeKit.h \
			$$(TONATIUH_ROOT)/src/Vector3D.h


SOURCES = src/*.cpp \
			$$(TONATIUH_ROOT)/src/BBox.cpp \
           	$$(TONATIUH_ROOT)/src/DifferentialGeometry.cpp \
 			$$(TONATIUH_ROOT)/src/InstanceNode.cpp \
           	$$(TONATIUH_ROOT)/src/Matrix4x4.cpp \
			$$(TONATIUH_ROOT)/src/NormalVector.cpp \
           	$$(TONATIUH_ROOT)/src/Photon.cpp \
			$$(TONATIUH_ROOT)/src/Point3D.cpp \
           	$$(TONATIUH_ROOT)/src/Ray.cpp \
           	$$(TONATIUH_ROOT)/src/RefCount.cpp \
			$$(TONATIUH_ROOT)/src/tgf.cpp \
			$$(TONATIUH_ROOT)/src/Trace.cpp  \
           	$$(TONATIUH_ROOT)/src/Transform.cpp \
			$$(TONATIUH_ROOT)/src/TShape.cpp \
           	$$(TONATIUH_ROOT)/src/TShapeKit.cpp \
			$$(TONATIUH_ROOT)/src/Vector3D.cpp

RESOURCES += src/ShapePrism.qrc


DESTDIR       = $$(TONATIUH_ROOT)/plugins/ShapePrism
TARGET        = ShapePrism

LIBS +=-L$$(TDE_ROOT)/local/lib -lCoin -lSoQt

 
CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapePrism	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapePrism
}
