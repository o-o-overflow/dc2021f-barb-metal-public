from ubuntu:20.04 as builder
run sed -i 's|http://|http://us.|g' /etc/apt/sources.list

run apt-get update \
    && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get install --assume-yes --no-install-recommends \
    	build-essential wget ca-certificates
run mkdir -p /build/src
copy src/killondisconnect.c /build/src/killondisconnect.c
copy src/rf.c /build/src/rf.c
copy src/schitzo.c /build/src/schitzo.c
run gcc -o /killondisconnect /build/src/killondisconnect.c
run gcc -o /rf /build/src/rf.c
run gcc -o /schitzo /build/src/schitzo.c
run wget https://github.com/krallin/tini/releases/download/v0.19.0/tini-static
run chmod +x /tini-static

from ubuntu:20.04
run sed -i 's|http://|http://us.|g' /etc/apt/sources.list

run apt-get update \
    && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get install --assume-yes --no-install-recommends \
    	qemu-system-i386 xinetd ruby ruby-dev libssl-dev build-essential

run gem install -N sinatra
run gem install -N sinatra-websocket

copy --from=builder /tini-static /tini
copy --from=builder /killondisconnect /killondisconnect
copy --from=builder /rf /rf
copy --from=builder /schitzo /schitzo

copy startup /startup
copy wrapper /wrapper
copy service.conf /service.conf
copy banner_fail /banner_fail
copy service /service
copy payload.bin /payload.bin
run mkdir /barb-web
copy src/frontend/barb.html /barb-web/barb.html
copy src/frontend/s.rb /barb-web/s.rb

# The flagbot will periodically echo the flag > this file (flag_path in info.yaml)
run touch /flag && chmod 644 /flag && echo "000AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA1" > /flag

expose 7829


# cmd chmod go-rwx /proc && xinetd -filelog /dev/stderr -dontfork -f /service.conf
# # ^^ Assuming that ps would be too revealing.
# #    AFAIK, this also disables the built-in printf(%n) protection. Alternatively:

ENTRYPOINT ["/tini", "-p", "SIGTERM", "-g", "--"]
# cmd ["/usr/sbin/xinetd", "-filelog", "/dev/stderr", "-dontfork", "-f", "/service.conf"]
cmd ["/bin/bash", "/startup"]
