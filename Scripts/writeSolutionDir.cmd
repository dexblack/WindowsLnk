@echo off
set status=0
setlocal EnableExtensions 
for %%i in ("%~dp0..") do set solution_dir=%%~fi\
echo solution_dir %solution_dir%
set output_file_prefix=%solution_dir%TestLnk\SolutionDir
set output_file=%output_file_prefix%.cpp
set output_file_new=%output_file_prefix%New.cpp
set solution_dir=%solution_dir:\=\\%
rem echo %solution_dir%
rem echo Writing $(SolutionDir) to %output_file_new%
echo #include "pch.h" > %output_file_new%
echo #include "SolutionDir.h" >> %output_file_new%
echo. >> %output_file_new%
echo const std::string TestData::SolutionDir { "%solution_dir%"}; >> %output_file_new%
rem echo %output_file_new% created
rem echo Checking for differences
fc /L "%output_file%" "%output_file_new%" >NUL
if ERRORLEVEL 1 goto :CHANGED
rem echo Unchanged
if exist %output_file_new% del %output_file_new% >NUL
goto :END
:CHANGED
set status=1
rem echo Replacing "%output_file%"
del "%output_file%" >NUL
move "%output_file_new%" "%output_file%" >NUL
:END
rem exit /b %status%
endlocal
