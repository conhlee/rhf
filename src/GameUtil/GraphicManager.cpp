#include "GraphicManager.hpp"

#include "Mem.hpp"

#include <revolution/VI.h>

#include <nw4r/g3d.h>

static GXRenderModeObj sRenderModeObj;

CGraphicManager::CGraphicManager(void) {}

CGraphicManager::~CGraphicManager(void) {
    _08();
}

void CGraphicManager::_10(void) {}

void CGraphicManager::_08(void) {}

void CGraphicManager::_14(GXRenderModeObj *renderMode, GXColor *clearColor, u32 clearZ) {
    mScreenBlack = true;

    fn_801D6514(renderMode);

    u32 frameBufferSize = (u16)ROUND_UP(mRenderMode->fbWidth, 16) * mRenderMode->xfbHeight * 2;
    mFrameBufferSize = frameBufferSize;

    mFifoMem = new (eHeap_MEM1, 32) u8[0x40000];
    mFrameBuffers[0] = new (eHeap_MEM2, 32) u8[frameBufferSize];
    mFrameBuffers[1] = new (eHeap_MEM2, 32) u8[frameBufferSize];

    mFifoObj = GXInit(mFifoMem, 0x40000);

    if (clearColor != NULL) {
        GXSetCopyClear(*clearColor, clearZ);
    }

    GXSetViewport(0.0f, 0.0f, mRenderMode->fbWidth, mRenderMode->efbHeight, 0.0f, 1.0f);
    GXSetScissor(0, 0, mRenderMode->fbWidth, mRenderMode->efbHeight);

    f32 yScaleFactor = GXGetYScaleFactor(mRenderMode->efbHeight, mRenderMode->xfbHeight);
    u16 yLineCount = GXSetDispCopyYScale(yScaleFactor);

    GXSetDispCopySrc(0, 0, mRenderMode->fbWidth, mRenderMode->efbHeight);
    GXSetDispCopyDst(mRenderMode->fbWidth, yLineCount);

    GXSetCopyFilter(mRenderMode->aa, mRenderMode->sample_pattern, TRUE, mRenderMode->vfilter);

    GXSetDispCopyGamma(GX_GM_1_0);

    if (mRenderMode->aa != 0) {
        GXSetPixelFmt(GX_PF_RGBA565_Z16, GX_ZC_LINEAR);
    }
    else {
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }

    GXCopyDisp(mFrameBuffers[1], TRUE);

    VIConfigure(mRenderMode);

    VISetNextFrameBuffer(mFrameBuffers[0]);
    mCurrentFrameBuf = mFrameBuffers[1];

    VIFlush();
    VIWaitForRetrace();
    if (mRenderMode->tvInfo & 1) {
        VIWaitForRetrace();
    }

    nw4r::g3d::G3dInit(true);
}

void CGraphicManager::fn_801D63B4(void) {
    GXRenderModeObj *rmode = mRenderMode;

    if (rmode->field_rendering != 0) {
        u32 nextField = VIGetNextField();
        GXSetViewportJitter(0.0f, 0.0f, rmode->fbWidth, rmode->efbHeight, 0.0f, 1.0f, nextField);
    }
    else {
        GXSetViewport(0.0f, 0.0f, rmode->fbWidth, rmode->efbHeight, 0.0f, 1.0f);
    }

    GXInvalidateVtxCache();
    GXInvalidateTexAll();

    nw4r::g3d::G3dReset();
}

void CGraphicManager::fn_801D6478(void) {
    GXSetZMode(TRUE, GX_LEQUAL, TRUE);
    GXSetColorUpdate(TRUE);
    GXCopyDisp(mCurrentFrameBuf, TRUE);
    GXDrawDone();

    VISetNextFrameBuffer(mCurrentFrameBuf);
    if (mScreenBlack) {
        VISetBlack(FALSE);
        mScreenBlack = false;
    }

    VIFlush();
    VIWaitForRetrace();

    if (mCurrentFrameBuf == mFrameBuffers[0]) {
        mCurrentFrameBuf = mFrameBuffers[1];
    }
    else {
        mCurrentFrameBuf = mFrameBuffers[0];
    }
}

void CGraphicManager::fn_801D6514(GXRenderModeObj *renderMode) {
    if (renderMode != NULL) {
        sRenderModeObj = *renderMode;
        mRenderMode = &sRenderModeObj;
    }
    else {
        u32 tvFormat = VIGetTvFormat();
        switch (tvFormat) {
        case VI_TVFORMAT_NTSC:
            mRenderMode = &GXNtsc480IntDf;
            break;
        case VI_TVFORMAT_PAL:
            mRenderMode = &GXPal528IntDf;
            break;
        case VI_TVFORMAT_EURGB60:
            mRenderMode = &GXEurgb60Hz480IntDf;
            break;
        case VI_TVFORMAT_MPAL:
            mRenderMode = &GXMpal480IntDf;
            break;
        }

        GXAdjustForOverscan(mRenderMode, &sRenderModeObj, 0, 16);

        mRenderMode = &sRenderModeObj;
    }
}

void CGraphicManager::fn_801D6680(void) {
    GXCopyDisp(mCurrentFrameBuf, TRUE);
}
