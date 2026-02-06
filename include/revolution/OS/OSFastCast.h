#ifndef RVL_SDK_OS_FAST_CAST_H
#define RVL_SDK_OS_FAST_CAST_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define OS_GQR_TYPE_U8  4
#define OS_GQR_TYPE_U16 5
#define OS_GQR_TYPE_S8  6
#define OS_GQR_TYPE_S16 7

// Initializes GQRs 2-5 with the following configuration:
// GQR2: unsigned 8 bit
// GQR3: unsigned 16 bit
// GQR4: signed 8 bit
// GQR5: signed 16 bit
static inline void OSInitFastCast(void) {
    #ifdef __MWERKS__
    asm {
        li r3, OS_GQR_TYPE_U8
        oris r3, r3, OS_GQR_TYPE_U8
        mtgqr2 r3

        li r3, OS_GQR_TYPE_U16
        oris r3, r3, OS_GQR_TYPE_U16
        mtgqr3 r3

        li r3, OS_GQR_TYPE_S8
        oris r3, r3, OS_GQR_TYPE_S8
        mtgqr4 r3
        
        li r3, OS_GQR_TYPE_S16
        oris r3, r3, OS_GQR_TYPE_S16
        mtgqr5 r3
    }
    #endif
}

static inline void OSSetGQR6(register u32 type, register u32 scale) {
    register u32 val = ((scale << 8 | type) << 16) | ((scale << 8) | type);

    #ifdef __MWERKS__
    asm {
        mtspr 0x396, val
    }
    #endif
}

static inline void OSSetGQR7(register u32 type, register u32 scale) {
    register u32 val = ((scale << 8 | type) << 16) | ((scale << 8) | type);

    #ifdef __MWERKS__
    asm {
        mtspr 0x397, val
    }
    #endif
}

static f32 __OSu8tof32(register u8* in) {
    register f32 ret;

    #ifdef __MWERKS__
    asm {
        psq_l ret, 0(in), 1, 2
    }
    #endif

    return ret;
}

static void OSu8tof32(u8* in, volatile f32* out) {
    *out = __OSu8tof32(in);
}

static inline f32 __OSu16tof32(register const u16* arg) {
    register f32 ret;

    #ifdef __MWERKS__
    asm {
        psq_l ret, 0(arg), 1, 3
    }
    #endif

    return ret;
}

static inline void OSu16tof32(const u16* in, f32* out) {
    *out = __OSu16tof32(in);
}

static u8 __OSf32tou8(register f32 arg) {
    f32 a;
    register f32 *ptr = &a;
    u8 r;

    #ifdef __MWERKS__
    asm {
        psq_st arg, 0(ptr), 1, 2
    }
    #endif

    r = *(u8 *)ptr;
    return r;
}

static void OSf32tou8(f32 *in, volatile u8 *out) {
    *out = __OSf32tou8(*in);
}

static inline u16 __OSf32tou16(register f32 arg) {
    f32 a;
    register f32* ptr = &a;
    u16 r;

    #ifdef __MWERKS__
    asm {
        psq_st arg, 0(ptr), 1, 3
    }
    #endif

    r = *(u16*)ptr;
    return r;
}

static inline void OSf32tou16(const f32* in, u16* out) {
    *out = __OSf32tou16(*in);
}

static inline f32 __OSs16tof32(register const s16* arg) {
    register f32 ret;

    #ifdef __MWERKS__
    asm {
        psq_l ret, 0(arg), 1, 5
    }
    #endif

    return ret;
}

static inline void OSs16tof32(const s16* in, f32* out) {
    *out = __OSs16tof32(in);
}

static inline s16 __OSf32tos16(register f32 arg) {
    f32 a;
    register f32* ptr = &a;
    s16 r;

    #ifdef __MWERKS__
    asm {
        psq_st arg, 0(ptr), 1, 5
    }
    #endif

    r = *(s16*)ptr;
    return r;
}

static inline void OSf32tos16(const f32* in, s16* out) {
    *out = __OSf32tos16(*in);
}

#ifdef __cplusplus
}
#endif
#endif