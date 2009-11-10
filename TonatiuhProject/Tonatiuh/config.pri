
VERSION = 0.9.2

INCLUDEPATH += 	. \
				src \
				$$(TDE_ROOT)/local \
				$$(TDE_ROOT)/local/include

win32 {
	DEFINES+= COIN_DLL SOQT_DLL
}


contains( CONFIG, plugin ){  
	LIBS +=-L$$(TDE_ROOT)/local/lib -lCoin -lSoQt
}
else{
	INCLUDEPATH += $$(TDE_ROOT)/local/include/marble
	LIBS +=-L$$(TDE_ROOT)/local/lib -lCoin -lSoQt -lmarblewidget
}

