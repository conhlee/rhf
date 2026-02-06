#ifndef NW4R_G3D_PLATFORM_CPU_H
#define NW4R_G3D_PLATFORM_CPU_H
#include <nw4r/types_nw4r.h>

#include <revolution/OS.h>

namespace nw4r {
namespace g3d {

namespace fastcast {

inline f32 U8_0ToF32(const u8 *p) {
    f32 x;
    OSu8tof32(const_cast<u8 *>(p), &x);
    return x;
}

inline f32 U16_0ToF32(const u16 *p) {
    f32 x;
    OSu16tof32(const_cast<u16 *>(p), &x);
    return x;
}

inline f32 S7_8ToF32(register const s16 *p) {
    register f32 f;

    #ifdef __MWERKS__
    asm {
        psq_l f, 0(p), 1, 7
    }
    #endif

    return f;
}

inline f32 S10_5ToF32(register const s16 *p) {
    register f32 f;

    #ifdef __MWERKS__
    asm {
        psq_l f, 0(p), 1, 6
    }
    #endif

    return f;
}

inline u8 F32ToU8_0(f32 f) {
    u8 x;
    OSf32tou8(&f, &x);
    return x;
}

inline s16 F32ToS10_5(register f32 f) {
    s16 x;
    register s16 *p = &x;

    #ifdef __MWERKS__
    asm {
        psq_st f, 0(p), 1, 6
    }
    #endif

    return x;
}

inline void SetGQR6_S10_5() {
    OSSetGQR6(OS_GQR_TYPE_S16, 5);
}
inline void SetGQR7_S7_8() {
    OSSetGQR7(OS_GQR_TYPE_S16, 8);
}

namespace detail {

inline void Init() {
    OSInitFastCast();
    SetGQR6_S10_5();
    SetGQR7_S7_8();
}

} // namespace detail

} // namespace fastcast

namespace DC {

inline void StoreRange(void* pBase, u32 size) {
    DCStoreRange(pBase, size);
}

inline void StoreRangeNoSync(void* pBase, u32 size) {
    DCStoreRangeNoSync(pBase, size);
}

inline void FlushRangeNoSync(void* pBase, u32 size) {
    DCFlushRangeNoSync(pBase, size);
}

inline void InvalidateRange(void* pBase, u32 size) {
    DCInvalidateRange(pBase, size);
}

} // namespace DC

namespace detail {

void Copy32ByteBlocks(void*, const void*, u32);
void ZeroMemory32ByteBlocks(void*, u32);

} // namespace detail

inline void InitFastCast() {
    fastcast::detail::Init();
}

} // namespace g3d
} // namespace nw4r

#endif
