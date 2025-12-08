#ifndef NW4R_UT_INLINES_H
#define NW4R_UT_INLINES_H

#include <revolution/types.h>

namespace nw4r {
namespace ut {

class NonCopyable
{
// methods
public:
    // cdtors
    NonCopyable() {}
    ~NonCopyable() {}

// deleted methods
private:
    // copy ctor, copy assignment operator
    NonCopyable(NonCopyable const &other) /* = delete */;
    NonCopyable &operator=(NonCopyable const &other) /* = delete */;
}; // size 0x01 (0x00 for inheritance)

inline u32 GetIntPtr(void const *ptr)
{
    return reinterpret_cast<u32>(ptr);
}

inline int ComparePtr(void const *a, void const *b)
{
    return static_cast<int>(GetIntPtr(a) - GetIntPtr(b));
}

template <typename T>
inline void const *AddOffsetToPtr(void const *ptr, T offset)
{
    return reinterpret_cast<void const *>(GetIntPtr(ptr) + offset);
}

template <typename T>
inline void *AddOffsetToPtr(void *ptr, T offset)
{
    return reinterpret_cast<void *>(GetIntPtr(ptr) + offset);
}

inline s32 GetOffsetFromPtr(void *start, void *end)
{
    return reinterpret_cast<u32>(end) - reinterpret_cast<u32>(start);
}

template <typename T>
inline T Abs(T x)
{
    return x < 0 ? static_cast<T>(-x) : static_cast<T>(x);
}

#if defined(__MWERKS__)
template <>
inline f32 Abs(register f32 x)
{
    register f32 ret;

#if 0 // ret existing probably indicates inline asm over intrinsic
    ret = __fabs(ret);
#else
    asm { fabs ret, x }
#endif

    return ret;
}
#endif

template <typename T>
inline T Min(T a, T b)
{
    return a > b ? b : a;
}

template <typename T>
inline T Max(T a, T b)
{
    return a > b ? a : b;
}

template <typename T>
inline T Clamp(T x, T low, T high)
{
    return x > high ? high : x < low ? low : x;
}

template <typename T>
inline T RoundDown(T x, unsigned int base)
{
    return x & ~(base - 1);
}

template <typename T>
inline void *RoundDown(T *x, unsigned int base)
{
    return reinterpret_cast<void *>(
        (reinterpret_cast<u32>(x)) & ~(base - 1));
}

template <typename T>
inline T RoundUp(T x, unsigned int base)
{
    return (x + (base - 1)) & ~(base - 1);
}

template <typename T>
inline void *RoundUp(T *x, unsigned int base)
{
    return reinterpret_cast<void *>(
        (reinterpret_cast<u32>(x) + (base - 1)) & ~(base - 1));
}

} // namespace ut
} // namespace nw4r

#endif
