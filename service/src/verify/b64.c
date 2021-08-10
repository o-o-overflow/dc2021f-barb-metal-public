#include "vm_config.h"
#include <stdint.h>
#include "mrubyc.h"
#include "b64.h"

static const char  table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//static const int   BASE64_INPUT_SIZE = 57;

static int isbase64(char c) {
        return c && strchr(table, c) != NULL;
}

static char value(char c) {
        const char *p = strchr(table, c);
        if(p) {
                return p-table;
        } else {
                return 0;
        }
}

static int UnBase64(unsigned char *dest, const unsigned char *src, int srclen) {
        *dest = 0;
        if(*src == 0)
        {
                return 0;
        }
        unsigned char *p = dest;
        do
        {

                char a = value(src[0]);
                char b = value(src[1]);
                char c = value(src[2]);
                char d = value(src[3]);
                *p++ = (a << 2) | (b >> 4);
                *p++ = (b << 4) | (c >> 2);
                *p++ = (c << 6) | d;
                if(!isbase64(src[1]))
                {
                        p -= 2;
                        break;
                }
                else if(!isbase64(src[2]))
                {
                        p -= 2;
                        break;
                }
                else if(!isbase64(src[3]))
                {
                        p--;
                        break;
                }
                src += 4;
                while(*src && (*src == 13 || *src == 10)) src++;
        }
        while(srclen-= 4);
        *p = 0;
        return p-dest;
}



/* read
 *
 * s = $uart.read(n)
 *
 * param n - number of bytes to read
 * return String - received data
 * return Nil - not enough data
 */
static void c_b64_decode(mrbc_vm *vm, mrbc_value v[], int argc) {
	mrbc_value ret;
	unsigned char *instr = (unsigned char *)GET_STRING_ARG(1);
	unsigned char *outbuf = mrbc_alloc(vm , strlen((char *)instr));
	if (!outbuf) {
		ret = mrbc_nil_value();
		SET_RETURN(ret);
		return;
	}
	int len = UnBase64(outbuf, instr, strlen((char *)instr));
	ret = mrbc_string_new_alloc(vm, outbuf, len);
	//mrbc_free(vm, outbuf);
	SET_RETURN(ret);
	return;
}

static void c_b64_new(mrbc_vm *vm, mrbc_value v[], int argc) {
	return;
}

void mrbc_init_class_b64(struct VM *vm) {
	mrbc_class *b64;
	b64 = mrbc_define_class(0, "Base64",		mrbc_class_object);
	mrbc_define_method(0, b64, "new",		c_b64_new);
	mrbc_define_method(0, b64, "decode64",		c_b64_decode);
}
