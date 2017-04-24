set SCRIPTS_DIR=%CD%
%SCRIPTS_DIR%\..\third_party\install\bin\protoc --proto_path=%SCRIPTS_DIR% --cpp_out=%SCRIPTS_DIR% %SCRIPTS_DIR%\monik.proto