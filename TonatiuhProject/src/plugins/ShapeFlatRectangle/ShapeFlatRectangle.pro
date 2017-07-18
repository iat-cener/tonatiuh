TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

INCLUDEPATH += . \
			src \
            $$(TONATIUH_ROOT)/plugins \
            $$(TONATIUH_ROOT)/src/nodes

# Input
HEADERS = src/*.h 

SOURCES = src/*.cpp  
           	
RESOURCES += src/ShapeFlatRectangle.qrc		

TARGET        = ShapeFlatRectangle

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeFlatRectangle	
	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeFlatRectangle
}

