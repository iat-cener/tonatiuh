
TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

TARGET        = ShapeTrumpet

# Input
HEADERS = src/*.h \
          $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
          $$(TONATIUH_ROOT)/src/source/raytracing/TShape.h
SOURCES = src/*.cpp \
          $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
          $$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp
          
RESOURCES += src/ShapeTrumpet.qrc
 
CONFIG(debug, debug|release) {
    DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeTrumpet
}
else { 
    DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeTrumpet
}
