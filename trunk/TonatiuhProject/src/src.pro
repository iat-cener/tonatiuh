TEMPLATE = app


CONFIG       += qt warn_on thread debug_and_release

include( ../config.pri )

TARGET = Tonatiuh   

DEPENDPATH += . \
                $$(TONATIUH_ROOT)/geometry \
                $$(TONATIUH_ROOT)/src \
                $$(TONATIUH_ROOT)/src/source \
                $$(TONATIUH_ROOT)/src/source/analyzer \
                $$(TONATIUH_ROOT)/src/source/application \
                $$(TONATIUH_ROOT)/src/source/auxiliary \
                $$(TONATIUH_ROOT)/src/source/geometry \
                $$(TONATIUH_ROOT)/src/source/gui \
                $$(TONATIUH_ROOT)/src/source/raytracing \
                $$(TONATIUH_ROOT)/src/source/statistics
              
 
              
QT += xml opengl svg  script network


 
win32: {
	RC_FILE = Tonatiuh.rc
}

mac:{
	ICON = icons/Tonatiuh.icns
}

# Input
HEADERS += source/analyzer/*.h \
           source/application/*.h \
           source/auxiliary/*.h \
           source/geometry/*.h \
           source/gui/*.h \
           source/raytracing/*.h \
           source/statistics/*.h 
FORMS += source/gui/*.ui
SOURCES += source/analyzer/*.cpp \
           source/application/*.cpp \
           source/auxiliary/*.cpp \
           source/geometry/*.cpp \
           source/gui/*.cpp \
           source/raytracing/*.cpp  \
           source/statistics/*.cpp 
RESOURCES += tonatiuh.qrc
 

CONFIG(debug, debug|release) {
	DESTDIR = ../bin/debug
}
else{
	DESTDIR=../bin/release
}
	
QMAKE_CLEAN -= *.rc  

src.target= src
QMAKE_EXTRA_TARGETS += src
