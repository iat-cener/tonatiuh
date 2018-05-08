
TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

QT -=gui	

INCLUDEPATH += . \
				src \
               	$$(TONATIUH_ROOT)/src/geometry \
                $$(TONATIUH_ROOT)/src/nodes  \
				$$(TONATIUH_ROOT)/src/plugins 

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry$$VER_MAJ -lnodes$$VER_MAJ
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry$$VER_MAJ -lnodes$$VER_MAJ
}

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


