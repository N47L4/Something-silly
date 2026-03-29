@echo off
if not exist inventory.exe (
    echo inventory.exe not found. Run build.bat first.
    pause
    exit /b 1
)

inventory.exe
