#!/bin/bash

# output version
bash .ci/printinfo.sh

make clean > /dev/null

echo "checking..."
./helper.pl --check-all || exit 1

exit 0

# ref:         HEAD -> master, origin/master, origin/HEAD
# git commit:  26edbfa0164e8efed06197be0dff225945834640
# commit time: 2021-08-10 11:33:39 +0200
