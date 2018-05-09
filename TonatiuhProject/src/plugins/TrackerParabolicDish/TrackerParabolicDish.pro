TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

QT -=gui	

INCLUDEPATH += src \
               	$$(TONATIUH_ROOT)/src/geometry \
                $$(TONATIUH_ROOT)/src/nodes   \
				$$(TONATIUH_ROOT)/src/plugins 

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry -lnodes
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry -lnodes
}

# Input
HEADERS = src/*.h   \
			$$(TONATIUH_ROOT)/src/auxiliary/Trace.h \
			$$(TONATIUH_ROOT)/src/auxiliary/Tracer.h 

SOURCES = src/*.cpp    \
			$$(TONATIUH_ROOT)/src/auxiliary/Trace.cpp\
			$$(TONATIUH_ROOT)/src/auxiliary/Tracer.cpp 

RESOURCES += src/TrackerParabolicDish.qrc	

TARGET        = TrackerParabolicDish

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerParabolicDish	
	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerParabolicDish
}
