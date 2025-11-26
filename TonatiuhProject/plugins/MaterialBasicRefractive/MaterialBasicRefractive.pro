TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

				
INCLUDEPATH += . \
				src \
                $$(TONATIUH_ROOT)/plugins \
				$$(TONATIUH_ROOT)/src 

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/geometry/tgf.h \		
			$$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.h 

SOURCES = src/*.cpp \
            $$(TONATIUH_ROOT)/src/source/geometry/tgf.cpp \		
			$$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.cpp

RESOURCES += src/MaterialBasicRefractive.qrc

TARGET        = MaterialBasicRefractive
 
CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/MaterialBasicRefractive	
	unix {
		TARGET = $$member(TARGET, 0)_debug
	}
	else {
		TARGET = $$member(TARGET, 0)d
	}
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/MaterialBasicRefractive
}

