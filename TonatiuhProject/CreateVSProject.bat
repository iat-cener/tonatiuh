SET TONATIUH_ROOT=%CD%
SET COMPILER=MSVC


REM TODO before run this script:
REM      Define Env variable COINDIR_4VS = C:\sde\VS\Coin-3.1.3
REM      Define Env variable QMAKESPEC_4VS = win32-msvc2008
REM      Define Env variable QTDIR_4VS = C:\sde\VS\Qt\4.7.3
REM      Define Env variable MARBLEDIR_4VS = C:\sde\VS\Marble

@if NOT "%COINDIR_4VS%"=="" SET COINDIR=%COINDIR_4VS%
@if NOT "%QMAKESPEC_4VS%"=="" SET QMAKESPEC=%QMAKESPEC_4VS%
@if NOT "%QTDIR_4VS%"=="" SET QTDIR=%QTDIR_4VS%
@if NOT "%MARBLEDIR_4VS%"=="" SET MARBLEDIR=%MARBLEDIR_4VS%

@if "%COINDIR%"=="" goto error_no_COINDIR 
@if "%QMAKESPEC%"=="" goto error_no_QMAKESPEC 
@if "%QTDIR%"=="" goto error_no_QTDIR 
@if "%MARBLEDIR%"=="" goto error_no_MARBLEDIR

@SET PATH=%PATH%;%QTDIR%\bin;%COINDIR%\bin;%MARBLEDIR%

cd %TONATIUH_ROOT%
qmake -r -tp vc -spec %QMAKESPEC%


@goto end

:error_no_COINDIR
@echo ERROR: COINDIR_4VS variable is not set. 
@goto end

:error_no_QTDIR
@echo ERROR:  QTDIR_4VS variable is not set. 
@goto end

:error_no_QMAKESPEC
@echo ERROR: QMAKESPEC_4VS variable is not set. 
@goto end

:error_no_MARBLEDIR
@echo ERROR: MARBLEDIR_4VS variable is not set. 
@goto end


:end

pause
