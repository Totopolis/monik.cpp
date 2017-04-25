call setupenv.bat
cd %BUILD_DIR%

if not exist rabbitmq-c (
git clone https://github.com/alanxz/rabbitmq-c
)
cd rabbitmq-c
mkdir build
cd build
mkdir %TOOLCHAINTAG%
cd  %TOOLCHAINTAG%

%CMAKE% %CMAKEGENERATOR% -DBUILD_EXAMPLES=OFF -DBUILD_TOOLS=OFF -DBUILD_TOOLS_DOCS=OFF -DBUILD_TESTS=OFF -DBUILD_API_DOCS=OFF -DENABLE_SSL_SUPPORT=OFF -DCMAKE_INSTALL_PREFIX=%DEPLOY_DIR% -DBUILD_STATIC_LIBS=ON ..\..
%CMAKE% --build . --target install --config %BUILD_TYPE%
cd %SCRIPTS_DIR%
