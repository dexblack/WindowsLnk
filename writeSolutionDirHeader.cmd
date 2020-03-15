@echo off
setlocal EnableExtensions 
set output_dir=%~dp0
set output_file=%output_dir%TestLnk\SolutionDir.h
set output_file_new=%output_dir%TestLnk\SolutionDirNew.h
set output_dir=%output_dir:\=\\%
rem echo %output_dir%
rem echo Writing $(SolutionDir) to %output_file_new%
echo struct TestData { char const* const SolutionDir { "%output_dir%"}; }; > %output_file_new%
echo %output_file_new% created
rem echo Checking for differences
fc /L "%output_file%" "%output_file_new%" >NUL
if ERRORLEVEL 1 goto :CHANGED
rem echo Unchanged
if exist %output_file_new% del %output_file_new%
goto :END
:CHANGED
echo Replacing "%output_file%"
del "%output_file%"
move "%output_file_new%" "%output_file%"
:END
endlocal
