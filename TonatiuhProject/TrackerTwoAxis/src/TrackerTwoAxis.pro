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
			$$(TONATIUH_ROOT)/src/Trace.h \
			$$(TONATIUH_ROOT)/src/TTracker.h 


SOURCES = *.cpp \
			$$(TONATIUH_ROOT)/src/Trace.cpp  \
			$$(TONATIUH_ROOT)/src/TTracker.cpp

RESOURCES += TrackerTwoAxis.qrc


DESTDIR       = $$(TONATIUH_ROOT)/plugins/TrackerTwoAxis.qrc
TARGET        = TrackerTwoAxis.qrc

contains(TEMPLATE,lib) {  
	CONFIG(debug, debug|release) {
		INCLUDEPATH += $$(TDE_ROOT)/local/debug/include
		DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerTwoAxis	
		unix {
			LIBS +=-L$$(TDE_ROOT)/local/debug/lib -lCoin_debug -lSoQt_debug
			TARGET = $$member(TARGET, 0)_debug
		}
		else {
			LIBS +=-L$$(TDE_ROOT)/local/debug/lib -lCoind -lSoQtd
			TARGET = $$member(TARGET, 0)d
		}
	}
	else { 
		INCLUDEPATH += $$(TDE_ROOT)/local/release/include
		LIBS +=-L$$(TDE_ROOT)/local/release/lib -lCoin -lSoQt
		DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerTwoAxis
	}
}
