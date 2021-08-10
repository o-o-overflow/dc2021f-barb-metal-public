#!/bin/bash -ex
GDBSCRIPT=/tmp/no-flag-patch.gdbinit
FLAG=$(cat /flag)

cat <<EOF > $GDBSCRIPT
set pagination off
set print elements 0
set print repeats 0
add-symbol-file /service.dbg
target remote 127.0.0.1:1234
p/s FLAG_BUF
quit
EOF

export EXTRA_QEMU_FLAGS=-s
bash -c 'sleep 1000;' | /wrapper &
sleep 5

set +e
grep "$FLAG" <(gdb --command $GDBSCRIPT)
RETVAL=$?
set -e
killall qemu-system-i386

exit $RETVAL
