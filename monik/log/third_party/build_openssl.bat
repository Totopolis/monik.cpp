call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
set SCRIPTS_DIR=%CD%
if not exist install (
mkdir install
)
if not exist openssl (
git clone -b OpenSSL_1_1_0-stable git://git.openssl.org/openssl.git
)
cd openssl
perl Configure VC-WIN64A
nmake
echo %SCRIPTS_DIR%\install
nmake install DESTDIR=%SCRIPTS_DIR%\install
cd %SCRIPTS_DIR%