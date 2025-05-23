@echo off
call "c:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
set compilerflags=/Od /Zi /EHsc /std:c++17 /I include
set linkerflags=/OUT:bin\main.exe /DEBUG /INCREMENTAL /DYNAMICBASE 
REM set linkerflags=/OUT:bin\main.exe
cl.exe %compilerflags% src\*.cpp /link %linkerflags%
del bin\*.ilk *.obj *.pdb