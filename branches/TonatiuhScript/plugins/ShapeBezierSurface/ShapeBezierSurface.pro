
TEMPLATE      = lib
CONFIG       += plugin debug_and_release


include( $$(TONATIUH_ROOT)/config.pri)


INCLUDEPATH += . \
			src 

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/geometry/NormalVector.h \
            $$(TONATIUH_ROOT)/src/source/geometry/Point3D.h \
            $$(TONATIUH_ROOT)/src/source/geometry/Vector3D.h \                                                      
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.h

SOURCES = src/*.cpp \
            $$(TONATIUH_ROOT)/src/source/geometry/NormalVector.cpp \
            $$(TONATIUH_ROOT)/src/source/geometry/Point3D.cpp \
            $$(TONATIUH_ROOT)/src/source/geometry/Vector3D.cpp \                                                        
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp

RESOURCES += src/ShapeBezierSurface.qrc
		
TARGET        = ShapeBezierSurface

CONFIG(debug, debug|release) {  
        unix {
            TARGET = $$member(TARGET, 0)_debug
        }
        else {
            TARGET = $$member(TARGET, 0)d
        }
    }
    
CONFIG(debug, debug|release)
{
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeBezierSurface	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeBezierSurface
}

