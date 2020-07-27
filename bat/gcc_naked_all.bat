@echo off
set PATH=%DEV_ROOT%\GCC\bin;%PATH%
set C_INCLUDE_PATH=%DEV_ROOT%\GCC\%1\include
set CPLUS_INCLUDE_PATH=%DEV_ROOT%\GCC\%1\include
if [%2] == [] (
    set LIBRARY_PATH=%DEV_ROOT%\GCC\%1\lib
) else (
    set LIBRARY_PATH=%DEV_ROOT%\GCC\%1\lib\%2
)
