#include "VarParam.hpp"

#include <revolution/GX.h>

#include "DebugPrint.hpp"

CVarParamManager::CVarParamManager(void) {
    mDisplayMode = 0;
    mUnk0C = 0;
}

CVarParamManager::~CVarParamManager(void) {
    _08();
}

void CVarParamManager::_10(s32 paramCount) {
    // @bug The heap won't end up being able to fit exactly as much CVarParam
    //      instances as paramCount might suggest because of heap- header &
    //      block data.
    u32 heapSize = ROUND_UP(paramCount * sizeof(CVarParam), 32);

    mHeapStart = new (eHeap_MEM2, 32) u8[heapSize];
    mHeap = MEMCreateExpHeap(mHeapStart, heapSize);

    mVarParamHead = NULL;
}

void CVarParamManager::_14(void) {}

void CVarParamManager::_08(void) {
    mVarParamHead->removeAll();
    mVarParamHead = NULL;

    MEMDestroyExpHeap(mHeap);
    delete[] mHeapStart;
}

CW_FORCE_STRINGS(GameUtil_VarParamManager_cpp, "==== VarParam Info ====\n", "========\n");

void CVarParamManager::fn_801ED2AC(void) {
    s32 y;
    s32 temp_r27;
    s32 paramNum;
    s32 i;
    s32 paramDrawnNum;
    
    if (mDisplayMode == 0) {
        return;
    }
    fn_801ED44C();
    
    CVarParam *param = mVarParamHead;
    if (param == NULL) {
        gDebugPrint->fn_801EC674(20, 20, 1, "No Entry");
        return;
    }
    
    
    u8 rawParamNum = 0;
    for (CVarParam *it = param; it != NULL; it = static_cast<CVarParam *>(it->getNext())) {
        rawParamNum++;
    }

    s32 temp_r5 = mUnk0C / 18;
    if (temp_r5 == ((u8)((rawParamNum - 1) / 18 + 1)) - 1) {
        paramNum = rawParamNum % 18;
        if (paramNum == 0) {
            paramNum = 18;
        }
    }
    else {
        paramNum = 18;
    }
    
    temp_r27 = temp_r5 * 18;
    i = 0;
    paramDrawnNum = 0;
    y = 40;
    for (CVarParam *it = param; it != NULL; it = static_cast<CVarParam *>(it->getNext()), i++) {
        if (i < temp_r27) {
            continue;
        }

        it->fn_801ECEC4(40, y);
        if (i == mUnk0C) {
            if (mDisplayMode == 2) {
                gDebugPrint->fn_801EC674(20, y, 1, "⇒");
            }
            else {
                gDebugPrint->fn_801EC674(20, y, 1, "→");
            }
        }
        paramDrawnNum++;
        y += 20;
        if (paramDrawnNum >= paramNum) {
            return;
        }
    }
}

void CVarParamManager::fn_801ED44C(void) {
    f32 coord[4];
    coord[0] = 0.02f;
    coord[1] = 0.02f;
    coord[2] = 0.98f;
    coord[3] = 0.98f;
    
    Mtx44 projMtx;
    C_MTXOrtho(projMtx, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 500.0f);
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
    
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0, 0, 0, 0xD8 });

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
