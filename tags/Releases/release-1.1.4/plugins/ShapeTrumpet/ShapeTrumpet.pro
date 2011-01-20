
TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri)

TARGET        = ShapeTrumpet

# Input
HEADERS = src/*.h \
          $$(TONATIUH_ROOT)/src/source/geometry/BBox.h \
          $$(TONATIUH_ROOT)/src/source/geometry/Matrix4x4.h \
          $$(TONATIUH_ROOT)/src/source/geometry/NormalVector.h \
          $$(TONATIUH_ROOT)/src/source/geometry/Point3D.h \
          $$(TONATIUH_ROOT)/src/source/geometry/Ray.h \
          $$(TONATIUH_ROOT)/src/source/geometry/RefCount.h \
          $$(TONATIUH_ROOT)/src/source/geometry/tgf.h \
          $$(TONATIUH_ROOT)/src/source/geometry/Transform.h \
          $$(TONATIUH_ROOT)/src/source/geometry/Vector3D.h \
          $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
          $$(TONATIUH_ROOT)/src/source/raytracing/TShape.h
SOURCES = src/*.cpp \
          $$(TONATIUH_ROOT)/src/source/geometry/BBox.cpp \
          $$(TONATIUH_ROOT)/src/source/geometry/Matrix4x4.cpp \
          $$(TONATIUH_ROOT)/src/source/geometry/NormalVector.cpp \
          $$(TONATIUH_ROOT)/src/source/geometry/Point3D.cpp \
          $$(TONATIUH_ROOT)/src/source/geometry/RefCount.cpp \
          $$(TONATIUH_ROOT)/src/source/geometry/tgf.cpp \
          $$(TONATIUH_ROOT)/src/source/geometry/Transform.cpp \
          $$(TONATIUH_ROOT)/src/source/geometry/Vector3D.cpp \
          $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
          $$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp
          
RESOURCES += src/ShapeTrumpet.qrc
 
CONFIG(debug, debug|release) {
    DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeTrumpet
}
else { 
    DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeTrumpet
}
