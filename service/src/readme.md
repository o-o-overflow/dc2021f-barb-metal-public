# BARB-Metal

# OOO's brand new IOT platform security whitepaper.

## introduction

OOO designed the barb-metal platform with security at its core.
When we set out to create the best possible IOT platform, we drew from hours
of reading stackoverflow to build an entirely new architecture.

Every barb-metal device combines ruby, c, and x86 designed for maximum security and
transparent user experience.

barb-metal not only has some memory safety through the ruby runtime, but also
enforces secure boot of payloads, to prevent users from running insecurely patched
versions of barbOS.

This document is organized into the following topic areas

	- system security : how the flag is secured from attackers
	- encryption : the design of the secure boot
	- app security : the runtime


## system security

the flag is read via UART and stored in the x86 platform's memory. it is not accessible
from any app running within the ruby app. the barbOS is implemented in ruby,
with a memory-safe language preventing access of memory outside the vm's memory
pool.

## encryption

barbOS blobs are loaded via uart, and are signed by OOO, and only by OOO.
the format for these blobs is defined in this struct.

```
typedef struct barbOSblob {
  uint32_t size;           // the size of the remainder of the struct
  uint8_t  signature[256]; // the cryptographic signature of the struct
  uint8_t  blob[0];        // variable size blob.
} barbOSblob_t;
```

the boot process starts by reading in the flag via UART, and then by reading the
signed OS blob. The barb-metal machine has the cryptographic keys built into the
early boot stage, and if verification of the signature fails, version 0.0 of
barbOS will be booted.

## app security

only OOO has gotten ruby to run on bare metal x86. ruby is a safe language, and
all barbOS apps run in the ruby interpreter's runtime.
