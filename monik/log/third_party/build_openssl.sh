#!/usr/bin/env sh
export SCRIPTS_DIR=$(pwd)
if [ ! -d install ]; then
mkdir install
fi
if [ ! -d openssl ]; then
git clone -b OpenSSL_1_1_0-stable git://git.openssl.org/openssl.git
fi
cd openssl
./config
make
cd ${SCRIPTS_DIR}