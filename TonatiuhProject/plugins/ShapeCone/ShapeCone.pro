
TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri)

INCLUDEPATH += . \
			src \
			$$(TONATIUH_ROOT)/src
			
# Input
HEADERS = src/*.h \ 
			$$(TONATIUH_ROOT)/src/BBox.h \
           	$$(TONATIUH_ROOT)/src/DifferentialGeometry.h \
 			$$(TONATIUH_ROOT)/src/InstanceNode.h \
			$$(TONATIUH_ROOT)/src/Matrix2x2.h \
           	$$(TONATIUH_ROOT)/src/Matrix4x4.h \
           	$$(TONATIUH_ROOT)/src/NormalVector.h \
           	$$(TONATIUH_ROOT)/src/Photon.h \
           	$$(TONATIUH_ROOT)/src/Point3D.h \
           	$$(TONATIUH_ROOT)/src/Ray.h \
           	$$(TONATIUH_ROOT)/src/RefCount.h \
           	$$(TONATIUH_ROOT)/src/tgf.h \
           	$$(TONATIUH_ROOT)/src/TMaterial.h \
           	$$(TONATIUH_ROOT)/src/Trace.h  \
           	$$(TONATIUH_ROOT)/src/Transform.h \
           	$$(TONATIUH_ROOT)/src/TShape.h \ 
           	$$(TONATIUH_ROOT)/src/TShapeKit.h \
           	$$(TONATIUH_ROOT)/src/Vector3D.h

SOURCES = src/*.cpp  \ 
			$$(TONATIUH_ROOT)/src/BBox.cpp \
           	$$(TONATIUH_ROOT)/src/DifferentialGeometry.cpp \
 			$$(TONATIUH_ROOT)/src/InstanceNode.cpp \
			$$(TONATIUH_ROOT)/src/Matrix2x2.cpp \
           	$$(TONATIUH_ROOT)/src/Matrix4x4.cpp \
           	$$(TONATIUH_ROOT)/src/NormalVector.cpp \ 
           	$$(TONATIUH_ROOT)/src/Photon.cpp \
           	$$(TONATIUH_ROOT)/src/Point3D.cpp \
           	$$(TONATIUH_ROOT)/src/Ray.cpp \
           	$$(TONATIUH_ROOT)/src/RefCount.cpp \
           	$$(TONATIUH_ROOT)/src/tgf.cpp \
           	$$(TONATIUH_ROOT)/src/TMaterial.cpp \
           	$$(TONATIUH_ROOT)/src/Trace.cpp  \
           	$$(TONATIUH_ROOT)/src/Transform.cpp \
           	$$(TONATIUH_ROOT)/src/TShape.cpp \ 
           	$$(TONATIUH_ROOT)/src/TShapeKit.cpp \
           	$$(TONATIUH_ROOT)/src/Vector3D.cpp

RESOURCES += src/ShapeCone.qrc	
		
TARGET        = ShapeCone

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeCone	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeCone
}

