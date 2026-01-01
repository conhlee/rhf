#include "HBMManager.hpp"

#include <cstring>

#include <revolution/OS.h>
#include <revolution/SC.h>
#include <revolution/GX.h>
#include <revolution/VI.h>

#include <nw4r/snd.h>

#include "Scene.hpp"
#include "ExScene.hpp"

#include "FileManager.hpp"

#include "GameManager.hpp"

#include "BackupManager.hpp"

#include "TickFlowManager.hpp"

#include "InputCheckManager.hpp"

#include "SoundManager.hpp"

#include "SceneManager.hpp"

#include "Controller.hpp"

#include "code_801ED7D4.hpp"

namespace {

class CDummyScene : public CScene {
public:
    virtual ~CDummyScene(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);

    SCENE_DECL_CREATE_FN()
};

} // namespace

static int soundcallback(int evt, int num) {
    return 0;
}

CHBMManager::CHBMManager(void) {}

CHBMManager::~CHBMManager(void) {
    _08();
}

// HomeButton2 is used; this game has no Operation Guide. 
static const char hbmRootDir[] = "hbm/HomeButton2";

void CHBMManager::_10(void) {
    char pathBuf[32];

    SCInit();
    while (SCCheckStatus() != 0);

    OSInitFastCast();

    strcpy(pathBuf, hbmRootDir);
    
    mHBMInfo.region = SCGetLanguage();
    switch (mHBMInfo.region) {
    case SC_LANG_FR: {
        mHBMInfo.region = SC_LANG_FR;
        strcat(pathBuf, "/homeBtn_FRA.arc");
    } break;

    case SC_LANG_SP: {
        mHBMInfo.region = SC_LANG_SP;
        strcat(pathBuf, "/homeBtn_SPA.arc");
    } break;
    
    case SC_LANG_EN:
    default: {
        mHBMInfo.region = SC_LANG_EN;
        strcat(pathBuf, "/homeBtn_ENG.arc");
    } break;
    }

    mHBMInfo.layoutBuf = gFileManager->fn_801D3C4C(pathBuf);
    gFileManager->fn_801D3D94();

    strcpy(pathBuf, hbmRootDir);
    strcat(pathBuf, "/SpeakerSe.arc");
    mHBMInfo.spkSeBuf = gFileManager->fn_801D3C4C(pathBuf);
    gFileManager->fn_801D3D94();

    strcpy(pathBuf, hbmRootDir);
    strcat(pathBuf, "/home.csv");
    mHBMInfo.msgBuf = gFileManager->fn_801D3C4C(pathBuf);
    gFileManager->fn_801D3D94();

    strcpy(pathBuf, hbmRootDir);
    strcat(pathBuf, "/config.txt");
    mHBMInfo.configBuf = gFileManager->fn_801D3C4C(pathBuf);
    gFileManager->fn_801D3D94();

    mHBMInfo.configBufSize = strlen(static_cast<char *>(mHBMInfo.configBuf));

    mHBMWorkMem = new (eHeap_MEM1, 32) u8[0x80000];

    mHBMInfo.mem = mHBMWorkMem;
    mHBMInfo.memSize = 0x80000;
    mHBMInfo.sound_callback = soundcallback;
    mHBMInfo.backFlag = TRUE;
    mHBMInfo.cursor = 0;
    mHBMInfo.adjust.x = (832.0f / 608.0f);
    mHBMInfo.adjust.y = 1.0f;
    mHBMInfo.frameDelta = 1.0f;
    mHBMInfo.messageFlag = FALSE;
    mHBMInfo.pAllocator = NULL;
    HBMCreate(&mHBMInfo);
    HBMSetAdjustFlag(SCGetAspectRatio() != SC_ASPECT_STD);

    strcpy(pathBuf, hbmRootDir);
    strcat(pathBuf, "/HomeButtonSe.arc");
    mHBMSoundData = gFileManager->fn_801D3C4C(pathBuf);
    gFileManager->fn_801D3D94();

    mHBMSoundMem = new (eHeap_MEM2, 32) u8[0x18700];
    HBMCreateSound(mHBMSoundData, mHBMSoundMem, 0x18700);

    strcpy(pathBuf, hbmRootDir);
    strcat(pathBuf, "/homeBtnIcon.tpl");
    mHBMIconPal = static_cast<TPLPalette *>(gFileManager->fn_801D3C4C(pathBuf));
    gFileManager->fn_801D3D94();

    fn_801ED7D4(mHBMIconPal);
}

void CHBMManager::_08(void) {
    delete[] static_cast<u8 *>(mHBMWorkMem);
    delete[] static_cast<u8 *>(mHBMSoundData);
    delete[] static_cast<u8 *>(mHBMSoundMem);
}

void CHBMManager::_14(void) {
    OSSetPowerCallback(powercallback);
    OSSetResetCallback(resetcallback);

    mHBMSelectedButton = HBM_SELECT_NULL;

    mUnk414 = 0;
    mUnk415 = 0;
    mUnk416 = 1;

    mPowerCalled = false;
    mResetCalled = false;

    mUnk420 = 0;
    mProhibitIconStage = eProhibitIconStage_Null;
    mUnk419 = 0;
}

extern "C" bool lbl_80320274;

void CHBMManager::_18(void) {
    fn_80087314();

    if (gGameManager->_30()) {
        if (mUnk414 == 0) {
            bool temp0 = mUnk416 != 0;
            if (!mResetCalled && !mPowerCalled) {
                if (mUnk419 != 0) {
                    mUnk419--;
                    temp0 = false;
                }

                if (temp0) {
                    CController *controller0 = gControllerManager->fn_801D5FF0(0);
                    CController *controller1 = gControllerManager->fn_801D5FF0(1);

                    bool hasMulti = false;
                    if (
                        lbl_80320274 &&
                        (gSceneManager->fn_8008B058(0) == eScene_RhythmFighterVS)
                    ) {
                        hasMulti = true;
                    }

                    if (hasMulti) {
                        
                    }
                }
            }
        }
    }
}

void CHBMManager::_1C(void) {
    bool preparedGX = false;

    if (mUnk414) {
        fn_80086B90();
        HBMDraw();
        preparedGX = true;
    }

    if ((mProhibitIconStage != 3) && (mHBMIconPal != NULL)) {
        if (!preparedGX) {
            fn_80086B90();
        }
        fn_800870BC();
    }
}

void CHBMManager::fn_80086B90(void) {
    Mtx44 projMtx;
    if (SCGetAspectRatio() == SC_ASPECT_STD) {
        C_MTXOrtho(projMtx, 228.0f, -228.0f, -304.0, 304.0f, 0.0f, 500.0f);
    }
    else {
        C_MTXOrtho(projMtx, 228.0f, -228.0f, -416.0, 416.0f, 0.0f, 500.0f);
    }

    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

    Mtx mtx;
    PSMTXIdentity(mtx);
    GXLoadPosMtxImm(mtx, 3);

    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_BLEND);
    
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZMode(TRUE, GX_LEQUAL, TRUE);

    GXSetCurrentMtx(3);
}

extern "C" void fn_8008D000(s32, s32);
extern "C" void fn_8008D004(s32, s32);

void CHBMManager::fn_80086CCC(void) {
    mHBMSelectedButton = HBMGetSelectBtnNum();
    mUnk414 = 0;

    switch (mHBMSelectedButton) {
    case HBM_SELECT_BTN1: {
        gGameManager->_20(CDummyScene::create, eHeapGroup_Scene);
        if (gBackupManager != NULL) {
            gBackupManager->fn_80084FC8(0);
        }
    } break;

    case HBM_SELECT_BTN2: {
        gGameManager->_20(CDummyScene::create, eHeapGroup_Scene);
        if (gBackupManager != NULL) {
            gBackupManager->fn_80084FC8(0);
        }
    } break;
    
    default: {
        if (
            (gGameManager->getCurrentScene() == NULL) ||
            !gGameManager->getCurrentScene<CExScene>()->fn_80009AA0()
        ) {
            gTickFlowManager->fn_801E4048(false);
            gInputCheckManager->setGamePaused(false);
            gSoundManager->fn_801E6F98(false);
            gSoundManager->fn_801E6F40(false);
        }

        GXSetColorUpdate(TRUE);
        GXSetAlphaUpdate(TRUE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXSetZMode(FALSE, GX_ALWAYS, FALSE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);

        for (s32 chan = 0; chan < WPAD_MAX_CONTROLLERS; chan++) {
            nw4r::snd::SoundSystem::GetRemoteSpeaker(chan).Shutdown(fn_8008D004);
            nw4r::snd::SoundSystem::GetRemoteSpeaker(chan).Setup(fn_8008D000);
        }
    } break;
    }
}

SCENE_IMPL_CREATE_FN(CDummyScene)

void CHBMManager::_20(void) {
    if (mPowerCalled) {
        if (gBackupManager != NULL) {
            gBackupManager->waitAsync();
        }

        bool doShutdown;
        if ((gBackupManager != NULL) && (gBackupManager->getUnk4C() != 0)) {
            doShutdown = false;
        }
        else {
            VISetBlack(TRUE);
            VIFlush();

            WPADControlMotor(0, FALSE);
            WPADControlMotor(1, FALSE);

            VIWaitForRetrace();
            VIWaitForRetrace();
            
            doShutdown = true;
        }

        if (doShutdown) {
            OSReport("OSShutdownSystem()\n");
            OSShutdownSystem();
        }
    }
    else {
        if (mHBMSelectedButton == HBM_SELECT_BTN1) {
            if (gBackupManager != NULL) {
                gBackupManager->waitAsync();
            }

            bool doMenu;
            if ((gBackupManager != NULL) && (gBackupManager->getUnk4C() != 0)) {
                doMenu = false;
            }
            else {
                VISetBlack(TRUE);
                VIFlush();
    
                WPADControlMotor(0, FALSE);
                WPADControlMotor(1, FALSE);
    
                VIWaitForRetrace();
                VIWaitForRetrace();
                
                doMenu = true;
            }

            if (doMenu) {
                OSReport("OSReturnToMenu()\n");
                OSReturnToMenu();
            }
        }
        else if (mResetCalled || (mHBMSelectedButton == HBM_SELECT_BTN2)) {
            if (gBackupManager != NULL) {
                gBackupManager->waitAsync();
            }

            bool doRestart;
            if ((gBackupManager != NULL) && (gBackupManager->getUnk4C() != 0)) {
                doRestart = false;
            }
            else {
                VISetBlack(TRUE);
                VIFlush();
    
                WPADControlMotor(0, FALSE);
                WPADControlMotor(1, FALSE);
    
                VIWaitForRetrace();
                VIWaitForRetrace();
                
                doRestart = true;
            }

            if (doRestart) {
                OSReport("OSRestart()\n");
                OSRestart(0);
            }
        }
    }
}

void CHBMManager::fn_800870BC(void) {
    GXClearVtxDesc();

    GXSetVtxAttrFmt(GX_VTXFMT5, GX_VA_POS, GX_POS_XY, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT5, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, FALSE, 125);

    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0xFF, 0xFF, 0xFF, mProhibitIconAlpha });
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZMode(FALSE, GX_LEQUAL, FALSE);

    GXSetCurrentMtx(3);

    GXTexObj texObj;
    TPLGetGXTexObjFromPalette(mHBMIconPal, &texObj, 0);
    GXLoadTexObj(&texObj, GX_TEXMAP0);

    s32 x = 312;
    s32 y = 136;
    s32 size = 56;

    if (SCGetAspectRatio() == SC_ASPECT_STD) {
        x = 214;
    }

    s32 xEnd = x + size;
    s32 yEnd = y + size;

    GXBegin(GX_QUADS, GX_VTXFMT5, 4);
    
    GXPosition2s16(x, y);
    GXTexCoord2s16(0, 1);

    GXPosition2s16(x, yEnd);
    GXTexCoord2s16(0, 0);

    GXPosition2s16(xEnd, yEnd);
    GXTexCoord2s16(1, 0);

    GXPosition2s16(xEnd, y);
    GXTexCoord2s16(1, 1);

    GXEnd();
}

void CHBMManager::fn_80087314(void) {
    f32 msecs = OS_TICKS_TO_MSEC(OSGetTick() - mUnk420);

    switch (mProhibitIconStage) {
    case eProhibitIconStage_FadeIn: {
        mProhibitIconAlpha = ((msecs / 250.0f) * 255.9f);
        if (msecs >= 250.0f) {
            mUnk420 = OSGetTick();
            mProhibitIconStage = eProhibitIconStage_Fixed;
            mProhibitIconAlpha = 0xFF;
        }
    } break;

    case eProhibitIconStage_Fixed: {
        if ((mUnk414 != 0) || (msecs >= 1000.0f)) {
            mUnk420 = OSGetTick();
            mProhibitIconStage = eProhibitIconStage_FadeOut;
        }
    } break;

    case eProhibitIconStage_FadeOut: {
        mProhibitIconAlpha = ((250.0f - msecs) / 250.0f) * 255.9f;
        if (msecs >= 250.0f) {
            mProhibitIconAlpha = 0x00;
            mProhibitIconStage = eProhibitIconStage_Null;
        }
    } break;
    
    case eProhibitIconStage_Null:
    default:
        break;
    }
}

void CHBMManager::powercallback(void) {
    OSReport("powercallback()\n");
    gHBMManager->mPowerCalled = true;
}

void CHBMManager::resetcallback(void) {
    OSReport("resetcallback\n");
    gHBMManager->mResetCalled = true;
}

void CDummyScene::_20(void) {}

void CDummyScene::_1C(void) {}

void CDummyScene::_18(void) {
    if (gBackupManager != NULL) {
        gBackupManager->fn_8008414C();
    }
}

void CDummyScene::_14(void) {}

void CDummyScene::_10(void) {}

CDummyScene::~CDummyScene(void) {}
