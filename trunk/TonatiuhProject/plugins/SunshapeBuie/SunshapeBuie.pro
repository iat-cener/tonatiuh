
TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri)

INCLUDEPATH += . \
			src \
            $$(TONATIUH_ROOT)/plugins \
			$$(TONATIUH_ROOT)/src

# Input
HEADERS = src/*.h \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TSunShape.h 


SOURCES = src/*.cpp \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TSunShape.cpp

RESOURCES += src/SunshapeBuie.qrc	
TARGET        = SunshapeBuie

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/SunshapeBuie	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/SunshapeBuie
}
