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
    switch (mUnk04) {
    case 2:
        if (mUnk08 != 0) {
            mUnk08--;
        }
        else {
            if (gFileManager->fn_801D431C()) {
                mUnk04 = 3;
                mUnk08 = 2;
            }
        }
        break;
    
    case 3:
        mUnk08--;
        if (mUnk08 == 0) {
            mUnk04 = 0;
        }
        break;
    }

    if (mUnk10 != 0.0f && !gTickFlowManager->getPaused()) {
        mUnk0C += mUnk10;
        if ((mUnk10 > 0.0f) && (mUnk0C >= 255.0f)) {
            mUnk0C = 255.0f;
            mUnk10 = 0.0f;
        }
        else if ((mUnk10 < 0.0f) && (mUnk0C <= 0.0f)) {
            mUnk0C = 0.0f;
            mUnk10 = 0.0f;
        }
    }
}

void CFaderFlash::_0C(void) {
    if (mUnk04 < 1 || mUnk04 > 3) {
        return;
    }
    _24(0xFFu);
}

void CFaderFlash::fn_80007DD8(void) {
    if (mUnk0C == 0.0f) {
        return;
    }
    _24(mUnk0C);
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
    PSMTXIdentity(mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);

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
    mUnk08 = 5;
    mUnk04 = 2;
}

void CFaderFlash::_18(void) {}

void CFaderFlash::_1C(void) {
    mUnk0C = 0.0f;
    mUnk10 = 0.0f;
}

void CFaderFlash::_20(void) {}

void CFaderFlash::fn_800080B0(f32 f1) {
    mUnk10 = 256.0f / f1;
}

void CFaderFlash::fn_800080C0(f32 f1) {
    mUnk10 = -256.0f / f1;
}

bool CFaderFlash::fn_800080D0(void) {
    return (mUnk10 < -0.01) || (0.01 < mUnk10);
}

CFaderFlash::~CFaderFlash(void) {
    _20();
}
