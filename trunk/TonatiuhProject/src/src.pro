TEMPLATE = app
VERSION = 1.1.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

CONFIG       += qt warn_on thread debug_and_release

include( ../config.pri )

TARGET = Tonatiuh   

DEPENDPATH += . \
              src \
              src/source \
              src/source/application \
              src/source/auxiliary \
              src/source/geometry \
              src/source/gui \
              src/source/raytracing \
              src/source/statistics
              
 
              
QT += xml opengl svg  script network


 
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
	DESTDIR = ../bin/debug
}
else{
	DESTDIR=../bin/release
}
	
QMAKE_CLEAN -= *.rc  