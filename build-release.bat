@echo off
call "c:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
@REM set compilerflags=/O2 /EHsc /std:c++latest /I include
set compilerflags=/O2 /GL /EHsc /std:c++latest /I include
set linkerflags=/OUT:bin\main.exe /OPT:REF /OPT:ICF /LTCG /INCREMENTAL:NO /DYNAMICBASE
@REM set linkerflags=/OUT:bin\main.exe

cl.exe %compilerflags% src\*.cpp /link %linkerflags%
del bin\*.ilk *.obj *.pdb