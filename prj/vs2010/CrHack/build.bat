@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
devenv /build Release CrHack.sln
pause
