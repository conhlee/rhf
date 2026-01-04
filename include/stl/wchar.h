#ifndef MSL_WCHAR_H
#define MSL_WCHAR_H
#include <internal/mbstring.h>
#include <internal/wprintf.h>
#include <internal/wstring.h>
#include <limits.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef WCHAR_MIN
#define WCHAR_MIN SHRT_MIN
#endif

#ifndef WCHAR_MAX
#define WCHAR_MAX USHRT_MAX
#endif

#ifdef __cplusplus
}
#endif
#endif