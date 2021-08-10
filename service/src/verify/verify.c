#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <mrubyc.h>

#define USE_LTM 1
#define LTM_DESC 1
#include <tomcrypt.h>
#include <tommath.h>
#include <tommath_private.h>

#include <stdint.h>
#include "printf.h"
#ifdef assert
#undef assert
#endif
#define assert(cond) if (!(cond)) { printf("ASSERTION FAILED! %d\n", __LINE__); while(1); }

#include "payload.mrb.h"
#include "public.der.h"

// custom ruby devices and objects and stuff
#include "b64.h"
#include "qemuart.h"
#include "thermostat.h"
#include "alarm.h"
#include "smartspeaker.h"

#define DEBUG 1

#if DEBUG
#define LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...) do {} while (0)
#endif

#define MEMORY_SIZE (1024*30)

void __assert_fail(const char * assertion, const char * file, unsigned int line, const char * function)
{
	while (1) {} 
}

static inline void print_hex(const unsigned char *data, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		printf("%02x", data[i]);
	}
}

/*
 * Returns 1 if signature is verified, otherwise 0.
 *
 * Key in DER format
 * Sig in PKCS-1.5
 */
static inline bool verify_signature(
	const unsigned char *key, size_t key_len,
	const unsigned char *sig, size_t sig_len,
	const unsigned char *msg, size_t msg_len
	)
{
	int r;

	static int initialized = 0;
	if (!initialized) {
		ltc_mp = ltm_desc;
		r = register_hash(&sha256_desc);
		assert(r >= 0 && "hash registration failed");
		initialized = 1;
	}

	rsa_key imported_key;
	r = rsa_import(key, key_len, &imported_key);
	assert(r == CRYPT_OK && "key import failed");
	assert(imported_key.type == PK_PUBLIC);
	LOG("Key import OK\n");

	LOG("Calculating payload digest...\n");
	unsigned char digest[32];
	hash_state hash_ctx;
	sha256_init(&hash_ctx);
	sha256_process(&hash_ctx, msg, msg_len);
	sha256_done(&hash_ctx, digest);
	LOG("SHA256: ");
#if DEBUG
	print_hex(digest, sizeof(digest));
#endif
	LOG("\n");

	int hash_idx = find_hash("sha256");
	r = hash_is_valid(hash_idx);
	assert(r == CRYPT_OK && "invalid hash type");

	/* this replaces libtom's implementation of rsa_verify_hash_ex */
	const rsa_key *rkey = &imported_key;

	// allocate for decoding signature
	uint8_t *tmp = malloc(sig_len);
	unsigned long x = sig_len;
	r = ltc_mp.rsa_me(sig, sig_len, tmp, &x, PK_PUBLIC, rkey);
	assert(r == CRYPT_OK && "rsa decoded");
	assert(x == sig_len && "output size correct");
	uint8_t *clearsig = tmp;
	assert(clearsig[0] == 0x00 && clearsig[1] == 0x01 && "signature marker");
	int sep_idx = 0;
	for (int i = 1; i < x; i++) {
		if (clearsig[i] == 0x00) {
			sep_idx = i;
			break;
		}
	}
	assert(sep_idx != 0 && "no separator?");
	uint8_t *clearsig_sep = &clearsig[sep_idx + 1];
	uint8_t ASN1_TABLE[] = {0x30,0x31,0x30,0x0d,0x06,0x09,0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x01,0x05,0x00,0x04,0x20};
	for (int i = 0; i < sizeof(ASN1_TABLE); i++) {
		assert(clearsig_sep[i] == ASN1_TABLE[i]);
	}
	uint8_t *sig_hash = &clearsig_sep[sizeof(ASN1_TABLE)];
	bool success = true;
	for (int i = 0; i < sizeof(digest); i++) {
		if (sig_hash[i] != digest[i]) {
			success = false;
		}
	}
	LOG("Calculating signature digest...\n");
	LOG("SHA256: ");
#if DEBUG
	print_hex(sig_hash, sizeof(digest));
#endif
	LOG("\n");

	return success == true;
}

extern int _bss_start_addr, _bss_end_addr;

void *memset(void *s, int c, size_t n)
{
	for (size_t i = 0; i < n; i++)
		((unsigned char *)s)[i] = (unsigned char)(c);
	return s;
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *_s1 = s1, *_s2 = s2;
	int d = 0;
	for (size_t i = 0; i < n && ((d = (int)_s1[i]-(int)_s2[i]) == 0); i++);
	return d;
}

void *malloc(size_t size)
{
	static char heap[1024*1024];
	static size_t p;
	assert(p+size <= sizeof(heap))
	void *out = &heap[p];
	p += size;
	return out;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *_dest = dest;
	const unsigned char *_src = src;
	for (size_t i = 0; i < n; i++) _dest[i] = _src[i];
	return dest;
}

void *realloc(void *ptr, size_t size)
{
	void *new = malloc(size);
	memcpy(new, ptr, size);
	return new;
}

void free(void *ptr)
{
}

void *calloc(size_t nmemb, size_t size)
{
	void *p = malloc(nmemb * size);
	memset(p, 0, nmemb * size);
	return p;
}

void outb(uint16_t port, uint8_t val)
{
	asm volatile ("outb %%al, %%dx;" : : "a" (val), "d" (port));
}

void outw(uint16_t port, uint16_t val)
{
	asm volatile ("outw %%ax, %%dx;" : : "a" (val), "d" (port));
}

uint8_t inb(uint16_t port)
{
	uint8_t val;
	asm volatile ("inb %%dx, %%al;" : "=a" (val) : "d" (port));
	return val;
}

#define SERIAL_THR 0
#define SERIAL_RBR 0
#define SERIAL_LSR 5
const int serial_port = 0x3f8;

void _putchar(char ch)
{
	while ((inb(serial_port + SERIAL_LSR) & (1<<5)) == 0);
	outb(serial_port + SERIAL_THR, ch);
}

int getchar(void)
{
	const int serial_port = 0x3f8;
	while ((inb(serial_port + SERIAL_LSR) & 1) == 0);
	return inb(serial_port + SERIAL_RBR);
}

void read_bytes(unsigned char *buf, size_t len)
{
	while (len--) {
		*buf++ = getchar();
	}
}

static void shutdown(void)
{
	printf("Shutting down...\n");
	outw(0x604, 0x2000);
}

char FLAG_BUF[49];

void read_flag(void)
{
	read_bytes((void*)FLAG_BUF, sizeof(FLAG_BUF));
}

void _start(void)
{
	asm volatile ("mov $stack_top, %esp;");
	memset(&_bss_start_addr, 0, &_bss_end_addr-&_bss_start_addr);
	read_flag();

	printf("OOO Boootloader\n");
	printf("========================================\n");

	uint32_t payload_len;
	printf("Waiting for 32b payload size...\n");
	read_bytes((void*)&payload_len, 4);
	printf("Ready to recv %zd bytes...\n", payload_len);
	unsigned char *payload = malloc(payload_len);
	read_bytes(payload, payload_len);
	uint32_t sig_len = 256;

	bool verified = false;
	if (payload_len > sig_len) {
		verified = verify_signature(public_der, public_der_len,
			                        payload, sig_len,
			                        &payload[sig_len], payload_len-sig_len);
	}

	const unsigned char *task_code = NULL;

	if (verified) {
		printf("Launching payload...\n\n");
		task_code = &payload[sig_len];
	} else {
		printf("Invalid payload signature. Launching backup payload...\n\n");
		task_code = payload_mrb;
	}

	static uint8_t memory_pool[MEMORY_SIZE];
	mrbc_init(memory_pool, MEMORY_SIZE);
	// TIME TO INITIALIZE RUBY CLASSES
	mrbc_init_class_uart(0);
	mrbc_init_class_b64(0);
	mrbc_init_class_alarm(0);
	mrbc_init_class_thermostat(0);
	mrbc_init_class_smartspeaker(0);
	if( mrbc_create_task(task_code, 0) != NULL ){
		mrbc_run();
	}

	shutdown();
	asm volatile ("cli; hlt");
}
