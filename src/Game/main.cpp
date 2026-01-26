#include <revolution/types.h>

#include <revolution/OS.h>
#include <revolution/VI.h>
#include <revolution/SC.h>
#include <revolution/GX.h>

#include <cstring>

#include "GameManager.hpp"
#include "BackupManager.hpp"
#include "FileManager.hpp"
#include "Sound.hpp"
#include "HBMManager.hpp"
#include "LayoutManager.hpp"
#include "MessageManager.hpp"
#include "GraphicManager.hpp"
#include "CellAnimManager.hpp"
#include "TickFlowManager.hpp"
#include "InputCheckManager.hpp"
#include "SceneManager.hpp"
#include "SaveData.hpp"
#include "PlayHistory.hpp"
#include "CafeTopic.hpp"
#include "RFLManager.hpp"
#include "SDManager.hpp"
#include "VarParam.hpp"
#include "DebugFont.hpp"
#include "DebugPrint.hpp"
#include "DummyManager.hpp"

#include "ExFlow.hpp"

#include "Controller.hpp"

#include "ExController.hpp"

#include "SNDHeap.hpp"

#include "rev_tengoku.rsid"

#include "FaderFlash.hpp"

#include "Strap/SceneStrap.hpp"
#include "Error/SceneError.hpp"

#include "Mem.hpp"

#include "SoundData.inc"

struct FontInitData {
    const char *binName;
    const char *arcPath;
};

static const char sFontBinName[] = "Riq_I4_28_RodinNTLGProDB.brfnt";
static const char sFontArcPath[] = "content2/Riq_I4_28_RodinNTLGProDB.szs";
static const FontInitData sFontInitData = { sFontBinName, sFontArcPath };

static const GXRenderModeObj sRenderModeNtscInt456 = {
    0,
    640, 456, 456,
    25, 12,
    670, 456,
    1,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 7, 7, 12, 12, 12, 7, 7 }
};

static const GXRenderModeObj sRenderModeNtscProg456 = {
    2,
    640, 456, 456,
    25, 12,
    670, 456,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 21, 22, 21, 0, 0 }
};

static const GXRenderModeObj sRenderModePalInt456 = {
    4,
    640, 456, 542,
    27, 16,
    666, 542,
    1,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 7, 7, 12, 12, 12, 7, 7 }
};

static const GXRenderModeObj sRenderModeEuRgb60Int456 = {
    20,
    640, 456, 456,
    25, 12,
    670, 456,
    1,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 7, 7, 12, 12, 12, 7, 7 }
};

static const GXRenderModeObj sRenderModeEuRgb60Prog456 = {
    22,
    640, 456, 456,
    25, 12,
    670, 456,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 21, 22, 21, 0, 0 }
};

extern "C" void fn_8008E430(void);

DONT_INLINE void fn_80038350(void);
DONT_INLINE void fn_80038AE4(void);
DONT_INLINE void fn_800393DC(void);
DONT_INLINE void fn_80039900(void);

// @bug Should be initialized to false, but left uninitialized.
// Used in DVD error functions to check if managers are up.
static bool sInitIsDone;

// DVD error has started (front)
void funcDVDErrorF(void) {
    OSReport("funcDVDErrorF\n");

    if (sInitIsDone) {
        gSoundManager->fn_801E6F98(true);

        gControllerManager->fn_801D5FF0(0)->_44();
        gControllerManager->fn_801D5FF0(1)->_44();
        gControllerManager->fn_801D5FF0(2)->_44();
        gControllerManager->fn_801D5FF0(3)->_44();
    }
}

// DVD error has ended (back)
void funcDVDErrorB(void) {
    OSReport("funcDVDErrorB\n");

    if (
        sInitIsDone &&
        (
            gGameManager->getCurrentScene() == NULL ||
            !gGameManager->getCurrentScene<CExScene>()->fn_80009AA0()
        ) &&
        (gHBMManager == NULL || !gHBMManager->getUnk414())
    ) {
        gSoundManager->fn_801E6F98(false);
    }
}

void main(int argc, char **argv) {
#pragma unused(argc)
#pragma unused(argv)

    fn_80038350();

    OSInit();
    OSInitFastCast();

    s32 tickBeforeInit = OSGetTick();

    fn_80038AE4();
    gGameManager->_18();
    fn_80039900();

    s32 tickAfterInit = OSGetTick();

    OSReport("Init Process Took %d msecs\n", OS_TICKS_TO_MSEC(tickAfterInit - tickBeforeInit));

    CExScene::fn_80009F70(false);

    sInitIsDone = true;

    if (gBackupManager->getUnk4C() == 0) {
        gGameManager->startMainLoop<CSceneStrap>();
    }
    else {
        gGameManager->startMainLoop<CSceneError>();
    }

    // End all managers
    fn_800393DC();

    // @bug gFileManager is used directly after destruction ..
    gFileManager->fn_801D41CC(0);

    // @bug This call requires gCellAnimManager and gFileManager, which are gone by now ..
    CExScene::fn_8000966C();
}

static GXRenderModeObj sRenderModeObj;
static bool sIs60FPS;

void fn_80038350(void) {
    VIInit();

    GXRenderModeObj ntscInt456 = sRenderModeNtscInt456;
    GXRenderModeObj ntscProg456 = sRenderModeNtscProg456;
    GXRenderModeObj palInt456 = sRenderModePalInt456;
    GXRenderModeObj euRgb60Int456 = sRenderModeEuRgb60Int456;
    GXRenderModeObj euRgb60Prog456 = sRenderModeEuRgb60Prog456;

    bool progressive = SCGetProgressiveMode() == SC_PROGRESSIVE;
    bool pal60 = SCGetEuRgb60Mode() == SC_EURGB_60_HZ;
    bool stdAspect = SCGetAspectRatio() == SC_ASPECT_STD;
    bool isNtsc = VIGetTvFormat() == VI_TVFORMAT_NTSC;

    if ((VIGetDTVStatus() == TRUE) && progressive) {
        if (isNtsc) {
            sRenderModeObj = ntscProg456;
            sIs60FPS = true;
        }
        else {
            sRenderModeObj = euRgb60Prog456;
            sIs60FPS = true;
        }
    }
    else {
        if (isNtsc) {
            sRenderModeObj = ntscInt456;
            sIs60FPS = true; 
        }
        else {
            if (pal60) {
                sRenderModeObj = euRgb60Int456;
                sIs60FPS = true;
            }
            else {
                sRenderModeObj = palInt456;
                sIs60FPS = false;
            }
        }
    }

    if (!stdAspect) {
        if (isNtsc) {
            sRenderModeObj.viWidth = 686;
            sRenderModeObj.viXOrigin = 17;
        }
        else {
            if (pal60) {
                sRenderModeObj.viWidth = 686;
                sRenderModeObj.viXOrigin = 17;
            }
            else {
                sRenderModeObj.viWidth = 682;
                sRenderModeObj.viXOrigin = 19;
            }
        }
    }
}

TFD_EXTERN(lbl_802BAD10);

void fn_80038AE4(void) {
    fn_801D3564();
    
    fn_801D3568();

    if (gGameManager == NULL) {
        gGameManager = new CGameManager;
    }

    gGameManager->_10();
    gGameManager->_14();

    if (gFileManager == NULL) {
        gFileManager = new CFileManager;
    }

    gFileManager->_10();
    gFileManager->_14();
    gFileManager->setDVDErrorFuncF(funcDVDErrorF);
    gFileManager->setDVDErrorFuncB(funcDVDErrorB);
    switch (SCGetLanguage()) {
    default:
    case SC_LANG_EN:
        gFileManager->setLocaleDir("EN/");
        break;
    }

    GXColor clearColor = (GXColor){ 0, 0, 0, 0 };

    if (gGraphicManager == NULL) {
        gGraphicManager = new CGraphicManager;
    }

    gGraphicManager->_10();
    gGraphicManager->_14(&sRenderModeObj, &clearColor, 0xFFFFFF);

    if (gControllerManager == NULL) {
        gControllerManager = new CControllerManager;
    }

    gControllerManager->_10(CExController::create);
    gControllerManager->_14();

    if (gLayoutManager == NULL) {
        gLayoutManager = new CLayoutManager;
    }

    gLayoutManager->_10();
    gLayoutManager->_14();

    if (gCellAnimManager == NULL) {
        gCellAnimManager = new CCellAnimManager;
    }

    gCellAnimManager->_10(0x2800);
    gCellAnimManager->_14();

    if (gHBMManager == NULL) {
        gHBMManager = new CHBMManager;
    }

    gHBMManager->_10();
    gHBMManager->_14();

    if (gMessageManager == NULL) {
        gMessageManager = new CMessageManager;
    }

    gMessageManager->fn_80088030();

    static char sndArcPath[64];
    sprintf(sndArcPath, "%s%s", gFileManager->getLocaleDir(), "content2/rev_tengoku.brsar");

    if (gSoundManager == NULL) {
        gSoundManager = new CSoundManager;
    }

    gSoundManager->_14(sndArcPath);
    gSoundManager->_1C();

    gSoundManager->fn_801E6E00(PLAYER_SE_SYSTEM);

    gSoundManager->fn_801E60D4(lbl_802E7370, ARRAY_LENGTH(lbl_802E7370));
    gSoundManager->fn_801E73C8(lbl_802E8210, ARRAY_LENGTH(lbl_802E8210));
    gSoundManager->fn_801E75B4(lbl_802E8720, ARRAY_LENGTH(lbl_802E8720));

    fn_8008357C();

    fn_80083630(GROUP_COMMON, eSoundHeap_0);
    fn_80083630(GROUP_PRACTICE, eSoundHeap_0);
    fn_8008367C(eSoundHeap_0);

    gSoundManager->fn_801E4D54();

    if (gTickFlowManager == NULL) {
        gTickFlowManager = new CTickFlowManager;
    }

    gTickFlowManager->_10(0x3F000);
    gTickFlowManager->registerFlowInitial<CExFlow>(NULL);

    gTickFlowManager->setUnkF8(lbl_802BAD10);

    gTickFlowManager->fn_801E4170(sIs60FPS);

    if (gInputCheckManager == NULL) {
        gInputCheckManager = new CInputCheckManager;
    }

    gInputCheckManager->_10(0x1F000);
    gInputCheckManager->_14();

    gInputCheckManager->setUnk414(fn_8008E430);

    if (gCheckPointManager == NULL) {
        gCheckPointManager = new CCheckPointManager;
    }

    gCheckPointManager->_10();
    gCheckPointManager->_14();

    if (gSceneManager == NULL) {
        gSceneManager = new CSceneManager;
    }

    gSceneManager->_10();
    gSceneManager->_14();
    
    if (gSaveData == NULL) {
        gSaveData = new CSaveData;
    }

    gSaveData->fn_80078E04();
    gSaveData->fn_80078E8C();

    if (gBackupManager == NULL) {
        gBackupManager = new CBackupManager;
    }

    gBackupManager->fn_80084054();

    s32 curBakAttempt = gBackupManager->fn_800843E4();
    if (curBakAttempt == 0) {
        curBakAttempt = gBackupManager->fn_8008452C();
    }
    if (curBakAttempt == 0) {
        curBakAttempt = gBackupManager->fn_80084A20();
    }
    if (curBakAttempt == 0) {
        curBakAttempt = gBackupManager->fn_8008516C();
    }
    if (curBakAttempt == 0) {
        gBackupManager->fn_80084D1C();
    }

    if (gPlayHistory == NULL) {
        gPlayHistory = new CPlayHistory;
    }

    gPlayHistory->fn_8009A704();

    if (gCafeTopic == NULL) {
        gCafeTopic = new CCafeTopic;
    }

    gCafeTopic->fn_8009CE34();

    if (gRFLManager == NULL) {
        gRFLManager = new CRFLManager;
    }

    gRFLManager->_10();

    if (gSDManager == NULL) {
        gSDManager = new CSDManager;
    }

    gSDManager->_10();
    gSDManager->_14();
    gSDManager->fn_801EC3A8("REVO_RIQ");

    if (gVarParamManager == NULL) {
        gVarParamManager = new CVarParamManager;
    }

    gVarParamManager->_10(0x80);
    gVarParamManager->_14();

    if (gDebugFont == NULL) {
        gDebugFont = new CDebugFont;
    }

    gDebugFont->_10();

    if (gDebugConsole == NULL) {
        gDebugConsole = new CDebugConsole;
    }

    gDebugConsole->_10(0);

    if (gDebugPrint == NULL) {
        gDebugPrint = new CDebugPrint;
    }

    gDebugPrint->_10();
}

void fn_800393DC(void) {
    gRFLManager->fn_800C2E04();
    gRFLManager->fn_800C2C98();

    if (gRFLManager != NULL) {
        delete gRFLManager;
    }
    gRFLManager = NULL;

    if (gCafeTopic != NULL) {
        delete gCafeTopic;
    }
    gCafeTopic = NULL;

    if (gPlayHistory != NULL) {
        delete gPlayHistory;
    }
    gPlayHistory = NULL;

    if (gSaveData != NULL) {
        delete gSaveData;
    }
    gSaveData = NULL;

    if (gSceneManager != NULL) {
        delete gSceneManager;
    }
    gSceneManager = NULL;

    if (gBackupManager != NULL) {
        delete gBackupManager;
    }
    gBackupManager = NULL;

    if (gHBMManager != NULL) {
        delete gHBMManager;
    }
    gHBMManager = NULL;

    if (gMessageManager != NULL) {
        delete gMessageManager;
    }
    gMessageManager = NULL;

    if (gDebugPrint != NULL) {
        delete gDebugPrint;
    }
    gDebugPrint = NULL;

    if (gDebugConsole != NULL) {
        delete gDebugConsole;
    }
    gDebugConsole = NULL;

    if (gDebugFont != NULL) {
        delete gDebugFont;
    }
    gDebugFont = NULL;

    if (gVarParamManager != NULL) {
        delete gVarParamManager;
    }
    gVarParamManager = NULL;

    if (gSDManager != NULL) {
        delete gSDManager;
    }
    gSDManager = NULL;

    if (gSoundManager != NULL) {
        delete gSoundManager;
    }
    gSoundManager = NULL;

    if (gGameManager != NULL) {
        delete gGameManager;
    }
    gGameManager = NULL;

    if (gCheckPointManager != NULL) {
        delete gCheckPointManager;
    }
    gCheckPointManager = NULL;

    if (gInputCheckManager != NULL) {
        delete gInputCheckManager;
    }
    gInputCheckManager = NULL;

    if (gTickFlowManager != NULL) {
        delete gTickFlowManager;
    }
    gTickFlowManager = NULL;

    if (gCellAnimManager != NULL) {
        delete gCellAnimManager;
    }
    gCellAnimManager = NULL;

    if (gDummyManager != NULL) {
        delete gDummyManager;
    }
    gDummyManager = NULL;

    if (gLayoutManager != NULL) {
        delete gLayoutManager;
    }
    gLayoutManager = NULL;

    if (gControllerManager != NULL) {
        delete gControllerManager;
    }
    gControllerManager = NULL;

    if (gGraphicManager != NULL) {
        delete gGraphicManager;
    }
    gGraphicManager = NULL;

    if (gFileManager != NULL) {
        delete gFileManager;
    }
    gFileManager = NULL;
}

void fn_80039900(void) {
    fn_801D369C(eHeapGroup_CommonAsset);

    gLayoutManager->fn_801D6DAC(1);

    const char *fontBinName = sFontInitData.binName;
    const char *fontArcPath = sFontInitData.arcPath;

    void *fontData = gFileManager->fn_801D3C4C(fontArcPath, eHeap_MEM2, -32);
    gFileManager->fn_801D3D94();

    if (strstr(fontArcPath, ".szs") != NULL) {
        fontData = gFileManager->fn_801D461C(fontData, TRUE);
    }

    if (gLayoutManager->fn_801D6E2C(fontData, fontBinName)) {
        delete[] static_cast<u8 *>(fontData);
    }

    gMessageManager->fn_80088034();
    gMessageManager->fn_80088088();

    CExScene::fn_80009028();

    fn_801D3644();
}
