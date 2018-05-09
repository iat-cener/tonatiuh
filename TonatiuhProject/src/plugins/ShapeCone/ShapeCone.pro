
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
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry -lnodes
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry -lnodes
}

# Input
HEADERS = src/*.h 

SOURCES = src/*.cpp  

RESOURCES += src/ShapeCone.qrc	
		
TARGET        = ShapeCone

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeCone	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeCone
}

