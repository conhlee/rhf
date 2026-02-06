#include "CellAnimRenderEff.hpp"

#include <revolution/GX.h>

static void fn_8009DF2C(void) {
    GXSetTevDirect(GX_TEVSTAGE0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX_IDENT, FALSE, GX_DUALMTX_IDENT);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZMode(FALSE, GX_ALWAYS, FALSE);
    GXSetCurrentMtx(0);
}

static void fn_8009E078(void) {
    fn_8009DF2C();
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
}

static void fn_8009E0B0(void) {
    fn_8009DF2C();
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
}

static void fn_8009E0E8(void) {
    fn_8009DF2C();
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetAlphaCompare(GX_LESS, 0xFF, GX_AOP_OR, GX_NEVER, 0x00);
}

static void fn_8009E138(void) {
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

static void fn_8009E150(void) {
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
}

static void fn_8009E168(void) {
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetAlphaCompare(GX_ALWAYS, 0x00, GX_AOP_OR, GX_ALWAYS, 0x00);
}

static ECellAnimRenderEff sCurrentEffType;

ECellAnimRenderEff fn_8009E1B4(ECellAnimRenderEff type) {
    ECellAnimRenderEff prevType = sCurrentEffType;
    sCurrentEffType = type;

    return prevType;
}

ECellAnimRenderEff fn_8009E1C4(void) {
    return fn_8009E1B4(eCellAnimRenderEff_None);
}

ECellAnimRenderEff fn_8009E1D4(void) {
    return fn_8009E1B4(eCellAnimRenderEff_Grayscale);
}

ECellAnimRenderEff fn_8009E1E4(void) {
    return fn_8009E1B4(eCellAnimRenderEff_Invert);
}

ECellAnimRenderEff fn_8009E1F4(void) {
    return fn_8009E1B4(eCellAnimRenderEff_ShowEdge);
}

CCellAnimManager::DrawSetupFn fn_8009E204(void) {
    switch (sCurrentEffType) {
    case eCellAnimRenderEff_None:
        return NULL;
    case eCellAnimRenderEff_Grayscale:
        return fn_8009E078;
    case eCellAnimRenderEff_Invert:
        return fn_8009E0B0;
    case eCellAnimRenderEff_ShowEdge:
        return fn_8009E0E8;
    
    default:
        return NULL;
    }
}

CCellAnimManager::DrawRestoreFn fn_8009E260(void) {
    switch (sCurrentEffType) {
    case eCellAnimRenderEff_None:
        return NULL;
    case eCellAnimRenderEff_Grayscale:
        return fn_8009E138;
    case eCellAnimRenderEff_Invert:
        return fn_8009E150;
    case eCellAnimRenderEff_ShowEdge:
        return fn_8009E168;
    
    default:
        return NULL;
    }
}
