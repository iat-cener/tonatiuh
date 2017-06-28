

TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

INCLUDEPATH += . \
			src \
			$$(TONATIUH_ROOT)/src

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
