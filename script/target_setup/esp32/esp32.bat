
@echo off
@REM This is generated file, do not edit it manually
call "C:\Users\bico\esp\v5.4.1\esp-idf\export.bat"
idf.py set-target esp32
idf.py fullclean
idf.py build
idf.py flash
