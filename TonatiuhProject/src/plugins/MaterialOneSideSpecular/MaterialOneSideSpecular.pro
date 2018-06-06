TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

#QT -= core	
QT -= gui	

DEFINES += TONATIUH_PLUGIN_SHARED_LIBRARY
				
INCLUDEPATH += . \
				src \
               	$$(TONATIUH_ROOT)/src/geometry \
                $$(TONATIUH_ROOT)/src/nodes  \
                $$(TONATIUH_ROOT)/src/plugins  \
                $$(TONATIUH_ROOT)/src/statistics 

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry -lnodes  -lstatistics
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry -lnodes  -lstatistics
}

# Input
HEADERS = 	src/*.h 
			
SOURCES = 	src/*.cpp  

RESOURCES += src/MaterialOneSideSpecular.qrc

TARGET        = MaterialOneSideSpecular
 
CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/MaterialOneSideSpecular	
	unix {
		TARGET = $$member(TARGET, 0)_debug
	}
	else {
		TARGET = $$member(TARGET, 0)d
	}
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/MaterialOneSideSpecular
}
