TEMPLATE = lib
CONFIG       += qt warn_on thread debug_and_release

CONFIG -= c++11
QMAKE_CXXFLAGS += -std=gnu++17
QMAKE_LFLAGS += -Wl,--export-all-symbols


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

QT +=core	
QT -=gui	

TARGET = nodes

DEFINES += NODELIBRARY_EXPORTS   

INCLUDEPATH += 	. \
               $$(TONATIUH_ROOT)/src/geometry \
               $$(TONATIUH_ROOT)/src/nodes \
               $$(TONATIUH_ROOT)/src/statistics


CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry  
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry
}

# Input
HEADERS += *.h 

SOURCES += *.cpp 
          
 

CONFIG(debug, debug|release) {
	DESTDIR = $$(TONATIUH_ROOT)/bin/debug
}
else{
	DESTDIR= $$(TONATIUH_ROOT)/bin/release
}

