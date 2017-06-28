
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
RESOURCES += src/ShapeSphericalRectangle.qrc	
TARGET        = ShapeSphericalRectangle

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeSphericalRectangle	
	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeSphericalRectangle
}


