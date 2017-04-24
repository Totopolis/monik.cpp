#!/usr/bin/env sh

. ./setup_env.sh

${DEPLOY_DIR}/bin/protoc --proto_path=${PROJECT_ROOT_DIR}/monik/protobuf --cpp_out=${PROJECT_ROOT_DIR}/monik/protobuf ${PROJECT_ROOT_DIR}/monik/protobuf/monik.proto

cd ${BUILD_DIR}
mkdir monik_build
cd monik_build
mkdir ${CMAKE_BUILD_TYPE}
cd ${CMAKE_BUILD_TYPE}

cmake -DCMAKE_INSTALL_PREFIX=${DEPLOY_DIR} -DDEPLOY_DIR=${DEPLOY_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DRabbitmqc_LIBRARY=rabbitmq ${PROJECT_ROOT_DIR}
cmake --build . --config ${CMAKE_BUILD_TYPE}
cd ${SCRIPTS_DIR}