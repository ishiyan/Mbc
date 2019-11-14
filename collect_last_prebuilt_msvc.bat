@echo off
set DATETIME=%date:~10,4%-%date:~4,2%-%date:~7,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%

nmake -f ./makefile.msvc.mk DATETIME=%DATETIME% collect_last_prebuilt
