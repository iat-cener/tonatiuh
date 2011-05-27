call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall"

cd C:\sde\VS\Marble

C:\sde\CMake\bin\cmake -G "NMake Makefiles" -DQTONLY=ON -DCMAKE_INSTALL_PREFIX=%MARBLEDIR% && nmake && nmake install 
C:\sde\CMake\bin\cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DQTONLY=ON -DCMAKE_INSTALL_PREFIX=%MARBLEDIR% && nmake release && nmake release install

mkdir %MARBLEDIR%\lib
copy src\lib\*.lib %MARBLEDIR%\lib\

xcopy /Y src\lib\*.h %MARBLEDIR%\include\marble
xcopy /Y src\lib\blendings\*.h %MARBLEDIR%\include\marble
xcopy /Y  src\lib\geodata\*.h %MARBLEDIR%\include\marble  
xcopy /Y  src\lib\geodata\data\*.h %MARBLEDIR%\include\marble  
xcopy /Y  src\lib\geodata\graphicsitem\*.h %MARBLEDIR%\include\marble  
xcopy /Y  src\lib\geodata\handlers\dgml\*.h %MARBLEDIR%\include\marble  
xcopy /Y  src\lib\geodata\handlers\gpx\*.h %MARBLEDIR%\include\marble  
xcopy /Y  src\lib\geodata\handlers\kml\*.h %MARBLEDIR%\include\marble  
xcopy /Y  src\lib\geodata\handlers\osm-namefinder\*.h %MARBLEDIR%\include\marble  
xcopy /Y  src\lib\geodata\parser\*.h %MARBLEDIR%\include\marble  
xcopy /Y  src\lib\geodata\scene\*.h %MARBLEDIR%\include\marble 
xcopy /Y  src\lib\geodata\writer\*.h %MARBLEDIR%\include\marble   
xcopy /Y  src\lib\geodata\writers\kml\*.h %MARBLEDIR%\include\marble 
xcopy /Y  src\lib\graphicsview\*.h %MARBLEDIR%\include\marble 
xcopy /Y  src\lib\graphicsview\screengraphicsitem\*.h %MARBLEDIR%\include\marble   
xcopy /Y  src\lib\Projections\*.h %MARBLEDIR%\include\marble 
xcopy /Y  src\lib\routing\*.h %MARBLEDIR%\include\marble


pause
