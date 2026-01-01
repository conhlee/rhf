#include "ExScene.hpp"

#include <revolution/SC.h>

#include <revolution/WPAD.h>

#include "ExFlow.hpp"

#include "TimeRatio.hpp"

#include "GameManager.hpp"

#include "TickFlowManager.hpp"

#include "HBMManager.hpp"

#include "SceneManager.hpp"

#include "LayoutManager.hpp"

#include "CellAnimManager.hpp"

#include "FileManager.hpp"

#include "BackupManager.hpp"

#include "CheckPointManager.hpp"

#include "InputCheckManager.hpp"

#include "SoundManager.hpp"

#include "MyCanvas.hpp"

#include "Controller.hpp"

#include "SaveData.hpp"

#include "CellAnimRenderEff.hpp"

#include "code_801ED7D4.hpp"

#include "IconSelectLayout.hpp"

#include "cellanim/permanent/rcad_permanent_labels.h"
#include "cellanim/remix/ver8/rcad_remix08_Mask_labels.h"

#include "rev_tengoku.rsid"

struct struct_80320198 {
    f32 unk00;

    struct_80320198(f32 value) {
        value = -value;
        unk00 = value;
    }
};

static struct_80320198 lbl_80320198 (1000.0f);

s32 lbl_8032019C;

static bool lbl_803201A0;
static bool lbl_803201A1;
static bool lbl_803201A2;

s32 lbl_803201A4;

static s32 lbl_803201A8;
static s32 lbl_803201AC;

static f32 lbl_803201B0;
static f32 lbl_803201B4;

static u32 lbl_803201BC; // Bitflag

static bool lbl_803201B8;
static bool lbl_803201B9;
static bool lbl_803201BA;

static bool lbl_803201C0;
static bool lbl_803201C1;
static bool lbl_803201C2;

static s32 lbl_803201C4;
static s32 lbl_803201C8;

static CCellAnim *lbl_803201CC;
static CCellAnim *lbl_803201D0;
static CCellAnim *lbl_803201D4;
static CCellAnim *lbl_803201D8;
static CCellAnim *lbl_803201DC;
static CCellAnim *lbl_803201E0;
static CCellAnim *lbl_803201E4;
static CCellAnim *lbl_803201E8;
static CCellAnim *lbl_803201EC;
static CCellAnim *lbl_803201F0;
static CCellAnim *lbl_803201F4;
static CCellAnim *lbl_803201F8;
static CCellAnim *lbl_803201FC;
static CCellAnim *lbl_80320200;
static CCellAnim *lbl_80320204;
static CCellAnim *lbl_80320208;
static CCellAnim *lbl_8032020C;
static CCellAnim *lbl_80320210;
static CCellAnim *lbl_80320214;
static CCellAnim *lbl_80320218;

static f32 lbl_8032021C;
static f32 lbl_80320220;
static f32 lbl_80320224;

static bool lbl_80320228;

static s32 lbl_8032022C;

static s32 lbl_80320230;

static CTRFloat lbl_8032A1C0;
static CTRFloat lbl_8032A200;
static CTRFloat lbl_8032A240;

CExScene::CExScene(void) {
    mPauseLayout = NULL;
    mCursorLayout = NULL;
    mLyricLayout = NULL;
}

void CExScene::_14(void) {
    // TODO: requires CExController
}

void CExScene::_18(void) {
    bool reset = false;

    lbl_8032019C++;

    for (s32 i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        CController *controller = gControllerManager->fn_801D5FF0(i);
        if (controller->_24() && (i != 0) && (i != 1)) {
            WPADDisconnect(i);
        }
    }

    if (gTickFlowManager->fn_801E40D8()) {
        bool temp = lbl_803201B0 >= lbl_803201B4;
        if (!temp) {
            lbl_803201B0 += gTickFlowManager->fn_801E2698();
        }

        if (gTickFlowManager->fn_801E4078()) {
            if (!lbl_803201CC->getEnabled() && temp) {
                lbl_803201CC->fn_801DD1F0(permanent_next_loop);
            }
        }
        else if (gTickFlowManager->fn_801E40A8()) {
            lbl_803201CC->fn_801DD24C(permanent_next, 0.0f);
        }
    }

    if (
        (mPauseLayout != NULL) && mPauseLayout->getPaused() &&
        !gHBMManager->getUnk414()
    ) {
        mPauseLayout->_18(gLayoutManager->getDrawInfo());

        if (mPauseLayout->getUnk24() == 2) {
            mPauseLayout->setUnk1D(false);
            reset = true;

            fn_8000A564(false, true);
            lbl_80320230 = 0;

            if (
                gFileManager->fn_801D42FC(50) &&
                gCellAnimManager->fn_801DB558(254) &&
                lbl_80320214 != NULL && lbl_80320214->getEnabled()
            ) {
                lbl_80320214->fn_801DCF94(-100);
            }
        }
    }
    else {
        if (!gHBMManager->getUnk414()) {
            _28();

            if (lbl_80320220 != 0.0) {
                lbl_8032021C += lbl_80320220;
                if (
                    (lbl_80320220 > 0.0f && lbl_8032021C >= lbl_80320224) ||
                    (lbl_80320220 < 0.0f && lbl_8032021C <= lbl_80320224)
                ) {
                    lbl_8032021C = lbl_80320224;
                    lbl_80320220 = 0.0f;
                }
                lbl_8032020C->setOpacity(lbl_8032021C);
            }
            else {
                if (lbl_8032A1C0.getIsTicking()) {
                    lbl_8032A1C0.fn_801E9DE8(FALSE);
                    lbl_8032020C->setOpacity(lbl_8032A1C0.getCurrent());
                }
            }

            if (!lbl_803201C1) {
                fn_8000A378();
            }

            f32 canvasScale = gMyCanvasManager->fn_8007C244(1).x;
            lbl_80320210->setEnabled(canvasScale < 0.99f);

            CCellAnim *remainNumAnim = lbl_803201F4;

            f32 scale = remainNumAnim->getScaleX();
            if (scale > 1.0) {
                scale *= 0.95f;
                if (scale < 1.0) {
                    scale = 1.0;
                }
                remainNumAnim->setScale(scale, scale);
            }

            gSceneManager->_18();
            gLayoutManager->_18();
            gCellAnimManager->_18();

            CController *controller = gControllerManager->fn_801D5FF0(0);
            CGCController *gcController = gControllerManager->fn_801D6000(0);
        }
    }

    gHBMManager->_18();
    gBackupManager->fn_8008414C();

    if (reset) {
        gSoundManager->fn_801E7008();
        gSoundManager->fn_801E7108();

        gTickFlowManager->fn_801E1E4C();
        gTickFlowManager->setUnkFC(false);

        gTickFlowManager->fn_801E2B9C(120.0f);
        gTickFlowManager->fn_801E2C04(1.0f);

        gCheckPointManager->setUnkDD(false);

        gInputCheckManager->fn_801E9D88();
        gInputCheckManager->fn_801E9D8C();
        gInputCheckManager->setUnk429(true);
        gInputCheckManager->setUnk484(false);
        gInputCheckManager->setUnk42A(true);
        gInputCheckManager->setUnk42B(true);
        gInputCheckManager->setUnk42C(true);
        gInputCheckManager->setUnk42D(true);

        gSceneManager->fn_8008A8D8();

        setUnk0E(true);
    }
}

void CExScene::_28(void) {}

static inline void cellAnimDoFrontDrw(CCellAnimManager *cellAnimManager) {
    cellAnimManager->_20(0, fn_8009E204(), fn_8009E260());
}

void CExScene::_1C(void) {
    lbl_803201D0->setEnabled(gTickFlowManager->getUnkFC());

    bool youEnable = lbl_80320200->getEnabled();
    bool remainEnable = lbl_803201F0->getEnabled();

    if (lbl_80320228) {
        lbl_80320200->setEnabled(false);
        lbl_803201F0->setEnabled(false);
        lbl_803201F4->setEnabled(false);
    }

    if (!gHBMManager->getUnk414()) {
        // TODO: Hmmm .....
        cellAnimDoFrontDrw(gCellAnimManager);

        gLayoutManager->_1C();

        gCellAnimManager->_24(0, NULL, NULL);

        if (mLyricLayout != NULL) {
            mLyricLayout->_1C(gLayoutManager->getDrawInfo());
        }
    }

    lbl_80320200->setEnabled(youEnable);
    lbl_803201F0->setEnabled(remainEnable);
    lbl_803201F4->setEnabled(remainEnable);

    u8 aspectRatio = SCGetAspectRatio();

    if (!gHBMManager->getUnk414()) {
        gGameManager->getFader()->fn_80007DD8();
    }
}

void CExScene::_08(void) {
    if (!gHBMManager->getUnk414()) {
        gCellAnimManager->_24(-299, NULL, NULL);

        if (mPauseLayout != NULL && (mPauseLayout->getPaused() || mPauseLayout->getUnk24() == 2)) {
            gLayoutManager->fn_801D6D78();
            mPauseLayout->_1C(gLayoutManager->getDrawInfo());
        }
    }
    gHBMManager->_1C();
}

void CExScene::_20(void) {
    lbl_80320200->setEnabled(false);
    lbl_80320204->setEnabled(false);
    lbl_80320208->setEnabled(false);

    lbl_80320200->setScale(1.0f, 1.0f);
    lbl_80320204->setScale(1.0f, 1.0f);
    lbl_80320208->setScale(1.0f, 1.0f);

    lbl_8032020C->setEnabled(false);
    lbl_8032020C->fn_801DCF94(0);

    lbl_803201A1 = false;

    lbl_803201F0->setEnabled(false);
    lbl_803201F4->setEnabled(false);

    // lbl_803201F0->setScale(1.0f, 1.0f);
    lbl_803201F4->setScale(1.0f, 1.0f);

    gMyCanvasManager->fn_8007BEF8(2);
    for (s32 i = 2; i <= 9; i++) {
        gMyCanvasManager->fn_8007BDAC(i);
    }

    lbl_80320200->setBase(gCellAnimManager->fn_801DC16C(), 0, false);
    lbl_80320204->setBase(gCellAnimManager->fn_801DC16C(), 0, false);
    lbl_80320208->setBase(gCellAnimManager->fn_801DC16C(), 0, false);

    gInputCheckManager->setUnk418(NULL);
    gInputCheckManager->fn_801E9204();
    gInputCheckManager->fn_801E9604(true);
    gInputCheckManager->fn_801E960C();
    gInputCheckManager->fn_801E9C40();

    gTickFlowManager->fn_801E2490(0);
    gTickFlowManager->registerFlow<CExFlow>();
    gTickFlowManager->setUnkFD(false);
    gTickFlowManager->setUnk100(0x1000); // TODO: what is this?

    gLayoutManager->fn_801D6BB0();
    gLayoutManager->_28();

    gHBMManager->_20();

    gControllerManager->fn_801D5FF0(0)->fn_801D5500(0x800, 6);
    gControllerManager->fn_801D5FF0(0)->fn_801D5500(0x400, 6);

    gControllerManager->fn_801D5FF0(1)->fn_801D5500(0x800, 6);
    gControllerManager->fn_801D5FF0(1)->fn_801D5500(0x400, 6);

    if (lbl_80320228) {
        fn_8000A564(false, true);
    }

    CExFlow::fn_8008E724(false);

    if (getUnk0E()) {
        lbl_803201B0 = 0.0f;
        lbl_803201B4 = static_cast<f32>(0xFFFFFFFF);

        lbl_803201CC->setEnabled(false);

        gInputCheckManager->fn_801E9C30(false);

        lbl_803201B8 = false;

        lbl_803201F8->fn_801DD0AC(permanent_perfect_icon_hit);
        lbl_803201F8->fn_801DCF38();
        lbl_803201F8->setEnabled(false);

        lbl_803201FC->fn_801DD1F0(permanent_perfect_msg_loop);
        lbl_803201FC->setEnabled(false);

        lbl_803201B9 = false;
        lbl_803201BA = false;

        fn_8009E1C4();

        if (gFileManager->fn_801D42FC(50) && gCellAnimManager->fn_801DB558(254)) {
            gCellAnimManager->fn_801DC068(254);
            gCellAnimManager->fn_801DBA98(254);

            gFileManager->fn_801D41CC(50);

            lbl_80320214 = NULL;
        }

        // NOTE: duplicated code for no reason
        if (gFileManager->fn_801D42FC(50) && gCellAnimManager->fn_801DB558(254)) {
            gCellAnimManager->fn_801DC068(254);
            gCellAnimManager->fn_801DBA98(254);

            gFileManager->fn_801D41CC(50);

            lbl_80320214 = NULL;
        }

        fn_80008EFC();

        gGameManager->getFader()->fn_800080C0(1.0f);
    }
}

void CExScene::fn_80008DF8(u8 param_1, s32 param_2) {
    lbl_8032021C = lbl_8032020C->getOpacity();
    lbl_80320224 = param_1;
    lbl_80320220 = (param_1 - lbl_8032021C) / param_2;
}

void CExScene::fn_80008E68(u8 opacity, f32 time) {
    lbl_8032A1C0.fn_801EA698(lbl_8032020C->getOpacity(), opacity);
    lbl_8032A1C0.fn_801EA514(0.0f, time);
}

void CExScene::fn_80008EF0(void) {
    lbl_80320218->fn_801DD118(permanent_save);
}

void CExScene::fn_80008EFC(void) {
    gFileManager->fn_801D443C();

    for (u8 i = 2; i < CFileManager::DEFAULT_MAX_ARCHIVE_COUNT; i++) {
        if ((i != 54) && (i != 94) && gFileManager->fn_801D42FC(i)) {
            gFileManager->fn_801D41CC(i);
        }
    }
}

void CExScene::fn_80008F68(bool update, u16 tempo) {
    gCellAnimManager->fn_801DC17C(update, tempo);
}

void CExScene::fn_80008F70(bool update, u16 soundID) {
    u16 tempo = gSoundManager->fn_801E75C0(soundID);
    fn_80008F68(update, tempo);
}

void CExScene::fn_80008FC0(bool update, u16 soundID) {
    u16 tempo = gSoundManager->get_wave_tempo(soundID);
    fn_80008F68(update, tempo);
}

void CExScene::fn_80009028(void) {
    gFileManager->fn_801D3F94(54, "content2/cellanim/permanent/cellanim.szs");
    gFileManager->fn_801D3F94(94, "content2/layout/layout_common.szs");

    gFileManager->fn_801D4364(54);
    gFileManager->fn_801D4364(94);

    void *permanent_brcadAddr = gFileManager->fn_801D4270(54, "./permanent.brcad");

    u32 permanent_tplLen = gFileManager->fn_801D422C(54, "./cellanim.tpl");
    void *permanent_tplAddr = gFileManager->fn_801D4270(54, "./cellanim.tpl");

    fn_801ED7D4(permanent_tplAddr);
    DCStoreRange(permanent_tplAddr, permanent_tplLen);

    gCellAnimManager->fn_801DB568(permanent_brcadAddr, permanent_tplAddr, 255);

    if (gMyCanvasManager == NULL) {
        gMyCanvasManager = new CMyCanvasManager;
    }
    gMyCanvasManager->fn_8007BC9C();

    gMyCanvasManager->fn_8007BEF8(1);

    lbl_803201CC = gCellAnimManager->fn_801DBE7C(255, permanent_next);
    lbl_803201D0 = gCellAnimManager->fn_801DBE7C(255, permanent_skip);
    lbl_803201D4 = gCellAnimManager->fn_801DBE7C(255, permanent_inst_example);
    lbl_803201D8 = gCellAnimManager->fn_801DBE7C(255, permanent_inst_example_msg);
    lbl_803201DC = gCellAnimManager->fn_801DBE7C(255, permanent_mask);
    lbl_803201E0 = gCellAnimManager->fn_801DBE7C(255, permanent_skip_base);
    lbl_803201E4 = gCellAnimManager->fn_801DBE7C(255, permanent_pause_in);
    lbl_803201E8 = gCellAnimManager->fn_801DBE7C(255, permanent_pause_base);
    lbl_803201EC = gCellAnimManager->fn_801DBE7C(255, permanent_exit);
    lbl_803201F0 = gCellAnimManager->fn_801DBE7C(255, permanent_remain);
    lbl_803201F4 = gCellAnimManager->fn_801DBE7C(255, permanent_remain_number);
    lbl_8032020C = gCellAnimManager->fn_801DBE7C(255, permanent_mask);
    lbl_80320210 = gCellAnimManager->fn_801DBE7C(255, permanent_frame_inst);
    lbl_80320218 = gCellAnimManager->fn_801DBE7C(255, permanent_save);

    lbl_803201F8 = gCellAnimManager->fn_801DBE7C(255, permanent_perfect_icon_hit);
    lbl_803201FC = gCellAnimManager->fn_801DBE7C(255, permanent_perfect_msg_loop);
    lbl_803201FC->setBase(lbl_803201F8, 0, true);

    gMyCanvasManager->fn_8007BEF8(2);

    lbl_80320200 = gCellAnimManager->fn_801DBE7C(255, permanent_you);
    lbl_80320204 = gCellAnimManager->fn_801DBE7C(255, permanent_you_1P);
    lbl_80320208 = gCellAnimManager->fn_801DBE7C(255, permanent_you_2P);

    lbl_803201F4->setBase(lbl_803201F0, 0, false);

    u8 aspectRatio = SCGetAspectRatio();
    lbl_803201E4->setBase(lbl_803201E8, (aspectRatio == SC_ASPECT_WIDE) ? 1 : 0, false);

    lbl_803201CC->fn_801DCF94(-100);
    lbl_803201D0->fn_801DCF94(-100);
    lbl_803201D4->fn_801DCF94(-100);
    lbl_803201D8->fn_801DCF94(-311);
    lbl_803201DC->fn_801DCF94(-50);
    lbl_803201E0->fn_801DCF94(-100);
    lbl_803201E4->fn_801DCF94(-300);
    lbl_803201E8->fn_801DCF94(-300);
    lbl_803201EC->fn_801DCF94(-300);
    lbl_803201F0->fn_801DCF94(-100);
    lbl_803201F4->fn_801DCF94(-100);
    lbl_803201F8->fn_801DCF94(-400);
    lbl_803201FC->fn_801DCF94(-400);
    lbl_80320200->fn_801DCF94(-100);
    lbl_80320204->fn_801DCF94(-100);
    lbl_80320208->fn_801DCF94(-100);
    lbl_8032020C->fn_801DCF94(0);
    lbl_80320210->fn_801DCF94(-310);
    lbl_80320218->fn_801DCF94(-400);

    lbl_803201CC->setEnabled(false);
    lbl_803201D0->setEnabled(false);
    lbl_803201D4->setEnabled(false);
    lbl_803201D8->setEnabled(false);
    lbl_803201DC->setEnabled(false);
    lbl_803201E4->setEnabled(false);
    lbl_803201EC->setEnabled(false);
    lbl_803201F0->setEnabled(false);
    lbl_803201F4->setEnabled(false);
    lbl_803201F8->setEnabled(false);
    lbl_803201FC->setEnabled(false);
    lbl_80320200->setEnabled(false);
    lbl_80320204->setEnabled(false);
    lbl_80320208->setEnabled(false);
    lbl_8032020C->setEnabled(false);
    lbl_80320210->setEnabled(false);
    lbl_80320218->setEnabled(false);

    lbl_803201CC->fn_801DCF18();

    gMyCanvasManager->fn_8007BF30(lbl_80320218, 0);

    lbl_803201D8->setScale(1.0f/0.7f, 1.0f/0.7f);

    lbl_803201DC->setBackColor(0, 0, 0);
    lbl_803201DC->setOpacity(96);
    lbl_803201DC->setScale(2.0f, 2.0f);

    lbl_803201CC->setPos(0.0f, 170.0f);

    u16 partIndex = 3;
    if (SCGetAspectRatio() == SC_ASPECT_WIDE) {
        partIndex = 7;
    }

    lbl_803201D0->setBase(lbl_803201E0, partIndex, false);
    lbl_803201D4->setBase(lbl_803201E0, partIndex + 8, false);

    lbl_803201EC->setPos(80.0f, 170.0f);

    s32 posX = -270;
    if (SCGetAspectRatio() == SC_ASPECT_WIDE) {
        posX = -360;
    }
    lbl_803201F8->setPos(posX, -180.0f);

    lbl_80320220 = 0.0f;

    lbl_8032A1C0.setEasing(0, 1);
    lbl_8032A1C0.fn_801EA698(0.0f, 1.0f);
    lbl_8032A1C0.fn_801EA550(0.0f, 96.0f, true);
    lbl_8032A1C0.setIsTicking(false);

    lbl_8032A200.setEasing(3, 3);
    lbl_8032A200.fn_801EA698(1.0f, 1.0f);
    lbl_8032A200.fn_801EA550(0.0f, 48.0f, true);
    lbl_8032A200.setIsTicking(false);

    lbl_8032A240.setEasing(0, 1);
    lbl_8032A240.fn_801EA698(0.0f, 0.0f);
    lbl_8032A240.fn_801EA550(0.0f, 48.0f, true);
    lbl_8032A240.setIsTicking(false);

    lbl_803201B0 = 0.0f;
    lbl_803201B4 = static_cast<f32>(0xFFFFFFFF);
}

void CExScene::fn_8000966C(void) {
    gMyCanvasManager->fn_8007BEA0();

    if (gMyCanvasManager != NULL) {
        delete gMyCanvasManager;
    }
    gMyCanvasManager = NULL;

    gCellAnimManager->fn_801DBA98(255);
    gCellAnimManager->fn_801DC068(255);

    gFileManager->fn_801D41CC(54);
    gFileManager->fn_801D41CC(94);
}

void CExScene::fn_800096EC(s32 x, s32 y) {
    lbl_803201CC->setPos(x, y);
}

void CExScene::fn_8000973C(void) {
    lbl_803201CC->fn_801DD118(permanent_next);
}

bool CExScene::fn_80009748(void) {
    return !lbl_803201CC->getPlaying();
}

void CExScene::fn_8000975C(s32 pos) {
    if (SCGetAspectRatio() == SC_ASPECT_WIDE) {
        pos += 4;
    }

    lbl_803201D0->setBase(lbl_803201E0, pos, false);
    lbl_803201D4->setBase(lbl_803201E0, pos + 8, false);
}

void CExScene::fn_800097C4(bool practice) {
    lbl_803201D0->fn_801DCE9C(practice ? permanent_skip_practice : permanent_skip);
    lbl_803201D0->setFrame(0.0f);
    lbl_803201D0->fn_801DCF18();
}

void CExScene::fn_80009814(void) {
    lbl_803201D4->fn_801DD0AC(permanent_inst_example);
}

void CExScene::fn_80009820(void) {
    lbl_803201D4->setEnabled(true);
}

void CExScene::fn_80009830(void) {
    lbl_803201D4->setEnabled(false);
}

bool CExScene::fn_80009840(void) {
    return lbl_803201D4->getEnabled();
}

void CExScene::fn_8000984C(void) {
    lbl_803201E4->fn_801DCE9C(permanent_pause_in);
    lbl_803201E4->setFrame(0.0f);
    lbl_803201E4->fn_801DCF18();
    lbl_803201E4->setEnabled(true);
}

void CExScene::fn_80009894(void) {
    lbl_803201E4->fn_801DCE9C(permanent_pause_out);
    lbl_803201E4->setFrame(0.0f);
    lbl_803201E4->fn_801DCF18();
}

void CExScene::fn_800098D0(void) {
    lbl_803201E4->fn_801DCE9C(permanent_pause_out);
    lbl_803201E4->fn_801DCF38();
}

void CExScene::fn_80009900(s32 x, s32 y) {
    lbl_803201EC->setPos(x, y);
}

void CExScene::fn_80009950(s32 x, s32 y) {
    lbl_80320200->setPos(x, y);
}

void CExScene::fn_800099A0(s32 x, s32 y) {
    lbl_80320204->setPos(x, y);
}

void CExScene::fn_800099F0(s32 x, s32 y) {
    lbl_80320208->setPos(x, y);
}

void CExScene::fn_80009A40(bool enabled) {
    lbl_80320200->setEnabled(enabled);
}

void CExScene::fn_80009A4C(bool enabled) {
    lbl_80320204->setEnabled(enabled);
}

void CExScene::fn_80009A58(bool enabled) {
    lbl_80320208->setEnabled(enabled);
}

void CExScene::fn_80009A64(bool enabled) {
    lbl_8032020C->setEnabled(enabled);
}

void CExScene::fn_80009A70(u8 opacity) {
    lbl_8032020C->setOpacity(opacity);
}

void CExScene::fn_80009A7C(u8 r, u8 g, u8 b) {
    lbl_8032020C->setBackColor(r, g, b);
}

void CExScene::fn_80009A90(s32 layer) {
    lbl_8032020C->fn_801DCF94(layer);
}

void CExScene::fn_80009A9C(void) {}

bool CExScene::fn_80009AA0(void) {
    if (mPauseLayout != NULL) {
        return mPauseLayout->getPaused();
    }
    return false;
}

void CExScene::fn_80009ABC(void) {
    lbl_803201A0 = true;
    fn_80009ABC(1);
}

void CExScene::fn_80009B54(s32 x, s32 y) {
    lbl_803201F0->setPos(x, y);
}

void CExScene::fn_80009BA4(u16 frame, s32 param_2, s32 param_3) {
    lbl_803201A1 = true;
    lbl_803201A8 = param_2;
    lbl_803201AC = param_3;

    lbl_803201F0->setFrame(frame);
    lbl_803201F4->setFrame(0.0f);
}

void CExScene::fn_80009BEC(void) {
    lbl_803201A1 = false;

    lbl_803201F0->setEnabled(false);
    lbl_803201F4->setEnabled(false);
    lbl_803201F4->setScale(1.0f, 1.0f);
}

void CExScene::fn_80009C28(s32 param_1) {
    if (lbl_803201A1) {
        s32 frame = lbl_803201AC - param_1;
        if ((frame > 0) && (frame <= lbl_803201A8)) {
            lbl_803201F0->setEnabled(true);
            lbl_803201F4->setEnabled(true);
            lbl_803201F4->setFrame(frame);

            u16 remainKey = lbl_803201F0->fn_801DD43C();
            if ((frame <= 1) && (remainKey <= 1)) {
                lbl_803201F0->fn_801DD2B4(remainKey + 2);
            }
            if ((2 <= frame) && (remainKey == 2 || remainKey == 3)) {
                lbl_803201F0->fn_801DD2B4(remainKey - 2);
            }
        }
        else {
            lbl_803201F0->setEnabled(false);
            lbl_803201F4->setEnabled(false);
        }
    }
    else {
        lbl_803201F0->setEnabled(false);
        lbl_803201F4->setEnabled(false);
    }
}

void CExScene::fn_80009D3C(void) {
    lbl_803201F4->setScale(1.4f, 1.4f);
}

void CExScene::fn_80009D60(void) {
    lbl_803201C0 = true;
}

bool CExScene::fn_80009D6C(void) {
    return lbl_803201C0;
}

void CExScene::fn_80009D74(bool param_1) {
    lbl_803201A2 = param_1;
}

bool CExScene::fn_80009D7C(void) {
    return lbl_803201A2;
}

void CExScene::fn_80009D84(f32 param_1) {
    lbl_803201B0 = 0.0f;
    lbl_803201B4 = param_1;
}

bool CExScene::fn_80009D94(void) {
    if (lbl_803201B8 == false) {
        return false;
    }
    return lbl_803201BA == FALSE; // ????
}

void CExScene::fn_80009DB8(bool param_1) {
    lbl_803201B8 = param_1;

    lbl_803201F8->fn_801DD0AC(permanent_perfect_icon_hit);
    lbl_803201F8->fn_801DCF38();
    lbl_803201F8->setEnabled(false);

    lbl_803201FC->fn_801DD1F0(permanent_perfect_msg_loop);
    lbl_803201FC->setEnabled(false);

    lbl_803201B9 = false;
    lbl_803201BA = false;
}

bool CExScene::fn_80009E1C(void) {
    return lbl_803201B8;
}

void CExScene::fn_80009E24(bool enabled) {
    if (!lbl_803201B8) {
        return;
    }

    lbl_803201F8->setEnabled(enabled);
    lbl_803201FC->setEnabled(enabled);
}

void CExScene::fn_80009E44(bool param_1) {
    if (!lbl_803201B8) {
        return;
    }

    lbl_803201B9 = param_1;
}

void CExScene::fn_80009E58(void) {
    if (!lbl_803201B8) {
        return;
    }
    if (!lbl_803201B9) {
        return;
    }
    if (lbl_803201B8 ? lbl_803201BA : false) { // NOTE: probably an inline
        return;
    }

    lbl_803201F8->fn_801DD0AC(permanent_perfect_icon_hit);
}

void CExScene::fn_80009ABC(u32 param_1) {
    if ((lbl_803201BC & param_1) && lbl_803201B8) {
        if (lbl_803201B9 && !(lbl_803201B8 ? lbl_803201BA : false)) {
            lbl_803201F8->fn_801DD0AC(permanent_perfect_icon_fail);
            lbl_803201FC->setEnabled(false);
            lbl_803201BA = true;
            gSoundManager->play(SE_PERFECT_FAIL);
        }
    }
}

const char * const lbl_802E5828[] = {
    "*-----",
    "**--------",
    "***----------",
    "****-----------",
    "*****-----------",
    "******-----------",
    "*******-----------",
    "********-----------",
    "*********-----------",
    "*****---------*----****----",
    "******---------*----",
    "*******---*----*-----*------*",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-"
};

void CExScene::fn_80009F24(s32 channel) {
    gControllerManager->fn_801D5FF0(channel)->_40(lbl_802E5828[4], false);
}

void CExScene::fn_80009F70(bool param_1) {
    lbl_803201C1 = param_1;
    if (!lbl_803201C1) {
        gMyCanvasManager->fn_8007BDAC(1);
        gLayoutManager->fn_801D717C(1.0f, 1.0f);
    }
}

bool CExScene::fn_80009FB4(void) {
    return lbl_803201C1;
}

void CExScene::fn_80009FBC(void) {
    if (lbl_803201C1) {
        lbl_80320210->fn_801DD0AC(permanent_frame_video_fade_in);
        lbl_80320210->setScale(1.0f/0.7f, 1.0f/0.7f);

        gMyCanvasManager->fn_8007BF6C(1, 0.7f, 0.7f);
        gLayoutManager->fn_801D717C(0.7f, 0.7f);

        gInputCheckManager->setUnk429(false);
    }
}

void CExScene::fn_8000A038(void) {
    if (lbl_803201C1) {
        lbl_80320210->fn_801DD0AC(permanent_frame_video_fade_out);
        lbl_80320210->setScale(1.0f/0.7f, 1.0f/0.7f);
    }
}

void CExScene::fn_8000A084(void) {
    gFileManager->fn_801D3F94(50, "content2/cellanim/remix/ver8/cellanim.szs");
}

void CExScene::fn_8000A0A4(void) {
    gCellAnimManager->fn_801DC068(254);
    gCellAnimManager->fn_801DBA98(254);

    gFileManager->fn_801D41CC(50);

    lbl_80320214 = NULL;
}

bool CExScene::fn_8000A0EC(void) {
    return gFileManager->fn_801D42FC(50);
}

void CExScene::fn_8000A0F8(void) {
    void *brcadAddr = gFileManager->fn_801D4270(50, "./remix08_Mask.brcad");

    u32 tplLen = gFileManager->fn_801D422C(50, "./cellanim.tpl");
    void *tplAddr = gFileManager->fn_801D4270(50, "./cellanim.tpl");

    fn_801ED7D4(tplAddr);
    DCStoreRange(tplAddr, tplLen);

    gCellAnimManager->fn_801DB568(brcadAddr, tplAddr, 254);

    s32 prevCanvas = gMyCanvasManager->fn_8007BF28();
    gMyCanvasManager->fn_8007BEF8(2);

    lbl_80320214 = gCellAnimManager->fn_801DBE7C(254, remix08_Mask_photo);
    lbl_80320214->setEnabled(false);

    gMyCanvasManager->fn_8007BEF8(prevCanvas);
}

void CExScene::fn_8000A1D4(bool enabled) {
    lbl_80320214->setEnabled(enabled);
}

void CExScene::fn_8000A1E0(s32 layer) {
    lbl_80320214->fn_801DCF94(layer);
}

void CExScene::fn_8000A1EC(void) {
    gFileManager->fn_801D3F94(50, "content2/cellanim/endless/cellanim.szs");
}

void CExScene::fn_8000A20C(void) {
    gCellAnimManager->fn_801DC068(254);
    gCellAnimManager->fn_801DBA98(254);

    gFileManager->fn_801D41CC(50);

    // NOTE: this is probably here because of a copy-paste error
    lbl_80320214 = NULL;
}

bool CExScene::fn_8000A254(void) {
    return gFileManager->fn_801D42FC(50);
}

void CExScene::fn_8000A260(void) {
    void *brcadAddr = gFileManager->fn_801D4270(50, "./score.brcad");

    u32 tplLen = gFileManager->fn_801D422C(50, "./cellanim.tpl");
    void *tplAddr = gFileManager->fn_801D4270(50, "./cellanim.tpl");

    fn_801ED7D4(tplAddr);
    DCStoreRange(tplAddr, tplLen);

    gCellAnimManager->fn_801DB568(brcadAddr, tplAddr, 254);
}

void CExScene::fn_8000A2FC(void) {
    lbl_803201C4 = 0;
    lbl_803201C8 = gSaveData->fn_80078F4C()->fn_80077E40(4);
}

void CExScene::fn_8000A334(bool param_1) {
    lbl_803201C2 = param_1;
    if (lbl_803201C2) {
        fn_8000A564(false, true);
        lbl_80320230 = 0;
    }
}

bool CExScene::fn_8000A370(void) {
    return lbl_803201C2;
}

void CExScene::fn_8000A378(void) {
    if (lbl_8032A200.getIsTicking()) {
        lbl_8032A200.fn_801E9DE8(FALSE);
    }

    f32 scale = lbl_8032A200.getCurrent();

    gMyCanvasManager->fn_8007BF6C(1, lbl_8032A200.getCurrent(), lbl_8032A200.getCurrent());
    gLayoutManager->fn_801D717C(scale, scale);

    if (lbl_80320228) {
        lbl_8032A240.fn_801E9DE8(FALSE);

        if (lbl_8032A240.hasEnded()) {
            lbl_8032A240.fn_801EA698(1000.0f, lbl_80320198.unk00);

            f32 time = gTickFlowManager->fn_801E2928(720.0f);
            lbl_8032A240.fn_801EA550(0.0f, time, true);
        }

        lbl_803201D8->setPosX(lbl_8032A240.getCurrent());

        if (gControllerManager->fn_801D5FF0(0)->getUnk133C() & WPAD_BUTTON_1) {
            fn_8000A564(false, false);

            lbl_803201D4->setEnabled(true);

            gInputCheckManager->fn_801E8560();

            gTickFlowManager->setUnkFC(true);
        }
    }
    else {
        if (gControllerManager->fn_801D5FF0(0)->getUnk133C() & WPAD_BUTTON_1) {
            if (fn_8000A6E0() && lbl_803201D4->getEnabled()) {
                lbl_80320210->fn_801DD0AC(permanent_frame_inst);
                lbl_80320210->setScale(1.0f, 1.0f);

                fn_8000A564(true, false);

                lbl_803201D4->setEnabled(false);

                gTickFlowManager->setUnkFC(false);
            }
        }
    }
}

void CExScene::fn_8000A564(bool param_1, bool param_2) {
    lbl_80320228 = param_1;
    if (lbl_80320228) {
        if (param_2) {
            lbl_8032A200.fn_801EA698(0.7f, 0.7f);
            lbl_8032A200.fn_801EA550(0.0f, 32.0f, true);
        }
        else {
            lbl_8032A200.fn_801EA698(lbl_8032A200.getCurrent(), 0.7f);
            lbl_8032A200.fn_801EA550(0.0f, 32.0f, true);
        }

        lbl_8032A240.fn_801EA698(1000.0f, lbl_80320198.unk00);

        f32 time = gTickFlowManager->fn_801E2928(720.0f);
        lbl_8032A240.fn_801EA550(0.0f, time, true);

        lbl_803201DC->setEnabled(true);
        lbl_803201D8->setEnabled(true);
    }
    else {
        if (param_2) {
            lbl_8032A200.fn_801EA698(1.0f, 1.0f);
            lbl_8032A200.fn_801EA550(0.0f, 20.0f, true);
        }
        else {
            lbl_8032A200.fn_801EA698(lbl_8032A200.getCurrent(), 1.0f);
            lbl_8032A200.fn_801EA550(0.0f, 20.0f, true);
        }

        lbl_803201DC->setEnabled(false);
        lbl_803201D8->setEnabled(false);
    }
}

bool CExScene::fn_8000A6B4(void) {
    return lbl_80320228;
}

void CExScene::fn_8000A6BC(void) {
    lbl_80320230 = 0;
}

void CExScene::fn_8000A6C8(void) {
    lbl_80320230++;
}

void CExScene::fn_8000A6D8(s32 param_1) {
    lbl_8032022C = param_1;
}

bool CExScene::fn_8000A6E0(void) {
    if ((lbl_8032022C > 0) && (lbl_80320230 >= lbl_8032022C)) {
        return true;
    }
    return false;
}

void CExScene::fn_8000A708(s32 param_1) {
    gGameManager->getFader()->fn_800080C0(param_1);
}

void CExScene::fn_8000A74C(s32 param_1) {
    gGameManager->getFader()->fn_800080B0(param_1);
}

void CExScene::fn_8000A790(s32 fadeFrames) {
    gSoundManager->fn_801E7114(0.0f, fadeFrames);
}

bool CExScene::fn_8000A7A0(void) {
    return gGameManager->getFader()->fn_800080D0();
}

void CExScene::fn_8000A7AC(
    u8 animDataID, u16 gameOverAnimID, u16 hiScoreAnimID,
    s32 iconSelectLayoutID,
    s32 layer
) {
    mEndlessGameOver = false;
    mEndlessGotHighScore = false;
    mEndlessScore = 0;

    mEndlessGameOverAnimID = gameOverAnimID;
    mEndlessHiScoreAnimID = hiScoreAnimID;
    mEndlessIconSelectLayoutID = iconSelectLayoutID;

    s32 prevCanvas = gMyCanvasManager->fn_8007BF28();
    gMyCanvasManager->fn_8007BEF8(1);

    mEndlessGameOverAnim = gCellAnimManager->fn_801DBE7C(animDataID, mEndlessGameOverAnimID);
    mEndlessGameOverAnim->setEnabled(false);

    mEndlessHiScoreAnim = gCellAnimManager->fn_801DBE7C(animDataID, mEndlessHiScoreAnimID);
    mEndlessHiScoreAnim->setEnabled(false);

    mEndlessGameOverAnim->fn_801DCF94(layer + 1);
    mEndlessHiScoreAnim->fn_801DCF94(layer);

    gMyCanvasManager->fn_8007BEF8(prevCanvas);
}

void CExScene::fn_8000A864(void) {
    gCellAnimManager->fn_801DBFA0(mEndlessGameOverAnim);
    gCellAnimManager->fn_801DBFA0(mEndlessHiScoreAnim);
}

void CExScene::fn_8000A8A4(void) {
    mEndlessGameOver = false;
    mEndlessGotHighScore = false;

    mEndlessGameOverAnim->setEnabled(false);
    mEndlessHiScoreAnim->setEnabled(false);
}

void CExScene::fn_8000A8C4(bool gameOver) {
    mEndlessGameOver = gameOver;
}

void CExScene::fn_8000A8CC(void) {
    mEndlessGameOverAnim->fn_801DD1F0(mEndlessGameOverAnimID);
    if (mEndlessGotHighScore) {
        mEndlessHiScoreAnim->fn_801DD1F0(mEndlessHiScoreAnimID);
        gSoundManager->play(SE_ENDLESS_GAME_OVER_HI_SCORE);
    }
    else {
        gSoundManager->play(SE_ENDLESS_GAME_OVER);
    }
}

bool CExScene::fn_8000A944(void) {
    return mEndlessGameOver;
}

void CExScene::fn_8000A94C(bool highScore, s32 score) {
    mEndlessGotHighScore = highScore;
    mEndlessScore = score;
}

bool CExScene::fn_8000A958(void) {
    return mEndlessGotHighScore;
}

void CExScene::fn_8000A960(void) {
    CIconSelectLayout *layout = gLayoutManager->getLayout<CIconSelectLayout>(mEndlessIconSelectLayoutID);
    layout->fn_800C1C10(false);
}

bool CExScene::fn_8000A998(void) {
    CIconSelectLayout *layout = gLayoutManager->getLayout<CIconSelectLayout>(mEndlessIconSelectLayoutID);
    return layout->fn_800C1DE0();
}

void CExScene::fn_8000A9CC(s32 gameIndex, bool twoPlay) {
    if (!twoPlay) {
        gSaveData->fn_80078F4C()->fn_80077C5C(gameIndex, mEndlessScore);
    }
    else {
        // TODO: fakeness !!!
        SaveDataFile_Sub00 temp_01;
        SaveDataFile_Sub00 temp_00;

        SaveDataFile_Sub00 out_05;
        SaveDataFile_Sub00 out_02;

        u8 out_04;
        bool out_03;
        u8 out_01;
        bool out_00;

        CIconSelectLayout *layout = gLayoutManager->getLayout<CIconSelectLayout>(mEndlessIconSelectLayoutID);
        layout->fn_800C1DF4(&out_00, &out_01, &out_02, &out_03, &out_04, &out_05);
        
        gSaveData->fn_80078F4C()->fn_80077C5C(gameIndex, mEndlessScore);

        if (out_00) {
            gSaveData->fn_80078F4C()->fn_80077C84(gameIndex);

            temp_00 = out_02;
            gSaveData->fn_80078F4C()->fn_80077CA4(gameIndex, &temp_00);
        }
        else {
            gSaveData->fn_80078F4C()->fn_80777C6C(gameIndex, out_01);
        }

        if (out_03) {
            gSaveData->fn_80078F4C()->fn_80077C94(gameIndex);

            temp_01 = out_05;
            gSaveData->fn_80078F4C()->fn_80077CC0(gameIndex, &temp_01);
        }
        else {
            gSaveData->fn_80078F4C()->fn_80077C78(gameIndex, out_04);
        }
    }

    gSaveData->fn_80078F68();
    if (gBackupManager != NULL) {
        gBackupManager->fn_80084FC8(true);
    }
}
