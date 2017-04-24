rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
set SCRIPTS_DIR=%CD%
if not exist install (
mkdir install
)
if not exist rabbitmq-c (
git clone https://github.com/alanxz/rabbitmq-c
)
cd rabbitmq-c
mkdir build
cd build
cmake -G "Visual Studio 14 Win64" -DBUILD_EXAMPLES=OFF -DBUILD_TOOLS=OFF -DBUILD_TOOLS_DOCS=OFF -DBUILD_TESTS=OFF -DBUILD_API_DOCS=OFF -DENABLE_SSL_SUPPORT=OFF -DCMAKE_INSTALL_PREFIX=%SCRIPTS_DIR%\install ..
cmake --build . --target install
cd %SCRIPTS_DIR%