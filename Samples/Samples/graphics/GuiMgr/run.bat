@echo off
cd rez
copy cresEN.dll cres.dll /Y
cd ..
cd bin
..\..\..\bin\release\lithtech -rez ..\..\..\bin\release\engine.rez -rez ..\rez