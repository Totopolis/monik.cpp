#!/usr/bin/env sh

. ./setup_env.sh
cd ${BUILD_DIR}

if [ ! -d rabbitmq-c ]; then
git clone https://github.com/alanxz/rabbitmq-c
fi
cd rabbitmq-c
mkdir build
cd build
mkdir ${CMAKE_BUILD_TYPE}
cd ${CMAKE_BUILD_TYPE}

cmake -DBUILD_EXAMPLES=OFF -DBUILD_TOOLS=OFF -DBUILD_TOOLS_DOCS=OFF -DBUILD_TESTS=OFF -DBUILD_API_DOCS=OFF -DENABLE_SSL_SUPPORT=OFF -DCMAKE_INSTALL_PREFIX=${DEPLOY_DIR} ../..
cmake --build . --target install --config ${CMAKE_BUILD_TYPE}
cd ${SCRIPTS_DIR}
