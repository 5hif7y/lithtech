@echo off

setlocal

rem Parameters:
rem    "/r" - Rebuild or "/c" - clean

echo - Building Release Samples

echo - Building the audio samples...
call _build_solution audio\music\music.sln release %1
call _build_solution audio\sounds\sounds.sln release %1


echo - Building the base samples...
call _build_solution base\mpflycam\mpflycam.sln release %1
call _build_solution base\samplebase\samplebase.sln release %1
call _build_solution base\simplephys\simplephys.sln release %1


echo - Building the debugging samples...
call _build_solution debugging\stacktrace\stacktrace.sln release %1


echo - Building the graphics samples...
call _build_solution graphics\bump\bump.sln release %1
call _build_solution graphics\clientfx\clientfx.sln release %1
call _build_solution graphics\drawprim\drawprim.sln release %1
call _build_solution graphics\effects\effects.sln release %1
call _build_solution graphics\fonts\fonts.sln release %1
call _build_solution graphics\GuiMgr\GuiMgr.sln release %1
call _build_solution graphics\renderdemo\renderdemo.sln release %1
call _build_solution graphics\shaders\shaders.sln release %1
call _build_solution graphics\specialeffects1\specialeffects1.sln release %1
call _build_solution graphics\video\video.sln release %1


echo - Building the models samples...
call _build_solution models\animations\animations.sln release %1
call _build_solution models\animations2\animations2.sln release %1
call _build_solution models\attachments\attachments.sln release %1
call _build_solution models\obb\obb.sln release %1


echo - Building the networking samples...
call _build_solution networking\nettest\nettest.sln release %1
call _build_solution networking\sealhunter\sealhunter.sln release %1


echo - Building the objects samples...
call _build_solution objects\doors\doors.sln release %1
call _build_solution objects\pickups\pickups.sln release %1
call _build_solution objects\projectiles\projectiles.sln release %1


endlocal
