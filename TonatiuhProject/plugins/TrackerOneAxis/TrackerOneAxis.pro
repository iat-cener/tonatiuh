TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

INCLUDEPATH += 	. \
				src \
				$$(TONATIUH_ROOT)/src \

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TDefaultTransmissivity.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/trt.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSceneKit.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerForAiming.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerFactory.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTransmissivity.h



SOURCES = src/*.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TDefaultTransmissivity.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSceneKit.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerForAiming.cpp  \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTransmissivity.cpp
RESOURCES += src/TrackerOneAxis.qrc
TARGET        = TrackerOneAxis

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerOneAxis	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerOneAxis
}
