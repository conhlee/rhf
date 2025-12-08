#ifndef NW4R_DB_ASSERT_H
#define NW4R_DB_ASSERT_H

#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace db {

// TODO: do these macros belong here?
#define NW4R_WARNING(line, ...) nw4r::db::Warning(__FILE__, line, __VA_ARGS__)
#define NW4R_PANIC(line, ...) nw4r::db::Panic(__FILE__, line, __VA_ARGS__)
#define NW4R_DB_ASSERT(exp, line, ...) \
    if(!(exp))                \
    nw4r::db::Panic(__FILE__, line, __VA_ARGS__)

#define NW4R_ASSERT_MSG(x, line, ...)                                                \
    ((x) && 1 || (nw4r::db::Panic(__FILE__, line, __VA_ARGS__), 0))

#define NW4R_ASSERT(x, line)                                                    \
    NW4R_ASSERT_MSG((x), line, "NW4R:Failed assertion " #x)

#define NW4R_ASSERT_PTR_NULL(ptr, line)                                                     \
    NW4R_ASSERT_MSG((ptr) != 0, \
                        line,                                                  \
                    "NW4R:Pointer must not be NULL (" #ptr ")")

#define NW4R_ASSERT_PTR(x, line)                                                     \
    NW4R_ASSERT_MSG((((u32)(x) & 0xff000000) == 0x80000000) ||                 \
                        (((u32)(x) & 0xff800000) == 0x81000000) ||             \
                        (((u32)(x) & 0xf8000000) == 0x90000000) ||             \
                        (((u32)(x) & 0xff000000) == 0xc0000000) ||             \
                        (((u32)(x) & 0xff800000) == 0xc1000000) ||             \
                        (((u32)(x) & 0xf8000000) == 0xd0000000) ||             \
                        (((u32)(x) & 0xffffc000) == 0xe0000000),               \
                        line,                                                  \
                    "NW4R:Pointer Error\n" #x "(=%p) is not valid pointer.", (x))

DECL_WEAK void VPanic(const char* file, int line, const char* fmt, std::va_list vlist, bool halt);
DECL_WEAK void Panic(const char* file, int line, const char* fmt, ...);
DECL_WEAK void VWarning(const char* file, int line, const char* fmt, std::va_list vlist);
DECL_WEAK void Warning(const char* file, int line, const char* msg, ...);

namespace detail {

void Log(const char* fmt, ...);

} // namespace detail

} // namespace db
} // namespace nw4r

#endif