
VERSION = 0.9.2

COINDIR = $$(TDE_ROOT)/local
MARBLEDIR = $$(TDE_ROOT)/local
BERKELEYDBDIR = $$(TDE_ROOT)/local

INCLUDEPATH += 	. \
				src \
				$${COINDIR}/include 

win32 {
	DEFINES+= COIN_DLL SOQT_DLL
}


LIBS += -L$${COINDIR}/lib -lCoin -lSoQt

contains( CONFIG, plugin ){  

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

