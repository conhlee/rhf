#ifndef RVL_SDK_GX_PIXEL_H
#define RVL_SDK_GX_PIXEL_H
#include <revolution/GX/GXTypes.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void GXSetFog(GXFogType type, GXColor color, f32 start, f32 end, f32 near,
              f32 far);

void GXSetBlendMode(GXBlendMode mode, GXBlendFactor src, GXBlendFactor dst,
    GXLogicOp op);

void GXSetColorUpdate(GXBool enable);
void GXSetAlphaUpdate(GXBool enable);
    
void GXSetZMode(GXBool enableTest, GXCompare func, GXBool enableUpdate);

void GXGetPixelFmt(GXPixelFmt *pixelFmt, GXZFmt *zFmt);
void GXSetPixelFmt(GXPixelFmt pixelFmt, GXZFmt zFmt);

void GXSetDither(GXBool enable);

#ifdef __cplusplus
}
#endif
#endif
