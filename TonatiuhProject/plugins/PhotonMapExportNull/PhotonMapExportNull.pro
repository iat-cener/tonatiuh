TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

				
INCLUDEPATH += . \
				src \
                $$(TONATIUH_ROOT)/plugins \
				$$(TONATIUH_ROOT)/src 

# Input
HEADERS = src/*.h  \
            $$(TONATIUH_ROOT)/src/source/geometry/*.h \  
            $$(TONATIUH_ROOT)/src/source/gui/InstanceNode.h  \
			$$(TONATIUH_ROOT)/src/source/gui/PathWrapper.h \
            $$(TONATIUH_ROOT)/src/source/gui/PhotonMapExport.h  \
            $$(TONATIUH_ROOT)/src/source/gui/PhotonMapExportParametersWidget.h  \
			$$(TONATIUH_ROOT)/src/source/gui/SceneModel.h \
			$$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/Photon.h \
			$$(TONATIUH_ROOT)/src/source/raytracing/TCube.h \
			$$(TONATIUH_ROOT)/src/source/raytracing/TDefaultMaterial.h\
			$$(TONATIUH_ROOT)/src/source/raytracing/TDefaultSunShape.h \
			$$(TONATIUH_ROOT)/src/source/raytracing/TDefaultTracker.h \
			$$(TONATIUH_ROOT)/src/source/raytracing/TLightKit.h \
			$$(TONATIUH_ROOT)/src/source/raytracing/TLightShape.h \
			$$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSceneTracker.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSeparatorKit.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShapeKit.h \
			$$(TONATIUH_ROOT)/src/source/raytracing/TSunShape.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerForAiming.h

SOURCES = src/*.cpp  \
            $$(TONATIUH_ROOT)/src/source/geometry/*.cpp \  
			$$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
            $$(TONATIUH_ROOT)/src/source/gui/InstanceNode.cpp \
			$$(TONATIUH_ROOT)/src/source/gui/PathWrapper.cpp \
            $$(TONATIUH_ROOT)/src/source/gui/PhotonMapExport.cpp \
            $$(TONATIUH_ROOT)/src/source/gui/PhotonMapExportParametersWidget.cpp \
			$$(TONATIUH_ROOT)/src/source/gui/SceneModel.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/Photon.cpp \
			$$(TONATIUH_ROOT)/src/source/raytracing/TCube.cpp \
			$$(TONATIUH_ROOT)/src/source/raytracing/TDefaultMaterial.cpp \
			$$(TONATIUH_ROOT)/src/source/raytracing/TDefaultSunShape.cpp \
			$$(TONATIUH_ROOT)/src/source/raytracing/TDefaultTracker.cpp \
			$$(TONATIUH_ROOT)/src/source/raytracing/TLightKit.cpp \
			$$(TONATIUH_ROOT)/src/source/raytracing/TLightShape.cpp \
			$$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSceneTracker.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TSeparatorKit.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShapeKit.cpp \
			$$(TONATIUH_ROOT)/src/source/raytracing/TSunShape.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerForAiming.cpp


#RESOURCES += src/PhotonMapExportNull.qrc

#FORMS += src/*.ui

TARGET        = PhotonMapExportNull
 
CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/PhotonMapExportNull	
	unix {
		TARGET = $$member(TARGET, 0)_debug
	}
	else {
		TARGET = $$member(TARGET, 0)d
	}
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/PhotonMapExportNull
}