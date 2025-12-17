#include "DynTex.hpp"

#include "GraphicManager.hpp"

#include "Mem.hpp"

#include <cstring> // for memset

#define MY_TEX_FORMAT (GX_TF_RGBA8)

CDynTex::CDynTex(DrwUpdateFn drwUpdateFn) {
    mDrwUpdateFn = drwUpdateFn;

    mOriginX = 0;
    mOriginY = 0;

    mWidth = gGraphicManager->getRenderMode()->fbWidth;
    mHeight = gGraphicManager->getRenderMode()->efbHeight;

    mTexDataSize = GXGetTexBufferSize(mWidth, mHeight, MY_TEX_FORMAT, FALSE, 0);
    mTexData = new (eHeap_MEM2, 32) u8[mTexDataSize];
    memset(mTexData, 0x00, mTexDataSize);
    DCFlushRange(mTexData, mTexDataSize);

    GXInitTexObj(&mTexObj, mTexData, mWidth, mHeight, MY_TEX_FORMAT, GX_CLAMP, GX_CLAMP, FALSE);
}

CDynTex::~CDynTex(void) {
    delete[] static_cast<u8 *>(mTexData);
}

void CDynTex::update(GXPixelFmt pixelFmt, GXZFmt zFmt) {
    GXPixelFmt prevPixelFmt; GXZFmt prevZFmt;
    GXGetPixelFmt(&prevPixelFmt, &prevZFmt);

    GXSetPixelFmt(pixelFmt, zFmt);

    GXSetCopyClamp(GX_CLAMP_ALL);

    GXSetTexCopySrc(mOriginX, mOriginY, mWidth, mHeight);
    GXSetTexCopyDst(mWidth, mHeight, MY_TEX_FORMAT, FALSE);

    gGraphicManager->fn_801D6680();

    mDrwUpdateFn(mTexData);

    GXDrawDone();

    DCFlushRange(mTexData, mTexDataSize);

    GXCopyTex(mTexData, TRUE);

    GXPixModeSync();

    GXSetPixelFmt(prevPixelFmt, prevZFmt);

    gGraphicManager->fn_801D6680();
}
