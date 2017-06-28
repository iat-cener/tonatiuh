TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

INCLUDEPATH += . \
			src \
            $$(TONATIUH_ROOT)/src/plugins \
            $$(TONATIUH_ROOT)/src/nodes

# Input
HEADERS = src/*.h 

SOURCES = src/*.cpp  
RESOURCES += src/SunshapePillbox.qrc	
TARGET        = SunshapePillbox

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/SunshapePillbox	
	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/SunshapePillbox
}
