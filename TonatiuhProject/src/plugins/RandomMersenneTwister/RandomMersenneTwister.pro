

TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

QT -=gui	

INCLUDEPATH += . \
			src \
			$$(TONATIUH_ROOT)/src/plugins \ 
			$$(TONATIUH_ROOT)/src/statistics 

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lstatistics$$VER_MAJ
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lstatistics$$VER_MAJ
}

# Input
HEADERS = src/*.h

SOURCES = src/*.cpp 

TARGET        = RandomMersenneTwister


CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/RandomMersenneTwister	

}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/RandomMersenneTwister
}
