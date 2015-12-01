TEMPLATE = lib
CONFIG       += debug_and_release

CONFIG       += qt warn_on thread debug_and_release
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
DEPENDPATH += . \
                $$(TONATIUH_ROOT)


win32 {
	DEFINES+= COIN_DLL SOQT_DLL
}

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry 
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry
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


TARGET = fields   

# Input
HEADERS += *.h \

SOURCES += *.cpp 
          
 

CONFIG(debug, debug|release) {
	DESTDIR = $$(TONATIUH_ROOT)/bin/debug
}
else{
	DESTDIR= $$(TONATIUH_ROOT)/bin/release
}


QMAKE_CLEAN += *.rc *.aps object_script*    
QMAKE_DISTCLEAN += -r   $$(TONATIUH_ROOT)/bin/debug/
QMAKE_DISTCLEAN += -r   $$(TONATIUH_ROOT)/bin/release/

