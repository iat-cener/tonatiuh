TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri)


INCLUDEPATH += . \
				src \
			$$(TONATIUH_ROOT)/src

# Input
HEADERS = src/*.h \          
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/trt.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.h \ 
            $$(TONATIUH_ROOT)/src/source/raytracing/TShapeKit.h


SOURCES = src/*.cpp  \   
            $$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TMaterial.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp \ 
            $$(TONATIUH_ROOT)/src/source/raytracing/TShapeKit.cpp



RESOURCES = src/ShapeTroughAsymmetricCPC.qrc	

LIBS +=-L$$(TDE_ROOT)/local/lib -lCoin -lSoQt
		
contains(TEMPLATE,lib) {  
	CONFIG(debug, debug|release) {
		DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeTroughAsymmetricCPC
		unix { 
			TARGET = $$member(TARGET, 0)_debug
		}
		else {
			TARGET = $$member(TARGET, 0)d
		}
	}
	else { 
		DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeTroughAsymmetricCPC
	}
}
