TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

#QT -=core	
QT -=gui	
	
INCLUDEPATH += . \
				src \
               	$$(TONATIUH_ROOT)/src/geometry \
                $$(TONATIUH_ROOT)/src/nodes  \
                $$(TONATIUH_ROOT)/src/plugins  \
                $$(TONATIUH_ROOT)/src/statistics 

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry$$VER_MAJ -lnodes$$VER_MAJ -lstatistics$$VER_MAJ
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry$$VER_MAJ -lnodes$$VER_MAJ -lstatistics$$VER_MAJ
}

# Input
HEADERS = src/*.h  
			
SOURCES = src/*.cpp  

RESOURCES += src/MaterialVirtual.qrc

TARGET        = MaterialVirtual
 
CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/MaterialVirtual	
	unix {
		TARGET = $$member(TARGET, 0)_debug
	}
	else {
		TARGET = $$member(TARGET, 0)d
	}
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/MaterialVirtual
}

