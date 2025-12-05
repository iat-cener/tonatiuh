
TEMPLATE      = lib
CONFIG       += plugin
include( ../../config.pri )

INCLUDEPATH += . \
			src \
            $$(TONATIUH_ROOT)/plugins \
            $$(TONATIUH_ROOT)/src

# Input
HEADERS = src/*.h \	
           	$$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TShape.h 

SOURCES = src/*.cpp  \ 
           	$$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp 

RESOURCES += src/ShapeTroughParabola.qrc
TARGET        = ShapeTroughParabola
 
CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeTroughParabola
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeTroughParabola
}
