TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

				
INCLUDEPATH += . \
				src \
                $$(TONATIUH_ROOT)/plugins \
                $$(TONATIUH_ROOT)/src 

# Input
HEADERS = src/*.h \				
			$$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.h 
			
SOURCES = src/*.cpp \										
			$$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.cpp 

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