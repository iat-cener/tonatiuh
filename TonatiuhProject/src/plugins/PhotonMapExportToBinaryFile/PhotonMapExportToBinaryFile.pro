TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

INCLUDEPATH += . \
			src 

# Input
HEADERS = src/*.h 

SOURCES = src/*.cpp \
               $$(TONATIUH_ROOT)/src/raytracing/Photon.cpp

RESOURCES += src/PhotonMapExportToBinaryFile.qrc	
		
TARGET        = PhotonMapExportToBinaryFile

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/PhotonMapExportToBinaryFile	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/PhotonMapExportToBinaryFile
}

