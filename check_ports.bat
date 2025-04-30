@echo off
REM This utility script helps check the active ports
echo Checking active ports on 5000 and 5001...
netstat -ano | findstr "5000 5001"
echo.
echo If you see ports in use, here are their process IDs.
echo To kill a process:  taskkill /F /PID [number]
echo.
pause
