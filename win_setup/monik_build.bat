call setupenv.bat

%DEPLOY_DIR%\bin\protoc --proto_path=%PROJECT_ROOT_DIR%\monik\protobuf --cpp_out=%PROJECT_ROOT_DIR%\monik\protobuf %PROJECT_ROOT_DIR%\monik\protobuf\monik.proto

cd %SCRIPTS_DIR%