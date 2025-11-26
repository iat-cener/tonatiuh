TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

				
INCLUDEPATH += . \
				src \
                $$(TONATIUH_ROOT)/plugins \
				$$(TONATIUH_ROOT)/src 

# Input
HEADERS = src/*.h   \
			$$(TONATIUH_ROOT)/src/source/gui/InstanceNode.h \
			$$(TONATIUH_ROOT)/src/source/gui/PhotonMapExport.h \
            $$(TONATIUH_ROOT)/src/source/gui/PhotonMapExportParametersWidget.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.h  \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShapeKit.h 

SOURCES = src/*.cpp  \
			$$(TONATIUH_ROOT)/src/source/gui/InstanceNode.cpp \
			$$(TONATIUH_ROOT)/src/source/gui/PhotonMapExport.cpp \
            $$(TONATIUH_ROOT)/src/source/gui/PhotonMapExportParametersWidget.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp  \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShapeKit.cpp 

RESOURCES += src/PhotonMapExportFile.qrc

FORMS += src/*.ui

TARGET        = PhotonMapExportFile
 
CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/PhotonMapExportFile	
	unix {
		TARGET = $$member(TARGET, 0)_debug
	}
	else {
		TARGET = $$member(TARGET, 0)d
	}
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/PhotonMapExportFile
}