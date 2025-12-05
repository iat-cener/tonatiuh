TEMPLATE      = lib
CONFIG       += plugin

include( ../../config.pri )

INCLUDEPATH += 	. \
				src \
                $$(TONATIUH_ROOT)/src/source/geometry \
                $$(TONATIUH_ROOT)/src/source/raytracing

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.h 


SOURCES = src/*.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.cpp

RESOURCES += src/TrackerZAxis.qrc
TARGET        = TrackerZAxis

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerZAxis	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerZAxis
}
