@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat"
cd /d "C:\Users\yeyebbc\rescale"
echo [Status] Starting compilation...
if not exist .build mkdir .build
if not exist publish mkdir publish
cl.exe /EHsc /O2 rescale.cpp change-dpi.cpp help.cpp /Fe:publish\Rescale.exe /Fo:.build\ /link /SUBSYSTEM:CONSOLE
