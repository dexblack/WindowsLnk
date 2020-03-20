@echo off
call writeSolutionDir.cmd
if ERRORLEVEL 1 goto :CHANGED
echo Already run.
goto :END
:CHANGED
echo Changed SourceDir.cpp
call writeSolutionDir.cmd
if ERRORLEVEL 1 goto :FAILED
echo Already run.
goto :END
:FAILED
echo Second run failed?
:END
