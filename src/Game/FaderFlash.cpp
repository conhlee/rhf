#include "FaderFlash.hpp"

#include <revolution/GX.h>

#include "FileManager.hpp"

#include "TickFlowManager.hpp"

CFaderFlash *CFaderFlash::fn_80007C28(void) {
    CFaderFlash *fader = new CFaderFlash;
    fader->_18();
    fader->_1C();

    return fader;
}

void CFaderFlash::_08(void) {
    switch (mState) {
    case eState_Wait1:
        if (mWaitFrames != 0) {
            mWaitFrames--;
        }
        else {
            if (gFileManager->getArcIdle()) {
                mState = eState_Wait2;
                mWaitFrames = 2;
            }
        }
        break;
    case eState_Wait2:
        mWaitFrames--;
        if (mWaitFrames == 0) {
            mState = eState_Idle;
        }
        break;
    default:
        break;
    }

    if ((mAlphaStep != 0.0f) && !gTickFlowManager->getPaused()) {
        mAlpha += mAlphaStep;
        if ((mAlphaStep > 0.0f) && (mAlpha >= 255.0f)) {
            mAlpha = 255.0f;
            mAlphaStep = 0.0f;
        }
        else if ((mAlphaStep < 0.0f) && (mAlpha <= 0.0f)) {
            mAlpha = 0.0f;
            mAlphaStep = 0.0f;
        }
    }
}

void CFaderFlash::_0C(void) {
    if ((mState != eState_1) && (mState != eState_Wait1) && (mState != eState_Wait2)) {
        return;
    }
    _24(0xFF);
}

void CFaderFlash::fn_80007DD8(void) {
    if (mAlpha == 0.0f) {
        return;
    }
    _24(static_cast<u8>(mAlpha));
}

void CFaderFlash::_24(u8 alpha) {
    GXColor color = (GXColor){ 0, 0, 0, 0 };
    color.a = alpha;
    
    f32 coord[4];
    coord[0] = 0.0f;
    coord[1] = 0.0f;
    coord[2] = 1.0f;
    coord[3] = 1.0f;

    Mtx44 projMtx;
    C_MTXOrtho(projMtx, coord[0], coord[2], coord[1], coord[3], 0.0f, 500.0f);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

    Mtx mtx;
    MTXIdentity(mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXClearVtxDesc();
    GXInvalidateVtxCache();
    GXInvalidateTexAll();

    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumIndStages(0);
    GXSetNumTevStages(1);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_RASC,GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_RASA,GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetColorUpdate(TRUE);
    GXSetAlphaUpdate(TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetZMode(FALSE, GX_ALWAYS, FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);

    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
    
    GXSetTevColor(GX_TEVREG0, color);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition2f32(coord[0], coord[1]);
    GXColor1u32(0xFFFFFFFF);

    GXPosition2f32(coord[0], coord[3]);
    GXColor1u32(0xFFFFFFFF);

    GXPosition2f32(coord[2], coord[3]);
    GXColor1u32(0xFFFFFFFF);

    GXPosition2f32(coord[2], coord[1]);
    GXColor1u32(0xFFFFFFFF);

    GXEnd();
}

void CFaderFlash::_10(void) {
    mWaitFrames = 5;
    mState = eState_Wait1;
}

void CFaderFlash::_18(void) {}

void CFaderFlash::_1C(void) {
    mAlpha = 0.0f;
    mAlphaStep = 0.0f;
}

void CFaderFlash::_20(void) {}

void CFaderFlash::fn_800080B0(f32 frames) {
    // @bug Should actually be 255.0f / frames
    mAlphaStep = 256.0f / frames;
}

void CFaderFlash::fn_800080C0(f32 frames) {
    // @bug Should actually be -255.0f / frames
    mAlphaStep = -256.0f / frames;
}

bool CFaderFlash::fn_800080D0(void) {
    // NOTE: mAlphaStep != 0.0f would have been OK, so this is odd
    return (mAlphaStep < -0.01) || (0.01 < mAlphaStep);
}

CFaderFlash::~CFaderFlash(void) {
    _20();
}
