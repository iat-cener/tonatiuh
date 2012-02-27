TEMPLATE = app
CONFIG += console debug_and_release
include( ../config.pri )

QT += xml opengl svg  script network

DEFINES += TEST_DIR=\\\"PWD/../tests\\\"

SOURCES += *.cpp 
           
CONFIG(debug, debug|release) {
    OBJECTS       +=    $$(TONATIUH_ROOT)/debug/BBox.o \
                        $$(TONATIUH_ROOT)/debug/DifferentialGeometry.o \
                        $$(TONATIUH_ROOT)/debug/Document.o \
                        $$(TONATIUH_ROOT)/debug/InstanceNode.o \
                        $$(TONATIUH_ROOT)/debug/Matrix4x4.o \
                        $$(TONATIUH_ROOT)/debug/moc_Document.o \
                        $$(TONATIUH_ROOT)/debug/moc_ParallelRandomDeviate.o \
                        $$(TONATIUH_ROOT)/debug/moc_SceneModel.o \
                        $$(TONATIUH_ROOT)/debug/moc_ScriptRayTracer.o \
                        $$(TONATIUH_ROOT)/debug/NormalVector.o \
                        $$(TONATIUH_ROOT)/debug/ParallelRandomDeviate.o \
                        $$(TONATIUH_ROOT)/debug/PathWrapper.o \
                        $$(TONATIUH_ROOT)/debug/Photon.o \
                        $$(TONATIUH_ROOT)/debug/Point3D.o \
                        $$(TONATIUH_ROOT)/debug/PluginManager.o \
                        $$(TONATIUH_ROOT)/debug/RayTracer.o \
                        $$(TONATIUH_ROOT)/debug/RayTracerNoTr.o \
                        $$(TONATIUH_ROOT)/debug/RayTracerPhoton.o \
                        $$(TONATIUH_ROOT)/debug/RefCount.o \
                        $$(TONATIUH_ROOT)/debug/SceneModel.o \
                        $$(TONATIUH_ROOT)/debug/ScriptRayTracer.o \
                        $$(TONATIUH_ROOT)/debug/sunpos.o \
                        $$(TONATIUH_ROOT)/debug/TAnalyzerKit.o \
                        $$(TONATIUH_ROOT)/debug/TAnalyzerLevel.o \
                        $$(TONATIUH_ROOT)/debug/TAnalyzerResult.o \
                        $$(TONATIUH_ROOT)/debug/TAnalyzerParameter.o \
                        $$(TONATIUH_ROOT)/debug/TAnalyzerResultKit.o \
                        $$(TONATIUH_ROOT)/debug/TCube.o \
                        $$(TONATIUH_ROOT)/debug/TDefaultMaterial.o \
                        $$(TONATIUH_ROOT)/debug/TDefaultSunShape.o \
                        $$(TONATIUH_ROOT)/debug/TDefaultTracker.o \
                        $$(TONATIUH_ROOT)/debug/TDefaultTransmissivity.o \
                        $$(TONATIUH_ROOT)/debug/tgf.o \
                        $$(TONATIUH_ROOT)/debug/TLightKit.o \
                        $$(TONATIUH_ROOT)/debug/TLightShape.o \
                        $$(TONATIUH_ROOT)/debug/TMaterial.o \
                        $$(TONATIUH_ROOT)/debug/tonatiuh_script.o \
                        $$(TONATIUH_ROOT)/debug/Transform.o \
                        $$(TONATIUH_ROOT)/debug/trf.o \
                        $$(TONATIUH_ROOT)/debug/TSceneTracker.o \
                        $$(TONATIUH_ROOT)/debug/TSceneKit.o \
                        $$(TONATIUH_ROOT)/debug/TSeparatorKit.o \
                        $$(TONATIUH_ROOT)/debug/TShape.o \
                        $$(TONATIUH_ROOT)/debug/TShapeKit.o \
                        $$(TONATIUH_ROOT)/debug/TSunShape.o \
                        $$(TONATIUH_ROOT)/debug/TSquare.o \
                        $$(TONATIUH_ROOT)/debug/TTracker.o \
                        $$(TONATIUH_ROOT)/debug/TTrackerForAiming.o \
                        $$(TONATIUH_ROOT)/debug/TTransmissivity.o \
                        $$(TONATIUH_ROOT)/debug/Vector3D.o
}                     
else { 
    OBJECTS       +=    $$(TONATIUH_ROOT)/release/BBox.o \
                        $$(TONATIUH_ROOT)/release/DifferentialGeometry.o \
                        $$(TONATIUH_ROOT)/release/Document.o \
                        $$(TONATIUH_ROOT)/release/InstanceNode.o \
                        $$(TONATIUH_ROOT)/release/Matrix4x4.o \
                        $$(TONATIUH_ROOT)/release/moc_Document.o \
                        $$(TONATIUH_ROOT)/release/moc_ParallelRandomDeviate.o \
                        $$(TONATIUH_ROOT)/release/moc_SceneModel.o \
                        $$(TONATIUH_ROOT)/release/moc_ScriptRayTracer.o \
                        $$(TONATIUH_ROOT)/release/NormalVector.o \
                        $$(TONATIUH_ROOT)/release/ParallelRandomDeviate.o \
                        $$(TONATIUH_ROOT)/release/PathWrapper.o \
                        $$(TONATIUH_ROOT)/release/Photon.o \
                        $$(TONATIUH_ROOT)/release/Point3D.o \
                        $$(TONATIUH_ROOT)/release/PluginManager.o \
                        $$(TONATIUH_ROOT)/release/RayTracer.o \
                        $$(TONATIUH_ROOT)/release/RayTracerNoTr.o \
                        $$(TONATIUH_ROOT)/release/RayTracerPhoton.o \
                        $$(TONATIUH_ROOT)/release/RefCount.o \
                        $$(TONATIUH_ROOT)/release/SceneModel.o \
                        $$(TONATIUH_ROOT)/release/ScriptRayTracer.o \
                        $$(TONATIUH_ROOT)/release/sunpos.o \
                        $$(TONATIUH_ROOT)/release/TAnalyzerKit.o \
                        $$(TONATIUH_ROOT)/release/TAnalyzerLevel.o \
                        $$(TONATIUH_ROOT)/release/TAnalyzerParameter.o \
                        $$(TONATIUH_ROOT)/release/TAnalyzerResult.o \
                        $$(TONATIUH_ROOT)/release/TAnalyzerResultKit.o \
                        $$(TONATIUH_ROOT)/release/TCube.o \
                        $$(TONATIUH_ROOT)/release/TDefaultMaterial.o \
                        $$(TONATIUH_ROOT)/release/TDefaultSunShape.o \
                        $$(TONATIUH_ROOT)/release/TDefaultTracker.o \
                        $$(TONATIUH_ROOT)/release/TDefaultTransmissivity.o \
                        $$(TONATIUH_ROOT)/release/tgf.o \
                        $$(TONATIUH_ROOT)/release/TLightKit.o \
                        $$(TONATIUH_ROOT)/release/TLightShape.o \
                        $$(TONATIUH_ROOT)/release/TMaterial.o \
                        $$(TONATIUH_ROOT)/release/tonatiuh_script.o \
                        $$(TONATIUH_ROOT)/release/Transform.o \
                        $$(TONATIUH_ROOT)/release/trf.o \
                        $$(TONATIUH_ROOT)/release/TSeparatorKit.o \
                        $$(TONATIUH_ROOT)/release/TSceneKit.o \
                        $$(TONATIUH_ROOT)/release/TSceneTracker.o \
                        $$(TONATIUH_ROOT)/release/TShape.o \
                        $$(TONATIUH_ROOT)/release/TShapeKit.o \
                        $$(TONATIUH_ROOT)/release/TSunShape.o \
                        $$(TONATIUH_ROOT)/release/TSquare.o \
                        $$(TONATIUH_ROOT)/release/TTracker.o \
                        $$(TONATIUH_ROOT)/release/TTrackerForAiming.o \
                        $$(TONATIUH_ROOT)/release/TTransmissivity.o \
                        $$(TONATIUH_ROOT)/release/Vector3D.o
}

LIBS += -L$$(TDE_ROOT)/local/lib -lgtest

TARGET = TonatiuhTests

CONFIG(debug, debug|release) {
    DESTDIR = ../bin/debug
}
else{
    DESTDIR=../bin/release
}

tests.target= tests

QMAKE_EXTRA_TARGETS += tests
