TEMPLATE = app

include( $$(TONATIUH_ROOT)/config.pri )

TARGET = Tonatiuh   

              
INCLUDEPATH += 	. \
               $$(TONATIUH_ROOT)/src/geometry \
               $$(TONATIUH_ROOT)/src/nodes \
               $$(TONATIUH_ROOT)/src/raytracing \
               $$(TONATIUH_ROOT)/src/statistics
              
QT += core xml	 
QT -= gui	

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent printsupport 
} 



# Input
HEADERS += $$(TONATIUH_ROOT)/src/source/application/*.h \
			$$(TONATIUH_ROOT)/src/raytracing/*.h \
			$$(TONATIUH_ROOT)/src/statistics/*.h
                
#FORMS += src/source/gui/*.ui

SOURCES +=	$$(TONATIUH_ROOT)/src/source/application/*.cpp  \
			$$(TONATIUH_ROOT)/src/raytracing/*.cpp  \
			$$(TONATIUH_ROOT)/src/statistics/*.cpp

#RESOURCES += tonatiuh.qrc
 

CONFIG(debug, debug|release) {
	DESTDIR = $$(TONATIUH_ROOT)/bin/debug
}
else{
	DESTDIR= $$(TONATIUH_ROOT)/bin/release
}
	
QMAKE_CLEAN -= *.rc  

src.target= src
QMAKE_EXTRA_TARGETS += src