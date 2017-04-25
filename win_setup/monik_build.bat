call setupenv.bat

%DEPLOY_DIR%\bin\protoc --proto_path=%PROJECT_ROOT_DIR%\monik\protobuf --cpp_out=%PROJECT_ROOT_DIR%\monik\protobuf %PROJECT_ROOT_DIR%\monik\protobuf\monik.proto

cd %BUILD_DIR%
mkdir monik_build
cd monik_build
mkdir %TOOLCHAINTAG%
cd %TOOLCHAINTAG%

rem -DRabbitmqc_LIBRARY=librabbitmq.4.lib
%CMAKE% %CMAKEGENERATOR% -DRabbitmqc_LIBRARY=librabbitmq.4.lib -DCMAKE_INSTALL_PREFIX=%DEPLOY_DIR% -DDEPLOY_DIR=%DEPLOY_DIR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% %PROJECT_ROOT_DIR%
%CMAKE% --build . --config %BUILD_TYPE%
cd %SCRIPTS_DIR%