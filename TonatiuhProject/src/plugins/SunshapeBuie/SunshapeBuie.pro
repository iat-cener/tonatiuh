TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

QT -=gui	

INCLUDEPATH += . \
				src \
               	$$(TONATIUH_ROOT)/src/geometry \
                $$(TONATIUH_ROOT)/src/nodes  \
				$$(TONATIUH_ROOT)/src/plugins \ 
                $$(TONATIUH_ROOT)/src/statistics 

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry -lnodes  -lstatistics
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry -lnodes  -lstatistics
}

# Input
HEADERS = src/*.h  \
			$$(TONATIUH_ROOT)/src/auxiliary/Trace.h \
			$$(TONATIUH_ROOT)/src/auxiliary/Tracer.h 

SOURCES = src/*.cpp   \
			$$(TONATIUH_ROOT)/src/auxiliary/Trace.cpp \
			$$(TONATIUH_ROOT)/src/auxiliary/Tracer.cpp 
RESOURCES += src/SunshapeBuie.qrc	

TARGET        = SunshapeBuie

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/SunshapeBuie	
	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/SunshapeBuie
}
