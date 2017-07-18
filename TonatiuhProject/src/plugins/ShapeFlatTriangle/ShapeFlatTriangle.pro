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

RESOURCES += src/ShapeFlatTriangle.qrc    
     
TARGET        = ShapeFlatTriangle

CONFIG(debug, debug|release) {
    DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeFlatTriangle     
}
else { 
    DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeFlatTriangle
}

