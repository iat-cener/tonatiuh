TEMPLATE = app


CONFIG       += qt warn_on thread debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

TARGET = Tonatiuh   

DEPENDPATH += . \
                $$(TONATIUH_ROOT)/geometry \
                $$(TONATIUH_ROOT)/src \
                $$(TONATIUH_ROOT)/src/source \
                $$(TONATIUH_ROOT)/src/source/application \
                $$(TONATIUH_ROOT)/src/source/auxiliary \
                $$(TONATIUH_ROOT)/src/source/geometry \
                $$(TONATIUH_ROOT)/src/source/gui \
                $$(TONATIUH_ROOT)/src/source/raytracing \
                $$(TONATIUH_ROOT)/src/source/statistics
              
 
              
QT += script 
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent printsupport 
} 

 
win32: {
	RC_FILE = Tonatiuh.rc
}

mac:{
	ICON = icons/Tonatiuh.icns
}


# Input
HEADERS += source/application/*.h \
           source/auxiliary/*.h \
           source/geometry/*.h \
           source/gui/*.h \
           source/raytracing/*.h \
           source/statistics/*.h 
FORMS += source/gui/*.ui
SOURCES += source/application/*.cpp \
           source/auxiliary/*.cpp \
           source/geometry/*.cpp \
           source/gui/*.cpp \
           source/raytracing/*.cpp  \
           source/statistics/*.cpp 
RESOURCES += tonatiuh.qrc
 

CONFIG(debug, debug|release) {
	DESTDIR = $$(TONATIUH_ROOT)/bin/debug
}
else{
	DESTDIR= $$(TONATIUH_ROOT)/bin/release
}
	
QMAKE_CLEAN -= *.rc  

src.target= src
QMAKE_EXTRA_TARGETS += src
