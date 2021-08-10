#!/bin/bash

# output version
bash .ci/printinfo.sh

bash .ci/build.sh " $1" "$2" "$3" "$4" "$5"
if [ -a testok.txt ] && [ -f testok.txt ]; then
   echo
else
   echo
   echo "Test failed"
   exit 1
fi

rm -f testok.txt
bash .ci/build.sh " $1" "$2" "$3 LTC_DEBUG=1" "$4" "$5"
if [ -a testok.txt ] && [ -f testok.txt ]; then
   echo
else
   echo
   echo "Test failed"
   exit 1
fi

rm -f testok.txt
bash .ci/build.sh " $1" "$2 -O2" "$3 IGNORE_SPEED=1" "$4" "$5"
if [ -a testok.txt ] && [ -f testok.txt ]; then
   echo
else
   echo
   echo "Test failed"
   exit 1
fi

rm -f testok.txt
bash .ci/build.sh " $1" "$2" "$3 IGNORE_SPEED=1 LTC_SMALL=1" "$4" "$5"
if [ -a testok.txt ] && [ -f testok.txt ]; then
   echo
else
   echo
   echo "Test failed"
   exit 1
fi

exit 0

# ref:         HEAD -> master, origin/master, origin/HEAD
# git commit:  26edbfa0164e8efed06197be0dff225945834640
# commit time: 2021-08-10 11:33:39 +0200
