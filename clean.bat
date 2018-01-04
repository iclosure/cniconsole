@echo off

echo clean.bat:         Deleting output

rem ****************************************
rem   Delete working folders
rem ****************************************

cd output
rd /q /s bin
cd ..

cd kwt
rd /q /s lib
cd ..

rem ****************************************
rem   Delete files
rem ****************************************

del /q /s /f *.user

#pause