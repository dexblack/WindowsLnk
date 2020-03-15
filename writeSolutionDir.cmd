@echo off
setlocal EnableExtensions 
set output_dir=%~dp0
set output_file_prefix=%output_dir%TestLnk\SolutionDir
set output_file=%output_file_prefix%.cpp
set output_file_new=%output_file_prefix%New.cpp
set output_dir=%output_dir:\=\\%
rem echo %output_dir%
rem echo Writing $(SolutionDir) to %output_file_new%
echo #include "pch.h" > %output_file_new%
echo #include "SolutionDir.h" >> %output_file_new%
echo. >> %output_file_new%
echo const std::string TestData::SolutionDir { "%output_dir%"}; >> %output_file_new%
rem echo %output_file_new% created
rem echo Checking for differences
fc /L "%output_file%" "%output_file_new%" >NUL
if ERRORLEVEL 1 goto :CHANGED
rem echo Unchanged
if exist %output_file_new% del %output_file_new% >NUL
goto :END
:CHANGED
echo Replacing "%output_file%"
del "%output_file%" >NUL
move "%output_file_new%" "%output_file%" >NUL
:END
endlocal
