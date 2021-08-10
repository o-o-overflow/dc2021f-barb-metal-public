#!/bin/bash

version=$(git describe --tags --always --dirty 2>/dev/null)
if [ ! -e ".git" ] || [ -z $version ]
then
   version=$(grep "^VERSION=" makefile_include.mk | sed "s/.*=//")
fi
echo "Testing version:" $version
#grep "VERSION=" makefile | perl -e "@a = split('=', <>); print @a[1];"`

# get uname
echo "uname="`uname -a`

# get gcc name
if [ -z ${CC} ]
then
   CC="gcc"
fi
echo "${CC}="`${CC} -dumpversion`
echo

# ref:         HEAD -> master, origin/master, origin/HEAD
# git commit:  26edbfa0164e8efed06197be0dff225945834640
# commit time: 2021-08-10 11:33:39 +0200
