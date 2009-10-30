TEMPLATE      = lib
CONFIG       += plugin debug_and_release

INCLUDEPATH += 	. \
				src \
				../Tonatiuh/src \
				$$(TDE_ROOT)/local/include

win32{
	DEFINES+= COIN_DLL SOQT_DLL
}

# Input
HEADERS = *.h \
			$$(TONATIUH_ROOT)/src/NormalVector.h \
			$$(TONATIUH_ROOT)/src/Trace.h \
			$$(TONATIUH_ROOT)/src/TTracker.h\
			$$(TONATIUH_ROOT)/src/TTrackerFactory.h \
			$$(TONATIUH_ROOT)/src/Vector3D.h 


SOURCES = *.cpp \
			$$(TONATIUH_ROOT)/src/NormalVector.cpp \
			$$(TONATIUH_ROOT)/src/Trace.cpp \
			$$(TONATIUH_ROOT)/src/TTracker.cpp \
			$$(TONATIUH_ROOT)/src/Vector3D.cpp

RESOURCES += TrackerOneAxisNS.qrc


DESTDIR       = $$(TONATIUH_ROOT)/plugins/TrackerOneAxisNS
TARGET        = TrackerOneAxisNS

contains(TEMPLATE,lib) {  
	CONFIG(debug, debug|release) {
		INCLUDEPATH += $$(TDE_ROOT)/local/include
		DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerOneAxisNS	
		unix {
			LIBS +=-L$$(TDE_ROOT)/local/lib -lCoin_debug -lSoQt_debug
			TARGET = $$member(TARGET, 0)_debug
		}
		else {
			LIBS +=-L$$(TDE_ROOT)/local/lib -lCoind -lSoQtd
			TARGET = $$member(TARGET, 0)d
		}
	}
	else { 
		INCLUDEPATH += $$(TDE_ROOT)/local/include
		LIBS +=-L$$(TDE_ROOT)/local/lib -lCoin -lSoQt
		DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerOneAxisNS
	}
}
