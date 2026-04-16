@echo off
cd /d "%~dp0"
g++ login.cpp -o login.exe -lws2_32
if %ERRORLEVEL% neq 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilation succeeded.
echo Starting server on http://localhost:8000
.\login.exe
pause
