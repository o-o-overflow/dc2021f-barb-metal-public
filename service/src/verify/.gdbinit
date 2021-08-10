add-symbol-file verify
target remote 127.0.0.1:1234
set disassembly-flavor intel
b _start
layout src
