@echo off
REM Start the Time Loop Game in Chrome App Mode
echo Starting Time Loop Game...

REM Get the current directory
set CURRENT_DIR=%~dp0

REM Define Chrome path (adjust as needed for your system)
set CHROME="C:\Program Files\Google\Chrome\Application\chrome.exe"
set CHROME_CANARY="C:\Program Files\Google\Chrome SxS\Application\chrome.exe"
set EDGE="C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe"

REM Try to find which browser is available
if exist %CHROME% (
    echo Using Chrome
    set BROWSER=%CHROME%
) else if exist %CHROME_CANARY% (
    echo Using Chrome Canary
    set BROWSER=%CHROME_CANARY%
) else if exist %EDGE% (
    echo Using Microsoft Edge
    set BROWSER=%EDGE%
) else (
    echo Chrome, Chrome Canary or Edge not found in the default location.
    echo Please edit this script to specify the correct browser path.
    pause
    exit /b 1
)

REM Launch in app mode
%BROWSER% --app=file:///%CURRENT_DIR%index.html --window-size=1280,720

echo App started!
