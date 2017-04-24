call setupenv.bat
cd %BUILD_DIR%

if not exist openssl (
git clone -b OpenSSL_1_1_0-stable git://git.openssl.org/openssl.git
)
cd openssl
perl Configure VC-WIN64A
nmake
nmake install DESTDIR=%DEPLOY_DIR%
cd %SCRIPTS_DIR%
