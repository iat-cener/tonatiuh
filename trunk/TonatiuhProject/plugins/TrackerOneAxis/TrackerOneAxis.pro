TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri)

INCLUDEPATH += 	. \
				src \
				$$(TONATIUH_ROOT)/src \

# Input
HEADERS = src/*.h \
			$$(TONATIUH_ROOT)/src/Trace.h \
			$$(TONATIUH_ROOT)/src/TTracker.h \
			$$(TONATIUH_ROOT)/src/TTrackerFactory.h


SOURCES = src/*.cpp \
			$$(TONATIUH_ROOT)/src/TTracker.cpp \
			$$(TONATIUH_ROOT)/src/Trace.cpp 

RESOURCES += src/TrackerOneAxis.qrc
TARGET        = TrackerOneAxis

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerOneAxis	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerOneAxis
}
