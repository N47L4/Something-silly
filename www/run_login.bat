@echo off
cd /d "%~dp0"
ng++ login.cpp -o login.exe
nif %ERRORLEVEL% neq 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)
necho Compilation succeeded.
necho Running login.exe...
.\login.exe
pause
