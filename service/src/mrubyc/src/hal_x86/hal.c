#include "hal.h"

static void outb(short port, short val) {
      asm volatile ("outb %%al, %%dx;" : : "a" (val), "d" (port));
}
static char inb(short port) {
      char val;
      asm volatile ("inb %%dx, %%al;" : "=a" (val) : "d" (port));
      return val;
}

static void _putchar_r(char ch) {
	const int port = 0x3f8;
	while ((inb(port + 5) & (1<<5)) == 0);
	outb(port + 0, ch);
}

int hal_write(int fd, const void *buf, int nbytes) {
	int i = nbytes;
	const char *p = buf;
	while (--i >= 0) {
		_putchar_r(*p++);
	}
	return nbytes;
}
int hal_flush(int fd) {
	return 0;
}
