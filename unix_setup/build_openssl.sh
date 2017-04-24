#!/usr/bin/env sh
. ./setup_env.sh
cd ${BUILD_DIR}

if [ ! -d openssl ]; then
git clone -b OpenSSL_1_1_0-stable git://git.openssl.org/openssl.git
fi
cd openssl
./config
make install DESTDIR=${DEPLOY_DIR}
cd ${SCRIPTS_DIR}
