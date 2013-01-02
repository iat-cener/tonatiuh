
TEMPLATE      = lib
CONFIG       += plugin debug_and_release


include( ../../config.pri )


INCLUDEPATH += . \
			src 

# Input
HEADERS = src/*.h \                                                    
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.h

SOURCES = src/*.cpp \                                                       
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp

RESOURCES += src/ShapeBezierSurface.qrc		
TARGET        = ShapeBezierSurface

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeBezierSurface	
	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeBezierSurface
}

