TEMPLATE      = lib
CONFIG       += plugin debug_and_release


win32{
	DEFINES+= COIN_DLL SOQT_DLL
	INCLUDEPATH += . \
				src \
				../Tonatiuh/src\ 
				C:/msys/usr/local/include 
				
	LIBS += -L"c:\msys\usr\local\lib" -lmingw32 -lqtmain  -lSoQt -lCoin -lopengl32
	DESTDIR =  ../Tonatiuh/plugins/TrackerHorizontal/
}

unix{
	INCLUDEPATH += 	. \
					../Tonatiuh/src\ 
					src
	LIBS +=-L/usr/local/lib -lCoin -lSoQt
	DESTDIR       = ../Tonatiuh/plugins/TrackerHorizontal
}

# Input
HEADERS = *.h \
			../Tonatiuh/src/BBox.h \
           	../Tonatiuh/src/DifferentialGeometry.h \
			../Tonatiuh/src/InstanceNode.h \
           	../Tonatiuh/src/Matrix4x4.h \
       		../Tonatiuh/src/MersenneTwister.h \
       		../Tonatiuh/src/NormalVector.h \
			../Tonatiuh/src/Point3D.h\
			../Tonatiuh/src/Photon.h\
          	../Tonatiuh/src/Ray.h \
           	../Tonatiuh/src/RefCount.h \
			../Tonatiuh/src/TDefaultPhotonMap.h\
			../Tonatiuh/src/tgf.h\
           	../Tonatiuh/src/TMaterial.h \
			../Tonatiuh/src/Trace.h\
			../Tonatiuh/src/Transform.h\
			../Tonatiuh/src/TPhotonMap.h\
			../Tonatiuh/src/TSeparatorKit.h\
           	../Tonatiuh/src/TShape.h \
           	../Tonatiuh/src/TShapeKit.h \
			../Tonatiuh/src/TTracker.h\
			../Tonatiuh/src/TTrackerFactory.h \			
			../Tonatiuh/src/Vector3D.h


SOURCES = *.cpp \
			../Tonatiuh/src/BBox.cpp \
           	../Tonatiuh/src/DifferentialGeometry.cpp \
			../Tonatiuh/src/InstanceNode.cpp \
           	../Tonatiuh/src/Matrix4x4.cpp \
       		../Tonatiuh/src/MersenneTwister.cpp \
       		../Tonatiuh/src/NormalVector.cpp \
			../Tonatiuh/src/Point3D.cpp\
			../Tonatiuh/src/Photon.cpp\
          	../Tonatiuh/src/Ray.cpp \
           	../Tonatiuh/src/RefCount.cpp \
			../Tonatiuh/src/TDefaultPhotonMap.cpp\
			../Tonatiuh/src/tgf.cpp \
           	../Tonatiuh/src/TMaterial.cpp \
			../Tonatiuh/src/TPhotonMap.cpp\
			../Tonatiuh/src/Trace.cpp\
			../Tonatiuh/src/Transform.cpp\
			../Tonatiuh/src/TSeparatorKit.cpp\
           	../Tonatiuh/src/TShapeKit.cpp \
           	../Tonatiuh/src/TShape.cpp \
			../Tonatiuh/src/TTracker.cpp\
			../Tonatiuh/src/Vector3D.cpp

RESOURCES += TrackerHorizontal.qrc
	
TARGET        = TrackerHorizontal
    contains(TEMPLATE,lib) {
    
       CONFIG(debug, debug|release) {
       
          unix	{
          
          	DESTDIR       = ../Tonatiuh/plugins/TrackerHorizontal
          	TARGET = $$member(TARGET, 0)_debug
          	
          }
          
          else:TARGET = $$member(TARGET, 0)d
       }
       
    }