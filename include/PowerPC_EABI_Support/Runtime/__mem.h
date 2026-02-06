#ifndef RUNTIME___MEM_H
#define RUNTIME___MEM_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

DECL_SECTION(".init") void *memcpy(void *dest, const void *src, size_t n);
DECL_SECTION(".init") void *memset(void *dest, int val, size_t n);

#ifdef __cplusplus
}
#endif
#endif