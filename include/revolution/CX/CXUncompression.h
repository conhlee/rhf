#ifndef RVL_SDK_CX_UNCOMPRESSION_H
#define RVL_SDK_CX_UNCOMPRESSION_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

u32 CXGetUncompressedSize(const void* src);
void CXUncompressLZ(const void* src, void* dst);

#ifdef __cplusplus
}
#endif
#endif
