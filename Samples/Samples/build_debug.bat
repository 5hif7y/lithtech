@echo off

setlocal

rem Parameters:
rem    "/r" - Rebuild or "/c" - clean

echo - Building Debug Samples

echo - Building the audio samples...
call _build_solution audio\music\music.sln debug %1
call _build_solution audio\sounds\sounds.sln debug %1


echo - Building the base samples...
call _build_solution base\mpflycam\mpflycam.sln debug %1
call _build_solution base\samplebase\samplebase.sln debug %1
call _build_solution base\simplephys\simplephys.sln debug %1


echo - Building the debug samples...
call _build_solution debugging\stacktrace\stacktrace.sln debug %1


echo - Building the graphics samples...
call _build_solution graphics\bump\bump.sln debug %1
call _build_solution graphics\clientfx\clientfx.sln debug %1
call _build_solution graphics\drawprim\drawprim.sln debug %1
call _build_solution graphics\effects\effects.sln debug %1
call _build_solution graphics\fonts\fonts.sln debug %1
call _build_solution graphics\GuiMgr\GuiMgr.sln debug %1
call _build_solution graphics\renderdemo\renderdemo.sln debug %1
call _build_solution graphics\shaders\shaders.sln debug %1
call _build_solution graphics\specialeffects1\specialeffects1.sln debug %1
call _build_solution graphics\video\video.sln debug %1


echo - Building the models samples...
call _build_solution models\animations\animations.sln debug %1
call _build_solution models\animations2\animations2.sln debug %1
call _build_solution models\attachments\attachments.sln debug %1
call _build_solution models\obb\obb.sln debug %1


echo - Building the networking samples...
call _build_solution networking\nettest\nettest.sln debug %1
call _build_solution networking\sealhunter\sealhunter.sln debug %1


echo - Building the objects samples...
call _build_solution objects\doors\doors.sln debug %1
call _build_solution objects\pickups\pickups.sln debug %1
call _build_solution objects\projectiles\projectiles.sln debug %1


endlocal
