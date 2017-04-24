call setupenv.bat
cd %BUILD_DIR%

if not exist protobuf (
git clone -b v3.2.0 https://github.com/google/protobuf.git
)
cd protobuf
cd cmake

mkdir build
cd build
mkdir %TOOLCHAINTAG%
cd  %TOOLCHAINTAG%

%CMAKE% %CMAKEGENERATOR% -Dprotobuf_MSVC_STATIC_RUNTIME=OFF -DCMAKE_INSTALL_PREFIX=%DEPLOY_DIR% -Dprotobuf_BUILD_TESTS=OFF %BUILD_DIR%/protobuf/cmake
%CMAKE% --build . --target install --config %BUILD_TYPE%
cd %SCRIPTS_DIR%
