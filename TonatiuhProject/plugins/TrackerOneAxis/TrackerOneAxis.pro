TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

INCLUDEPATH += 	. \
				src \
				$$(TONATIUH_ROOT)/src \

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/trt.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSceneKit.h \
            $$(TONATIUH_ROOT)/src/source/geometry/Vector3D.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerFactory.h


SOURCES = src/*.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSceneKit.cpp \
            $$(TONATIUH_ROOT)/src/source/geometry/Vector3D.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.cpp

RESOURCES += src/TrackerOneAxis.qrc
TARGET        = TrackerOneAxis

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerOneAxis	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerOneAxis
}
