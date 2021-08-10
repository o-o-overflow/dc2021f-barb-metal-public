#ifndef QEMUART_H_
#define QEMUART_H_

#ifdef __cplusplus
extern "C" {
#endif

struct VM;
void mrbc_init_class_uart(struct VM *vm);

#ifdef __cplusplus
}
#endif
#endif // QEMUART_H_
