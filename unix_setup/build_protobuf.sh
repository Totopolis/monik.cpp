#!/usr/bin/env sh

. ./setup_env.sh
cd ${BUILD_DIR}

if [ ! -d protobuf ]; then
git clone -b v3.2.0 https://github.com/google/protobuf.git
fi
cd protobuf

cd cmake
mkdir build
cd build

mkdir ${CMAKE_BUILD_TYPE}
cd ${CMAKE_BUILD_TYPE}

cmake -Dprotobuf_MSVC_STATIC_RUNTIME=OFF -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${DEPLOY_DIR} ${BUILD_DIR}/protobuf/cmake
cmake --build . --target install --config ${CMAKE_BUILD_TYPE}
cd ${SCRIPTS_DIR}
