/*! @file
  @brief
  mrubyc memory management.

  <pre>
  Copyright (C) 2015-2020 Kyushu Institute of Technology.
  Copyright (C) 2015-2020 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.

  Memory management for objects in mruby/c.

  </pre>
*/

#ifndef MRBC_SRC_ALLOC_H_
#define MRBC_SRC_ALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

/***** Feature test switches ************************************************/
/***** System headers *******************************************************/
#if defined(MRBC_ALLOC_LIBC)
#include <stdlib.h>
#endif

/***** Local headers ********************************************************/
/***** Constant values ******************************************************/
/***** Macros ***************************************************************/
/***** Typedefs *************************************************************/
struct VM;

/***** Global variables *****************************************************/
/***** Function prototypes and inline functions *****************************/
#if !defined(MRBC_ALLOC_LIBC)
/*
  Normally enabled
*/
void mrbc_init_alloc(void *ptr, unsigned int size);
void mrbc_cleanup_alloc(void);
void *mrbc_raw_alloc(unsigned int size);
void *mrbc_raw_alloc_no_free(unsigned int size);
void mrbc_raw_free(void *ptr);
void *mrbc_raw_realloc(void *ptr, unsigned int size);
#define mrbc_free(vm,ptr)		mrbc_raw_free(ptr)
#define mrbc_realloc(vm,ptr,size)	mrbc_raw_realloc(ptr, size)

// for statistics or debug. (need #define MRBC_DEBUG)
void mrbc_alloc_statistics(int *total, int *used, int *free, int *fragmentation);
void mrbc_alloc_print_memory_pool(void);


#if defined(MRBC_ALLOC_VMID)
// Enables memory management by VMID.
void *mrbc_alloc(const struct VM *vm, unsigned int size);
void mrbc_free_all(const struct VM *vm);
void mrbc_set_vm_id(void *ptr, int vm_id);
int mrbc_get_vm_id(void *ptr);

# else
#define mrbc_alloc(vm,size)	mrbc_raw_alloc(size)
#define mrbc_free_all(vm)	((void)0)
#define mrbc_set_vm_id(ptr,id)	((void)0)
#define mrbc_get_vm_id(ptr)	0
#endif


#elif defined(MRBC_ALLOC_LIBC)
/*
  use the system (libc) memory allocator.
*/
#if defined(MRBC_ALLOC_VMID)
#error "Can't use MRBC_ALLOC_LIBC with MRBC_ALLOC_VMID"
#endif

static inline void mrbc_init_alloc(void *ptr, unsigned int size) {}
static inline void mrbc_cleanup_alloc(void) {}
static inline void *mrbc_raw_alloc(unsigned int size) {
  return malloc(size);
}
static inline void *mrbc_raw_alloc_no_free(unsigned int size) {
  return malloc(size);
}
static inline void mrbc_raw_free(void *ptr) {
  free(ptr);
}
static inline void *mrbc_raw_realloc(void *ptr, unsigned int size) {
  return realloc(ptr, size);
}
static inline void mrbc_free(const struct VM *vm, void *ptr) {
  free(ptr);
}
static inline void * mrbc_realloc(const struct VM *vm, void *ptr, unsigned int size) {
  return realloc(ptr, size);
}
static inline void *mrbc_alloc(const struct VM *vm, unsigned int size) {
  return malloc(size);
}
static inline void mrbc_free_all(const struct VM *vm) {
}
static inline void mrbc_set_vm_id(void *ptr, int vm_id) {
}
static inline int mrbc_get_vm_id(void *ptr) {
  return 0;
}
#endif	// MRBC_ALLOC_LIBC


#ifdef __cplusplus
}
#endif
#endif
