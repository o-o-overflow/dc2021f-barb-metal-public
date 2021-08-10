# Set to n for native build
EMBEDDED ?= y

all:

CC = gcc
LIBTOMX_CFLAGS := $(CFLAGS) -Os -ffunction-sections
CFLAGS += -I$(PWD)/libtomcrypt/src/headers -I$(PWD)/libtommath/src/headers -I$(PWD)/libtommath -I$(PWD)/../mrubyc/src
CFLAGS += -Wall -Werror

TARGET = verify
LIBTOMCRYPT_A = libtomcrypt/libtomcrypt.a
LIBTOMMATH_A = libtommath/libtommath.a
OBJECTS = verify.o $(LIBTOMCRYPT_A) $(LIBTOMMATH_A)

ifeq ($(EMBEDDED),y)

LD = ld
LIBTOMX_CFLAGS += -fno-stack-protector -g -gdwarf-4 -m32
CFLAGS += -fno-stack-protector -ggdb3  -m32
LDFLAGS = --script=$(TARGET).ld -m elf_i386 --gc-sections
OBJECTS += printf.o qemuart.o thermostat.o alarm.o heap/o_heap.o smartspeaker.o b64.o
                   #/usr/lib/gcc/x86_64-linux-gnu/9/libgcc.a
verify.o: public.der.h payload.mrb.h
$(TARGET): $(TARGET).ld
CFLAGS += -DEMBEDDED

PRIVATE_KEY = private.pem
PUBLIC_KEY = public.pem
PUBLIC_KEY_DER = public.der

$(PRIVATE_KEY):
	openssl genrsa -3 -out $(PRIVATE_KEY) 2048

$(PUBLIC_KEY) $(PUBLIC_KEY_DER): $(PRIVATE_KEY)
	openssl rsa -in $(PRIVATE_KEY) -pubout -out $(PUBLIC_KEY)
	openssl rsa -pubin -in $(PUBLIC_KEY) -outform der -out $(PUBLIC_KEY_DER)

$(PUBLIC_KEY_DER).h: $(PUBLIC_KEY_DER)
	xxd -i $< > $@

else # EMBEDDED

LD = gcc
LDFLAGS = -Xlinker --gc-sections -fuse-ld=gold

endif # EMBEDDED

all: $(TARGET) test

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $(LDFLAGS) $(OBJECTS) ../mrubyc/src/libmrubyc.a ../libc.a /usr/lib/gcc/x86_64-linux-gnu/9/32/libgcc.a

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.FORCE:

$(LIBTOMMATH_A): .FORCE
	$(MAKE) -C libtommath -f makefile.unix CFLAGS="$(LIBTOMX_CFLAGS) -DMP_NO_DEV_URANDOM" libtommath.a

$(LIBTOMCRYPT_A): .FORCE
	$(MAKE) -C libtomcrypt -f makefile.unix CFLAGS="$(LIBTOMX_CFLAGS) -DUSE_LTM -DLTM_DESC -DLTC_NO_TEST -DLTC_NOTHING -DLTC_SHA256 -DLTC_DER -DLTC_MRSA -I../libtommath" libtomcrypt.a

payload.sig: payload.mrb $(PRIVATE_KEY)
	openssl dgst -sha256 -sign $(PRIVATE_KEY) -out $@ $<

payload.bin: payload.mrb payload.sig
	python3 pack_payload.py $^ $@

payload.mrb: payload.rb
	../mrbc -Eblah $<

payload.mrb.h: payload.mrb
	xxd -i $< > $@

.PHONY: test
ifeq ($(EMBEDDED),y)
test: $(TARGET) payload.bin
	cat flag payload.bin - | qemu-system-x86_64 -serial stdio -display none -kernel verify -s
else
test: $(TARGET)
	@./test.sh
endif

.PHONY: clean
clean:
	@rm -f $(TARGET) $(OBJECTS)
	@bash -c "pushd libtommath; git clean -fdxx ." >/dev/null 2>&1
	@bash -c "pushd libtomcrypt; git clean -fdxx ." >/dev/null 2>&1

$(TARGET).ld: $(TARGET).ld.in Makefile
	$(CC) -o $@ -x c -P -E \
		-DBASE_ADDRESS=0x100000 \
		$<
