
VERSION = 1.1.1

COINDIR = $$(TDE_ROOT)/local
MARBLEDIR = $$(TDE_ROOT)/local
BERKELEYDBDIR = $$(TDE_ROOT)/local

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

INCLUDEPATH += 	. \
				$$(TONATIUH_ROOT)/src \
				$$(TONATIUH_ROOT)/src/source \
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


LIBS += -L$${COINDIR}/lib -lCoin -lSoQt


contains( CONFIG, plugin ){  
	
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
	INCLUDEPATH += $${MARBLEDIR}/include/marble
	LIBS += -L$${MARBLEDIR}/lib -lmarblewidget
}


QMAKE_CLEAN += *.rc *.aps object_script*    