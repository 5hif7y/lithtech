@echo off

setlocal

rem Parameters:
rem    "/r" - Rebuild or "/c" - clean

echo - Building Samples


call build_debug %1
call build_release %1

endlocal
