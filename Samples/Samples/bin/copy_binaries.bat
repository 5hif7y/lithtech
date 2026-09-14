@echo off
title Copy Jupiter Engine binaries
echo Copy Jupiter Engine binaries
del build.log

echo Copy Jupiter Engine binaries	     >> build.log

mkdir debug
mkdir release

copy ..\..\Engine\built\release\Runtime\Exe_Lithtech\Lithtech.exe release  >> build.log
copy ..\..\Engine\built\release\Runtime\DLL_dx8\SndDrv.dll        release  >> build.log
copy ..\..\Engine\built\release\Runtime\DLL_LTMsg\LTMsg.dll       release  >> build.log
copy ..\..\Engine\built\release\Runtime\DLL_Server\server.dll     release  >> build.log


copy ..\..\Engine\built\debug\Runtime\Exe_Lithtech\Lithtech.exe   debug    >> build.log
copy ..\..\Engine\built\debug\Runtime\Exe_Lithtech\Lithtech.pdb   debug    >> build.log
copy ..\..\Engine\built\debug\Runtime\DLL_dx8\SndDrv.dll          debug    >> build.log
copy ..\..\Engine\built\debug\Runtime\DLL_dx8\SndDrv.pdb          debug    >> build.log
copy ..\..\Engine\built\debug\Runtime\DLL_LTMsg\LTMsg.dll         debug    >> build.log
copy ..\..\Engine\built\debug\Runtime\DLL_LTMsg\LTMsg.pdb         debug    >> build.log
copy ..\..\Engine\built\debug\Runtime\DLL_Server\server.dll       debug    >> build.log
copy ..\..\Engine\built\debug\Runtime\DLL_Server\server.pdb       debug    >> build.log

copy ..\..\Engine\sdk\rez\engine.rez       debug    >> build.log
copy ..\..\Engine\sdk\rez\engine.rez       release  >> build.log

