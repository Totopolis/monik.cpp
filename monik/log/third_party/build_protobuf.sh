#!/usr/bin/env sh
if [ $# -eq 0 ] 
then
   echo "No arguments supplied"
   export CMAKE_BUILD_TYPE=Release
else
   export CMAKE_BUILD_TYPE=$1
fi
echo CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

export SCRIPTS_DIR=$(pwd)
if [ ! -d install ]; then
mkdir install
fi
if [ ! -d protobuf ]; then
git clone -b v3.2.0 https://github.com/google/protobuf.git
fi
cd protobuf
cd cmake
mkdir build
cd build
cmake -Dprotobuf_MSVC_STATIC_RUNTIME=OFF -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${SCRIPTS_DIR}/install ..
cmake --build . --target install --config ${CMAKE_BUILD_TYPE}
cd ${SCRIPTS_DIR}
