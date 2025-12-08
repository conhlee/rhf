#include "Credit/SceneCredit.hpp"

#include "Credit/MyFlow.hpp"

#include "Credit/MyLayout.hpp"

#include "PauseLayout.hpp"
#include "CursorLayout.hpp"
#include "BalloonLayout.hpp"

#include <stdio.h>

#include "Mem.hpp"

#include "FileManager.hpp"
#include "InputCheckManager.hpp"
#include "CellAnimManager.hpp"
#include "LayoutManager.hpp"

#include "Controller.hpp"

#include "code_801ED7D4.hpp"

#include "cellanim/credit/ver0/rcad_chara_00_labels.h"
#include "cellanim/credit/ver0/rcad_title_labels.h"
#include "cellanim/credit/ver0/rcad_wipe_labels.h"
#include "cellanim/navi/ver0/rcad_navigator_labels.h"

namespace Credit {

s32 sceneVer;

} // namespace Credit

void CSceneCredit::fn_800C421C(u32) {}

SCENE_IMPL_CREATE_FN(CSceneCredit)

void CSceneCredit::fn_800C42AC(void) {
    fn_801D369C(eHeapGroup_SceneAsset);
    if (
        gFileManager->fn_801D42E0(2) && gFileManager->fn_801D42E0(3) &&
        gFileManager->fn_801D42E0(55) &&
        (Credit::sceneVer == 0)
    ) {
        gFileManager->fn_801D3F94(2, "content2/cellanim/credit/ver0/cellanim.szs");
        gFileManager->fn_801D3F94(3, "content2/cellanim/navi/ver0/cellanim.szs");
        gFileManager->fn_801D3F94(55, "content2/layout/layout_credit.szs");
    }
    fn_801D3644();
}

void CSceneCredit::_10(void) {
    fn_800C42AC();
}

bool CSceneCredit::_24(void) {
    return gFileManager->fn_801D42FC(2) && gFileManager->fn_801D42FC(3) &&
           gFileManager->fn_801D42FC(55);
}

char lbl_8034C860[24];

void CSceneCredit::_14(void) {
    this->CExScene::_14();

    gInputCheckManager->setUnk418(fn_800C421C);

    gTickFlowManager->registerFlow<Credit::CMyFlow>();

    u32 credit_tplLen = gFileManager->fn_801D422C(2, "./cellanim.tpl");
    u32 navi_tplLen = gFileManager->fn_801D422C(3, "./cellanim.tpl");

    void *credit_tplAddr = gFileManager->fn_801D4270(2, "./cellanim.tpl");
    void *navi_tplAddr = gFileManager->fn_801D4270(3, "./cellanim.tpl");

    fn_801ED7D4(credit_tplAddr);
    fn_801ED7D4(navi_tplAddr);

    DCStoreRange(credit_tplAddr, credit_tplLen);
    DCStoreRange(navi_tplAddr, navi_tplLen);

    for (u8 i = 0; i < CHARA_COUNT; i++) {
        sprintf(lbl_8034C860, "./chara_%02d.brcad", i);
        void *brcadAddr = gFileManager->fn_801D4270(2, lbl_8034C860);
        gCellAnimManager->fn_801DB568(brcadAddr, credit_tplAddr, CELL_INDEX_CHARA_START + i);
    }

    void *wipe_brcadAddr = gFileManager->fn_801D4270(2, "./wipe.brcad");
    void *title_brcadAddr = gFileManager->fn_801D4270(2, "./title.brcad");
    void *navigator_brcadAddr = gFileManager->fn_801D4270(3, "./navigator.brcad");

    gCellAnimManager->fn_801DB568(wipe_brcadAddr, credit_tplAddr, CELL_INDEX_WIPE);
    gCellAnimManager->fn_801DB568(title_brcadAddr, credit_tplAddr, CELL_INDEX_TITLE);
    gCellAnimManager->fn_801DB568(navigator_brcadAddr, navi_tplAddr, CELL_INDEX_NAVI);

    for (u8 i = CELL_INDEX_CHARA_START; i < CELL_INDEX_CHARA_END; i++) {
        mCharaAnim[i] = gCellAnimManager->fn_801DBE7C(i, chara_00_beat);
        
        mCharaAnim[i]->fn_801DCF94(2000);
        mCharaAnim[i]->fn_801DCF38();

        if (i > CELL_INDEX_CHARA_START) {
            mCharaAnim[i]->setBase(mCharaAnim[i - 1], 0, false);
            mCharaAnim[i]->setPosX(-832.0f);
        }
    }

    mMasterAnim = gCellAnimManager->fn_801DBE7C(0, chara_00_master_beat_00);

    mMasterAnim->fn_801DCF94(2000);
    mMasterAnim->setBase(mCharaAnim[CHARA_COUNT - 1], 0, false);
    mMasterAnim->setPosX(-832.0f);

    CCellAnim *bgAnim = gCellAnimManager->fn_801DBE7C(0, chara_00_bg);
    CCellAnim *counterAnim = gCellAnimManager->fn_801DBE7C(0, chara_00_counter);

    bgAnim->fn_801DCF94(3000);
    counterAnim->fn_801DCF94(1000);

    bgAnim->setBase(mMasterAnim, 0, false);
    counterAnim->setBase(mMasterAnim, 0, false);

    bgAnim->setPosX(-832.0f);
    counterAnim->setPosX(-832.0f);

    u16 mascotBodyAnimID[MASCOT_COUNT] = {
        navigator_c_body_down,
        navigator_a_body_down,
        navigator_b_body_down
    };
    u16 mascotFaceAnimID[MASCOT_COUNT] = {
        navigator_c_face,
        navigator_a_face_talk,
        navigator_b_face_talk
    };

    for (s32 i = 0; i < MASCOT_COUNT; i++) {
        mMascotBodyAnim[i] = gCellAnimManager->fn_801DBE7C(CELL_INDEX_NAVI, mascotBodyAnimID[i]);
        mMascotBodyAnim[i]->fn_801DCF94(2000);
        mMascotBodyAnim[i]->setBase(bgAnim, 0, false);
        mMascotBodyAnim[i]->fn_801DCF38();

        mMascotFaceAnim[i] = gCellAnimManager->fn_801DBE7C(CELL_INDEX_NAVI, mascotFaceAnimID[i]);
        mMascotFaceAnim[i]->fn_801DCF94(1999);
        mMascotFaceAnim[i]->setBase(mMascotBodyAnim[i], 0, false);
        mMascotFaceAnim[i]->fn_801DCF38();

        mMascotPosition[i].setEasing(0, 1);
    }

    mMascotBodyAnim[0]->setPos(-105.0f, 1000.0f);
    mMascotBodyAnim[1]->setPos(0.0f, 1000.0f);
    mMascotBodyAnim[2]->setPos(125.0f, 1000.0f);

    mWipeAnim = gCellAnimManager->fn_801DBE7C(CELL_INDEX_WIPE, wipe_wipe_show_00);
    mWipeAnim->fn_801DCF94(500);
    mWipeAnim->setEnabled(false);

    mTitleAnim = gCellAnimManager->fn_801DBE7C(CELL_INDEX_TITLE, title_title_wait);
    mTitleAnim->fn_801DCF94(300);

    gLayoutManager->_20(2);
    gLayoutManager->_24(94, ""); // layout_common
    gLayoutManager->_24(55, ""); // layout_credit

    gLayoutManager->fn_801D6AEC(4);
    gLayoutManager->registerLayout<Credit::CMyLayout>(); // Index 0
    gLayoutManager->registerLayout<CPauseLayout>();      // Index 1
    gLayoutManager->registerLayout<CCursorLayout>();     // Index 2
    gLayoutManager->registerLayout<CBalloonLayout>();    // Index 3

    gLayoutManager->getLayout<CBalloonLayout>(3)->fn_80039E00(0, 0);
    gLayoutManager->getLayout<CBalloonLayout>(3)->fn_80039E94(0.0f, -150.0f);
    gLayoutManager->getLayout<CBalloonLayout>(3)->fn_8003A030(480.0f, 120.0f);
    gLayoutManager->getLayout<CBalloonLayout>(3)->fn_80039E5C(false);
    gLayoutManager->getLayout<CBalloonLayout>(3)->fn_8003A11C();

    s16 bgColor[4] = { 0xFF, 0xFF, 0xFF, 0x00 };
    s16 fgColor[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

    gLayoutManager->getLayout<CBalloonLayout>(3)->fn_8003A074(bgColor, fgColor);

    mPauseLayout = gLayoutManager->getLayout<CPauseLayout>(1);
    mPauseLayout->setUnk1D(false);

    gLayoutManager->getLayout<CPauseLayout>(1)->setDrawEnable(0);
    gLayoutManager->getLayout<CCursorLayout>(2)->setDrawEnable(0);

    mScreenScroll.setEasing(2, 2);

    mCharaIndex = -1;

    mCharaAnim[0]->setPosX(-832.0f);
}

void CSceneCredit::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);

    if (mScreenScroll.getIsTicking()) {
        mScreenScroll.fn_801E9DE8(FALSE);
        mCharaAnim[0]->setPosX(mScreenScroll.getOutputStart());
    }

    for (s32 i = 0; i < MASCOT_COUNT; i++) {
        if (mMascotPosition[i].getIsTicking()) {
            mMascotPosition[i].fn_801E9DE8(FALSE);
            mMascotBodyAnim[i]->setPosY(mMascotPosition[i].getCurrentY());
        }
    }
}

void CSceneCredit::_1C(void) {
    this->CExScene::_1C();
}

void CSceneCredit::_20(void) {
    for (u8 i = 0; i < CELL_INDICES_END; i++) {
        gCellAnimManager->fn_801DBA98(i);
        gCellAnimManager->fn_801DC068(i);
    }

    gFileManager->fn_801D41CC(2);
    gFileManager->fn_801D41CC(3);
    gFileManager->fn_801D41CC(55);

    this->CExScene::_20();
}

void CSceneCredit::fn_800C4C70(u32, u32) {}

void CSceneCredit::fn_800C4C74(s32 ticks) {
    s32 nextCharaIndex = mCharaIndex + 1;

    mScreenScroll.fn_801EA698(mCharaIndex * 832, nextCharaIndex * 832);
    mScreenScroll.fn_801EA550(0.0f, ticks, true);

    mCharaIndex = nextCharaIndex;
}

void CSceneCredit::fn_800C4D2C(s32 mascotIdx, f32 startY, f32 endY, f32 ticks) {
    mMascotPosition[mascotIdx].fn_801EA8E0(0.0f, startY, 0.0f, 0.0f, endY, 0.0f, (endY - startY) * 1.05f);
    mMascotPosition[mascotIdx].fn_801EA550(0.0f, ticks, true);
}

void CSceneCredit::fn_800C4DB4(s32 mascotIdx, s32 times) {
    u16 animID[MASCOT_COUNT] = {
        navigator_c_face,
        navigator_a_face_talk,
        navigator_b_face_talk
    };

    for (s32 i = 0; i < times; i++) {
        if (i == 0) {
            mMascotFaceAnim[mascotIdx]->fn_801DD0AC(animID[mascotIdx]);
        }
        else {
            mMascotFaceAnim[mascotIdx]->fn_801DCEE8(animID[mascotIdx], NULL);
        }
    }
}

void CSceneCredit::fn_800C4E38(s32 mascotIdx) {
    u16 animID[MASCOT_COUNT] = {
        navigator_c_face_beat,
        navigator_a_face_beat,
        navigator_b_face_beat
    };

    mMascotFaceAnim[mascotIdx]->fn_801DD0AC(animID[mascotIdx]);
}

void CSceneCredit::_18(void) {
    this->CExScene::_18();
}
