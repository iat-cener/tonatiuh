#Tonatiuh verion:  major.minor.patch
VER_MAJ = 3# major.minor.patch
VER_MIN = 0# major.minor.patch
VER_PAT = 0# major.minor.patch


CONFIG -= c++11
QMAKE_CXXFLAGS += -std=gnu++17 



# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VER_MAJ"."$$VER_MIN"."$$VER_PAT\\\"


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