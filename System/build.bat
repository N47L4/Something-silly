@echo off
if exist inventory.exe del /Q inventory.exe
if exist main.obj del /Q main.obj

g++ main.cpp -o inventory.exe -std=c++17 -lws2_32
if %errorlevel% neq 0 (
    echo Build failed.
    pause
    exit /b %errorlevel%
)

echo Build succeeded: inventory.exe
