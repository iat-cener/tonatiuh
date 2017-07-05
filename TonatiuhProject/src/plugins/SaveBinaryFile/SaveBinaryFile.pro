TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

INCLUDEPATH += . \
			src 

# Input
HEADERS = src/*.h 

SOURCES = src/*.cpp  

RESOURCES += src/SaveBinaryFile.qrc	
		
TARGET        = SaveBinaryFile

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/SaveBinaryFile	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/SaveBinaryFile
}

