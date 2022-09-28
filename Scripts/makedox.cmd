@echo off
setlocal ENABLEEXTENSIONS
set script_path=%~dp0
rem Check for doxygen
where doxygen >NUL 2>&1
if ERRORLEVEL 1 goto :INSTALL
rem Check for configuration file path on command line
if "%1"=="" goto :ERR_PARAM
set parentdir=%~dp1
set cfgname=%~nx1
rem Check configuration file exists
if not exist %1 goto :NO_FILE
rem echo parentdir='%parentdir%'
rem echo cfgname='%cfgname%'
call :SET_PROJECTDIR %parentdir:~0,-1%
echo projectdir='%projectdir%'
if not exist %projectdir%\dox\%cfgname% goto :BAD_LAYOUT
pushd %projectdir%
doxygen dox\%cfgname%
popd
goto :END
:BAD_LAYOUT
echo Expected %projectdir%\dox\%cfgname%
echo Received %1
goto :END
:NO_FILE
echo Configuration file not found %1.
goto :END
:ERR_PARAM
echo Missing Configuration file path.
echo Expecting argument of the form %0 %script_path%..\WindowsLnk\dox\dox.cfg
goto :END
:INSTALL doxygen
echo doxygen not found.
echo Please install or configure PATH correctly.
goto :END
:SET_PROJECTDIR
set projectdir=%~dp1
goto :EOF
:END
endlocal
