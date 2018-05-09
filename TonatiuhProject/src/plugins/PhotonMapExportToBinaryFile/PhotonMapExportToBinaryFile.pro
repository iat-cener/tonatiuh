TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

QT -=gui	

INCLUDEPATH += . \
				src  \
               	$$(TONATIUH_ROOT)/src/geometry \
               	$$(TONATIUH_ROOT)/src/plugins \
			    $$(TONATIUH_ROOT)/src/raytracing 

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry -lraytracing
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry -lraytracing
}


# Input
HEADERS = src/*.h 

SOURCES = src/*.cpp 

RESOURCES += src/PhotonMapExportToBinaryFile.qrc	
		
TARGET        = PhotonMapExportToBinaryFile

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/PhotonMapExportToBinaryFile	
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/PhotonMapExportToBinaryFile
}

