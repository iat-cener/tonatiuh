TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )


INCLUDEPATH += . \
				src \
			$$(TONATIUH_ROOT)/src

# Input
HEADERS = src/*.h \        						
           	$$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.h \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TShape.h \


SOURCES = src/*.cpp  \    						
           	$$(TONATIUH_ROOT)/src/source/raytracing/DifferentialGeometry.cpp \
           	$$(TONATIUH_ROOT)/src/source/raytracing/TShape.cpp 


RESOURCES = src/ShapeTroughCPC.qrc	

LIBS +=-L$$(TDE_ROOT)/local/lib -lCoin -lSoQt
		

CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ShapeTroughCPC
	unix { 
		TARGET = $$member(TARGET, 0)_debug
	}
	else {
		TARGET = $$member(TARGET, 0)d
	}
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ShapeTroughCPC
}

