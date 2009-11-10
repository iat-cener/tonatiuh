
TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri)

INCLUDEPATH += . \
			src \
			$$(TONATIUH_ROOT)/src

# Input
HEADERS = *.h \
           	$$(TONATIUH_ROOT)/src/Trace.h \
           	$$(TONATIUH_ROOT)/src/TSunShape.h 


SOURCES = *.cpp \
           	$$(TONATIUH_ROOT)/src/Trace.cpp \
           	$$(TONATIUH_ROOT)/src/TSunShape.cpp

RESOURCES += SunshapePillbox.qrc	
TARGET        = SunshapePillbox

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/SunshapePillbox	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/SunshapePillbox
}
