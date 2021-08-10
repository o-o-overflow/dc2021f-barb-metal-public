# This container is built on top of the patched-service ones
ARG SERVICE=build--this--with--a--build-arg
FROM $SERVICE

run apt-get update

# Extra dependencies of these check scripts, if any
run apt-get -qq --no-install-recommends install file gdb psmisc

copy check_filetype.sh /
copy no-flag-patch.sh /
copy service.dbg /
