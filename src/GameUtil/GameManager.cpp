#include "GameManager.hpp"

#include "Mem.hpp"

#include "FileManager.hpp"
#include "TickFlowManager.hpp"
#include "InputCheckManager.hpp"
#include "CheckPointManager.hpp"
#include "SoundManager.hpp"
#include "GraphicManager.hpp"

#include "VarParam.hpp"

#include "Controller.hpp"

#include <revolution/OS.h>
#include <revolution/DVD.h>

#include "DiscMessages.hpp"

#include <revolution/VI.h>

#include <revolution/GX.h>

#include <nw4r/ut.h>

#include <nw4r/db/assert.h>

#include "LayoutManager.hpp"

#include "SNDHeap.hpp"

struct DVDMessageData {
    nw4r::ut::ResFont font;
    wchar_t *messageStr;

    ~DVDMessageData(void);

    void fn_801D77A4();
    void DONT_INLINE fn_801D7A74();
};
static DVDMessageData sDVDMessageData;

CGameManager::CGameManager(void) {}

CGameManager::~CGameManager(void) {
    _08();
}

void CGameManager::_10(void) {}

void CGameManager::_08(void) {}

extern u8 lbl_80316560[0x8180]; // DVD message font data.

void CGameManager::_14(void) {
    mCurrentScene = NULL;
    mNextSceneCreateFunc = NULL;
    mNextTickFlowCode = NULL;

    sDVDMessageData.font.SetResource(lbl_80316560);
}

void CGameManager::_18(void) {}

void CGameManager::_1C(CScene::CreateFn initSceneCreateFn, CFaderFlash *fader, u16 initSceneHeapGroup) {
    u32 freeInMEM1 = MEMGetTotalFreeSizeForExpHeap(lbl_80320F80); // Unused
    u32 freeInMEM2 = MEMGetTotalFreeSizeForExpHeap(lbl_80320F84); // Unused

    mCurrentScene = initSceneCreateFn(initSceneHeapGroup);
    mFader = fader;

    while (TRUE) {
        if (mCurrentScene->getState() == CScene::eState_Active) {
            gControllerManager->_18();
        }

        gFileManager->fn_801D49D4();
        gFileManager->fn_801D4544();

        gTickFlowManager->_18();

        gInputCheckManager->fn_801E8A08();

        gCheckPointManager->fn_801EAE20();

        if (mCurrentScene->getState() == CScene::eState_Dead) {
            u16 heapGroup = mCurrentScene->getHeapGroup();
            mCurrentScene->fn_801D83BC();

            fn_801D375C(heapGroup);
            fn_801D3770(heapGroup);

            mCurrentScene = mNextSceneCreateFunc(mNextSceneHeapGroup);
        }

        bool wasLoading = mCurrentScene->getState() == CScene::eState_Loading;

        mCurrentScene->fn_801D83DC();

        mFader->_08();

        gSoundManager->fn_801E4D60();
        
        bool nowReady = mCurrentScene->getState() == CScene::eState_Ready;

        if (wasLoading && nowReady && (mNextTickFlowCode != NULL)) {
            gTickFlowManager->fn_801E1CC0(mNextTickFlowCode);
            mNextTickFlowCode = NULL;
        }

        gGraphicManager->fn_801D63B4();

        mCurrentScene->fn_801D8554();

        mFader->_0C();

        mCurrentScene->_08();

        gVarParamManager->fn_801ED2AC();

        gGraphicManager->fn_801D6478();

        if (mCurrentScene->getState() == CScene::eState_Active) {
            gControllerManager->_1C();
        }
    }
}

void CGameManager::_20(CScene::CreateFn sceneCreateFn, u16 heapGroup) {
    mNextSceneCreateFunc = sceneCreateFn;
    mNextSceneHeapGroup = heapGroup;
    mCurrentScene->fn_801D8578();
}

static bool sDVDMesgIsPowerOff;
static void dvdMesgPowerCallback(void) {
    sDVDMesgIsPowerOff = true;
}

static bool sDVDMesgIsReset;
static void dvdMesgResetCallback(void) {
    sDVDMesgIsReset = true;
}

// Display the DVD error screen & wait for the issue to be resolved.
void CGameManager::fn_801D7538(s32 driveStatus) {
    switch (driveStatus) {
    case DVD_STATE_NO_DISK:
        sDVDMessageData.messageStr = lbl_80316400;
        break;
    case DVD_STATE_WRONG_DISK:
        sDVDMessageData.messageStr = lbl_8031645C;
        break;
    case DVD_STATE_RETRY:
        sDVDMessageData.messageStr = lbl_803164B8;
        break;
    default:
        break;
    }

    sDVDMesgIsPowerOff = false;
    sDVDMesgIsReset = false;

    OSStateCallback prevResetCallback = OSSetResetCallback(dvdMesgResetCallback);
    OSStateCallback prevPowerCallback = OSSetPowerCallback(dvdMesgPowerCallback);

    while (driveStatus == static_cast<s32>(DVDGetDriveStatus())) {
        gGraphicManager->fn_801D63B4();

        if (CSoundManager::fn_801E4D4C()) {
            gSoundManager->fn_801E4D60();
        }

        sDVDMessageData.fn_801D77A4();

        if (sDVDMesgIsReset) {
            VISetBlack(TRUE);
            VIFlush();
            VIWaitForRetrace();
            VIWaitForRetrace();
            OSReturnToMenu();
        }
        else if (sDVDMesgIsPowerOff) {
            VISetBlack(TRUE);
            VIFlush();
            VIWaitForRetrace();
            VIWaitForRetrace();
            OSShutdownSystem();
        }

        gGraphicManager->fn_801D6478();
    }

    while (
        (CSoundManager::fn_801E4D4C() && gSoundManager->fn_801E7334()) ||
        gFileManager->fn_801D3D58()
    ) {
        gGraphicManager->fn_801D63B4();

        if (CSoundManager::fn_801E4D4C()) {
            gSoundManager->fn_801E4D60();
        }

        sDVDMessageData.fn_801D77A4();

        if (sDVDMesgIsReset) {
            VISetBlack(TRUE);
            VIFlush();
            VIWaitForRetrace();
            VIWaitForRetrace();
            OSReturnToMenu();
        }
        else if (sDVDMesgIsPowerOff) {
            VISetBlack(TRUE);
            VIFlush();
            VIWaitForRetrace();
            VIWaitForRetrace();
            OSShutdownSystem();
        }

        gGraphicManager->fn_801D6478();
    }

    OSSetResetCallback(prevResetCallback);
    OSSetPowerCallback(prevPowerCallback);
}

DVDMessageData::~DVDMessageData(void) {}

void DVDMessageData::fn_801D77A4(void) {
    u32 sc[4];
    f32 vp[GX_VIEWPORT_SZ];

    GXGetViewportv(vp);
    GXGetScissor(&sc[0], &sc[1], &sc[2], &sc[3]);

    f32 near = 0.0f;
    f32 far = 1.0f;

    f32 top = 0.0f;
    f32 bottom = 456.0f;
    f32 left = 0.0f;
    f32 right = 608.0f;

    Mtx44 projMtx;
    C_MTXOrtho(projMtx, top, bottom, left, right, near, far);
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

    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 0xFF});

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition2f32(left, top);
    GXColor1u32(0xFFFFFFFF);

    GXPosition2f32(left, bottom);
    GXColor1u32(0xFFFFFFFF);

    GXPosition2f32(right, bottom);
    GXColor1u32(0xFFFFFFFF);

    GXPosition2f32(right, top);
    GXColor1u32(0xFFFFFFFF);

    GXEnd();

    fn_801D7A74();

    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetScissor(sc[0], sc[1], sc[2], sc[3]);
}

void DVDMessageData::fn_801D7A74(void) {
    nw4r::ut::WideTextWriter textWriter;

    textWriter.SetFont(this->font);
    textWriter.SetCharSpace(2.0f);
    textWriter.SetFontSize(15.0f, 22.5f);
    textWriter.SetTextColor(nw4r::ut::Color::WHITE);

    const wchar_t *string = this->messageStr;

    nw4r::ut::Rect textRect;
    textWriter.CalcStringRect(&textRect, string);

    textWriter.SetupGX();

    f32 cursorX = (608.0f - (textRect.right - textRect.left)) / 2.0f;
    f32 cursorY = (456.0f - (textRect.bottom - textRect.top)) / 2.0f;

    textWriter.SetCursor(cursorX, cursorY);
    textWriter.Print(string);
}

bool CGameManager::_2C(void) {
    return mFader->getUnk04() == 2;
}

void CGameManager::_28(void) {
    mFader->_10();
}

void CGameManager::_24(CFaderFlash *fader) {
    mFader->fn_801D859C();
    mFader = fader;
}
