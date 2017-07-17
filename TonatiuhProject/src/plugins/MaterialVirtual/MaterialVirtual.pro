TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

				
INCLUDEPATH += . \
				src \
                $$(TONATIUH_ROOT)/plugins \
                $$(TONATIUH_ROOT)/src/nodes  \
                $$(TONATIUH_ROOT)/src/statistics 

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

