call setupenv.bat

mkdir %BUILD_DIR%

rem force reinstall every time by recreating deploy directory
del /S /Q %DEPLOY_DIR%
rmdir /s /q %DEPLOY_DIR%

mkdir %DEPLOY_DIR_BASE%
mkdir %DEPLOY_DIR%

cd %SCRIPTS_DIR%
call deploy_dependencies.bat
cd %SCRIPTS_DIR%
call monik_build.bat

