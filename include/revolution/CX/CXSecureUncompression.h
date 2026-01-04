#ifndef RVL_SDK_CX_SECURE_UNCOMPRESSION_H
#define RVL_SDK_CX_SECURE_UNCOMPRESSION_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef s32 CXSecureResult;
#define CX_SECURE_ERR_OK                 0
#define CX_SECURE_ERR_BAD_FILE_TYPE     -1
#define CX_SECURE_ERR_BUFFER_TOO_SMALL  -2
#define CX_SECURE_ERR_BUFFER_TOO_LARGE  -3
#define CX_SECURE_ERR_BAD_FILE_SIZE     -4
#define CX_SECURE_ERR_BAD_FILE_TABLE    -5

CXSecureResult CXSecureUncompressRL(const void* src, u32 length, void* dst);
CXSecureResult CXSecureUncompressLZ(const void* src, u32 length, void* dst);
CXSecureResult CXSecureUncompressHuffman(const void* src, u32 length, void* dst);

#ifdef __cplusplus
}
#endif
#endif
