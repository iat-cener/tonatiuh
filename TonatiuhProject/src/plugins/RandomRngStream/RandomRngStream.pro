TEMPLATE      = lib
CONFIG       += plugin debug_and_release


include( $$(TONATIUH_ROOT)/config.pri )

INCLUDEPATH += . \
			src \
			$$(TONATIUH_ROOT)/src

# Input
HEADERS = src/*.h

SOURCES = src/*.cpp 

TARGET        = RandomRngStream

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/RandomRngStream	

}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/RandomRngStream
}
