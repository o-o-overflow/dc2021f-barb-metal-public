#ifndef __MALLOOOC
#define __MALLOOOC
#include<stddef.h>
void heap_init(void *ptr, size_t size);
void *o_malloc(size_t noOfBytes);
///void merge();
void o_free(void* ptr);
#endif
