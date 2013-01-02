TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

INCLUDEPATH += . \
            src \
            $$(TONATIUH_ROOT)/plugins \
            $$(TONATIUH_ROOT)/src

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.h


SOURCES = src/*.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp

RESOURCES += src/ShapeFlatTriangle.qrc         
TARGET        = ShapeFlatTriangle

CONFIG(debug, debug|release) {
    DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeFlatTriangle     
}
else { 
    DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeFlatTriangle
}

