#include "AgbTap/SceneAgbTap.hpp"

#include "AgbTap/MyFlow.hpp"

#include "AgbTap/MyFlowDecl.hpp"

#include <revolution/SC.h>

#include "GameManager.hpp"

#include "FileManager.hpp"

#include "InputCheckManager.hpp"

#include "CellAnimManager.hpp"

#include "LayoutManager.hpp"

#include "TickFlowManager.hpp"

#include "SoundManager.hpp"

#include "Controller.hpp"

#include "Random.hpp"

#include "Mem.hpp"

#include "PauseLayout.hpp"
#include "CursorLayout.hpp"
#include "BalloonLayout.hpp"

#include "code_801ED7D4.hpp"

#include "rev_tengoku.rsid"

#include "cellanim/agbTap/ver0/rcad_agb_tap_labels.h"
#include "cellanim/agbTap/ver0/rcad_agb_tap_bg_labels.h"

struct GirlAnimeData {
    s16 animID;
    u8 keyIndex;
    bool reverse;
    bool looping;
    u8 _pad06;
    u8 ticks;
    u8 _pad07;
};

#define GIRL_ANIME_DATA_BEGIN(tableName) \
    GirlAnimeData tableName[] = {

#define GIRL_ANIME_DATA_ENTRY(animID, keyIndex, reverse, looping, ticks) \
    { (animID), (keyIndex), (reverse), (looping), 0x00, (ticks) },

#define GIRL_ANIME_DATA_END() \
    };

struct MonkeyAnimeData {
    s16 animID;
    u8 keyIndex;
    bool reverse;
    bool looping;
    u8 _pad06;
    u8 ticks;
    u8 _pad07;
    s16 sfxID;
    u8 _pad09[3];
};

#define MONKEY_ANIME_DATA_BEGIN(tableName) \
    MonkeyAnimeData tableName[] = {

#define MONKEY_ANIME_DATA_ENTRY(animID, keyIndex, reverse, looping, ticks, sfxID) \
    { (animID), (keyIndex), (reverse), (looping), 0x00, (ticks), 0x00, (sfxID) },

#define MONKEY_ANIME_DATA_END() \
    };

void CSceneAgbTap::fn_800A8FA4(u32 button) {
    gGameManager->getCurrentScene<CSceneAgbTap>()->fn_800A9698(button, 2);
}

SCENE_IMPL_CREATE_FN(CSceneAgbTap)

void CSceneAgbTap::fn_800A9034(void) {
    fn_801D369C(eHeapGroup_SceneAsset);
    if (gFileManager->fn_801D42E0(2)) {
        gFileManager->fn_801D3F94(2, "content2/cellanim/agbTap/ver0/cellanim.szs");
    }
    fn_801D3644();
}

void CSceneAgbTap::_10(void) {
    fn_800A9034();
}

bool CSceneAgbTap::_24(void) {
    return gFileManager->fn_801D42FC(2);
}

void CSceneAgbTap::_14(void) {
    this->CExScene::_14();

    gInputCheckManager->setUnk418(fn_800A8FA4);

    gTickFlowManager->registerFlow<AgbTap::CMyFlow>();

    CController *controller = gControllerManager->fn_801D5FF0(0);
    controller->fn_801D5500(0x800, 3);

    void *agb_tap_brcadAddr = gFileManager->fn_801D4270(2, "./agb_tap.brcad");
    void *agb_tap_bg_brcadAddr = gFileManager->fn_801D4270(2, "./agb_tap_bg.brcad");

    u32 tplLen = gFileManager->fn_801D422C(2, "./cellanim.tpl");
    void *tplAddr = gFileManager->fn_801D4270(2, "./cellanim.tpl");

    fn_801ED7D4(tplAddr);
    DCStoreRange(tplAddr, tplLen);

    gCellAnimManager->fn_801DB568(agb_tap_brcadAddr, tplAddr, 0);
    gCellAnimManager->fn_801DB568(agb_tap_bg_brcadAddr, tplAddr, 1);

    CCellAnim *bgAnim = gCellAnimManager->fn_801DBE7C(1, agb_tap_bg_bg);
    bgAnim->fn_801DCF94(3000);

    mGirlAnim = gCellAnimManager->fn_801DBE7C(0, agb_tap_girl_beat);
    mGirlAnim->setPos(130.0f, 130.0f);
    mGirlAnim->fn_801DCF94(2000);

    CCellAnim *girlShadow = gCellAnimManager->fn_801DBE7C(0, agb_tap_shadow);
    girlShadow->setPos(128.0f, 125.0f);
    girlShadow->fn_801DCF94(2010);

    mMonkeyAnim[0] = gCellAnimManager->fn_801DBE7C(0, agb_tap_monkey_beat);
    mMonkeyAnim[1] = gCellAnimManager->fn_801DBE7C(0, agb_tap_monkey_beat);

    mMonkeyAnim[0]->setPos(-20.0f, 130.0f);
    mMonkeyAnim[1]->setPos(-120.0f, 130.0f);

    mMonkeyAnim[0]->fn_801DCF94(1990);
    mMonkeyAnim[1]->fn_801DCF94(1990);

    CCellAnim *monkeyShadow0 = gCellAnimManager->fn_801DBE7C(0, agb_tap_shadow);
    CCellAnim *monkeyShadow1 = gCellAnimManager->fn_801DBE7C(0, agb_tap_shadow);

    monkeyShadow0->setPos(-20.0f, 130.0f);
    monkeyShadow1->setPos(-120.0f, 130.0f);

    monkeyShadow0->setScale(0.7f, 0.7f);
    monkeyShadow1->setScale(0.7f, 0.7f);

    monkeyShadow0->fn_801DCF94(2000);
    monkeyShadow1->fn_801DCF94(2000);

    mGiraffeAnim = gCellAnimManager->fn_801DBE7C(0, agb_tap_giraffe_normal);
    mGiraffeAnim->fn_801DCF94(2100);
    mGiraffeAnim->setEnabled(false);

    gLayoutManager->_20(1);
    gLayoutManager->_24(94, ""); // layout_common

    gLayoutManager->fn_801D6AEC(3);
    gLayoutManager->registerLayout<CPauseLayout>();   // Index 0
    gLayoutManager->registerLayout<CCursorLayout>();  // Index 1
    gLayoutManager->registerLayout<CBalloonLayout>(); // Index 2

    mPauseLayout = gLayoutManager->getLayout<CPauseLayout>(0);

    gLayoutManager->getLayout<CPauseLayout>(0)->setDrawEnable(false);
    gLayoutManager->getLayout<CCursorLayout>(1)->setDrawEnable(false);

    mLastAnimeType = eAgbTapMove_Tap_0L;

    mGirlAnimeTimer = 0;
    mMonkeyAnimeTimer = 0;
    mGiraffeAnimeTimer = 0;

    mGirlPos.setEasing(0, 1);
    mMonkeyPos.setEasing(0, 1);
    mGiraffePos.setEasing(3, 2);
}

TFD_BEGIN(lbl_80267290)
    TFC_REST(12)
    TFC_AGBTAP_SET_ANIME_GIRL(eAgbTapMove_Round)
TFD_STOP()

TFD_BEGIN(lbl_802672A0)
    TFC_REST(12)
    TFC_AGBTAP_SET_ANIME_MONKEY(eAgbTapMove_Round)
TFD_STOP()

TFD_BEGIN(lbl_802672B0)
    TFC_MESG_PANE_VISIBLE(0, false)
TFD_STOP()

void CSceneAgbTap::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);

    if (mGirlPos.getIsTicking()) {
        mGirlPos.fn_801E9DE8(FALSE);
        mGirlAnim->setPosY(mGirlPos.getCurrentY());
    }
    if (mMonkeyPos.getIsTicking()) {
        mMonkeyPos.fn_801E9DE8(FALSE);
        mMonkeyAnim[0]->setPosY(mMonkeyPos.getCurrentY());
        mMonkeyAnim[1]->setPosY(mMonkeyPos.getCurrentY());
    }
    if (mGiraffePos.getIsTicking()) {
        mGiraffePos.fn_801E9DE8(FALSE);
        mGiraffeAnim->setPos(mGiraffePos.getCurrentX(), mGiraffePos.getCurrentZ());
    }

    if (mGirlAnimeTimer > 0) {
        mGirlAnimeTimer--;
    }
    if (mMonkeyAnimeTimer > 0) {
        mMonkeyAnimeTimer--;
    }
    if (mGiraffeAnimeTimer > 0) {
        mGiraffeAnimeTimer--;

        // TODO: we need to do this to generate two BNEs in a row .. truly a
        //       compiler of all time
        //       can we clean this up somehow?
        bool animeTimeUp = mGiraffeAnimeTimer == 0;
        if (animeTimeUp && animeTimeUp) {
            if (mGiraffeAnimeTimer == 0) {
                mGiraffeAnim->fn_801DD0AC(agb_tap_giraffe_normal);
                gTickFlowManager->fn_801E1CC0(lbl_802672B0);
            }
        }
    }
}

void CSceneAgbTap::_1C(void) {
    this->CExScene::_1C();
}

void CSceneAgbTap::_20(void) {
    gCellAnimManager->fn_801DBA98(0);
    gCellAnimManager->fn_801DBA98(1);

    gCellAnimManager->fn_801DC068(0);
    gCellAnimManager->fn_801DC068(1);

    gFileManager->fn_801D41CC(2);

    this->CExScene::_20();
}

void CSceneAgbTap::fn_800A9698(u32 button, s32 type) {
    if (button == 4) {
        if (type == 0) {
            fn_800A9924();
            gSoundManager->play(SE_AGB_TAP_KICK_LADY);
        }
        else if (type == 1) {
            fn_800A9924();
            gSoundManager->play(SE_AGB_TAP_OSII);

            mMissTapCount++;
        }
        else if (type == 2) {
            if (
                (mLastAnimeType != eAgbTapMove_Crouch) && (mLastAnimeType != eAgbTapMove_Jump) &&
                (mLastAnimeType != eAgbTapMove_Jump_Last) && (mLastAnimeType != eAgbTapMove_Pause) && 
                (mLastAnimeType != eAgbTapMove_Round) && (mLastAnimeType != eAgbTapMove_Land)
            ) {
                fn_800A9924();
                gSoundManager->play(SE_AGB_TAP_OSII);
            }
            mThroughTapCount++;
        }
    }
}

void CSceneAgbTap::fn_800A9758(void) {
    mThroughTapCount = 0;
    mMissTapCount = 0;
}

void CSceneAgbTap::fn_800A9768(void) {
    if (mGirlAnimeTimer == 0) {
        mGirlAnim->fn_801DD0AC(agb_tap_girl_beat);
    }
    if (mMonkeyAnimeTimer == 0) {
        for (s32 i = 0; i < (s32)ARRAY_LENGTH(mMonkeyAnim); i++) {
            mMonkeyAnim[i]->fn_801DD0AC(agb_tap_monkey_beat);
        }
    }
}

GIRL_ANIME_DATA_BEGIN(sGirlAnimeData)
    /* eAgbTapMove_Ready_L */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_ready_L, 0, false, false, 30)
    /* eAgbTapMove_Ready_R */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_ready_R, 0, false, false, 30)
    /* eAgbTapMove_Tap_1L_NG */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_tap_1L, 3, false, false, 30)
    /* eAgbTapMove_Tap_1R_NG */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_tap_1R, 3, false, false, 30)
    /* eAgbTapMove_Tap_0L */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_tap_0L, 0, false, false, 20)
    /* eAgbTapMove_Tap_0R */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_tap_0R, 0, false, false, 20)
    /* eAgbTapMove_Tap_1L */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_tap_1L, 0, false, false, 20)
    /* eAgbTapMove_Tap_1R */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_tap_1R, 0, false, false, 20)
    /* eAgbTapMove_Charge */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_charge, 0, false, false, 30)
    /* eAgbTapMove_Crouch */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_crouch, 1, false, false, 30)
    /* eAgbTapMove_Jump */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_jump, 0, false, false, 30)
    /* eAgbTapMove_Jump_Last */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_jump, 0, false, false, 30)
    /* eAgbTapMove_Pause */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_pause, 0, false, false, 20)
    /* eAgbTapMove_Round */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_round, 0, false, false, 30)
    /* eAgbTapMove_Land */ GIRL_ANIME_DATA_ENTRY(agb_tap_girl_land, 0, false, false, 20)
    /* eAgbTapMove_Ready_2 */ GIRL_ANIME_DATA_ENTRY(-1, 0, false, false, 30)
    /* eAgbTapMove_Ready_3_2 */ GIRL_ANIME_DATA_ENTRY(-1, 0, false, false, 30)
GIRL_ANIME_DATA_END()

void CSceneAgbTap::fn_800A97DC(EAgbTapMove type) {
    mLastAnimeType = type;

    GirlAnimeData *animeData = &sGirlAnimeData[static_cast<s32>(type)];
    
    if (animeData->animID >= 0) {
        mGirlAnim->fn_801DCE9C(animeData->animID);
        mGirlAnim->fn_801DD2B4(animeData->keyIndex);
        mGirlAnim->setLooping(animeData->looping);
        mGirlAnim->setPlaybackReverse(animeData->reverse);
        mGirlAnim->setPlaying(true);
    }

    bool doJumping = false;
    
    if (type == eAgbTapMove_Jump) {
        doJumping = true;
        gTickFlowManager->fn_801E1CC0(lbl_80267290);
    }
    if (type == eAgbTapMove_Jump_Last) {
        doJumping = true;
    }

    if (doJumping) {
        mGirlPos.fn_801EA8E0(0.0f, mGirlAnim->getPosY(), 0.0f, 0.0f, 130.0f, 0.0f, -160.0f);
        mGirlPos.fn_801EA550(0.0f, 48.0f, TRUE);
    }

    f32 frames = gTickFlowManager->fn_801E26B4(animeData->ticks);
    mGirlAnimeTimer = frames;
}

void CSceneAgbTap::fn_800A9924(void) {
    EAgbTapMove nextType = eAgbTapMove_Null;
    switch (mLastAnimeType) {
    case eAgbTapMove_Ready_L:
    case eAgbTapMove_Tap_1L_NG:
    case eAgbTapMove_Tap_0L:
        nextType = eAgbTapMove_Tap_0L;
        break;

    case eAgbTapMove_Ready_R:
    case eAgbTapMove_Tap_1R_NG:
    case eAgbTapMove_Tap_0R:
        nextType = eAgbTapMove_Tap_0R;
        break;

    case eAgbTapMove_Tap_1R:
    case eAgbTapMove_Charge:
        nextType = eAgbTapMove_Tap_1L;
        break;

    case eAgbTapMove_Tap_1L:
        nextType = eAgbTapMove_Tap_1R;
        break;

    case eAgbTapMove_Round:
        nextType = eAgbTapMove_Land;
        break;

    case eAgbTapMove_Jump_Last:
        nextType = eAgbTapMove_Pause;
        break;
    
    default:
        break;
    }

    if (nextType == eAgbTapMove_Null) {
        return;
    }

    fn_800A97DC(nextType);
}

MONKEY_ANIME_DATA_BEGIN(sMonkeyAnimeData)
    /* eAgbTapMove_Ready_L */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_ready_L, 0, false, false, 30, SE_AGB_TAP_READY_1)
    /* eAgbTapMove_Ready_R */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_ready_R, 0, false, false, 30, SE_AGB_TAP_READY_2)
    /* eAgbTapMove_Tap_1L_NG */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_tap_1L, 3, false, false, 30, -1)
    /* eAgbTapMove_Tap_1R_NG */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_tap_1R, 3, false, false, 30, -1)
    /* eAgbTapMove_Tap_0L */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_tap_0L, 0, false, false, 20, SE_AGB_TAP_KICK_MONKEY)
    /* eAgbTapMove_Tap_0R */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_tap_0R, 0, false, false, 20, SE_AGB_TAP_KICK_MONKEY)
    /* eAgbTapMove_Tap_1L */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_tap_1L, 0, false, false, 20, SE_AGB_TAP_KICK_MONKEY)
    /* eAgbTapMove_Tap_1R */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_tap_1R, 0, false, false, 20, SE_AGB_TAP_KICK_MONKEY)
    /* eAgbTapMove_Charge */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_charge, 0, false, false, 30, SE_AGB_TAP_READY_3_1)
    /* eAgbTapMove_Crouch */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_crouch, 1, false, false, 30, -1)
    /* eAgbTapMove_Jump */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_jump, 0, false, false, 30, SE_AGB_TAP_READY_JUMP)
    /* eAgbTapMove_Jump_Last */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_jump, 0, false, false, 30, SE_AGB_TAP_READY_JUMP_LAST)
    /* eAgbTapMove_Pause */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_pause, 0, false, false, 20, SE_AGB_TAP_KICK_MONKEY)
    /* eAgbTapMove_Round */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_round, 0, false, false, 30, -1)
    /* eAgbTapMove_Land */ MONKEY_ANIME_DATA_ENTRY(agb_tap_monkey_land, 0, false, false, 20, SE_AGB_TAP_KICK_MONKEY)
    /* eAgbTapMove_Ready_2 */ MONKEY_ANIME_DATA_ENTRY(-1, 0, false, false, 30, SE_AGB_TAP_READY_2)
    /* eAgbTapMove_Ready_3_2 */ MONKEY_ANIME_DATA_ENTRY(-1, 0, false, false, 30, SE_AGB_TAP_READY_3_2)
MONKEY_ANIME_DATA_END()

void CSceneAgbTap::fn_800A9988(EAgbTapMove type) {
    MonkeyAnimeData *animeData = &sMonkeyAnimeData[static_cast<s32>(type)];
    
    if (animeData->animID >= 0) {
        for (s32 i = 0; i < (s32)ARRAY_LENGTH(mMonkeyAnim); i++) {
            mMonkeyAnim[i]->fn_801DCE9C(animeData->animID);
            mMonkeyAnim[i]->fn_801DD2B4(animeData->keyIndex);
            mMonkeyAnim[i]->setLooping(animeData->looping);
            mMonkeyAnim[i]->setPlaybackReverse(animeData->reverse);
            mMonkeyAnim[i]->setPlaying(true);
        }
    }

    bool doJumping = false;
    
    if (type == eAgbTapMove_Jump) {
        doJumping = true;
        gTickFlowManager->fn_801E1CC0(lbl_802672A0);
    }
    if (type == eAgbTapMove_Jump_Last) {
        doJumping = true;
    }

    if (doJumping) {
        mMonkeyPos.fn_801EA8E0(0.0f, mMonkeyAnim[0]->getPosY(), 0.0f, 0.0f, 130.0f, 0.0f, -100.0f);
        mMonkeyPos.fn_801EA550(0.0f, 48.0f, TRUE);
    }

    f32 frames = gTickFlowManager->fn_801E26B4(animeData->ticks);
    mMonkeyAnimeTimer = static_cast<u16>(frames);

    if (animeData->sfxID >= 0) {
        gSoundManager->play(animeData->sfxID);
    }

    if (
        (type != eAgbTapMove_Tap_0L) && (type != eAgbTapMove_Tap_0R) && (type != eAgbTapMove_Tap_1L) &&
        (type != eAgbTapMove_Tap_1R) && (type != eAgbTapMove_Pause) && (type != eAgbTapMove_Land) &&
        (type != eAgbTapMove_Ready_2) && (type != eAgbTapMove_Ready_3_2)
    ) {
        fn_800A97DC(type);
    }
}

TFD_BEGIN(lbl_802672BC)
    TFC_MESG_PANE_VISIBLE(0, true)
    TFC_MESG_PANE_SET_TEXT(0, "0084100")
TFD_STOP()

TFD_BEGIN(lbl_802672D0)
    TFC_MESG_PANE_VISIBLE(0, true)
    TFC_MESG_PANE_SET_TEXT(0, "0084101")
TFD_STOP()

TFD_BEGIN(lbl_802672E4)
    TFC_MESG_PANE_VISIBLE(0, true)
    TFC_MESG_PANE_SET_TEXT(0, "0084102")
TFD_STOP()

TFD_BEGIN(lbl_802672F8)
    TFC_MESG_PANE_VISIBLE(0, true)
    TFC_MESG_PANE_SET_TEXT(0, "0084103")
TFD_STOP()

TFD_BEGIN(lbl_8026730C)
    TFC_MESG_PANE_VISIBLE(0, true)
    TFC_MESG_PANE_SET_TEXT(0, "0084104")
TFD_STOP()

void CSceneAgbTap::fn_800A9B20(bool param_1) {
    f32 a = -450.0f;
    f32 b = -300.0f;

    if (SCGetAspectRatio() == SC_ASPECT_WIDE) {
        a -= 100.0f;
        b -= 100.0f;
    }

    if (param_1) {
        mGiraffePos.fn_801EA8E0(a, 0.0f, 240.0f, b, 0.0f, 100.0f, 0.0f);
        mGiraffePos.fn_801EA550(0.0f, 48.0f, TRUE);
    }
    else {
        mGiraffePos.fn_801EA8E0(b, 0.0f, 100.0f, a, 0.0f, 240.0f, 0.0f);
        mGiraffePos.fn_801EA550(0.0f, 48.0f, TRUE);
    }

    mGiraffeAnim->setEnabled(true);
    mGiraffeAnim->setPos(mGiraffePos.getCurrentX(), mGiraffePos.getCurrentZ());
}

void CSceneAgbTap::fn_800A9C3C(void) {
    if (mGiraffeAnimeTimer == 0) {
        mGiraffeAnim->fn_801DD0AC(agb_tap_giraffe_normal);
        gTickFlowManager->fn_801E1CC0(lbl_802672B0);
    }
}

const TickFlowCode *lbl_802EF840[] = {
    lbl_802672BC,
    lbl_802672D0,
    lbl_802672E4,
    lbl_802672F8,
    lbl_8026730C,
};

void CSceneAgbTap::fn_800A9C84(u32 maxMissTapCount) {
    if (mGiraffeAnimeTimer == 0) {
        if ((mThroughTapCount != 0) || (mMissTapCount > maxMissTapCount)) {
            return;
        }

        mGiraffeAnim->fn_801DD0AC(agb_tap_giraffe_nice);

        s32 i = sRandom.nextU32(ARRAY_LENGTH(lbl_802EF840));
        if (i < 0) { // ???
            gTickFlowManager->fn_801E1CC0(lbl_802672B0);
        }
        else {
            gTickFlowManager->fn_801E1CC0(lbl_802EF840[i]);
        }
    }
}

void CSceneAgbTap::fn_800A9D20(void) {
    if (mGiraffeAnimeTimer == 0) {
        mGiraffeAnim->fn_801DD0AC(agb_tap_giraffe_bad);
        mGiraffeAnimeTimer = gTickFlowManager->fn_801E26B4(96.0f);

        gTickFlowManager->fn_801E1CC0(lbl_802672B0);
    }
}

void CSceneAgbTap::fn_800A9D90(void) {
    f32 x = (mMonkeyAnim[0]->getPos().x + mMonkeyAnim[1]->getPos().x) / 2.0f;

    gLayoutManager->getLayout<CBalloonLayout>(2)->fn_8003A1D0(
        0, x, 20.0f, 480.0f, 80.0f, false, 0.0f, 0.0f, false
    );
    gLayoutManager->getLayout<CBalloonLayout>(2)->fn_80039E00(0, FALSE);
}

void CSceneAgbTap::fn_800A9E24(void) {
    f32 x = mGiraffeAnim->getPos().x + 240.0f;

    gLayoutManager->getLayout<CBalloonLayout>(2)->fn_8003A1D0(
        0, x, -170.0f, 400.0f, 80.0f, true, -130.0f, 35.0f, false
    );
}

void CSceneAgbTap::_18(void) {
    this->CExScene::_18();
}
