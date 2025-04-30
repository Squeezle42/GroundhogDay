@echo off
echo ===== Git Repository Initialization =====

:: Check if git is installed
where git >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Git is not installed or not in your PATH.
    echo Please install Git from https://git-scm.com/
    pause
    exit /b 1
)

:: Check if .git folder already exists
if exist ".git" (
    echo Git repository already initialized.
    echo.
    
    :: Show current status
    echo Current Git Status:
    git status
    
    echo.
    echo If you want to reinitialize, please delete the .git folder first.
    pause
    exit /b 0
)

:: Initialize repository
echo Initializing Git repository...
git init

:: Add all files
echo Adding files to Git...
git add .

:: Initial commit
echo Making initial commit...
git commit -m "Initial commit"

:: Instructions for remote repository
echo.
echo ===== Next Steps =====
echo.
echo 1. Create a repository on GitHub/GitLab/etc.
echo 2. Add the remote repository:
echo.
echo    git remote add origin https://github.com/your-username/groundhog-day-game.git
echo.
echo 3. Push your code:
echo.
echo    git push -u origin main
echo.
echo ===== Repository Initialized Successfully =====

pause
