@SET TONATIUH_ROOT=C:\sde\VS\workspace\TonatiuhProject
@SET COMPILER=MSVC



REM TODO before run this script:
REM      Define Env variable COINDIR = C:\sde\VS\Coin-3.1.3
REM      Define Env variable QMAKESPEC = win32-msvc2008
REM      Define Env variable QTDIR= C:\sde\VS\Qt\4.7.3
REM      Add in path %QTDIR%\bin;%COINDIR%\bin;

@if "%COINDIR%"=="" goto error_no_COINDIR 
@if "%QMAKESPEC%"=="" goto error_no_QMAKESPEC 
@if "%QTDIR%"=="" goto error_no_QTDIR 


cd %TONATIUH_ROOT%
qmake -r -tp vc -spec win32-msvc2008


@goto end

:error_no_COINDIR
@echo ERROR: COINDIR variable is not set. 
@goto end

:error_no_QTDIR
@echo ERROR:  QTDIR variable is not set. 
@goto end

:error_no_QMAKESPEC
@echo ERROR: QMAKESPEC variable is not set. 
@goto end


:end

pause
