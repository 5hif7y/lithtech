@echo off

setlocal

rem Parameters:
rem    solutionname
rem    solutionconfig (debug, release, demo, tune)
rem    "/r" - Rebuild or "/c" - clean

set BuildType=/build

if "%3" == "/R" set BuildType=/rebuild
if "%3" == "/r" set BuildType=/rebuild
if "%3" == "/C" set BuildType=/clean
if "%3" == "/c" set BuildType=/clean

if "%3" == "/clean" set BuildType=/clean
if "%3" == "/rebuild" set BuildType=/rebuild

if "%GAME_TOOLS_DIR%" == "" goto notool

rem Set command line paths options etc for 

call %GAME_TOOLS_DIR%\vcvars.exe 2003 > vcvars.bat
call vcvars.bat

@echo on
devenv %1 %BuildType% %2 >> build.log
@echo off
if errorlevel 1 echo -    Found errors

goto done

:notool
@echo -
@echo - Jupiter game tools dir not defined. ex. "Set GAME_TOOLS_DIR = e:\Jupiter\tools"
@echo -
:done


endlocal
