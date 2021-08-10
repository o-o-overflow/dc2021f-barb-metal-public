#include "vm_config.h"
#include <stdint.h>
#include "mrubyc.h"
#include "qemuart.h"

#if !defined(MRBC_NUM_UART)
#define MRBC_NUM_UART 1
#endif

#define SERIAL_THR 0
#define SERIAL_RBR 0
#define SERIAL_LSR 5
static uint8_t inb(uint16_t port)
{
	uint8_t val;
	asm volatile ("inb %%dx, %%al;" : "=a" (val) : "d" (port));
	return val;
}

static int getchar(void) {
	const int serial_port = 0x3f8;
	while ((inb(serial_port + SERIAL_LSR) & 1) == 0);
	return inb(serial_port + SERIAL_RBR);
}

static void read_bytes(unsigned char *buf, size_t len) {
	while(len--) { *buf++ = getchar(); }
}

static void c_uart_new(mrbc_vm *vm, mrbc_value v[], int argc) {
	return;
}


/* read
 *
 * s = $uart.read(n)
 *
 * param n - number of bytes to read
 * return String - received data
 * return Nil - not enough data
 */
static void c_uart_read(mrbc_vm *vm, mrbc_value v[], int argc) {
	mrbc_value ret;
	int need_length = GET_INT_ARG(1);
	char *buf = mrbc_alloc(vm , need_length + 1);
	if (!buf) {
		ret = mrbc_nil_value();
		SET_RETURN(ret);
		return;
	}
	read_bytes((unsigned char *)buf, need_length);
	ret = mrbc_string_new_alloc(vm, buf, need_length);
	SET_RETURN(ret);
	return;
}

/* gets
 *
 * s = $uart.gets()
 *
 * return String - received data
 * return Nil - not enough data
 */
static void c_uart_gets(mrbc_vm *vm, mrbc_value v[], int argc) {
	mrbc_value ret;
	int cnt = 32;
	int i = 0;
	char *buf = mrbc_alloc(vm , cnt);
	if (!buf) {
		goto RETNULL;
	}
	do {
		buf[i] = getchar();
		if (i + 1 > cnt) {
			cnt += 20;
			char *buf2 = mrbc_raw_realloc(buf, cnt);
			if (!buf2) {
				goto RETNULL;
			}
			memcpy(buf2, buf, i);
			buf = buf2;
		}
		i++;
	} while (buf[i - 1] != 13); // return is 13 i guess?
	ret = mrbc_string_new_alloc(vm, buf, i);
	SET_RETURN(ret);
	return;
RETNULL:
	ret = mrbc_nil_value();
	SET_RETURN(ret);
	return;
}

void mrbc_init_class_uart(struct VM *vm) {
	mrbc_class *uart;
	uart = mrbc_define_class(0, "UART",		mrbc_class_object);
	mrbc_define_method(0, uart, "new",		c_uart_new);
	mrbc_define_method(0, uart, "read",		c_uart_read);
	mrbc_define_method(0, uart, "gets",		c_uart_gets);
}
