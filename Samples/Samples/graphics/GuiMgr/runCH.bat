@echo off
cd rez
copy cresCH.dll cres.dll /Y
cd ..
cd bin
..\..\..\bin\release\lithtech -rez ..\..\..\bin\release\engine.rez -rez ..\rez