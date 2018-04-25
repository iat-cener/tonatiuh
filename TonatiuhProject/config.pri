VERSION = 3.0.0

CONFIG -= c++11
QMAKE_CXXFLAGS += -std=gnu++17 


# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

INCLUDEPATH += 	. \
			   $$(TONATIUH_ROOT)/src/auxiliary \
               $$(TONATIUH_ROOT)/src/geometry \
               $$(TONATIUH_ROOT)/src/nodes \
               $$(TONATIUH_ROOT)/src/raytracing \
               $$(TONATIUH_ROOT)/src/statistics


CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry -lnodes
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry -lnodes
}

	
CONFIG(debug, debug|release) {
    QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage
    QMAKE_CXXFLAGS +=-fprofile-arcs -ftest-coverage
} 
else{
	macx{
		QMAKE_CFLAGS_RELEASE -= -O2 
    	QMAKE_CFLAGS_RELEASE += -O3 -mfpmath=sse
		QMAKE_CXXFLAGS_RELEASE -= -O2
		QMAKE_CXXFLAGS_RELEASE += -O3 -mfpmath=sse
	}
	else{
   		QMAKE_CFLAGS_RELEASE -= -O2 
    	QMAKE_CFLAGS_RELEASE += -O3 -march=native -mfpmath=sse
		QMAKE_CXXFLAGS_RELEASE -= -O2
		QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -mfpmath=sse
		
	}
	
}

CONFIG(debug, debug|release) {
		OBJECTS_DIR = $$(TONATIUH_ROOT)/debug
		MOC_DIR = $$(TONATIUH_ROOT)/debug
		OBJMOC = $$(TONATIUH_ROOT)/debug
		RCC_DIR = $$(TONATIUH_ROOT)/debug
}
else { 
	OBJECTS_DIR = $$(TONATIUH_ROOT)/release
	MOC_DIR = $$(TONATIUH_ROOT)/release
	OBJMOC = $$(TONATIUH_ROOT)/release
	RCC_DIR  = $$(TONATIUH_ROOT)/release
}



QMAKE_CLEAN += *.rc *.aps object_script*    
QMAKE_DISTCLEAN += -r   $$(TONATIUH_ROOT)/bin/debug/
QMAKE_DISTCLEAN += -r   $$(TONATIUH_ROOT)/bin/release/