#!/usr/bin/env sh
export SCRIPTS_DIR=$(pwd)
if [ ! -d install ]; then
mkdir install
fi
if [ ! -d rabbitmq-c ]; then
git clone https://github.com/alanxz/rabbitmq-c
fi
cd rabbitmq-c
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=${SCRIPTS_DIR}/install ..
cmake --build . --target install
#cmake ..
#make
#sudo make install
cd ${SCRIPTS_DIR}
