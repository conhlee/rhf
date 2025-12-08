#ifndef RVL_SDK_MTX_MTX_H
#define RVL_SDK_MTX_MTX_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

typedef struct Vec {
    f32 x, y, z;
} Vec;

typedef struct Vec2 {
    f32 x, y;
} Vec2;

typedef struct Quaternion {
    f32 x, y, z, w;
} Quaternion;

#define MTXDegToRad(a) ((a) * 0.01745329252f)
#define MTXRadToDeg(a) ((a) * 57.29577951f)

#define MTXRotDeg(m, axis, deg) \
    MTXRotRad(m, axis, MTXDegToRad(deg))

// MTX
// C version
void C_MTXIdentity(Mtx m);
void C_MTXCopy(const Mtx src, Mtx dst);
void C_MTXConcat(const Mtx a, const Mtx b, Mtx ab);
void C_MTXConcatArray(const Mtx a, const Mtx* srcBase, Mtx* dstBase, u32 count);
void C_MTXTranspose(const Mtx src, Mtx xPose);
u32 C_MTXInverse(const Mtx src, Mtx inv);
u32 C_MTXInvXpose(const Mtx src, Mtx invX);
void C_MTXRotRad(Mtx m, char axis, f32 rad);
void C_MTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);
void C_MTXRotAxisRad(Mtx m, const Vec* axis, f32 rad);
void C_MTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
void C_MTXTransApply(const Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT);
void C_MTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void C_MTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS);
void C_MTXQuat(Mtx m, const Quaternion* q);
void C_MTXReflect(Mtx m, const Vec* p, const Vec* n);

// PS version
void PSMTXIdentity(Mtx m);
void PSMTXCopy(const Mtx src, Mtx dst);
void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
void PSMTXConcatArray(const Mtx a, const Mtx* srcBase, Mtx* dstBase, u32 count);
void PSMTXTranspose(const Mtx src, Mtx xPose);
u32 PSMTXInverse(const Mtx src, Mtx inv);
u32 PSMTXInvXpose(const Mtx src, Mtx invX);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
void PSMTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);
void PSMTXRotAxisRad(Mtx m, const Vec* axis, f32 rad);
void PSMTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
void PSMTXTransApply(const Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT);
void PSMTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void PSMTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS);
void PSMTXQuat(Mtx m, const Quaternion* q);
void PSMTXReflect(Mtx m, const Vec* p, const Vec* n);

#ifdef DEBUG
#define MTXIdentity   C_MTXIdentity
#define MTXCopy       C_MTXCopy
#define MTXConcat     C_MTXConcat
#define MTXInverse    C_MTXInverse
#define MTXTranspose  C_MTXTranspose
#define MTXInverse    C_MTXInverse
#define MTXInvXpose   C_MTXInvXpose
#define MTXRotRad     C_MTXRotRad
#define MTXRotTrig    C_MTXRotTrig
#define MTXRotAxisRad C_MTXRotAxisRad
#define MTXTrans      C_MTXTrans
#define MTXTransApply C_MTXTransApply
#define MTXScale      C_MTXScale
#define MTXScaleApply C_MTXScaleApply
#define MTXQuat       C_MTXQuat
#define MTXReflect    C_MTXReflect
#else
#define MTXIdentity   PSMTXIdentity
#define MTXCopy       PSMTXCopy
#define MTXConcat     PSMTXConcat
#define MTXInverse    PSMTXInverse
#define MTXTranspose  PSMTXTranspose
#define MTXInverse    PSMTXInverse
#define MTXInvXpose   PSMTXInvXpose
#define MTXRotRad     PSMTXRotRad
#define MTXRotTrig    PSMTXRotTrig
#define MTXRotAxisRad PSMTXRotAxisRad
#define MTXTrans      PSMTXTrans
#define MTXTransApply PSMTXTransApply
#define MTXScale      PSMTXScale
#define MTXScaleApply PSMTXScaleApply
#define MTXQuat       PSMTXQuat
#define MTXReflect    PSMTXReflect
#endif

#ifdef __cplusplus
}
#endif
#endif
