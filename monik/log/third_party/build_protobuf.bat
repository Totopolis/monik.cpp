rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
set SCRIPTS_DIR=%CD%
if not exist install (
mkdir install
)
if not exist protobuf (
git clone -b v3.2.0 https://github.com/google/protobuf.git
)
cd protobuf
cd cmake
mkdir build
cd build
cmake -G "Visual Studio 14 Win64" -Dprotobuf_MSVC_STATIC_RUNTIME=OFF -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=%SCRIPTS_DIR%\install ..
cmake --build . --target install --config Release
cmake --build . --target install --config Debug
cd %SCRIPTS_DIR%