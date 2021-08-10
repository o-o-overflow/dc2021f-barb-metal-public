from ubuntu:20.04
run sed -i 's|http://|http://us.|g' /etc/apt/sources.list

run apt-get update \
    && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get install --assume-yes --no-install-recommends \
    	build-essential gcc-multilib openssl xxd python3 python3-pip python3-dev wget ca-certificates \
        libgmp-dev libmpfr-dev libmpc-dev libssl-dev

run pip3 install pycrypto gensafeprime gmpy gmpy2

run mkdir /build
copy . /build/src

run cd /build/src/mrubyc/; ./build.sh
run cd /build/src/verify; make -j8 verify
run cd /build/src/verify; make payload.bin
run cd /build/src/expl; make
