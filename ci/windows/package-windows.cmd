call "%~dp0..\ci_includes.generated.cmd"

mkdir package
cd package

REM git rev-parse --short HEAD > package-version.txt
REM set /p PackageVersion=<package-version.txt
REM del package-version.txt
set PackageVersion=1.0.0

REM Package ZIP archive
7z a "%PluginName%-%PackageVersion%-Windows.zip" "..\release\*"

REM Build installer
iscc ..\installer\installer-Windows.generated.iss /O. /F"%PluginName%-%PackageVersion%-Windows-Installer"
