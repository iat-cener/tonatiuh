TEMPLATE      = lib
CONFIG       += plugin debug_and_release

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
RESOURCES += src/TrackerOneAxis.qrc
TARGET        = TrackerOneAxis

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerOneAxis	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerOneAxis
}
