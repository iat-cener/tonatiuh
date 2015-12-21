VERSION = 2.1.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

COMPILER = $$(COMPILER)
contains( COMPILER, MSVC ) {
	COINDIR = $$(COINDIR)
}

isEmpty( COINDIR ) {
	COINDIR = $$(TDE_ROOT)/local
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
} 

INCLUDEPATH += 	. \
                $$(TONATIUH_ROOT)/fields \
                $$(TONATIUH_ROOT)/geometry \
				$$(TONATIUH_ROOT)/src \
				$$(TONATIUH_ROOT)/src/source \
                $$(TONATIUH_ROOT)/src/source/analyzer \
                $$(TONATIUH_ROOT)/src/source/application \
                $$(TONATIUH_ROOT)/src/source/auxiliary \
                $$(TONATIUH_ROOT)/src/source/geometry \
                $$(TONATIUH_ROOT)/src/source/gui \
                $$(TONATIUH_ROOT)/src/source/raytracing \
                $$(TONATIUH_ROOT)/src/source/statistics \
			    $${COINDIR}/include 

win32 {
	DEFINES+= COIN_DLL SOQT_DLL


}

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry -lfields
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry -lfields
}

COMPILER = $$(COMPILER)
contains( COMPILER, MSVC ) {
	CONFIG(debug, debug|release) {
        LIBS += -L$${COINDIR}/lib -lcoin3d -lSoQt1d
	}
	else{
		LIBS += -L$${COINDIR}/lib -lCoin -lSoQt
	}
}
else {
	LIBS += -L$${COINDIR}/lib -lCoin -lSoQt

}


QMAKE_CXXFLAGS += -std=c++11
		 
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

contains( CONFIG, plugin ){  
	
	CONFIG(debug, debug|release) {
		OBJECTS_DIR = $$(TONATIUH_ROOT)/debug/plugins
		MOC_DIR = $$(TONATIUH_ROOT)/debug/plugins
		OBJMOC = $$(TONATIUH_ROOT)/debug/plugins
		RCC_DIR = $$(TONATIUH_ROOT)/debug/plugins
	}
	else { 
		OBJECTS_DIR = $$(TONATIUH_ROOT)/release/plugins
		MOC_DIR = $$(TONATIUH_ROOT)/release/plugins
		OBJMOC = $$(TONATIUH_ROOT)/release/plugins
		RCC_DIR  = $$(TONATIUH_ROOT)/release/plugins
	}

	QMAKE_CLAGS+= -fPIC
	QMAKE_CXXLAGS+= -fPIC
	INCLUDEPATH += $$(TONATIUH_ROOT)/plugin

	CONFIG(debug, debug|release) {	
		unix {
			TARGET = $$member(TARGET, 0)_debug
		}
		else {
			TARGET = $$member(TARGET, 0)d
		}
	}
}
else{
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

}


QMAKE_CLEAN += *.rc *.aps object_script*    
QMAKE_DISTCLEAN += -r   $$(TONATIUH_ROOT)/bin/debug/
QMAKE_DISTCLEAN += -r   $$(TONATIUH_ROOT)/bin/release/