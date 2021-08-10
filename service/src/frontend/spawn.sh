#!/bin/bash
websocketd --port=8080 qemu-system-i386 -monitor /dev/null -serial stdio -display none -kernel ../verify/verify -s
