@echo off
echo ===== VSCode Terminal Health Check =====
echo.

echo Testing basic terminal functionality:
echo ----------------------------------
echo Command: echo "Hello World"
echo "Hello World"

echo.
echo Testing Node.js:
echo ----------------------------------
echo Command: node --version
node --version

echo.
echo Testing file system access:
echo ----------------------------------
echo Command: dir
dir

echo.
echo Testing npm:
echo ----------------------------------
echo Command: npm --version
npm --version

echo.
echo Testing environment variables:
echo ----------------------------------
echo Command: echo %PATH%
echo %PATH%

echo.
echo If all tests above produced output, your terminal is functioning correctly.
echo If any tests failed, restart VSCode or use reset_terminal.bat
echo.
