TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

#QT -=core	
QT -=gui	

INCLUDEPATH += . \
				src  \
               	$$(TONATIUH_ROOT)/src/geometry \
               	$$(TONATIUH_ROOT)/src/plugins \
			    $$(TONATIUH_ROOT)/src/raytracing 

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry$$VER_MAJ -lraytracing$$VER_MAJ
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry$$VER_MAJ -lraytracing$$VER_MAJ
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

