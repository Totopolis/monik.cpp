call setupenv.bat
cd %BUILD_DIR%

if not exist openssl (
git clone -b OpenSSL_1_0_1-stable git://git.openssl.org/openssl.git
)
cd openssl
perl Configure VC-WIN64A --prefix=%DEPLOY_DIR%
call ms\do_win64a.bat
nmake -f ms\nt.mak
nmake -f ms\nt.mak install
cd %SCRIPTS_DIR%